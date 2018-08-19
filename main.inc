; solid colors
bgs_0 = $29
bgs_1 = $2a
bgs_2 = $2b
bgs_3 = $2c

; pieces of asterisk
bg_01 = $27
bg_02 = $28
bg_03 = $37
bg_04 = $43
bg_05 = $44
bg_06 = $45
bg_07 = $47
bg_08 = $53
bg_09 = $55
bg_10 = $56
bg_11 = $58
bg_12 = $63
bg_13 = $66
bg_14 = $68
bg_15 = $68
bg_16 = $73
bg_17 = $74
bg_18 = $77
bg_19 = $78
bg_20 = $85
bg_21 = $86
bg_22 = $87

main:
  jsr load_palettes
  jsr clear_bg
  ; jsr clear_attr
  jsr draw_asterisk
  jsr place_blood_drops

  lda #%00011110 ; enable sprites, enable background
  sta $2001

  lda #%10010000 ;enable NMI, sprites from Pattern 0, background from Pattern 1
  sta $2000

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

place_blood_drops:
  ; first we initialize things
  lda #$00
  sta last_drop_appeared

  lda #$00
  sta $2003  ; set the low byte (00) of the RAM address
  lda #$02
  sta $4014  ; set the high byte (02) of the RAM address, start the transfer

  ldx #0
  ldy #0
@place_drip_loop:
  lda drip_positions, x ; start with the initial drip position
  cpy #0 ; if y is not zero, we want to write a to the spot
         ; we do this because we want to place these drips off-screen, initially
  bne @copy_drip_position

  lda #$ef
  sta $0200, x ; sprite should be off-screen
  jmp @incrementors

@copy_drip_position:
  sta $0200, x ; store it in the sprite memory

@incrementors:  ; increment x and y
  inx
  iny

  cpy #04 ; reset y if it hits 4 since we want to have it go 0,1,2,3,0,1,2,3
  bne :+

  ldy #0

:
  cpx #$20 ; we're loading 32 ($20) bytes
  bne @place_drip_loop

  ; now let's initialize the starting timing and velocities
  ldx #0 
@create_drip_loop:
  lda #1
  sta drip_velocity, x ; first set the starting velocity to 1
  lda drip_starting_timing, x ; then initialize all of the timings
  sta last_drop_appeared, x
  inx
  cpx #$08 ; only do it 8x
  bne @create_drip_loop

  ; set velocity to 1
  lda #1
  sta drip_velocity

  lda #%00010000 ; enable sprites
  sta $2001
  rts

clear_bg:
  lda $2002             ; read PPU status to reset the high/low latch
  lda #$20
  sta $2006             ; write the high byte of $2000 address
  lda #$00
  sta $2006             ; write the low byte of $2000 address

  lda #bgs_0
  ldy #30
@clear_bg_loop:
  ldx #32
  :
    sta $2007
    dex
    bne :-
  dey
  bne @clear_bg_loop

clear_attr:
  ldx #64
  lda #%00000000
@clear_attr_loop:
  sta $2007 
  dex
  bne @clear_attr_loop

  rts

; fill number of blocks as stored in X
blank_fill:
  lda #bgs_0
  :
  sta $2007
  dey
  bne :-
  rts

draw_asterisk:
  lda $2002             ; read PPU status to reset the high/low latch
  lda #$20
  sta $2006             ; write the high byte of $2000 address
  lda #$00
  sta $2006             ; write the low byte of $2000 address

  ldy #32
  jsr blank_fill
  ldy #32
  jsr blank_fill
  ldy #32
  jsr blank_fill

  ldx #0              ; start out at 0
@draw_asterisk_loop:
  lda asterisk_tiles, x
  sta $2007 ; write to PPU
  inx
  cpx #84
  bne @draw_asterisk_loop

  rts

load_attr:
  lda $2002             ; read PPU status to reset the high/low latch
  lda #$23
  sta $2006             ; write the high byte of $23C0 address
  lda #$C0
  sta $2006             ; write the low byte of $23C0 address

  ldx #$00
:
  lda #$00
  sta $2007
  inx
  cpx #20
  bne :-

  ldx #$00              ; start out at 0
load_attr_loop:
  lda asterisk_attrs ,x
  sta $2007
  inx
  cpx #10
  bne load_attr_loop

  rts ; return from draw_asterisk subroutine


forever:
  jmp forever

