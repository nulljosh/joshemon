// Sound: the GBA's four tone channels plus the sample channel. Squares 1 and 2 play square parts, the wave channel a
// triangle for the bass, noise the drums, and up to four sampled instruments (build/samples.c, tools/samples.py) are
// mixed in software into Direct Sound A. Sound effects and cries borrow square 1 and noise while they run.
// Everything runs from the VBlank interrupt, so the music keeps time even when a screen takes long to draw.
#define REG8(a) (*(volatile u8 *)(a))
#define SND1_SWEEP REG16(0x04000060)
#define SND1_ENV   REG16(0x04000062)
#define SND1_FREQ  REG16(0x04000064)
#define SND2_ENV   REG16(0x04000068)
#define SND2_FREQ  REG16(0x0400006C)
#define SND3_SEL   REG16(0x04000070)
#define SND3_VOL   REG16(0x04000072)
#define SND3_FREQ  REG16(0x04000074)
#define SND4_ENV   REG16(0x04000078)
#define SND4_FREQ  REG16(0x0400007C)
#define SND_CNT_L  REG16(0x04000080)
#define SND_CNT_H  REG16(0x04000082)
#define SND_CNT_X  REG16(0x04000084)
#define WAVE_RAM   ((volatile u32 *)0x04000090)

static int song = -1, song_end;          // what plays; set when a song without a loop has finished
static struct { int i, left, off, midi, t; } chs[12]; // per part: note index, 1/256 frames left, when it stops, the note, frames into it
static int fx_t = -1, fx_len;            // a sound effect or cry running on square 1 and noise, frame count
typedef struct { short at, len; u16 f0, f1; u8 vol, duty, noise; } Seg; // noise: 0 square segment, else the hiss rate x10
static Seg fx[4]; static int nfx;

#include "../build/samples.c"
#define SND_TM0D    REG16(0x04000100)
#define SND_TM0CNT  REG16(0x04000102)
#define SND_DMA1SAD REG32(0x040000BC)
#define SND_DMA1DAD REG32(0x040000C0)
#define SND_DMA1CNT REG16(0x040000C6)
#define IRQ_DISPSTAT REG16(0x04000004)
#define IRQ_IE      REG16(0x04000200)
#define IRQ_IF      REG16(0x04000202)
#define IRQ_IME     REG16(0x04000208)
#define MIXN 224                        // samples a frame: 280896 cycles / 1254 per sample = 13379 Hz
typedef struct { const s8 *d; u32 pos, inc, end, back, base; int env, vol, stage, vib, t; const Inst *in; } Voice; // stage 0 attack 1 decay 2 hold 3 release
#define NV 6
static Voice dsv[NV];
static int reverb = 40; // of 128: last frame's mix fed back in, the GBA games' echo
static s8 mixbuf[2][MIXN] __attribute__((aligned(4))); static int mixcur;
static int mixacc[MIXN];
FAST static void ds_mix(s8 *out) { // adds every sounding voice into one frame of samples
  for (int i = 0; i < MIXN; i++) mixacc[i] = 0;
  for (int v = 0; v < NV; v++) {
    Voice *o = &dsv[v]; if (!o->d) continue;
    int g = o->env * o->vol >> 8; if (!g) continue;
    const s8 *d = o->d; u32 pos = o->pos, inc = o->inc, end = o->end;
    for (int i = 0; i < MIXN; i++) {
      mixacc[i] += d[pos >> 16] * g; pos += inc;
      if (pos >= end) { if (!o->back) { o->d = 0; break; } pos -= o->back; }
    }
    o->pos = pos;
  }
  const s8 *old = mixbuf[mixcur ^ 1]; // the frame playing now, 16.7 ms back
  for (int i = 0; i < MIXN; i++) { int s = (mixacc[i] >> 8) + (old[i] * reverb >> 7); out[i] = s > 127 ? 127 : s < -128 ? -128 : s; }
}
static void sound_tick(void);
static void (*volatile irq_tick)(void) = sound_tick; // through a pointer: IWRAM code cannot reach ROM with a plain call
static volatile u32 vblanks; // counted by the VBlank interrupt; present() reads it to spot a frame that ran long
FAST static void isr(void) {
  u16 f = IRQ_IF;
  if (f & 1) { // VBlank: play the frame mixed last time, mix the next, step the music
    vblanks++;
    SND_DMA1CNT = 0; SND_DMA1SAD = (u32)mixbuf[mixcur]; SND_DMA1CNT = 0xB640; // on, FIFO timing, repeat, 32-bit, fixed dest
    mixcur ^= 1; ds_mix(mixbuf[mixcur]); irq_tick();
  }
  IRQ_IF = f; REG16(0x03007FF8) |= f;
}
static void ds_note(int v, int inst, int midi, int vol, int vib) {
  const Inst *in = &INSTS[inst]; Voice *o = &dsv[v];
  if (!in->per) in = &INSTS[midi == 36 ? 6 : midi == 38 ? 7 : 8]; // the drum kit
  o->d = 0; o->in = in; o->pos = 0; o->inc = in->per ? DS_STEP[midi] * in->per : 65536; o->end = (u32)in->len << 16;
  o->back = in->loop < 0 ? 0 : (u32)(in->len - in->loop) << 16; o->vol = vol;
  o->env = in->att ? 0 : 255; o->stage = in->att ? 0 : 1; o->base = o->inc; o->vib = vib; o->t = 0; o->d = in->d;
}
static void ds_off(int v) { if (dsv[v].d && dsv[v].in->per) dsv[v].stage = 3; } // drums ring out
static void ds_tick(void) { // the envelopes, once a frame
  for (int v = 0; v < NV; v++) {
    Voice *o = &dsv[v]; if (!o->d) continue; const Inst *in = o->in;
    if (o->vib && ++o->t > 12) { static const s8 T8[8] = { 0, 1, 2, 1, 0, -1, -2, -1 }; o->inc = o->base + (int)(o->base >> 8) * T8[(o->t >> 1) & 7] * o->vib / 2; } // ~ the squares' vibrato in cents
    if (o->stage == 0) { o->env += 255 / in->att; if (o->env >= 255) { o->env = 255; o->stage = 1; } }
    else if (o->stage == 1) { o->env -= in->dec; if (o->env <= in->sus) { o->env = in->sus; o->stage = 2; } }
    else if (o->stage == 3) { o->env -= in->rel; if (o->env <= 0) { o->env = 0; o->d = 0; } }
  }
}

