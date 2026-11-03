// Pixel glyphs for the footer headings, drawn from 8x8 bitmaps so they share the cartridge's own pixel grid.
(function () {
  var G = {
    cart:  ['.######.', '.#....#.', '.#.##.#.', '.#....#.', '.######.', '########', '#.#..#.#', '########'],
    map:   ['##..##..', '#.##.###', '#.#..#.#', '#..#.#.#', '#.#..#.#', '#.#.##.#', '###.##.#', '..##..##'],
    code:  ['########', '#......#', '#.#....#', '#..#...#', '#.#..###', '#......#', '########', '...##...'],
    heart: ['.##..##.', '########', '########', '########', '.######.', '..####..', '...##...', '........']
  };
  document.querySelectorAll('.px[data-g]').forEach(function (svg) {
    var rows = G[svg.getAttribute('data-g')], d = '';
    rows.forEach(function (r, y) { for (var x = 0; x < 8; x++) if (r[x] === '#') d += 'M' + x + ' ' + y + 'h1v1h-1z'; });
    svg.setAttribute('viewBox', '0 0 8 8'); svg.setAttribute('shape-rendering', 'crispEdges');
    svg.innerHTML = '<path fill="currentColor" d="' + d + '"/>';
  });
})();

// The hero footage and the footer landscape go through one small shader: snapped to the GBA's 240x160 pixels,
// ordered (Bayer) dither in chunky 2px cells, and CRT scanlines. The hero keeps its colour on a short palette;
// the footer is drawn in the page's own navy, line blue and gold. Without WebGL the plain video and image stay.
function crt(canvas, src, duo) {
  var gl = canvas.getContext('webgl', { antialias: false, alpha: false, premultipliedAlpha: false });
  if (!gl) return;
  var FRAG = [
    'precision mediump float;',
    'uniform vec2 res; uniform float dpr, time, duo, fy; uniform sampler2D img; uniform vec3 c0, c1, c2;',
    'float b2(vec2 a) { a = floor(a); return fract(a.x / 2.0 + a.y * a.y * 0.75); }',
    'float bayer(vec2 a) { return b2(0.5 * a) * 0.25 + b2(a); }',   // 4x4 ordered dither, 0..1
    'void main() {',
    '  vec2 px = gl_FragCoord.xy, uv = px / res;',
    '  float asp = res.x / res.y, ga = 1.5;',   // cover-fit the 3:2 frame
    '  vec2 t = uv - 0.5; if (asp > ga) t.y *= ga / asp; else t.x *= asp / ga; t += vec2(0.5, fy);   // fy: which band a wide strip keeps',
    '  t = (floor(t * vec2(240.0, 160.0)) + 0.5) / vec2(240.0, 160.0);',   // GBA pixels, crisp
    '  vec3 c = texture2D(img, vec2(t.x, 1.0 - t.y)).rgb;',
    '  float b = bayer(px / (3.0 * dpr)) - 0.5;   // 3px cells: big enough to read as dither',
    '  float scan = mod(px.y, 3.0 * dpr) < dpr ? 0.62 : 1.0;',   // a dark line every third pixel row
    '  float roll = 1.0 + 0.05 * sin((uv.y + time * 0.07) * 6.2832);',   // a slow bright band drifts up, like a TV
    '  vec3 col;',
    '  if (duo > 0.5) {',
    '    float l = dot(c, vec3(0.299, 0.587, 0.114)) * smoothstep(1.0, 0.35, uv.y);',   // fades out toward the links above
    '    float q = floor(clamp(l * 2.4 + b * 0.9, 0.0, 2.99));',
    '    col = q < 1.0 ? c0 : q < 2.0 ? c1 : c2;',
    '  } else {',
    '    col = floor(c * 4.0 + b + 0.5) / 4.0;',   // five levels a channel
    '    col *= 1.0 - 0.35 * pow(length(uv - 0.5) * 1.3, 2.0);',
    '  }',
    '  gl_FragColor = vec4(col * scan * roll, 1.0);',
    '}'].join('\n');
  var prog = gl.createProgram();
  [[gl.VERTEX_SHADER, 'attribute vec2 p; void main() { gl_Position = vec4(p, 0.0, 1.0); }'], [gl.FRAGMENT_SHADER, FRAG]].forEach(function (s) {
    var sh = gl.createShader(s[0]); gl.shaderSource(sh, s[1]); gl.compileShader(sh); gl.attachShader(prog, sh);
  });
  gl.linkProgram(prog);
  if (!gl.getProgramParameter(prog, gl.LINK_STATUS)) return;
  gl.useProgram(prog);
  gl.bindBuffer(gl.ARRAY_BUFFER, gl.createBuffer());
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array([-1, -1, 3, -1, -1, 3]), gl.STATIC_DRAW);
  gl.enableVertexAttribArray(0); gl.vertexAttribPointer(0, 2, gl.FLOAT, false, 0, 0);
  gl.bindTexture(gl.TEXTURE_2D, gl.createTexture());
  [gl.TEXTURE_WRAP_S, gl.TEXTURE_WRAP_T].forEach(function (k) { gl.texParameteri(gl.TEXTURE_2D, k, gl.CLAMP_TO_EDGE); });
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST); gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
  function u(n) { return gl.getUniformLocation(prog, n); }
  var cs = getComputedStyle(document.documentElement);
  function rgb(v) { var h = cs.getPropertyValue(v).trim(); return [1, 3, 5].map(function (i) { return parseInt(h.slice(i, i + 2), 16) / 255; }); }
  gl.uniform3fv(u('c0'), rgb('--bg')); gl.uniform3fv(u('c1'), rgb('--line')); gl.uniform3fv(u('c2'), rgb('--dim'));
  gl.uniform1f(u('duo'), duo ? 1 : 0); gl.uniform1f(u('fy'), duo ? 0.42 : 0.5); gl.uniform1i(u('img'), 0);
  var still = matchMedia('(prefers-reduced-motion: reduce)').matches, on = false, raf = 0, ready = false, t0 = performance.now();
  function draw() {
    var dpr = Math.min(devicePixelRatio || 1, 2);   // ponytail: capped at 2, a 3x canvas costs memory for nothing visible
    var box = canvas.parentNode, w = Math.round(box.clientWidth * dpr), h = Math.round(box.clientHeight * dpr);   // the canvas itself stays hidden until its first frame
    if (!w || !h) return;
    if (canvas.width !== w || canvas.height !== h) { canvas.width = w; canvas.height = h; }
    if (src.tagName === 'VIDEO') { if (src.readyState < 2) return; gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGB, gl.RGB, gl.UNSIGNED_BYTE, src); }
    if (!ready) { if (src.tagName !== 'VIDEO') gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGB, gl.RGB, gl.UNSIGNED_BYTE, src); ready = true; box.classList.add('live'); }
    gl.viewport(0, 0, w, h);
    gl.uniform2f(u('res'), w, h); gl.uniform1f(u('dpr'), dpr); gl.uniform1f(u('time'), still ? 0 : (performance.now() - t0) / 1000);
    gl.drawArrays(gl.TRIANGLES, 0, 3);
  }
  function loop() { draw(); if (on && (!still || src.tagName === 'VIDEO')) raf = requestAnimationFrame(loop); }
  new IntersectionObserver(function (e) { on = e[0].isIntersecting; cancelAnimationFrame(raf); if (on) loop(); }).observe(canvas.parentNode);
  addEventListener('resize', draw);
}
crt(document.querySelector('.hero .crt'), document.querySelector('.hero video'));
(function () { var pic = new Image(); pic.onload = function () { crt(document.querySelector('.land .crt'), pic, true); }; pic.src = 'foot.png'; })();
