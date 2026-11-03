# The soundtrack, part 2: the songs after caught, in SONG_* order. tools/compose.py reads songs.py, then this.
SONGS = [
dict(NAME='legend', LEAD_INST=('brass', .3), PAD_INST=('strings', .15), BASS_INST=('bass', .3), KIT=.48, BPM=140, LOOP=1, STYLE='battle', LEAD=dict(vol=.1, vib=3),
  CHORDS='Dm:4 Bb:4 C:4 A:4 Dm:4 Bb:4 Gm:4 A:4 Bb:4 C:4 Am:4 Dm:4 Gm:4 A:4 Dm:4 A:4',
  MELODY='d5:.5 f5:.5 a5:.5 d6:.5 d6:1 a5:1  a#5:1.5 a5:.5 f5:1 d5:1  e5:.5 g5:.5 c6:.5 e6:.5 c6:1 c6:1  c#6:2 a5:1 e5:1 '
         'f5:.5 a5:.5 d6:1 e6:.5 f6:.5 d6:1  f6:1.5 d6:.5 a#5:2  a#5:1 d6:.5 a#5:.5 g5:2  a5:.5 c#6:.5 e6:.5 g6:.5 a6:2 '
         'f6:1 d6:.5 f6:.5 a#6:2  g6:1 e6:.5 c6:.5 g5:2  a5:.5 c6:.5 e6:1 d6:.5 c6:.5 a5:1  d6:2 a5:1 f5:1 '
         'g5:.5 a#5:.5 d6:1 g6:1 f6:1  e6:1 c#6:.5 e6:.5 a6:2  f6:.5 e6:.5 d6:.5 a5:.5 f5:1 d5:1  e5:.5 a5:.5 c#6:.5 e6:.5 a6:2'),
dict(NAME='town', LEAD_INST=('flute', .3), KIT=.32, BPM=100, LOOP=1, STYLE='town', LEAD=dict(vib=3),
  CHORDS='G:4 Em:4 C:4 D:4 G:4 Em:4 Am:4 D:4 C:4 D:4 Bm:4 Em:4 Am:4 D:4 G:8',
  MELODY='d5:1 b4:.5 d5:.5 g5:1.5 f#5:.5  e5:2 b4:1 r:1  c5:1 e5:.5 g5:.5 e5:1 c5:1  d5:1.5 e5:.5 f#5:1 a5:1 '
         'b5:1.5 a5:.5 g5:1 d5:1  e5:1 g5:.5 b5:.5 g5:2  a5:1 g5:.5 e5:.5 c5:1 e5:1  d5:3 r:1 '
         'e5:1 g5:.5 e5:.5 c5:1 g4:1  f#4:1 a4:.5 d5:.5 f#5:2  d5:1 f#5:.5 b5:.5 b5:1 f#5:1  g5:1.5 f#5:.5 e5:2 '
         'c6:1 b5:.5 a5:.5 e5:1 c5:1  d5:1 f#5:.5 a5:.5 d6:1 a5:1  b5:2 a5:.5 f#5:.5 a5:1 g5:3 r:1'),
dict(NAME='lumen', LEAD_INST=('piano', .3), PAD_INST=('strings', .12), KIT=.32, BPM=92, LOOP=1, STYLE='ballad', HARMONY='echo', LEAD=dict(vib=4),
  CHORDS='Em:4 C:4 G:4 D:4 Em:4 C:4 Am:4 B:4 C:4 D:4 G:4 Em:4 Am:4 B:4 Em:8',
  MELODY='b4:1 e5:1 g5:1.5 f#5:.5  e5:2 c5:1 g4:1  d5:1 g5:1 b5:1.5 a5:.5  f#5:3 r:1 '
         'g5:1 b5:1 e6:1.5 d6:.5  c6:1.5 b5:.5 g5:2  a5:1 c6:.5 a5:.5 e5:2  d#5:2 f#5:1 b5:1 '
         'c6:1.5 b5:.5 g5:1 e5:1  f#5:1 a5:.5 d6:.5 d6:1 a5:1  b5:2 d6:1 g6:1  e6:3 r:1 '
         'c6:1 b5:.5 a5:.5 e5:1 a5:1  b5:1.5 a5:.5 f#5:1 d#5:1  e5:4 b4:2 r:2'),
dict(NAME='gym', BPM=128, LOOP=1, STYLE='route', LEAD=dict(vib=3), LEAD_INST=('brass', .3), PAD_INST=('strings', .12), BASS_INST=('bass', .28), KIT=.45,
  CHORDS='C:4 G:4 Am:4 F:4 C:4 G:4 F:4 G:4 Am:4 Em:4 F:4 C:4 Dm:4 F:4 G:8',
  MELODY='c5:.5 e5:.5 g5:1 g5:.5 a5:.5 g5:1  d5:1.5 g5:.5 b5:2  c6:1 b5:.5 a5:.5 e5:1 a5:1  f5:1.5 a5:.5 c6:2 '
         'e6:1 d6:.5 c6:.5 g5:1 c6:1  b5:1.5 a5:.5 g5:1 d5:1  a5:.5 g5:.5 f5:.5 a5:.5 c6:1 a5:1  g5:3 r:1 '
         'e5:.5 a5:.5 c6:1 b5:.5 a5:.5 e5:1  g5:1.5 e5:.5 b5:2  a5:1 c6:.5 a5:.5 f5:1 a5:1  g5:1.5 e5:.5 c5:2 '
         'd5:.5 f5:.5 a5:1 d6:1 c6:1  c6:1 a5:.5 c6:.5 f6:2  d6:2 b5:1 g5:1 d6:3 r:1'),
dict(NAME='leader', BPM=170, LOOP=1, STYLE='battle', LEAD=dict(vib=2), LEAD_INST=('brass', .22), PAD_INST=('strings', .11), BASS_INST=('bass', .3), KIT=.5,
  CHORDS='Bm:4 G:4 A:4 F#:4 Bm:4 G:4 Em:4 F#:4 G:4 A:4 D:4 Bm:4 Em:4 F#:4 Bm:4 F#:4',
  MELODY='b4:.25 d5:.25 f#5:.5 b5:.5 f#5:.5 a5:.5 b5:.5 d6:1  b5:1.5 g5:.5 d5:1 g5:1  a5:.5 c#6:.5 e6:.5 c#6:.5 a5:1 e5:1  a#5:1.5 c#6:.5 f#6:2 '
         'f#6:.5 e6:.5 d6:.5 b5:.5 f#5:1 b5:1  d6:1.5 b5:.5 g5:1 d6:1  e6:1 d6:.5 b5:.5 g5:1 e5:1  f#5:.5 a#5:.5 c#6:.5 e6:.5 f#6:2 '
         'g5:.5 b5:.5 d6:1 b5:.5 g5:.5 d5:1  e5:.5 a5:.5 c#6:1 b5:.5 a5:.5 e5:1  f#5:.5 a5:.5 d6:.5 f#6:.5 d6:1 d6:1  d6:1.5 c#6:.25 b5:.25 f#5:2 '
         'g5:.5 b5:.5 e6:1 d6:.5 b5:.5 g5:1  a#5:.5 c#6:.5 f#6:1 e6:.5 c#6:.5 a#5:1  b5:1 d6:1 f#6:1 e6:.5 d6:.5  c#6:1 a#5:.5 c#6:.5 f#5:2'),
dict(NAME='rival', BPM=150, LOOP=1, STYLE='battle', LEAD=dict(vib=2), LEAD_INST=('brass', .2), BASS_INST=('bass', .3), KIT=.5,
  CHORDS='G:4 D:4 Em:4 C:4 G:4 D:4 C:4 D:4 Em:4 C:4 G:4 D:4 C:4 D:4 G:4 D:4',
  MELODY='g5:.5 b5:.5 d6:.5 b5:.5 g5:.5 a5:.5 b5:1  a5:1.5 f#5:.5 d5:1 a5:1  g5:.5 b5:.5 e6:1 d6:.5 b5:.5 g5:1  e5:.5 g5:.5 c6:1 e6:2 '
         'd6:.5 c6:.5 b5:.5 a5:.5 g5:1 b5:1  a5:.5 b5:.5 a5:.5 f#5:.5 d5:2  e5:.5 g5:.5 c6:.5 g5:.5 e6:1 c6:1  d6:2 f#5:1 a5:1 '
         'b5:1 e6:.5 b5:.5 g5:1 e5:1  c6:1 e6:.5 c6:.5 g5:2  b5:.5 d6:.5 g6:1 f#6:.5 e6:.5 d6:1  a5:1.5 b5:.25 a5:.25 f#5:2 '
         'g5:.5 a5:.5 c6:1 e6:1 c6:1  d6:.5 e6:.5 f#6:1 a6:2  g6:1 d6:.5 b5:.5 g5:2  f#5:.5 a5:.5 d6:.5 f#6:.5 a6:2'),
dict(NAME='ashbattle', BPM=156, LOOP=1, STYLE='battle', LEAD=dict(duty=2, vib=2), LEAD_INST=('brass', .24), PAD_INST=('strings', .11), BASS_INST=('bass', .3), KIT=.5,
  CHORDS='Cm:4 Ab:4 Bb:4 G:4 Cm:4 Ab:4 Fm:4 G:4 Ab:4 Bb:4 Gm:4 Cm:4 Fm:4 G:4 Ab:4 G:4',
  MELODY='c5:.25 d#5:.25 g5:.5 c6:.5 g5:.5 a#5:.5 c6:.5 d#6:1  c6:1.5 g#5:.5 d#5:1 g#5:1  a#5:.5 d6:.5 f6:.5 d6:.5 a#5:1 f5:1  b5:1.5 d6:.5 g6:2 '
         'g6:.5 f6:.5 d#6:.5 c6:.5 g5:1 c6:1  d#6:1.5 c6:.5 g#5:1 d#6:1  f6:1 d#6:.5 c6:.5 g#5:1 f5:1  g5:.5 b5:.5 d6:.5 f6:.5 g6:2 '
         'g#5:.5 c6:.5 d#6:1 c6:.5 g#5:.5 d#5:1  f5:.5 a#5:.5 d6:1 c6:.5 a#5:.5 f5:1  g5:.5 a#5:.5 d6:.5 g6:.5 g6:1 d6:1  d#6:1.5 d6:.25 c6:.25 g5:2 '
         'g#5:.5 c6:.5 f6:1 d#6:.5 c6:.5 g#5:1  b5:.5 d6:.5 g6:1 f6:.5 d6:.5 b5:1  c6:1 d#6:1 g#6:1 g6:.5 f6:.5  g6:1 d6:.5 b5:.5 g5:2'),
dict(NAME='mart', COLOR=1, BPM=108, LOOP=1, STYLE='town', LEAD=dict(vib=3, vol=.07), LEAD_INST=('piano', .34), BASS_INST=('bass', .26), KIT=.3,
  CHORDS='Cmaj7:4 Am7:4 Dm7:4 G7:4 Cmaj7:4 Am7:4 Dm7:4 G7:4 Fmaj7:4 Em7:4 Dm7:4 G7:4 Cmaj7:4 Am7:4 Dm7:4 G7:4',
  MELODY='e5:.5 g5:.5 b5:1 a5:.5 g5:.5 e5:1  c5:1.5 e5:.5 g5:2  f5:.5 a5:.5 c6:1 a5:.5 f5:.5 d5:1  b4:1.5 d5:.5 f5:1 g5:1 '
         'e5:.5 g5:.5 b5:1 c6:.5 b5:.5 g5:1  a5:1.5 g5:.5 e5:2  d5:.5 f5:.5 a5:.5 c6:.5 b5:1 g5:1  g5:3 r:1 '
         'a5:1 c6:.5 a5:.5 e5:1 f5:1  g5:1.5 e5:.5 b4:2  d5:.5 f5:.5 a5:1 c6:1 a5:1  b5:1 g5:.5 f5:.5 d5:2 '
         'e5:1 g5:.5 b5:.5 d6:1 b5:1  c6:1 a5:.5 e5:.5 c5:2  f5:.5 a5:.5 c6:.5 d6:.5 c6:1 a5:1  b5:2 g5:1 r:1'),
dict(NAME='rest', BPM=100, LOOP=1, STYLE='town', LEAD=dict(vib=3, vol=.07), LEAD_INST=('piano', .3), PAD_INST=('strings', .1), KIT=.28,
  CHORDS='D:4 Bm:4 G:4 A:4 D:4 F#m:4 G:4 A:4 G:4 A:4 F#m:4 Bm:4 Em:4 A:4 D:8',
  MELODY='f#5:1 a5:1 d6:1.5 c#6:.5  b5:1 a5:.5 f#5:.5 d5:2  g5:1 b5:1 d6:1 b5:1  a5:3 r:1 '
         'f#5:1 a5:.5 f#5:.5 d5:1 f#5:1  f#5:1.5 f#5:.5 c#5:2  d5:1 e5:.5 g5:.5 b5:1 a5:1  a5:2 c#6:1 e6:1 '
         'd6:1.5 b5:.5 g5:2  c#6:1.5 a5:.5 e5:2  f#5:1 a5:.5 c#6:.5 f#6:1 e6:1  d6:3 r:1 '
         'e5:1 g5:.5 b5:.5 e6:1 d6:1  c#6:1 e6:.5 c#6:.5 a5:2  d6:3 a5:1 d5:3 r:1'),
dict(NAME='evo', BPM=120, LOOP=1, STYLE='cave', HARMONY='off', LEAD=dict(vib=2), PAD_INST=('strings', .14), KIT=.3,
  CHORDS='Am:4 Bb:4 Am:4 Bb:4 Am:4 Bb:4 B:4 E:4',
  MELODY='a4:.5 c5:.5 e5:.5 c5:.5 a4:.5 c5:.5 e5:.5 c5:.5  a#4:.5 d5:.5 f5:.5 d5:.5 a#4:.5 d5:.5 f5:.5 d5:.5 '
         'a4:.5 c5:.5 e5:.5 a5:.5 e5:.5 c5:.5 e5:.5 a5:.5  a#4:.5 d5:.5 f5:.5 a#5:.5 f5:.5 d5:.5 f5:.5 a#5:.5 '
         'c6:.5 a5:.5 e5:.5 a5:.5 c6:.5 e6:.5 c6:.5 a5:.5  d6:.5 a#5:.5 f5:.5 a#5:.5 d6:.5 f6:.5 d6:.5 a#5:.5 '
         'd#6:.5 b5:.5 f#5:.5 b5:.5 d#6:.5 f#6:.5 d#6:.5 b5:.5  e6:.5 g#5:.5 b5:.5 e6:.5 g#6:2'),
dict(NAME='frost', BPM=88, LOOP=1, STYLE='ballad', LEAD=dict(vib=4, vol=.08), LEAD_INST=('piano', .32), PAD_INST=('strings', .14), KIT=.25,
  CHORDS='Am:4 F:4 C:4 G:4 Am:4 F:4 Dm:4 E:4 F:4 G:4 Em:4 Am:4 Dm:4 E:4 Am:8',
  MELODY='e5:1 a5:1 b5:.5 c6:.5 b5:1  a5:2 f5:1 c5:1  e5:1 g5:1 c6:1.5 b5:.5  d6:3 r:1 '
         'c6:1 b5:.5 a5:.5 e5:2  f5:1 a5:1 c6:2  d6:1 c6:.5 a5:.5 f5:2  g#5:3 r:1 '
         'a5:1.5 c6:.5 f6:2  d6:1.5 b5:.5 g5:2  e6:1 b5:1 g5:1 e5:1  a5:3 r:1 '
         'f5:1 a5:1 d6:1.5 c6:.5  b5:1.5 g#5:.5 e5:2  a5:2 c6:1 e6:1 a6:3 r:1'),
dict(NAME='drake', BPM=104, LOOP=1, STYLE='march', LEAD=dict(vib=3, vol=.08), LEAD_INST=('brass', .3), PAD_INST=('strings', .14), KIT=.3,
  CHORDS='D:4 C:4 G:4 D:4 Bm:4 G:4 A:4 A:4 D:4 C:4 G:4 Em:4 G:4 A:4 D:8',
  MELODY='d5:1 a5:1.5 f#5:.5 a5:1  g5:1 e5:.5 c5:.5 e5:1 g5:1  d5:1.5 g5:.5 b5:1 d6:1  a5:3 r:1 '
         'b5:1 f#5:.5 d5:.5 f#5:1 b5:1  d6:1.5 b5:.5 g5:2  e5:1 a5:1 c#6:1 e6:1  c#6:3 r:1 '
         'd6:1 a5:.5 f#5:.5 a5:1 d6:1  e6:1.5 c6:.5 g5:1 e5:1  d5:1 g5:.5 b5:.5 d6:1 b5:1  g5:1.5 e5:.5 b4:2 '
         'd5:1 g5:1 b5:1 d6:1  c#6:1.5 e6:.5 a5:1 c#6:1  d6:3 a5:1 d5:3 r:1'),
dict(NAME='victory', BPM=120, LOOP=1, STYLE='cave', LEAD=dict(vib=3, vol=.08), LEAD_INST=('brass', .28), PAD_INST=('strings', .14), KIT=.3,
  CHORDS='Em:4 C:4 D:4 Bm:4 Em:4 C:4 Am:4 B:4 Em:4 C:4 D:4 G:4 C:4 Am:4 B:8',
  MELODY='e5:1 g5:.5 b5:.5 e6:1 b5:1  c6:1.5 g5:.5 e5:2  f#5:1 a5:.5 d6:.5 f#6:1 d6:1  b5:3 r:1 '
         'g5:1 b5:1 e6:1.5 d6:.5  e6:1 c6:.5 g5:.5 c6:2  a5:1 c6:1 e6:1 c6:1  d#6:3 r:1 '
         'e6:1 b5:.5 g5:.5 e5:1 g5:1  c6:1.5 e6:.5 g6:2  f#6:1 d6:.5 a5:.5 f#5:1 a5:1  g5:1.5 b5:.5 d6:2 '
         'e6:1 c6:1 g5:1 e6:1  e6:1.5 c6:.5 a5:2  f#5:1 b5:1 d#6:1 f#6:1 b5:3 r:1'),
dict(NAME='elite', BPM=160, LOOP=1, STYLE='battle', LEAD=dict(vib=2), LEAD_INST=('brass', .22), PAD_INST=('strings', .11), BASS_INST=('bass', .3), KIT=.5,
  CHORDS='Cm:4 Ab:4 Bb:4 G:4 Cm:4 Ab:4 Fm:4 G:4 Ab:4 Bb:4 Eb:4 Cm:4 Fm:4 G:4 Cm:4 G:4',
  MELODY='c5:.5 d#5:.5 g5:.5 c6:.5 g5:.5 d#5:.5 c5:1  c6:1.5 g#5:.5 d#5:1 g#5:1  d6:.5 a#5:.5 f5:.5 a#5:.5 d6:1 f6:1  d6:1.5 b5:.5 g5:2 '
         'g5:.5 c6:.5 d#6:1 d6:.5 c6:.5 g5:1  g#5:1 c6:.5 d#6:.5 g#6:2  f6:1 c6:.5 g#5:.5 f5:1 g#5:1  g5:.5 b5:.5 d6:.5 f6:.5 g6:2 '
         'g#5:.5 c6:.5 d#6:1 c6:.5 g#5:.5 d#5:1  a#5:.5 d6:.5 f6:1 d6:.5 a#5:.5 f5:1  g5:1 a#5:1 d#6:1.5 d6:.5  c6:1 g5:.5 d#5:.5 c5:2 '
         'f5:.5 g#5:.5 c6:1 g#5:.5 c6:.5 f6:1  g6:1.5 f6:.25 d6:.25 b5:2  c6:1 d#6:1 g6:1 d#6:1  d6:1.5 b5:.5 g5:2'),
dict(NAME='champion', BPM=168, LOOP=1, STYLE='battle', LEAD=dict(vib=2), LEAD_INST=('brass', .22), PAD_INST=('strings', .12), BASS_INST=('bass', .3), KIT=.5,
  CHORDS='Am:4 F:4 G:4 E:4 Am:4 F:4 Dm:4 E:4 F:4 G:4 C:4 Am:4 Dm:4 G:4 C:4 E:4',
  MELODY='a4:.25 c5:.25 e5:.5 a5:.5 e5:.5 c6:1 a5:1  c6:1.5 a5:.5 f5:1 c6:1  b5:.5 d6:.5 g6:1 d6:.5 b5:.5 g5:1  g#5:1.5 b5:.5 e6:2 '
         'e6:.5 c6:.5 a5:.5 c6:.5 e6:1 a6:1  f6:1.5 c6:.5 a5:2  d6:1 f6:.5 d6:.5 a5:1 f5:1  e5:.5 g#5:.5 b5:.5 d6:.5 e6:2 '
         'a5:.5 c6:.5 f6:1 e6:.5 c6:.5 a5:1  b5:.5 d6:.5 g6:1 f6:.5 d6:.5 b5:1  c6:1 e6:1 g6:1.5 e6:.5  a6:1.5 e6:.5 c6:2 '
         'd6:.5 f6:.5 a6:1 f6:.5 d6:.5 a5:1  g5:.5 b5:.5 d6:.5 g6:.5 d6:1 b5:1  e6:1 g6:1 c7:1.5 g6:.5  g#6:1.5 e6:.5 b5:2'),
dict(NAME='hof', BPM=92, LOOP=1, STYLE='ballad', LEAD=dict(vib=4, vol=.08), LEAD_INST=('brass', .3), PAD_INST=('strings', .16), KIT=.3,
  CHORDS='C:4 G:4 Am:4 F:4 C:4 G:4 F:4 G:4 C:4 E:4 Am:4 F:4 Dm:4 G:4 C:8',
  MELODY='e5:1 g5:1 c6:1.5 g5:.5  b5:1.5 d6:.5 g5:2  a5:1 c6:1 e6:1.5 c6:.5  f6:3 r:1 '
         'g5:1 c6:1 e6:1 g6:1  g6:1.5 f6:.5 d6:2  c6:1 f6:1 a6:1.5 g6:.5  g6:3 r:1 '
         'e6:1 c6:.5 g5:.5 c6:1 e6:1  g#6:1.5 e6:.5 b5:2  a5:1 c6:1 e6:1 a6:1  a6:1.5 g6:.5 f6:2 '
         'd6:1 f6:1 a6:1.5 f6:.5  g6:1 d6:1 b5:1 d6:1  c6:3 g5:1 c6:4'),
dict(NAME='cove', LEAD_INST=('flute', .3), PAD_INST=('strings', .1), BASS_INST=('bass', .28), KIT=.3, BPM=100, LOOP=1, STYLE='town', HARMONY='arp8', LEAD=dict(vib=4),
  CHORDS='G:4 C:4 D:4 G:4 Em:4 C:4 Am:4 D:4 G:4 C:4 D:4 Bm:4 Em:4 Am:4 D:4 G:4',
  MELODY='d5:1 g5:1 b5:1.5 a5:.5  g5:2 e5:1 c5:1  d5:1 f#5:1 a5:1.5 g5:.5  g5:3 r:1 '
         'b5:1 g5:.5 b5:.5 e6:1.5 d6:.5  c6:2 g5:1 e5:1  a5:1 c6:.5 a5:.5 e5:1 a5:1  f#5:3 r:1 '
         'b5:1.5 c6:.5 d6:1 b5:1  c6:1 e6:1 g5:2  a5:1 f#5:.5 a5:.5 d6:1 a5:1  b5:2 f#5:1 d5:1 '
         'e5:1 g5:.5 b5:.5 e6:2  c6:1 a5:.5 e5:.5 a5:1 c6:1  d6:1.5 c6:.5 a5:1 f#5:1  g5:3 r:1'),
]
