.include "vars.inc"
.include "ascii.inc"
.include "anus.inc"
.include "drip.inc"
.include "splash.inc"
.include "controller.inc"

init:
  jsr init_splash

init_main:
  inc nmi_lock ; prevent NMI entry, which will update PPU
  jsr disable_rendering

  jsr init_score

  ; initialization
  jsr load_palettes

  ; blank the background and draw the 2 main screens
  ; this touches the PPU background tiles
  jsr clear_bg
  jsr clear_attr
  jsr render_score_text
  jsr draw_big_anus
  jsr draw_sm_anus

  ; initialize the drops
  ; this only touches sprites
  jsr place_blood_drops

  ; switch mode to game
  lda #ON_MAIN
  sta game_state

  ; finish up
  jsr enable_rendering
  dec nmi_lock ; allow NMI entry again

  ; fall through to main loop
  ;
main_loop:
  jsr read_controller

  ; wait for nmi to complete a cycle
  ; the NMI will increment nmi_latch when it completes
  ; so don't do anything until we've rendered a frame
  lda nmi_latch
  cmp #0
  beq main_loop

  ; scroll the big anus first
  ; we need t do this to ensure that the header isn't scrolled
  jsr scroll_big_anus

  ; handle split scroll
  @sprite_0_wait_clear:
    bit PPUSTATUS
    bvs @sprite_0_wait_clear
  @sprite_0_wait:
    bit PPUSTATUS
    bvc @sprite_0_wait

  ; if controller_1 has A pressed, then clench
  ; otherwise, don't clench
  lda controller_1
  and #%10000000
  beq @not_clenched

  @is_clenched:
    jsr scroll_sm_anus
    jsr increment_counter
    lda did_clench ; short-circuit if we're clenched
    bne :+

    lda #1 ; mark clenched
    sta did_clench
    

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
  
  jmp main_loop

disable_rendering:
  lda #0
  sta PPUCTRL
  sta PPUMASK
  rts

enable_rendering:
  lda #%00011110 ; enable sprites, enable background
  sta PPUMASK

  lda #%10010000 ;enable NMI, sprites from Pattern 0, background from Pattern 1
  sta PPUCTRL

  rts


load_palettes:
  lda PPUSTATUS ; read PPU status to reset the latch
  lda #$3f
  sta PPUADDR ; write the hi byte
  lda #$00
  sta PPUADDR ; write the low byte

  ldx #$00
  @load_palettes_loop:
    lda main_palette, x  ; load palette byte
    sta PPUDATA       ; write to ppu

    inx             ; move to next byte

    cpx #$20
    bne @load_palettes_loop  ; if x = $20, we copied all 32 bytes, so we're done

    rts


clear_bg:
  jsr @clear_bg_a
  jsr @clear_bg_b

  @clear_bg_a:
    lda PPUSTATUS             ; read PPU status to reset the high/low latch
    lda #$20
    sta PPUADDR             ; write the high byte of $2000 address
    lda #$00
    sta PPUADDR             ; write the low byte of $2000 address

  lda #solids_0
  ldy #HEIGHT_TILES
  @clear_bg_a_loop:
    ldx #WIDTH_TILES
    :
      sta PPUDATA
      dex
      bne :-
    dey
    bne @clear_bg_a_loop

  rts

  @clear_bg_b:
    lda PPUSTATUS             ; read PPU status to reset the high/low latch
    lda #$24
    sta PPUADDR             ; write the high byte of $2000 address
    lda #$00
    sta PPUADDR             ; write the low byte of $2000 address

  lda #solids_0
  ldy #HEIGHT_TILES
  @clear_bg_b_loop:
    ldx #WIDTH_TILES
    :
      sta PPUDATA
      dex
      bne :-
    dey
    bne @clear_bg_b_loop

  rts

; fill number of blocks as stored in X
blank_fill:
  lda #solids_0
  :
    sta PPUDATA
    dey
    bne :-

  rts

clear_attr:
  lda PPUSTATUS             ; read PPU status to reset the high/low latch
  lda #$23
  sta PPUADDR             ; write the high byte of $23C0 address
  lda #$C0
  sta PPUADDR             ; write the low byte of $23C0 address

  ldx #$00
  lda #$00
  :
    sta PPUDATA
    inx
    cpx #64
    bne :-

  lda PPUSTATUS             ; read PPU status to reset the high/low latch
  lda #$27
  sta PPUADDR             ; write the high byte of $23C0 address
  lda #$C0
  sta PPUADDR             ; write the low byte of $23C0 address

  ldx #$00
  lda #$00
  :
    sta PPUDATA
    inx
    cpx #64
    bne :-

  rts ; return from draw_anus subroutine

