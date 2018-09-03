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

.include "anus.inc"
.include "drip.inc"

main:
  inc nmi_lock ; prevent NMI entry, which will update PPU

  ; initialization
  jsr load_palettes
  jsr clear_bg
  jsr draw_big_anus
  jsr draw_sm_anus

  jsr place_blood_drops
  jsr init_score
  jsr render_starting_score
  jsr scroll_big_anus

  jsr enable_rendering

  dec nmi_lock ; allow NMI entry again

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


clear_bg:
  jsr @clear_bg_a
  jsr @clear_bg_b

@clear_bg_a:
  lda $2002             ; read PPU status to reset the high/low latch
  lda #$20
  sta $2006             ; write the high byte of $2000 address
  lda #$00
  sta $2006             ; write the low byte of $2000 address

  lda #solids_0
  ldy #HEIGHT_TILES
@clear_bg_a_loop:
  ldx #WIDTH_TILES
  :
    sta $2007
    dex
    bne :-
  dey
  bne @clear_bg_a_loop

  rts

@clear_bg_b:
  lda $2002             ; read PPU status to reset the high/low latch
  lda #$24
  sta $2006             ; write the high byte of $2000 address
  lda #$00
  sta $2006             ; write the low byte of $2000 address

  lda #solids_0
  ldy #HEIGHT_TILES
@clear_bg_b_loop:
  ldx #WIDTH_TILES
  :
    sta $2007
    dex
    bne :-
  dey
  bne @clear_bg_b_loop

  rts

; fill number of blocks as stored in X
blank_fill:
    lda #solids_0
    :
  sta $2007
  dey
  bne :-
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
  lda PPUSTATUS            ; read PPU status to reset the high/low latch
  lda #$20
  sta PPUADDR             ; write the high byte of $2000 address
  lda #$40+10
  sta PPUADDR             ; write the low byte of $2000 address

  lda #chr_s
  sta PPUDATA
  lda #chr_c
  sta PPUDATA
  lda #chr_o
  sta PPUDATA
  lda #chr_r
  sta PPUDATA
  lda #chr_e
  sta PPUDATA

  ; then fall through to rendering the score itself.

render_score:
  lda PPUSTATUS             ; read PPU status to reset the high/low latch
  lda #$20
  sta PPUADDR             ; write the high byte of $2000 address
  lda #$40+20
  sta PPUADDR             ; write the low byte of $2000 address

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

  ; wait for nmi to complete a cycle
  lda nmi_latch
  cmp #0
  beq forever

  ; scroll the big anus first
  ; we need t do this to ensure that the header isn't scrolled
  jsr scroll_big_anus

  ; handle split scroll
  @sprite_0_wait_clear:
    bit $2002
    bvs @sprite_0_wait_clear
  @sprite_0_wait:
    bit $2002
    bvc @sprite_0_wait

  ; if controller_1 has A pressed, then clench
  ; otherwise, don't clench
  lda controller_1
  beq @not_clenched

  @is_clenched:
    jsr scroll_sm_anus
    lda did_clench ; short-circuit if we're clenched
    bne :+

    lda #1 ; mark clenched
    sta did_clench
    
    jsr increment_score

    jmp :+

  @not_clenched: ; relax...
    lda #0
    sta did_clench ; turn off clench, always

    jsr scroll_big_anus
  
  :
  jsr animate_frame

  ; reset the latch back to zero
  lda #0
  sta nmi_latch
  
  jmp forever

