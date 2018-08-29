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
  ; initialization
  jsr load_palettes
  jsr clear_bg
  jsr draw_big_anus
  jsr draw_sm_anus

  jsr place_blood_drops

  jsr scroll_big_anus

  jsr init_score
  jsr render_starting_score

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
  jsr clear_bg_a
  jsr clear_bg_b

clear_bg_a:
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

clear_bg_b:
  lda $2002             ; read PPU status to reset the high/low latch
  lda #$24
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
  lda #$a0
  sta $2006             ; write the low byte of $2000 address

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

  lda $2002   ; read PPU status to reset the high/low latch
  lda #$22
  sta $2006   ; write the high byte of $2000 address
  lda #$e0    ; skip 80 tiles ahead
  sta $2006   ; write the low byte of $2000 address

  jsr draw_blurb

  rts

draw_sm_anus:
  lda $2002   ; read PPU status to reset the high/low latch
  lda #$24
  sta $2006   ; write the high byte of $2000 address
  lda #$00    ; skip $60 tiles (3 rows) ahead
  sta $2006   ; write the low byte of $2000 address

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
  sta $2007 ; write to PPU
  inx
  cpx #192
  bne @draw_sm_anus_top_loop

  ldx #0              ; start out at 0
@draw_sm_anus_bottom_loop:
  lda sm_anus_bottom, x
  sta $2007 ; write to PPU
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
  sta $2007
  inx
  cpx #16
  bne @draw_sm_blurb_loop_1

  ldy #16
  jsr blank_fill

  ldx #0
@draw_sm_blurb_loop_2:
  lda blurb_tiles_2, x
  sta $2007
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

animate_frame:
  ; we want to iterate over each drop's last_drop_appeared value
  ; if it's zero, we want to animate that drop.
  ; if it's non-zero, we want to decrement it til it hits zero
  ; if it hits zero after decrementing, move it to default position and animate
  ; when it's off-screen, we want to re-set its last_appeared value to default.

  ldx #$00
  ldy #$00
drip_loop:
  lda last_drop_appeared, x
  cmp #$00 ; check if it's zero. if so, it's gonna be animated
  beq @animate_drop

  ; we only want to respawn drops if we're not clenched
  lda controller_1
  bne @next_drop

  dec last_drop_appeared, x ; decrement the value

  lda last_drop_appeared, x ; load it so we can check it
  cmp #$00 ; check if it's zero
  bne @next_drop

  ; the thing hit zero, so we need to initialize it before we start animating
  lda drip_positions, y
  sta $0200, y
  lda #0
  sta $0201, y

@animate_drop:
  lda $0200, y
  clc
  adc drip_velocity, x
  sta $0200, y

  lda drip_velocity, x
  cmp #MAX_VELOCITY ; if it's at max velocity stop incrementing
  beq :+ ; skip incrementing
  inc drip_velocity, x ;increase drip velocity

:
  ; if drip is at the end of it's fall
  lda $0200, y ; read in the y coordinate
  clc
  sbc #DRIP_END ; check if it's off screen
  bcc @update_drop_colors; carry flag not set, so it's not off-screen

  ; it is at the end
  lda #$01 
  sta drip_velocity, x  ; reset the drip velocity to 1
  lda #$20
  sta last_drop_appeared, x ; reset the drip timer
  lda #OFFSCREEN
  sta $0200, y   ; move the drip off-screen

@update_drop_colors:
  ; update the drop colors basd on their y coordinate
  ; this is to simulate the fading as they fall
  lda $0200, y ; get the y coordinate of the drop
  clc
  sbc #DRIP_CHANGE_1     ; check if we're past this point.
  bcc :+       ; if so, use the next color drop 

  lda #1 ; load the sprite index for this drop
  sta $0201, y ; store it back

:
  lda $0200, y ; get the y coordinate of the drop
  clc
  sbc #DRIP_CHANGE_2     ; check if we're past this point
  bcc :+       ; if so, update teh color of the drop again

  lda #2
  sta $0201, y

:
@next_drop:
  inx
  iny
  iny
  iny
  iny

  cpx #DRIP_COUNT ; check if we're at the 8th sprite
  beq @end_animate_frame  ; end our thing if so.

  jmp drip_loop

@end_animate_frame:
  rts

read_controller:
  ; latch controller
  lda #$01
  sta $4016
  lda #$00
  sta $4016     ; TELL BOTH THE CONTROLLERS TO LATCH BUTTONS

  lda $4016     ; player 1 - A
  AND #%00000001

  sta controller_1
  
  rts

scroll_big_anus:
  lda #0
  sta did_clench ; turn off clench, always

  lda $2002
  lda #0 ; scroll back to the big anus screen
  sta $2005
  sta $2005

  rts

scroll_sm_anus:
  lda did_clench ; short-circuit if we're clenched
  bne @end

  lda #1 ; mark clenched
  sta did_clench
  
  lda $2002
  lda #248 ; scroll to 256 across (one complete screen)
  sta $2005
  lda #0
  sta $2005

  jsr increment_score

  @end:
  rts

init_score:
  lda #0
  sta score

  ; fill the rest of the score with $FF so we don't render it
  ldx #1
  lda #$ff
  @init_score_loop:
    sta score, x
    inx
    cpx #SCORE_SIZE
    bne @init_score_loop

  rts

;
; The score is basically just a byte array representing the decimal value
; of each digit of the score.
; when incrementing we want to increment the right-most byte and then
; if the carry flag got set, we increment the next one. repeat.
;
increment_score:
  ldx #0
  @increment_score_loop:
    cpx #SCORE_SIZE
    beq @end

    lda score, x ; read the current value

    ; since $ff is a placeholder,
    ; if we're on it, we need to bump to zero so we increment correctly
    cmp #$ff 
    bne :+
    lda #0

    :
    clc
    adc #1 ; increment
    sta score, x

    cmp #10 ; did we hit 10 and need to carry
    bne @end ; stop if we don't need to carry

    ; roll it over to zero before continuing.
    lda #0
    sta score, x

    inx

    bcs @increment_score_loop
  
  @end:
  
  rts

render_starting_score:
  lda $2002             ; read PPU status to reset the high/low latch
  lda #$20
  sta $2006             ; write the high byte of $2000 address
  lda #$40+29
  sta $2006             ; write the low byte of $2000 address

  lda #0
  sta $2007

  rts


render_score:
  jsr render_big_score  
  ; jsr render_sm_score
  rts


render_big_score:
  lda $2002             ; read PPU status to reset the high/low latch
  lda #$20
  sta $2006             ; write the high byte of $2000 address
  lda #$40+20
  sta $2006             ; write the low byte of $2000 address

  ldx #SCORE_SIZE-1
  @render_score_loop:
    cpx #$ff
    beq @end

    lda score, x ; read teh score digit

    cmp #$ff ; $FF tiles are empty
    bne :+   ; if it's not blank, go right to writing the value

    lda #solids_0 ; override the value with an empty tile

    :
      sta $2007 ; write the value to PPU

      dex
      jmp @render_score_loop


  @end:
  rts

forever:
  jsr read_controller

  lda nmi_latch
  cmp #0
  beq forever

  jsr animate_frame

  ; reset the latch back to zero
  lda #0
  sta nmi_latch
  
  jmp forever

