.include "vars.inc"

; chars
chr_a = $0a
chr_b = $0b
chr_c = $0c
chr_d = $0d
chr_e = $0e
chr_f = $0f
chr_g = $10
chr_h = $11
chr_i = $12
chr_j = $13
chr_k = $14
chr_l = $15
chr_m = $16
chr_n = $17
chr_o = $18
chr_p = $19
chr_q = $1a
chr_r = $1b
chr_s = $1c
chr_t = $1d
chr_u = $1e
chr_v = $1f
chr_w = $20
chr_x = $21
chr_y = $22
chr_z = $23
chr_qm = $24 ; questionmark

main:
  jsr load_palettes
  jsr clear_bg
  jsr draw_big_anus
  jsr draw_blurb
  jsr place_blood_drops

  jsr enable_rendering

  jmp forever

disable_rendering:
  lda #0
  sta $2000
  sta $2001
  rts

enable_rendering:

  lda #%00011110 ; enable sprites, enable background
  sta $2001

  lda #%10010000 ;enable NMI, sprites from Pattern 0, background from Pattern 1
  sta $2000

  rts


load_palettes:
  lda $2002 ; read PPU status to reset the latch
  lda #$3f
  sta $2006 ; write the hi byte
  lda #$00
  sta $2006 ; write the low byte
  ldx #$00
@load_palettes_loop:
  lda main_palette, x  ; load palette byte
  sta $2007       ; write to ppu
  inx             ; move to next byte
  cpx #$20
  bne @load_palettes_loop  ; if x = $20, we copied all 32 bytes, so we're done
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
  cpx #DRIP_COUNT ; only do it 8x
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

  lda #solids_0
  ldy #HEIGHT_TILES
@clear_bg_loop:
  ldx #WIDTH_TILES
  :
    sta $2007
    dex
    bne :-
  dey
  bne @clear_bg_loop

  rts

; fill number of blocks as stored in X
blank_fill:
    lda #solids_0
    :
  sta $2007
  dey
  bne :-
  rts

draw_big_anus:
  lda $2002             ; read PPU status to reset the high/low latch
  lda #$20
  sta $2006             ; write the high byte of $2000 address
  lda #$00
  sta $2006             ; write the low byte of $2000 address

  ldy #WIDTH_TILES
  jsr blank_fill
  ldy #WIDTH_TILES
  jsr blank_fill
  ldy #WIDTH_TILES
  jsr blank_fill

  ldx #0              ; start out at 0
@draw_big_anus_top_loop:
  lda big_anus_top, x
  sta $2007 ; write to PPU
  inx
  cpx #192
  bne @draw_big_anus_top_loop

  ldx #0              ; start out at 0
@draw_big_anus_bottom_loop:
  lda big_anus_bottom, x
  sta $2007 ; write to PPU
  inx
  cpx #224
  bne @draw_big_anus_bottom_loop

  rts

draw_sm_anus:
  lda $2002             ; read PPU status to reset the high/low latch
  lda #$20
  sta $2006             ; write the high byte of $2000 address
  lda #$00
  sta $2006             ; write the low byte of $2000 address

  ldy #WIDTH_TILES
  jsr blank_fill
  ldy #WIDTH_TILES
  jsr blank_fill
  ldy #WIDTH_TILES
  jsr blank_fill

  ldx #0              ; start out at 0
@draw_sm_anus_top_loop:
  lda sm_anus_top, x
  sta $2007 ; write to PPU
  inx
  cpx #192
  bne @draw_sm_anus_top_loop

  ldx #0              ; start out at 0
@draw_sm_anus_bottom_loop:
  lda sm_anus_bottom, x
  sta $2007 ; write to PPU
  inx
  cpx #224
  bne @draw_sm_anus_bottom_loop

  rts

draw_blurb:
  ldy #WIDTH_TILES
  jsr blank_fill
  ldy #WIDTH_TILES
  jsr blank_fill
  ldy #WIDTH_TILES
  jsr blank_fill
  ldy #WIDTH_TILES
  jsr blank_fill
  ldy #WIDTH_TILES
  jsr blank_fill
  ldy #WIDTH_TILES
  jsr blank_fill
  ldy #8
  jsr blank_fill

  ldx #0
@draw_blurb_loop_1:
  lda blurb_tiles_1, x
  sta $2007
  inx
  cpx #16
  bne @draw_blurb_loop_1

  ldy #16
  jsr blank_fill

  ldx #0
@draw_blurb_loop_2:
  lda blurb_tiles_2, x
  sta $2007
  inx
  cpx #16
  bne @draw_blurb_loop_2

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
  lda anus_attrs ,x
  sta $2007
  inx
  cpx #10
  bne load_attr_loop

  rts ; return from draw_anus subroutine


forever:
  jmp forever