increment_counter:
  inc score_counter
  
  ; now check if it's greater than the SCORE_INC_TIMER
  lda score_counter
  clc ; clear the carry
  sbc #SCORE_INC_TIMER ; check if it's equal to the thing
  bcc @end ; carry flag not set, so it's not quite there, yet
    lda #0
    sta score_counter ; reset the score counter
    jsr increment_score ; increment the score

  @end:
  rts

;
; set the score to 0 and fill out the rest of the
; score with $ff values (blank tiles)
;
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

;
; subtracts one fromt he score. this is done for every drop of blood that escapes
; the anus. it happens when the blood appears, not when it hits the ground.
; this is accomplished by subtracting the least significant digit and carrying if necessary
; the inverse of increment_score
;
decrement_score:
  ; save registers
	pha
	txa
	pha
	tya
	pha

  ldx #0
  @loop:
    cpx #SCORE_SIZE
    beq @end ; stop when we get to the end

    lda score, x
    
    ; skip if we're on a placeholder ($ff)
    cmp #$ff
    beq @end ; we done!

    cmp #0
    beq @deal_with_carry

    ; if we're not at zero,
    ; decrement, store it and finish
    ; BUT, if we hit zero on this one, we want to convert to a placeholder
    ; ONLY if the next digit is a placeholder
    ; so look ahead, check if placeholder and act accordingly

    ; check to see if we're at '1', if not (we're greater than one)
    ; we can just decrement the value and be done.
    cmp #1
    bne @decrement_and_finish

    ; we are at 1, so lookahead
    jsr score_lookahead

    ; check to see if next digit is a placeholder
    ; if so, check if current digit is last digit

    cmp #$ff ; is it a placeholder
    bne @decrement_and_finish  ; it's not, so we don't do anything special

    ; (current digit is 1, next item is placeholder)
    ; check if we're last digit, and if so, don't do anything special.
    cpx #0
    beq @decrement_and_finish ; we are last digit, so nothing special

    ; (current digit is 1, next item is a placeholder)
    ; so just set current digit to placeholder rather than going to zero
    lda #$ff
    sta score, x
    jmp @end

  @decrement_and_finish:
    dec score, x ; decrement
    jmp @end     ; we done!

  @deal_with_carry:
    ; we are at zero and will go below
    ; so we need to try to carry

    ; if we're on the last digit and the next one is a $ff,
    ; then we should stay at zero and do nothing more
    cpx #0 ; check if we're on the first digit
    bne @dec_and_carry; we are not at last digit, so no special handling, decrement and carry

      ; we are on the last digit so look ahead and see if it's a $ff
      jsr score_lookahead

      cmp #$ff ; check if it's a placeholder
      beq @end ; if it is, let's just finish
               ; this means we're at a total score of '0'

      ; otherwise, we'll continue (decrement and move on)
  
  @dec_and_carry:
    ; if we get here, it means we're gonna deal with carrying.
    ; the current digit is 0, so we need to go down to 9 and then go to next digit
    ; set it to 9 and go to next number
    lda #9
    sta score, x

    inx ; next iteration
    jmp @loop

  @end:

    ; restore registers and stuff
    pla
    tay
    pla
    tax
    pla

  rts

;
; this will read the *next* byte in the score into register A
; it does this by incrementing X, reading score + x, then resetting X
;
score_lookahead:
  inx
  lda score, x ; read the score byte
  dex ; reset the x counter to where it was before lookahead

  rts

render_score_text:
  lda PPUSTATUS            ; read PPU status to reset the high/low latch
  lda #>NAMETABLE1
  sta PPUADDR             ; write the high byte of $2000 address
  lda #<NAMETABLE1+$40+8
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
  lda #solids_0
  sta PPUDATA

  ; then fall through to rendering the score itself.
  rts

render_score:
  lda PPUSTATUS             ; read PPU status to reset the high/low latch
  lda #>NAMETABLE1
  sta PPUADDR             ; write the high byte
  lda #<NAMETABLE1+$40+14
  sta PPUADDR             ; write the low byte

  ldx #SCORE_SIZE-1
  @render_score_loop:
    cpx #$ff ; we are counting down to zero
    beq @end ; when we go past, we're done.

    lda score, x ; read teh score digit

    cmp #$ff ; $FF tiles are empty
    bne :+   ; if it's not blank, go right to writing the value
      lda #solids_0 ; override the value with an empty tile

    :
      sta PPUDATA ; write the value to PPU

      dex
      jmp @render_score_loop

  @end:
  rts