static void sound_init(void) {
  SND_CNT_X = 0x80;            // sound on
  SND_CNT_L = 0xFF77;          // all four channels, both speakers, full master volume
  SND_CNT_H = 2;               // PSG at 100%
  SND3_SEL = 0x40;             // write the triangle into the bank we will not play from
  static const u32 TRI[4] = { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476 }; // 0..15..0, two samples a byte
  for (int i = 0; i < 4; i++) WAVE_RAM[i] = TRI[i];
  SND3_SEL = 0;                // play bank 0 (the one just written), off until a note
  IRQ_IME = 0;
  SND_CNT_H = 2 | 4 | 0x300 | 0x800; // PSG 100%, sample channel A 100% on both speakers from timer 0, its queue reset
  SND_TM0CNT = 0; SND_TM0D = 65536 - 1254; SND_TM0CNT = 0x80;
  SND_DMA1DAD = 0x040000A0;
  for (int v = 0; v < NV; v++) dsv[v].d = 0;
  REG32(0x03007FFC) = (u32)isr;
  IRQ_DISPSTAT |= 8; IRQ_IE |= 1; IRQ_IME = 1;
#ifdef QA_DSONLY
  SND_CNT_L = 0; // the sample channel alone, for checking the mixer
#endif
}
static void sq_note(int ch, int midi, int vol, int duty, int decay) {
  int env = (vol > 15 ? 15 : vol) << 12 | (decay & 7) << 8 | duty << 6; // decay: the volume steps down every decay/64 s
  if (ch == 0) { SND1_SWEEP = 0x08; SND1_ENV = env; SND1_FREQ = SQ_RATE[midi] | 0x8000; }
  else { SND2_ENV = env; SND2_FREQ = SQ_RATE[midi] | 0x8000; }
}
static void sq_off(int ch) { if (ch == 0) { SND1_ENV = 0; SND1_FREQ = 0x8000; } else { SND2_ENV = 0; SND2_FREQ = 0x8000; } }
static void hiss(int vol, int frames, int rate10) { // a burst of noise that fades over its length (audio.js hiss)
  int step = vol ? frames * 64 / 60 / vol : 0; if (step > 7) step = 7; if (step < 1) step = 1;
  int shift = rate10 >= 20 ? 1 : rate10 >= 10 ? 3 : rate10 >= 6 ? 4 : 5;
  SND4_ENV = (vol > 15 ? 15 : vol) << 12 | step << 8;
  SND4_FREQ = shift << 4 | 1 | 0x8000;
}
// the drum kit on the noise channel: kick (deep, short), snare (mid), closed hat (high, tiny), open hat (high, rings)
static void drum(int m, int vol) {
  int shift, frames, v = vol;
  if (m == 36) { shift = 7; frames = 4; v = vol + 3; } else if (m == 38 || m == 72) { shift = 4; frames = 7; }
  else if (m == 46) { shift = 1; frames = 10; v = vol * 7 / 10; } else { shift = 1; frames = 3; v = vol * 6 / 10; }
  if (v > 15) v = 15;
  if (v < 1) v = 1;
  int step = frames * 64 / 60 / v; if (step < 1) step = 1; if (step > 7) step = 7;
  SND4_ENV = v << 12 | step << 8; SND4_FREQ = shift << 4 | 1 | 0x8000;
}
static int song_vol(int wave, int vol1000) { return wave == 2 ? vol1000 * 150 / 1000 : vol1000 * 130 / 1000; }

