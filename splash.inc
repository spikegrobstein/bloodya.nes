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
  ; jsr load_splash_attr

  jsr place_splash_tracking_sprite

  jsr enable_rendering
  dec nmi_lock

  ; fall through to splash loop
  
splash_loop:
  jsr read_controller

  ; if start is pressed, jump to main
  lda controller_1
  and #%10010000
  beq :+
    jmp init_main

  :

  lda nmi_latch
  cmp #0
  beq splash_loop

  lda splash_offset
  cmp #0
  beq @splash_loop_end
    dec splash_offset
    dec splash_offset

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

  lda PPUSTATUS
  lda #0
  sta PPUSCROLL
  sta PPUSCROLL

  @splash_loop_end:
  lda #0
  sta nmi_latch

  jmp splash_loop
  ; end

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
  lda #$ab
  sta PPUADDR

  lda #chr_b
  sta PPUDATA
  lda #chr_l
  sta PPUDATA
  lda #chr_o
  sta PPUDATA
  lda #chr_o
  sta PPUDATA
  lda #chr_d
  sta PPUDATA
  lda #chr_y
  sta PPUDATA
  lda #solids_0
  sta PPUDATA
  lda #chr_a
  sta PPUDATA
  lda #chr_n
  sta PPUDATA
  lda #chr_e
  sta PPUDATA
  lda #chr_s
  sta PPUDATA

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

  rts

render_splash_press_start:
  lda PPUSTATUS
  lda #$22
  sta PPUADDR
  lda #$4b
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
