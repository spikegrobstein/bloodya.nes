# Bloodya.nes

> Pronounced "Bloody Anus"

A port of [Bloodyan.us](https://bloodyan.us) to the Nintendo Entertainment System using 6502 Assembly.

The built ROM can be downloaded from [here](https://bloodyan.us/bloodya.nes) and there's a live demo available using an in-browser NES
emulator [here](https://bloodyan.us/nes.html).

## What? Why?

Back in 2011, I bought the domain `bloodyan.us` as a joke and in 2012, thought of something to put up there.
I was just getting into JavaScript animation and took a stab at an animation with a large asterisk with bloody
dots falling out of it and fading out, which I completed in an evening. This site sat there, without any major
update for about 6 years.

Fast forward to 2018. I've always been super into the NES and have hacked on graphics in the past. I was
inspired to rewrite an old tool I wrote back in 2003 for extracting/replacing graphics in ROMs, and while
working on that, saw a [video](https://www.youtube.com/watch?v=kXbMCKMJXXQ&) on YouTube that inspired me to
try out NES programming for myself and after a few hours of reading, I was animating my first sprites. Trying
to think of a simple project to work on, I remembered `bloodyan.us` and came up with a new pun: `bloodya.nes`.
I just *had* to build this.

After about a month of work, I managed to get not only the base animation functioning very similarly to the
original source material, but also designed a splash screen and learned a TON about NES development along the
way, including dealing with scrolling, vblanking, converting PNG files into CHRs, GIMP, and all of the good
stuff that's included in the development of an NES game. The level of respect I have for developers of NES
games in the 80s and early 90s is immeasurable.

At this point, I consider this ROM pretty much done and I'm ready to move on to a new project.

## Playing

This is a one-button, stamina-based game. The goal is to clench the anus by pressing A and keep it clenched
for as long as possible. As the anus is clenched, you will gain points and your score will count up. Release
and blood will spill out, subtracting a point from your score for each drop of blood that spills.

How long can you clench your anus for?

## Building

Building the ROM has the following prerequisites:

 * [nesromtool2](https://github.com/sadistech/nesromtool2)
 * cc65 (can generally be installed with homebrew or your OS's package manager)

Once those are installed and the tools are available in your PATH, run the following to build the rom:

    make

If the above executes successfully, it will output a `bloodya.nes` ROM file that can be loaded into your
favourite emulator.

## Acknowledgements

Special thanks to:

 * [nesdev wiki](http://forums.nesdev.com/)
 * [nesdev forums](http://wiki.nesdev.com/)
 * [NintendoAge Nerdy Nights posts](http://nintendoage.com/pub/faq/NA/index.html?load=nerdy_nights_out.html)
 * [FCEUX/FCEUXD SP](http://www.fceux.com/web/version.html)
 * [jsnes](https://github.com/bfirsh/jsnes)

Without the resources provided by the above sites, none of this would have been possible.

## Disclaimer

`bloodya.nes` is in no way affiliated, endorsed or licensed by Nintendo.