static void music(int s) {
  if (s == song && !song_end) return;
  IRQ_IME = 0;
  song = s; song_end = 0;
  for (int c = 0; c < 12; c++) { chs[c].i = 0; chs[c].left = 0; chs[c].off = 0; }
  for (int v = 0; v < NV; v++) dsv[v].d = 0;
  sq_off(1); SND3_SEL = 0; if (fx_t < 0) { sq_off(0); SND4_ENV = 0; }
  IRQ_IME = 1;
}
// one frame of the song: a part starts its next note when the last one's time is up
static void music_tick(void) {
  if (song < 0 || song_end) return;
  const Song *sg = &SONGS[song]; int sq = 0, ds = 0, done = 0;
  for (int c = 0; c < sg->nch; c++) {
    const SongCh *ch = &sg->ch[c]; int hw = ch->wave == 0 ? sq++ : ch->wave == 1 ? 2 : ch->wave == 2 ? 3 : 4 + ds++; // 4+: sample voices
    if (chs[c].left > 0) {
      chs[c].left -= 256; chs[c].t++;
      if (ch->vib && hw < 2 && chs[c].midi && chs[c].t > 12 && chs[c].off > 0 && !(hw == 0 && fx_t >= 0)) { // vibrato after the note settles
        static const s8 TRI8[8] = { 0, 1, 2, 1, 0, -1, -2, -1 };
        int q = SQ_RATE[chs[c].midi], r = q + TRI8[(chs[c].t >> 1) & 7] * ch->vib * (2048 - q) / 512; // the same width in cents at any pitch
        if (hw == 0) SND1_FREQ = r; else SND2_FREQ = r;
      }
      if (chs[c].off > 0 && (chs[c].off -= 256) <= 0) { if (hw < 2 && !(hw == 0 && fx_t >= 0)) sq_off(hw); else if (hw == 2) SND3_SEL = 0; else if (hw >= 4) ds_off(hw - 4); }
      if (chs[c].left > 0) continue;
    }
    if (chs[c].i >= ch->n) { if (!sg->loop) { done++; continue; } chs[c].i = 0; }
    int m = ch->notes[chs[c].i * 2], half = ch->notes[chs[c].i * 2 + 1]; chs[c].i++;
    int len = half * 900 * 256 / sg->bpm; // a quarter beat is 15/bpm seconds = 900/bpm frames; x256
    chs[c].left += len; chs[c].off = len * 92 / 100; chs[c].midi = m; chs[c].t = 0;
    if (!m) continue;
    int v = song_vol(ch->wave, ch->vol);
    if (hw < 2) { if (!(hw == 0 && fx_t >= 0)) sq_note(hw, m, v, ch->duty, ch->decay); }
    else if (hw == 2) { SND3_SEL = 0x80; SND3_VOL = ch->vol >= 160 ? 1 << 13 : ch->vol >= 110 ? 1 << 15 : ch->vol >= 70 ? 2 << 13 : 3 << 13; SND3_FREQ = WV_RATE[m] | 0x8000; }
    else if (hw >= 4) ds_note(hw - 4, ch->duty, m, ch->vol * 255 / 1000, ch->vib);
    else if (hw == 3 && fx_t < 0) drum(m, v);
  }
  if (done && done == sg->nch) song_end = 1;
}

