place_blood_drops:
  ; first we initialize things
  lda #$00
  sta last_drop_appeared

  lda #$00
  sta $2003  ; set the low byte (00) of the RAM address
  lda #$02
  sta $4014  ; set the high byte (02) of the RAM address, start the transfer

  ; first, let's create the placeholder sprite
  lda #24 ; 3 rows down
  sta $0200
  lda #3
  sta $0201
  lda #%00000000
  sta $0202
  lda #128
  sta $0203

  ldx #0
  ldy #0
@place_drip_loop:
  lda drip_positions, x ; start with the initial drip position
  cpy #0 ; if y is not zero, we want to write a to the spot
         ; we do this because we want to place these drips off-screen, initially
  bne :+
    lda #$ef ; make sure this badboy is offscreen

  :
  sta $0200+4, x ; store it in the sprite memory

  inx ; increment x and y
  iny

  cpy #04 ; reset y if it hits 4 since we want to have it go 0,1,2,3,0,1,2,3
  bne :+
    ldy #0

  :
  cpx #32 ; we're loading 32 bytes
  bne @place_drip_loop

  ; now let's initialize the starting timing and velocities
  ldx #0 
@create_drip_loop:
  lda #1
  sta drip_velocity, x ; first set the starting velocity to 1

  lda drip_starting_timing, x ; then initialize all of the timings
  sta last_drop_appeared, x

  inx ; increment x

  cpx #DRIP_COUNT ; only do it 8x
  bne @create_drip_loop

  lda #%00010000 ; enable sprites
  sta $2001
  rts

animate_frame:
  ; we want to iterate over each drop's last_drop_appeared value
  ; if it's zero, we want to animate that drop.
  ; if it's non-zero, we want to decrement it til it hits zero
  ; if it hits zero after decrementing, move it to default position and animate
  ; when it's off-screen, we want to re-set its last_appeared value to default.

  ldx #0
  ldy #0
@drip_loop:
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
  sta $0200+4, y
  lda #0
  sta $0201+4, y

@animate_drop:
  lda $0200+4, y       ; first move the drop down 'velocity' pixels
  clc
  adc drip_velocity, x
  sta $0200+4, y

  lda drip_velocity, x
  cmp #MAX_VELOCITY ; if it's at max velocity stop incrementing
  beq :+ ; skip incrementing
    inc drip_velocity, x ;increase drip velocity

  :
  ; if drip is at the end of it's fall
  lda $0200+4, y ; read in the y coordinate
  clc
  sbc #DRIP_END ; check if it's off screen
  bcc @update_drop_colors; carry flag not set, so it's not off-screen

  ; it is at the end
  lda #$01 
  sta drip_velocity, x  ; reset the drip velocity to 1
  lda #$20
  sta last_drop_appeared, x ; reset the drip timer
  lda #OFFSCREEN
  sta $0200+4, y   ; move the drip off-screen

@update_drop_colors:
  ; update the drop colors basd on their y coordinate
  ; this is to simulate the fading as they fall
  lda $0200+4, y ; get the y coordinate of the drop
  clc
  sbc #DRIP_CHANGE_1     ; check if we're past this point.
  bcc :+       ; if so, use the next color drop 

  lda #1 ; load the sprite index for this drop
  sta $0201+4, y ; store it back

:
  lda $0200+4, y ; get the y coordinate of the drop
  clc
  sbc #DRIP_CHANGE_2     ; check if we're past this point
  bcc :+       ; if so, update teh color of the drop again

  lda #2
  sta $0201+4, y

:
@next_drop:
  inx
  iny
  iny
  iny
  iny

  cpx #DRIP_COUNT ; check if we're at the 8th sprite
  beq @end_animate_frame  ; end our thing if so.

  jmp @drip_loop

@end_animate_frame:
  rts
