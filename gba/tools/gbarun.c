// Headless cartridge runner for QA, built on mGBA's libmgba (brew mgba).
// gbarun ROM SCRIPT OUTDIR [WAV [RAW]] : runs the ROM with no window, follows SCRIPT, writes PPM screenshots to OUTDIR,
// given WAV everything the cartridge played (32768 Hz mono), given RAW every frame as 240x160 RGB24 (for ffmpeg).
// Script lines:  wait N | press KEYS N (hold for N frames, then release) | shot NAME
// KEYS: any of A B SELECT START RIGHT LEFT UP DOWN R L joined by '+'.
// With "qa ADDR" (the cartridge's QA struct: battle, talking, map, x, y, dir) scripts can also
//   mash KEYS FIELD [VALUE]  press KEYS every 12 frames while the field is not 0, or until it equals VALUE (6000 frames at most)
//   waitfor FIELD VALUE  run until the field equals VALUE (at most 6000 frames)
//   expect FIELD VALUE   print FAIL when it does not
#include <mgba-util/vfs.h>
#include <mgba/core/core.h>
#include <mgba/core/config.h>
#include <mgba/core/blip_buf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *KEYNAMES[] = { "A", "B", "SELECT", "START", "RIGHT", "LEFT", "UP", "DOWN", "R", "L" };

static unsigned keymask(char *s) {
  unsigned m = 0;
  for (char *t = strtok(s, "+"); t; t = strtok(NULL, "+"))
    for (int i = 0; i < 10; i++) if (!strcmp(t, KEYNAMES[i])) m |= 1u << i;
  return m;
}

int main(int argc, char **argv) {
  if (argc < 4) { fprintf(stderr, "usage: gbarun ROM SCRIPT OUTDIR\n"); return 2; }
  struct mCore *core = mCoreFind(argv[1]);
  if (!core || !core->init(core)) { fprintf(stderr, "no core for %s\n", argv[1]); return 1; }
  mCoreInitConfig(core, NULL);
  unsigned w, h; core->desiredVideoDimensions(core, &w, &h);
  color_t *buf = calloc(w * h, sizeof(color_t));
  core->setVideoBuffer(core, buf, w);
  if (!mCoreLoadFile(core, argv[1])) { fprintf(stderr, "cannot load %s\n", argv[1]); return 1; }
  core->loadSave(core, VFileMemChunk(NULL, 0)); // a fresh save in memory every run, so scripts never see a game an earlier run saved
  core->reset(core);
  FILE *sc = fopen(argv[2], "r"); if (!sc) { perror(argv[2]); return 1; }
  FILE *wav = argc > 4 ? fopen(argv[4], "wb") : 0, *raw = argc > 5 ? fopen(argv[5], "wb") : 0; long samples = 0;
  if (wav) { core->setAudioBufferSize(core, 2048); for (int c = 0; c < 2; c++) blip_set_rates(core->getAudioChannel(core, c), core->frequency(core), 32768); fseek(wav, 44, SEEK_SET); }
#define FRAME() do { core->runFrame(core); if (wav) { short b[2048]; blip_t *bl = core->getAudioChannel(core, 0); int n; while ((n = blip_read_samples(bl, b, 2048, 0)) > 0) { fwrite(b, 2, n, wav); samples += n; } while (blip_read_samples(core->getAudioChannel(core, 1), b, 2048, 0) > 0); } if (raw) { static unsigned char px[240 * 160 * 3]; for (unsigned i = 0; i < w * h; i++) { unsigned c = buf[i]; px[i * 3] = c; px[i * 3 + 1] = c >> 8; px[i * 3 + 2] = c >> 16; } fwrite(px, 1, sizeof px, raw); } } while (0) // right channel drained too, or it overflows
  char line[256]; int shots = 0;
  while (fgets(line, sizeof line, sc)) {
    char cmd[32] = "", arg[128] = ""; int n = 0;
    if (sscanf(line, "%31s %127s %d", cmd, arg, &n) < 1 || cmd[0] == '#') continue;
    static unsigned qa = 0;
    static const char *F[7] = { "battle", "talking", "map", "x", "y", "dir", "song" };
    int fi = -1; char a2[64] = ""; sscanf(line, "%*s %*s %63s", a2);
    if (!strcmp(cmd, "qa")) { qa = strtoul(arg, 0, 16); continue; }
    if (!strcmp(cmd, "mash") || !strcmp(cmd, "waitfor") || !strcmp(cmd, "expect")) {
      const char *field = !strcmp(cmd, "mash") ? a2 : arg;
      for (int i = 0; i < 7; i++) if (!strcmp(field, F[i])) fi = i;
      if (fi < 0 || !qa) { fprintf(stderr, "bad %s line: %s", cmd, line); continue; }
    }
#define QAF() (core->busRead8(core, qa + fi))
    if (!strcmp(cmd, "mash")) { unsigned m = keymask(arg); char a3[16] = ""; int until = sscanf(line, "%*s %*s %*s %15s", a3) == 1, v = atoi(a3);
      for (int t = 0; t < 6000 && (until ? QAF() != v : QAF()); t += 12) { core->setKeys(core, m); FRAME(); FRAME(); core->setKeys(core, 0); for (int k = 0; k < 10; k++) FRAME(); } continue; }
    if (!strcmp(cmd, "waitfor")) { int v = atoi(a2); for (int t = 0; t < 6000 && QAF() != v; t++) FRAME(); continue; }
    if (!strcmp(cmd, "expect")) { int v = atoi(a2); if (QAF() != v) printf("FAIL expect %s %d, got %d\n", arg, v, QAF()); else printf("ok %s %d\n", arg, v); continue; }
    if (!strcmp(cmd, "wait")) { for (int i = atoi(arg); i > 0; i--) FRAME(); }
    else if (!strcmp(cmd, "press")) {
      core->setKeys(core, keymask(arg)); for (int i = n ? n : 2; i > 0; i--) FRAME();
      core->setKeys(core, 0); FRAME(); FRAME();
    } else if (!strcmp(cmd, "shot")) {
      char path[512]; snprintf(path, sizeof path, "%s/%02d-%s.ppm", argv[3], ++shots, arg);
      FILE *f = fopen(path, "wb"); fprintf(f, "P6\n%u %u\n255\n", w, h);
      for (unsigned i = 0; i < w * h; i++) { unsigned c = buf[i]; unsigned char px[3] = { c & 0xFF, (c >> 8) & 0xFF, (c >> 16) & 0xFF }; fwrite(px, 1, 3, f); }
      fclose(f); printf("shot %s\n", path);
    }
  }
  if (wav) { // the RIFF header, now that the length is known
    unsigned hdr[11] = { 0x46464952, 36 + samples * 2, 0x45564157, 0x20746d66, 16, 0x00010001, 32768, 65536, 0x00100002, 0x61746164, samples * 2 };
    fseek(wav, 0, SEEK_SET); fwrite(hdr, 4, 11, wav); fclose(wav);
  }
  if (raw) fclose(raw);
  core->deinit(core);
  return 0;
}
