sourcefiles= \
	src/bloodya.s \
	src/beep.inc \
	src/main.inc \
	src/controller.inc \
	src/anus.inc \
	src/drip.inc \
	src/ascii.inc \
	src/splash.inc \
	src/vars.inc \
	src/data/palette.inc \
	src/data/anus.inc \
	src/data/drip.inc \
	src/data/splash.inc

bloodya.nes: bloodya.o bloodya.cfg
	ld65 -o $@ -C bloodya.cfg bloodya.o -m bloodya.map.txt -Ln bloodya.labels.txt --dbgfile bloodya.nes.dbg

bloodya.o: $(sourcefiles) src/sprites.chr src/background.chr
	ca65 src/bloodya.s -g -o bloodya.o

img/background.png: img/char.png img/solids.png img/big_anus.png img/sm_anus.png img/splash.png
	tilec --varfile src/vars.inc --outfile $@ $^

src/sprites.chr: img/sprites.png
	png2chr --size 256 --outdir src $^

src/background.chr: img/background.png
	png2chr --size 256 --outdir src $^

run: bloodya.nes
	fceux $^

clean:
	rm -rf \
		bloodya.o \
		bloodya.nes \
		bloodya.map.txt \
		bloodya.labels.txt \
		bloodya.nes.ram.nl \
		bloodya.nes.0.nl \
		bloodya.nes.1.nl \
		bloodya.nes.dbg \
		src/background.chr \
		src/sprites.chr \
		img/background.png

.PHONY:
