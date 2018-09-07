init_splash:
  inc nmi_lock
  jsr disable_rendering

  jsr load_splash_palette

  ; init the splash scrolling offset.
  lda #200
  sta splash_offset

  jsr clear_bg

  jsr render_splash_title
  jsr render_splash_press_start

  jsr place_splash_tracking_sprite

  jsr enable_rendering
  dec nmi_lock

  ; fall through to splash loop
  
splash_loop:
  jsr read_controller

  ; TODO: make this a subroutine
  ; if start is pressed, jump to main
  lda controller_1
  and #%10010000
  beq @continue_scroll
    lda splash_offset
    cmp #0 ; if we finished scrolling and user pressed
    bne :+
      jsr wait_for_controller_release
      jmp init_main ; then let's start the game!

    :
    lda #0
    sta splash_offset

    jsr wait_for_controller_release

  @continue_scroll:

  ; wait for latch to fire (loop until latch != 0)
  lda nmi_latch
  cmp #0
  beq splash_loop

  ; if we're done scrolling, go to the end
  lda splash_offset ; read how far we are scrolling the title screen
  cmp #0            ; if we finished (it's at zero)
  beq @skip_animate_title ; then we're done.

  ; if we're still scrolling,
  ; decrement the splash offset by 2
  dec splash_offset
  dec splash_offset

  @skip_animate_title:

  ; scroll the splash offset
  lda PPUSTATUS
  lda splash_offset
  sta PPUSCROLL
  lda #0
  sta PPUSCROLL

  ; handle split scroll
  @sprite_0_wait_clear:
    bit PPUSTATUS
    bvs @sprite_0_wait_clear
  @sprite_0_wait:
    bit PPUSTATUS
    bvc @sprite_0_wait

  lda splash_offset
  cmp #0
  bne @hide_press_start

  lda flash_state
  cmp #1
  bne @hide_press_start

  @show_press_start:
    ; scroll to zero once we hit the scroll tracker
    lda PPUSTATUS
    lda #0
    sta PPUSCROLL
    sta PPUSCROLL

    jmp @splash_loop_end

  @hide_press_start:
    lda PPUSTATUS
    lda #$ff
    sta PPUSCROLL
    lda #0
    sta PPUSCROLL


  ; finish everything up
  @splash_loop_end:

  jsr flash_press_start

  ; reset the latch to zero
  lda #0
  sta nmi_latch

  jmp splash_loop

  ; end and loop

;
; count the flash counter to zero
; if it hits zero, flip the state and reset counter
flash_press_start:
  lda flash_counter
  cmp #0
  beq @handle_zero

    ; counter isn't zero, so decrement
    dec flash_counter
    rts ; and we're done

  @handle_zero:
    ; we are at zero, so first, reset the counter
    lda #FLASH_START_CADENCE
    sta flash_counter

    ; now, update the state
    lda flash_state
    cmp #0 ; it is off
    beq @turn_on
      ; turn it off
      lda #0
      jmp @update_state

    @turn_on:
      lda #1 ; turn it on

    @update_state:

    sta flash_state

  rts

load_splash_palette:
  lda PPUSTATUS
  lda #$3f
  sta PPUADDR
  lda #$00
  sta PPUADDR

  ldx #$00
  :
    lda splash_palette, x
    sta PPUDATA

    inx

    cpx #$20
    bne :-

  rts

render_splash_title:
  lda PPUSTATUS
  lda #$20
  sta PPUADDR
  lda #$80
  sta PPUADDR

  ldx #0
  :
    lda splash_title_top, x
    sta PPUDATA
    inx
    cpx #160
    bne :-

  ldx #0
  :
    lda splash_title_bottom, x
    sta PPUDATA
    inx
    cpx #160
    bne :-

  ; draw tracking tile row
  lda PPUSTATUS
  lda #$22
  sta PPUADDR
  lda #$00
  sta PPUADDR

  ldx #0
  :
    lda #solids_1
    sta PPUDATA

    inx
    cpx #32
    bne :-

  ; draw tracking tile row
  lda PPUSTATUS
  lda #$26
  sta PPUADDR
  lda #$00
  sta PPUADDR

  ldx #0
  :
    lda #solids_1
    sta PPUDATA

    inx
    cpx #32
    bne :-

  ; 20da
  lda PPUSTATUS
  lda #$23
  sta PPUADDR
  lda #$e0
  sta PPUADDR

  ldx #0
  :
    lda #%00000101
    sta PPUDATA

    inx
    cpx #8
    bne :-

  ; 20da
  lda PPUSTATUS
  lda #$27
  sta PPUADDR
  lda #$e0
  sta PPUADDR

  ldx #0
  :
    lda #%00000101
    sta PPUDATA

    inx
    cpx #8
    bne :-

  ; set up the palette attributes
  lda PPUSTATUS
  lda #$23
  sta PPUADDR
  lda #$C8
  sta PPUADDR

  ldx #0
  :
    lda #%10101010
    sta PPUDATA
    inx
    cpx #24
    bne :-

  rts

render_splash_press_start:
  lda PPUSTATUS
  lda #$22
  sta PPUADDR
  lda #$ab
  sta PPUADDR

  ldx #0
  @render_splash_press_start_loop:
    lda splash_press_start, x
    sta PPUDATA

    inx
    
    cpx #11
    bne @render_splash_press_start_loop

  rts

place_splash_tracking_sprite:
  lda #8*16-1
  sta $0200 ; write y
  lda #0
  sta $0201 ; write tile index
  lda #%00000000
  sta $0202 ; write attributes
  lda #8*16
  sta $0203 ; write x

  rts

load_splash_attr:
  lda PPUSTATUS             ; read PPU status to reset the high/low latch
  lda #$23
  sta PPUADDR             ; write the high byte of $23C0 address
  lda #$C0
  sta PPUADDR             ; write the low byte of $23C0 address

  ; store zeros
  ; ldx #$00
  ; :
    ; lda #%
    ; sta PPUDATA
    ; inx
    ; cpx #20
    ; bne :-

  ; load splash attribures
  ldx #$00              ; start out at 0
  :
    lda splash_attrs, x
    sta PPUDATA
    inx
    cpx #10
    bne :-

  rts ; return from draw_anus subroutine
