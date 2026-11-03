# The soundtrack, one spec per song, in SONG_* order (data/world.h). python3 tools/compose.py writes them into
# data/world.c. Every melody here is our own; the style (bright square lead with vibrato, arpeggios or an echo voice,
# a moving triangle bass, a drum kit with fills) follows the GBA games.
# CHORDS: "C:4 Am:4" (beats per chord). MELODY: "e5:1 g5:.5 r:1" (note, beats); its beats must match the chords.
SONGS = [
dict(NAME='title', LEAD_INST=('brass', .4), PAD_INST=('strings', .16), KIT=.45, BPM=96, LOOP=1, STYLE='march', LEAD=dict(vol=.08, vib=4),
  CHORDS='D:4 A:4 Bm:4 G:4 D:4 A:4 G:4 A:4 Bm:4 G:4 D:4 A:4 G:4 A:4 D:8',
  MELODY='a4:1 d5:1 f#5:1.5 a5:.5  a5:2 e5:1 c#5:1  d5:1 f#5:1 b5:1.5 a5:.5  g5:2 d5:2 '
         'f#5:1 a5:1 d6:1.5 c#6:.5  c#6:1.5 b5:.5 a5:2  b5:1 g5:.5 b5:.5 d6:1 b5:1  a5:3 r:1 '
         'b5:1.5 c#6:.5 d6:2  d6:1 c#6:.5 b5:.5 g5:2  a5:1.5 b5:.5 f#5:2  e5:1 a5:1 c#6:2 '
         'd6:1.5 c#6:.5 b5:1 d6:1  e6:1.5 d6:.5 c#6:2  d6:3 a5:1 f#5:2 r:2'),
dict(NAME='world', LEAD_INST=('brass', .26), PAD_INST=('strings', .12), KIT=.42, BPM=108, LOOP=1, STYLE='route', LEAD=dict(vib=3),
  CHORDS='D:4 Bm:4 G:4 A:4 D:4 Bm:4 Em:4 A:4 G:4 A:4 F#m:4 Bm:4 G:4 A:4 D:8',
  MELODY='a4:.5 d5:.5 f#5:1 e5:.5 d5:.5 e5:1  f#5:1.5 d5:.5 b4:2  g4:.5 b4:.5 d5:1 e5:.5 d5:.5 b4:1  c#5:1.5 e5:.5 a5:2 '
         'a4:.5 d5:.5 f#5:1 g5:.5 f#5:.5 e5:1  d5:1.5 f#5:.5 b5:2  g5:.5 f#5:.5 e5:1 b4:.5 e5:.5 g5:1  a5:3 r:1 '
         'b5:1 a5:.5 g5:.5 d5:1 b4:1  c#5:1 e5:.5 a5:.5 a5:1 e5:1  f#5:1 a5:.5 f#5:.5 c#5:1 a4:1  b4:.5 c#5:.5 d5:1 f#5:2 '
         'g5:1 f#5:.5 e5:.5 d5:1 b4:1  c#5:.5 d5:.5 e5:1 a5:1 g5:1  f#5:2 e5:.5 d5:.5 c#5:1 d5:2 r:1 a4:1'),
dict(NAME='battle', PAD_INST=('strings', .11), BASS_INST=('bass', .3), KIT=.5, BPM=156, LOOP=1, STYLE='battle', LEAD=dict(vib=2, decay=0),
  CHORDS='Em:4 Em:4 C:4 D:4 Em:4 Em:4 C:4 B:4 Am:4 Em:4 Am:4 B:4 C:4 D:4 Em:4 B:4',
  MELODY='e5:.5 b4:.5 e5:.5 g5:.5 f#5:.5 e5:.5 d5:.5 e5:.5  b5:1.5 a5:.25 g5:.25 g5:1 d5:1 '
         'e5:.5 c5:.5 e5:.5 g5:.5 c6:1 b5:.5 g5:.5  a5:1.5 f#5:.5 d5:1 a4:1 '
         'e5:.5 b4:.5 e5:.5 g5:.5 b5:.5 a5:.5 g5:.5 b5:.5  e6:2 d6:.5 b5:.5 g5:1 '
         'c6:1 b5:.5 a5:.5 g5:1 e5:1  d#5:.5 f#5:.5 b5:.5 d#6:.5 f#6:2 '
         'e6:1.5 c6:.5 a5:1 e5:1  g5:1 b5:.5 g5:.5 e5:2  a5:.5 c6:.5 e6:.5 c6:.5 a5:1 b5:1  f#5:1 b5:.5 a5:.5 f#5:1 d#5:1 '
         'e5:.5 g5:.5 c6:1 b5:.5 a5:.5 g5:1  f#5:.5 a5:.5 d6:1 c6:.5 a5:.5 f#5:1  g5:1 e5:.5 g5:.5 b5:2  a5:.5 g5:.5 f#5:.5 d#5:.5 b4:2'),
dict(NAME='win', LEAD_INST=('brass', .4), PAD_INST=('strings', .16), BPM=150, LOOP=0, STYLE='fanfare', LEAD=dict(vol=.1, vib=3),
  CHORDS='C:4 F:2 G:2 C:4 G:2 C:2',
  MELODY='c5:.5 e5:.5 g5:.5 c6:1.5 g5:.5 c6:.5  a5:1 c6:1 b5:1 d6:1  e6:.5 d6:.5 c6:.5 g5:.5 e5:1 g5:1  d6:1 b5:1 c6:2'),
dict(NAME='harbor', LEAD_INST=('flute', .32), BASS_INST=('bass', .3), KIT=.38, BPM=120, LOOP=1, STYLE='town', HARMONY='echo', LEAD=dict(vib=3),
  CHORDS='A:4 F#m:4 D:4 E:4 A:4 C#m:4 D:4 E:4 F#m:4 D:4 A:4 E:4 D:4 E:4 A:8',
  MELODY='e5:.75 c#5:.25 e5:.5 a5:.5 g#5:.5 a5:.5 b5:1  c#6:1.5 a5:.5 f#5:2  d5:.75 f#5:.25 a5:.5 d6:.5 c#6:.5 b5:.5 a5:1  g#5:1.5 e5:.5 b4:2 '
         'e5:.75 c#5:.25 e5:.5 a5:.5 b5:.5 c#6:.5 e6:1  e6:.5 d#6:.5 c#6:1 g#5:2  f#6:.5 e6:.5 d6:.5 c#6:.5 a5:1 a5:1  b5:3 r:1 '
         'a5:1 c#6:.5 a5:.5 f#5:1 c#5:1  d5:.5 f#5:.5 a5:1 d6:1.5 c#6:.5  c#6:1 b5:.5 a5:.5 e5:1 c#5:1  b4:.5 e5:.5 g#5:1 b5:2 '
         'a5:1 f#5:.5 a5:.5 d6:1 c#6:.5 b5:.5  b5:1 g#5:.5 b5:.5 e6:2  c#6:2 b5:.5 a5:.5 e5:1 a5:3 r:1'),
dict(NAME='cave', COLOR=1, LEAD_INST=('flute', .28), PAD_INST=('strings', .15), KIT=.3, BPM=84, LOOP=1, STYLE='cave', LEAD=dict(vol=.07, vib=4, decay=0), HARM_OCT=3, HARM_VOL=.035,
  CHORDS='Dm:8 Bb:8 Gm:8 A:8 Dm:8 C:8 Bb:8 A:8',
  MELODY='d5:2 f5:1 e5:1 a4:4  d5:2 f5:1 a#5:1 a5:4  g5:2 a#5:1 a5:1 g5:2 d5:2  c#5:3 e5:1 a4:4 '
         'd5:1 e5:1 f5:2 a5:2 g5:1 f5:1  e5:2 g5:2 c6:2 a#5:1 g5:1  a#5:2 a5:1 f5:1 d5:4  e5:2 c#5:2 a4:4'),
dict(NAME='trainer', LEAD_INST=('brass', .22), PAD_INST=('strings', .11), BASS_INST=('bass', .3), KIT=.5, BPM=160, LOOP=1, STYLE='battle', LEAD=dict(vib=2),
  CHORDS='Am:4 F:4 G:4 E:4 Am:4 F:4 Dm:4 E:4 F:4 G:4 Em:4 Am:4 Dm:4 E:4 F:4 E:4',
  MELODY='a4:.25 c5:.25 e5:.5 a5:.5 e5:.5 g5:.5 a5:.5 c6:1  a5:1.5 f5:.5 c5:1 f5:1  g5:.5 b5:.5 d6:.5 b5:.5 g5:1 d5:1  g#5:1.5 b5:.5 e6:2 '
         'e6:.5 d6:.5 c6:.5 a5:.5 e5:1 a5:1  c6:1.5 a5:.5 f5:1 c6:1  d6:1 c6:.5 a5:.5 f5:1 d5:1  e5:.5 g#5:.5 b5:.5 d6:.5 e6:2 '
         'f5:.5 a5:.5 c6:1 a5:.5 f5:.5 c5:1  d5:.5 g5:.5 b5:1 a5:.5 g5:.5 d5:1  e5:.5 g5:.5 b5:.5 e6:.5 e6:1 b5:1  c6:1.5 b5:.25 a5:.25 e5:2 '
         'f5:.5 a5:.5 d6:1 c6:.5 a5:.5 f5:1  g#5:.5 b5:.5 e6:1 d6:.5 b5:.5 g#5:1  a5:1 c6:1 f6:1 e6:.5 d6:.5  e6:1 b5:.5 g#5:.5 e5:2'),
dict(NAME='spotted', BASS_INST=('bass', .3), KIT=.45, BPM=150, LOOP=1, STYLE='battle', LEAD=dict(vib=0),
  CHORDS='Am:4 Am:4 F:4 G:4',
  MELODY='a5:.25 c6:.25 e6:.5 a5:.5 e6:.5 d6:.5 c6:.5 b5:.5 a5:.5  e5:.5 a5:.5 c6:.5 e6:.5 a6:2 '
         'f6:.5 e6:.5 c6:.5 a5:.5 c6:1 f6:1  g6:.5 f6:.5 d6:.5 b5:.5 g5:.5 b5:.5 d6:1'),
dict(NAME='heal', LEAD_INST=('piano', .42), PAD_INST=('strings', .12), BPM=120, LOOP=0, STYLE='fanfare', LEAD=dict(vol=.1, vib=3),
  CHORDS='C:2 G:1 C:2', MELODY='c6:.5 e6:.5 g6:.5 c6:.5 b5:.5 d6:.5 c6:2'),
dict(NAME='bike', LEAD_INST=('brass', .26), BASS_INST=('bass', .36), KIT=.45, BPM=144, LOOP=1, STYLE='route', LEAD=dict(vib=2),
  CHORDS='G:4 C:4 D:4 G:4 Em:4 C:4 A:4 D:4 G:4 C:4 D:4 Bm:4 C:4 D:4 G:8',
  MELODY='g5:.5 a5:.5 b5:1 d6:1 b5:1  c6:1 e6:.5 c6:.5 g5:2  a5:.5 b5:.5 c6:1 a5:1 f#5:1  g5:2 d5:1 g5:1 '
         'e5:.5 g5:.5 b5:1 e6:1 d6:1  c6:.5 b5:.5 a5:.5 g5:.5 e5:2  c#6:1 a5:.5 c#6:.5 e6:1 c#6:1  d6:2 d6:1 a5:1 '
         'b5:1 g5:.5 b5:.5 d6:2  e6:1 d6:.5 c6:.5 g5:2  f#5:.5 a5:.5 d6:1 d6:1 a5:1  b5:1 f#5:1 d5:2 '
         'e5:.5 g5:.5 c6:1 b5:.5 a5:.5 g5:1  a5:.5 b5:.5 c6:1 d6:2  g6:2 d6:1 b5:1 g5:3 r:1'),
dict(NAME='voltara', LEAD_INST=('piano', .32), BASS_INST=('bass', .36), KIT=.42, BPM=126, LOOP=1, STYLE='town', HARMONY='arp16', HARM_VOL=.03, LEAD=dict(vib=3),
  CHORDS='E:4 C#m:4 A:4 B:4 E:4 G#m:4 A:4 B:4 C#m:4 A:4 E:4 B:4 A:4 B:4 E:8',
  MELODY='b4:.5 e5:.5 g#5:.5 b5:.5 g#5:1 g#5:1  e5:1.5 c#5:.5 g#5:2  a5:.5 c#6:.5 e6:1 c#6:.5 b5:.5 a5:1  f#5:2 d#5:1 b4:1 '
         'g#5:.5 b5:.5 e6:1 d#6:.5 e6:.5 b5:1  g#5:1 b5:1 d#6:2  e6:1 c#6:.5 a5:.5 e5:1 a5:1  b5:3 r:1 '
         'c#6:1 e6:.5 c#6:.5 g#5:2  a5:1 c#6:.5 a5:.5 e5:2  g#5:.5 a5:.5 b5:1 e6:1 b5:1  d#6:1 c#6:.5 b5:.5 f#5:2 '
         'a5:.5 b5:.5 c#6:1 e6:1 c#6:1  b5:1 d#6:1 f#6:2  e6:2 b5:1 g#5:1 e5:3 r:1'),
dict(NAME='ash', LEAD_INST=('brass', .3), PAD_INST=('strings', .12), BASS_INST=('bass', .26), KIT=.44, BPM=100, LOOP=1, STYLE='march', LEAD=dict(duty=2, vib=2),
  CHORDS='Cm:4 Ab:4 Bb:4 G:4 Cm:4 Ab:4 Fm:4 G:4 Ab:4 Bb:4 Cm:4 Cm:4 Fm:4 G:4 Cm:4 G:4',
  MELODY='c5:.5 c5:.5 r:.5 c5:.5 d#5:1 g5:1  g#5:1.5 g5:.5 d#5:2  f5:.5 f5:.5 r:.5 f5:.5 a#5:1 d6:1  b5:2 g5:1 d5:1 '
         'c6:1 a#5:.5 g5:.5 d#5:1 c5:1  c5:.5 d#5:.5 g#5:1 g#5:1 d#5:1  f5:1 g#5:.5 f5:.5 c5:2  d5:.5 f5:.5 g5:.5 b5:.5 d6:2 '
         'd#6:1.5 c6:.5 g#5:2  d6:1.5 a#5:.5 f5:2  g5:1 d#5:.5 g5:.5 c6:2  d#6:1 d6:.5 c6:.5 g5:2 '
         'g#5:1 f5:.5 g#5:.5 c6:2  b5:1 d6:.5 b5:.5 g5:2  c6:.5 g5:.5 d#5:.5 c5:.5 g4:2  b4:.5 d5:.5 f5:.5 g5:.5 b5:2'),
dict(NAME='mirewood', LEAD_INST=('flute', .32), PAD_INST=('strings', .12), KIT=.32, BPM=92, LOOP=1, STYLE='ballad', LEAD=dict(vib=4),
  CHORDS='Am:4 G:4 F:4 G:4 Am:4 Em:4 F:4 E:4 Dm:4 Am:4 F:4 C:4 Dm:4 E:4 Am:8',
  MELODY='e5:1.5 a5:.5 c6:1 c6:1  b5:2 g5:1 d5:1  a5:1.5 g5:.5 f5:1 c5:1  d5:3 r:1 '
         'e5:1 a5:1 c6:1 e6:1  e6:1.5 b5:.5 g5:2  a5:1 c6:.5 a5:.5 f5:2  g#5:2 e5:1 r:1 '
         'f5:1 a5:1 d6:1.5 c6:.5  c6:1 b5:.5 a5:.5 e5:2  f5:.5 g5:.5 a5:1 c6:1 a5:1  g5:2 e5:1 c5:1 '
         'd5:1 f5:.5 a5:.5 d6:1 c6:1  b5:1.5 g#5:.5 e5:2  a5:3 e5:1 a4:3 r:1'),
dict(NAME='surf', LEAD_INST=('flute', .28), PAD_INST=('strings', .12), KIT=.38, BPM=112, LOOP=1, STYLE='ballad', LEAD=dict(vib=4),
  CHORDS='C:4 Am:4 F:4 G:4 C:4 Em:4 F:4 G:4 Am:4 F:4 C:4 G:4 F:4 G:4 C:8',
  MELODY='g4:1 c5:1 e5:1.5 d5:.5  c5:1 e5:1 a5:2  a5:1 g5:.5 f5:.5 c5:1 f5:1  g5:2.5 f5:.5 e5:.5 d5:.5 '
         'e5:1 g5:1 c6:1.5 b5:.5  b5:1 g5:1 e5:2  f5:1 a5:1 c6:1 a5:1  g5:3 r:1 '
         'c6:1 b5:.5 a5:.5 e5:2  f5:1 a5:.5 c6:.5 a5:2  g5:1 e5:.5 g5:.5 c6:2  b5:1 d6:1 g5:2 '
         'a5:1.5 g5:.5 f5:1 a5:1  g5:1.5 f5:.5 d5:1 b4:1  c5:2 e5:1 g5:1 c6:3 r:1'),
dict(NAME='intro', LEAD_INST=('flute', .32), PAD_INST=('strings', .15), KIT=.28, BPM=90, LOOP=1, STYLE='ballad', LEAD=dict(vib=4),
  CHORDS='C:4 Am:4 F:4 G:4 Em:4 Am:4 Dm:4 G:4 F:4 G:4 Em:4 Am:4 Dm:4 G:4 C:8',
  MELODY='e5:2 g5:1 c6:1  c6:1.5 a5:.5 e5:2  f5:1 a5:1 c6:1 a5:1  g5:3 d5:1 '
         'e5:1.5 g5:.5 b5:2  c6:1 b5:.5 a5:.5 e5:2  d5:1 f5:1 a5:1 d6:1  b5:3 r:1 '
         'a5:1.5 c6:.5 f6:2  d6:1.5 b5:.5 g5:2  e6:1 d6:.5 b5:.5 g5:2  a5:1 c6:1 e6:2 '
         'f6:1 e6:.5 d6:.5 a5:2  b5:1 d6:1 g6:2  g6:2 e6:1 d6:1 c6:3 r:1'),
dict(NAME='levelup', LEAD_INST=('brass', .36), BPM=170, LOOP=0, STYLE='fanfare', LEAD=dict(vol=.1, vib=2),
  CHORDS='C:1 G:1 C:2', MELODY='c6:.25 e6:.25 g6:.5 b5:.5 d6:.5 c6:2'),
dict(NAME='caught', LEAD_INST=('brass', .4), PAD_INST=('strings', .15), BPM=140, LOOP=0, STYLE='fanfare', LEAD=dict(vol=.1, vib=3),
  CHORDS='G:2 C:2 D:2 G:4', MELODY='d5:.5 g5:.5 b5:1 c6:.5 e6:.5 g6:1 f#6:.5 e6:.5 d6:.5 a5:.5 g6:3 r:1'),
]
