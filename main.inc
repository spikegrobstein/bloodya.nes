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

  lda #$10
  sta $0200; put sprite 0 in center of screen vertically
  lda #$80
  sta $0203 ; put sprite 0 in center of screen horizontally
  lda #$00
  sta $0201; tile number = 0
  sta $0202; color = 0, no flip

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

