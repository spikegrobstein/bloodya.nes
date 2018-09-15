;
; set the score to 0 and fill out the rest of the
; score with SCORE_PLACEHOLDER values ($ff)
;
init_score:
  lda #0
  sta score

  ; fill the rest of the score with $FF so we don't render it
  ldx #1
  lda #SCORE_PLACEHOLDER
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

    ; if we're on a placeholder,
    ; we need to bump to zero so we increment correctly
    cmp #SCORE_PLACEHOLDER 
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
    
    ; skip if we're on a placeholder
    cmp #SCORE_PLACEHOLDER
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

    cmp #SCORE_PLACEHOLDER ; is it a placeholder
    bne @decrement_and_finish  ; it's not, so we don't do anything special

    ; (current digit is 1, next item is placeholder)
    ; check if we're last digit, and if so, don't do anything special.
    cpx #0
    beq @decrement_and_finish ; we are last digit, so nothing special

    ; (current digit is 1, next item is a placeholder)
    ; so just set current digit to placeholder rather than going to zero
    lda #SCORE_PLACEHOLDER
    sta score, x
    jmp @end

  @decrement_and_finish:
    dec score, x ; decrement
    jmp @end     ; we done!

  @deal_with_carry:
    ; we are at zero and will go below
    ; so we need to try to carry

    ; if we're on the last digit and the next one is a placeholder,
    ; then we should stay at zero and do nothing more
    cpx #0 ; check if we're on the first digit
    bne @dec_and_carry; we are not at last digit, so no special handling, decrement and carry

      ; we are on the last digit so look ahead and see if it's a placeholder
      jsr score_lookahead

      cmp #SCORE_PLACEHOLDER ; check if it's a placeholder
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

;
; this subroutine renders the word "SCORE" on the screen
;
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

  rts

;
; This renders the value of the current score to the screen.
; SCORE_PLACEHOLDER values are rendered as empty (solids_0) tiles
;
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

    cmp #SCORE_PLACEHOLDER ; $FF tiles are empty
    bne @write_digit_to_ppu   ; if it's not blank, go right to writing the value
      lda #solids_0 ; override the value with an empty tile

  @write_digit_to_ppu:
    sta PPUDATA ; write the value to PPU

    dex   ; decrement x and continue with loop
    jmp @render_score_loop

  @end:
  rts

