read_controller:
  ; latch controller
  lda #$01
  sta $4016
  lda #$00
  sta $4016     ; TELL BOTH THE CONTROLLERS TO LATCH BUTTONS

  ldx #8
  :
    lda $4016
    lsr a            ; bit0 -> Carry
    rol controller_1 ; bit0 <- Carry
    dex
    bne :-
  rts

; loop until controller is released
wait_for_controller_release:
  jsr read_controller
  lda controller_1
  cmp #0
  bne wait_for_controller_release

  rts