// sound effects and cries: segments on square 1 (a held tone or a slide) and noise
static void fx_start(void) { fx_t = -1; nfx = 0; fx_len = 0; } // sfx() and cry() set fx_t = 0 once the segments are in
static void seg(int at, int len, int f0, int f1, int vol, int duty, int noise) {
  if (nfx < 4) fx[nfx++] = (Seg){ at, len, f0, f1, vol, duty, noise };
  if (at + len > fx_len) fx_len = at + len;
}
static int hz_of(int m) { return 131072 / (2048 - SQ_RATE[m]); }
static void fx_tick(void) {
  if (fx_t < 0) return;
  for (int k = 0; k < nfx; k++) {
    Seg *s = &fx[k];
    if (fx_t == s->at) {
      if (s->noise) hiss(s->vol, s->len, s->noise);
      else { int step = s->f0 == s->f1 ? 0 : s->len * 64 / 60 / (s->vol ? s->vol : 1); if (step > 7) step = 7; SND1_SWEEP = 0x08; SND1_ENV = s->vol << 12 | (step ? 0 << 11 | step << 8 : 0) | s->duty << 6; SND1_FREQ = (2048 - 131072 / s->f0) | 0x8000; }
    } else if (!s->noise && fx_t > s->at && fx_t < s->at + s->len && s->f0 != s->f1) { // slide: straight line in Hz
      int f = s->f0 + (s->f1 - s->f0) * (fx_t - s->at) / s->len; if (f < 65) f = 65;
      SND1_FREQ = 2048 - 131072 / f;
    } else if (!s->noise && fx_t == s->at + s->len) sq_off(0);
  }
  if (++fx_t > fx_len) { fx_t = -1; sq_off(0); }
}
static int ms(int v) { return v * 60 / 1000; } // milliseconds to frames
enum { SFX_CURSOR, SFX_SELECT, SFX_BACK, SFX_BUMP, SFX_HIT, SFX_SUPER, SFX_WEAK, SFX_MISS, SFX_BALL, SFX_SHAKE, SFX_FAINT, SFX_WIPE, SFX_HOP, SFX_DOOR, SFX_BUY, SFX_LOWHP, SFX_HEAL };
static void sfx(int k) { // audio.js sfx, volumes x130 like the songs
  fx_start();
  switch (k) {
    case SFX_CURSOR: seg(0, ms(50), hz_of(84), hz_of(84), 10, 2, 0); break;
    case SFX_SELECT: seg(0, ms(60), hz_of(88), hz_of(88), 12, 2, 0); seg(ms(60), ms(80), hz_of(95), hz_of(95), 12, 2, 0); break;
    case SFX_BACK: seg(0, ms(70), hz_of(76), hz_of(76), 10, 2, 0); break;
    case SFX_BUMP: seg(0, ms(50), 0, 0, 13, 0, 3); break;
    case SFX_HIT: seg(0, ms(140), 0, 0, 15, 0, 8); seg(0, ms(100), 220, 80, 15, 2, 0); break;
    case SFX_SUPER: seg(0, ms(200), 0, 0, 15, 0, 12); seg(0, ms(180), 400, 90, 15, 2, 0); break;
    case SFX_WEAK: seg(0, ms(80), 0, 0, 15, 0, 4); break;
    case SFX_MISS: seg(0, ms(120), 500, 200, 13, 1, 0); break;
    case SFX_BALL: seg(0, ms(250), 300, 700, 10, 2, 0); break;
    case SFX_SHAKE: seg(0, ms(60), hz_of(60), hz_of(60), 13, 2, 0); break;
    case SFX_FAINT: seg(0, ms(500), 400, 60, 15, 2, 0); break;
    case SFX_WIPE: seg(0, ms(500), 200, 1200, 7, 3, 0); break;
    case SFX_HOP: seg(0, ms(150), 300, 700, 15, 1, 0); break;
    case SFX_DOOR: seg(0, ms(120), 0, 0, 15, 0, 5); seg(ms(50), ms(80), hz_of(64), hz_of(64), 8, 2, 0); break;
    case SFX_BUY: seg(0, ms(60), hz_of(91), hz_of(91), 10, 2, 0); seg(ms(70), ms(120), hz_of(96), hz_of(96), 10, 2, 0); break;
    case SFX_LOWHP: seg(0, ms(80), hz_of(96), hz_of(96), 7, 2, 0); seg(ms(120), ms(80), hz_of(96), hz_of(96), 7, 2, 0); break;
    case SFX_HEAL: for (int i = 0; i < 4; i++) seg(ms(100) * i, ms(100), hz_of(72 + i * 4), hz_of(72 + i * 4), 10, 2, 0); break;
  }
  fx_t = 0;
}
// audio.js cry: built from the creature's number, so each always has the same voice; fainting is slower and lower
static void cry(int id, int faint) {
  u32 h = (u32)id * 2654435761u; int r = faint ? 72 : 100;
  int base = (260 + (int)(h % 520)) * r / 100, len = (320 + (int)((h >> 9) % 30) * 10) * 100 / r; // ms
  int duty = (h >> 5) % 3, up = base * (13 + (int)((h >> 13) % 5)) / 10;
  fx_start();
  seg(0, ms(len * 4 / 10), base, up, 14, duty, 0);
  seg(ms(len * 35 / 100), ms(len * 65 / 100), base * 3 / 2, base * 65 / 100, 12, duty, 0);
  seg(0, ms(len * 8 / 10), 0, 0, 7, 0, 5 + (int)(h % 7));
  fx_t = 0;
}
static void sound_mute(int on) { SND_CNT_L = on ? 0 : 0xFF77; }
static void sound_tick(void) { music_tick(); ds_tick(); fx_tick(); }
