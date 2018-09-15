.include "vars.inc"
.include "ascii.inc"
.include "anus.inc"
.include "drip.inc"
.include "splash.inc"
.include "controller.inc"

init:
  jsr start_splash

start_main:
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


