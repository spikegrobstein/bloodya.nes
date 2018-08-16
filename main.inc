main:
  jsr load_palettes
  jsr place_blood_drop
  jmp forever

load_palettes:
  lda $2002 ; read PPU status to reset the latch
  lda #$3f
  sta $2006 ; write the hi byte
  lda #$00
  sta $2006 ; write the low byte
  ldx #$00
load_palettes_loop:
  lda main_palette, x  ; load palette byte
  sta $2007       ; write to ppu
  inx             ; move to next byte
  cpx #$20
  bne load_palettes_loop  ; if x = $20, we copied all 32 bytes, so we're done
  rts

place_blood_drop:
  ; first we initialize things
  lda #$00
  sta last_drop_appeared

  lda #$00
  sta $2003  ; set the low byte (00) of the RAM address
  lda #$02
  sta $4014  ; set the high byte (02) of the RAM address, start the transfer

  ldx #0
:
  lda drip_positions, x ; start with the initial drip position
  sta $0200, x ; store it in the sprite memory
  inx
  cpx #$20 ; we're loading 32 ($20) bytes
  bne :-

  ; now let's initialize the starting timing and velocities
  ldx #0 
:
  lda #1
  sta drip_velocity, x ; first set the starting velocity to 1
  lda drip_starting_timing, x ; then initialize all of the timings
  sta last_drop_appeared, x
  inx
  cpx #$08 ; only do it 8x
  bne :-

  ; set velocity to 1
  lda #1
  sta drip_velocity

  lda #%10000000 ; enable nmi, sprites from pattern table 0
  sta $2000

  lda #%00010000 ; enable sprites
  sta $2001
  rts

forever:
  jmp forever

