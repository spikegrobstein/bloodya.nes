srcdir=src
imgdir=img

varfile=$(srcdir)/vars.inc

sourcefiles= \
	$(srcdir)/bloodya.s \
	$(srcdir)/beep.inc \
	$(srcdir)/main.inc \
	$(srcdir)/controller.inc \
	$(srcdir)/anus.inc \
	$(srcdir)/drip.inc \
	$(srcdir)/ascii.inc \
	$(srcdir)/splash.inc \
	$(srcdir)/data/palette.inc \
	$(srcdir)/data/anus.inc \
	$(srcdir)/data/drip.inc \
	$(srcdir)/data/splash.inc \
	$(varfile)

background_images= \
	$(imgdir)/char.png \
	$(imgdir)/solids.png \
	$(imgdir)/big_anus.png \
	$(imgdir)/sm_anus.png \
	$(imgdir)/splash.png

chrfiles= \
	$(srcdir)/sprites.chr \
	$(srcdir)/background.chr


bloodya.nes: bloodya.o bloodya.cfg
	ld65 -o $@ -C bloodya.cfg bloodya.o -m bloodya.map.txt -Ln bloodya.labels.txt --dbgfile bloodya.nes.dbg

bloodya.o: $(sourcefiles) $(chrfiles)
	ca65 $(srcdir)/bloodya.s -g -o bloodya.o

$(varfile): $(imgdir)/background.png

$(imgdir)/background.png: $(background_images)
	tilec --varfile $(varfile) --outfile $@ $^

$(srcdir)/sprites.chr: $(imgdir)/sprites.png
	png2chr --size 256 --outdir $(srcdir) $^

$(srcdir)/background.chr: $(imgdir)/background.png
	png2chr --size 256 --outdir $(srcdir) $^

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
		$(chrfiles) \
		$(imgdir)/background.png

.PHONY:
