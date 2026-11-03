# Joshemon cartridge: make -> build/joshemon.gba. Needs arm-none-eabi-gcc (brew) and a host cc.
# The painter (tools/paint.c) draws every map and person from data/world.c into build/maps.c first.
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
CFLAGS = -mcpu=arm7tdmi -mthumb -mthumb-interwork -O2 -Wall -ffreestanding -fno-builtin -nostdlib -fno-strict-aliasing
LDFLAGS = -mcpu=arm7tdmi -nostdlib -T gba.ld -Wl,--gc-sections -Wl,--no-warn-rwx-segments
MGBA = /opt/homebrew/Cellar/mgba/0.10.5_2
OBJ = $(patsubst src/%.c,build/%.o,$(wildcard src/*.c)) build/maps.o build/world.o build/dex.o build/art.o build/crt0.o

build/joshemon.gba: build/joshemon.elf
	$(OBJCOPY) -O binary $< $@
	python3 tools/gbafix.py $@

build/joshemon.elf: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ -lgcc

build/%.o: src/%.c $(wildcard src/*.inc) src/gba.h src/rules.h $(wildcard data/*.h) build/maps.h build/samples.c
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: data/%.c $(wildcard data/*.h) $(wildcard data/parts/*.inc) build/songs.inc
	$(CC) $(CFLAGS) -c $< -o $@

# the baked creature, scene and title art is kept gzipped (data/art.c.gz) and unpacked here; tools/legend.py edits it in place
build/art.c: data/art.c.gz
	@mkdir -p build
	gunzip -c $< > $@

build/art.o: build/art.c $(wildcard data/*.h)
	$(CC) $(CFLAGS) -I data -c $< -o $@

build/songs.inc: $(wildcard music/songs*.py) tools/compose.py
	python3 tools/compose.py

build/maps.o: build/maps.c build/maps.h
	$(CC) $(CFLAGS) -I data -c $< -o $@

build/maps.c build/maps.h: build/paint
	build/paint build

build/paint: tools/paint.c $(wildcard tools/parts/*.inc) data/world.c $(wildcard data/parts/*.inc) data/world.h build/songs.inc
	@mkdir -p build
	cc -O2 -I data tools/paint.c data/world.c -o $@ -lm

build/samples.c: tools/samples.py
	@mkdir -p build
	python3 tools/samples.py > $@

build/crt0.o: crt0.s
	$(CC) -mcpu=arm7tdmi -c $< -o $@

build/rules_test: tools/rules_test.c src/rules.c data/dex.c $(wildcard data/parts/*.inc) data/dex.h
	cc -I data tools/rules_test.c src/rules.c data/dex.c -o $@

test: build/rules_test
	build/rules_test

build/gbarun: tools/gbarun.c
	clang -O2 -I$(MGBA)/include -L$(MGBA)/lib -lmgba -Wl,-rpath,$(MGBA)/lib $< -o $@

# QA builds: -DQA_FIELD (every badge but the newest, BIKE, rod; START+L+R+B starts at LUMEN ISLE) and -DQA_EVO
build/qa_field.gba: $(OBJ) src/main.c
	$(CC) $(CFLAGS) -DQA_FIELD -I build -c src/main.c -o build/qa_field.o
	$(CC) $(LDFLAGS) -o build/qa_field.elf build/qa_field.o $(filter-out build/main.o,$(OBJ)) -lgcc
	$(OBJCOPY) -O binary build/qa_field.elf $@
	python3 tools/gbafix.py $@

build/qa_juke.gba: $(OBJ) src/main.c
	$(CC) $(CFLAGS) -DQA_JUKE -I build -c src/main.c -o build/qa_juke.o
	$(CC) $(LDFLAGS) -o build/qa_juke.elf build/qa_juke.o $(filter-out build/main.o,$(OBJ)) -lgcc
	$(OBJCOPY) -O binary build/qa_juke.elf $@

# -DQA_REAL on top: six creatures at 54 to 56 in the LEAGUE lobby, real damage and PP, to check the league is fair
build/qa_real.gba: $(OBJ) src/main.c
	$(CC) $(CFLAGS) -DQA_FIELD -DQA_REAL -I build -c src/main.c -o build/qa_real.o
	$(CC) $(LDFLAGS) -o build/qa_real.elf build/qa_real.o $(filter-out build/main.o,$(OBJ)) -lgcc
	$(OBJCOPY) -O binary build/qa_real.elf $@

# -DQA_DIPLOMA on top: a CHAMPION with all 105 caught, to see PROF. CEDAR's DIPLOMA
build/qa_dip.gba: $(OBJ) src/main.c
	$(CC) $(CFLAGS) -DQA_FIELD -DQA_DIPLOMA -I build -c src/main.c -o build/qa_dip.o
	$(CC) $(LDFLAGS) -o build/qa_dip.elf build/qa_dip.o $(filter-out build/main.o,$(OBJ)) -lgcc
	$(OBJCOPY) -O binary build/qa_dip.elf $@
qa: build/joshemon.gba build/qa_field.gba build/gbarun

clean:
	rm -f build/*.o build/*.elf build/*.gba build/maps.c build/maps.h build/paint build/rules_test
