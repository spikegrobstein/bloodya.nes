scroll_big_anus:
  lda PPUSTATUS
  lda #0 ; scroll back to the big anus screen
  sta PPUSCROLL
  sta PPUSCROLL

  rts

scroll_sm_anus:
  lda PPUSTATUS
  lda #248 ; scroll to 256 - 8 across (one complete screen - one tile)
  sta PPUSCROLL
  lda #0
  sta PPUSCROLL

  rts

draw_big_anus:
  lda PPUSTATUS             ; read PPU status to reset the high/low latch
  lda #$20
  sta PPUADDR             ; write the high byte of $2000 address
  lda #$a0
  sta PPUADDR             ; write the low byte of $2000 address

  ldx #0              ; start out at 0
  @draw_big_anus_top_loop:
    lda big_anus_top, x
    sta PPUDATA ; write to PPU
    inx
    cpx #192
    bne @draw_big_anus_top_loop

  ldx #0              ; start out at 0
  @draw_big_anus_bottom_loop:
    lda big_anus_bottom, x
    sta PPUDATA ; write to PPU
    inx
    cpx #224
    bne @draw_big_anus_bottom_loop

  lda PPUSTATUS   ; read PPU status to reset the high/low latch
  lda #$22
  sta PPUADDR   ; write the high byte of $2000 address
  lda #$e0    ; skip 80 tiles ahead
  sta PPUADDR   ; write the low byte of $2000 address

  jsr draw_blurb

  rts

draw_sm_anus:
  lda PPUSTATUS   ; read PPU status to reset the high/low latch
  lda #$24
  sta PPUADDR   ; write the high byte of $2000 address
  lda #$00    ; skip $60 tiles (3 rows) ahead
  sta PPUADDR   ; write the low byte of $2000 address

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

  ; skip one more line
  ldy #WIDTH_TILES
  jsr blank_fill

  ldx #0              ; start out at 0
@draw_sm_anus_top_loop:
  lda sm_anus_top, x
  sta PPUDATA ; write to PPU
  inx
  cpx #192
  bne @draw_sm_anus_top_loop

  ldx #0              ; start out at 0
@draw_sm_anus_bottom_loop:
  lda sm_anus_bottom, x
  sta PPUDATA ; write to PPU
  inx
  cpx #160
  bne @draw_sm_anus_bottom_loop

  ; skip one more line
  ldy #WIDTH_TILES
  jsr blank_fill

  jsr draw_gap
  jsr draw_sm_blurb

  rts

draw_gap:
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

  rts

draw_sm_blurb:
  ldy #7
  jsr blank_fill

  ldx #0
@draw_sm_blurb_loop_1:
  lda blurb_tiles_1, x
  sta PPUDATA
  inx
  cpx #16
  bne @draw_sm_blurb_loop_1

  ldy #16
  jsr blank_fill

  ldx #0
@draw_sm_blurb_loop_2:
  lda blurb_tiles_2, x
  sta PPUDATA
  inx
  cpx #16
  bne @draw_sm_blurb_loop_2

  ldy #7
  jsr blank_fill

  ldy #WIDTH_TILES
  jsr blank_fill
  ldy #WIDTH_TILES
  jsr blank_fill
  ldy #WIDTH_TILES
  jsr blank_fill

  rts

draw_blurb:
  ldy #8
  jsr blank_fill

  ldx #0
@draw_blurb_loop_1:
  lda blurb_tiles_1, x
  sta PPUDATA
  inx
  cpx #16
  bne @draw_blurb_loop_1

  ldy #16
  jsr blank_fill

  ldx #0
@draw_blurb_loop_2:
  lda blurb_tiles_2, x
  sta PPUDATA
  inx
  cpx #16
  bne @draw_blurb_loop_2

  ldy #8
  jsr blank_fill

  ldy #WIDTH_TILES
  jsr blank_fill
  ldy #WIDTH_TILES
  jsr blank_fill


  ldy #WIDTH_TILES
  jsr blank_fill
  ldy #WIDTH_TILES
  jsr blank_fill

  rts
