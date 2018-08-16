bloodya.nes: bloodya.o bloodya.cfg chr1.chr
	ld65 -o $@ -C bloodya.cfg bloodya.o -m bloodya.map.txt -Ln bloodya.labels.ttx --dbgfile bloodya.nes.dbg

bloodya.o: bloodya.chr bloodya.s main.inc
	ca65 bloodya.s -g -o bloodya.o

chr1.chr: chr1.png
	png2chr chr1.png

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
		bloodya.nes.dbg

.PHONY:
