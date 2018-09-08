bloodya.nes: bloodya.o bloodya.cfg src/chr1.chr
	ld65 -o $@ -C bloodya.cfg bloodya.o -m bloodya.map.txt -Ln bloodya.labels.txt --dbgfile bloodya.nes.dbg

bloodya.o: src/bloodya.s src/main.inc src/anus.inc src/drip.inc src/splash.inc src/chr1.chr
	ca65 src/bloodya.s -g -o bloodya.o

src/chr1.chr: chr1.png
	png2chr --outdir src chr1.png

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
		src/chr1.chr

.PHONY:
