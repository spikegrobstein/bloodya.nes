; solid colors
bgs_0 = $29
bgs_1 = $2a
bgs_2 = $2b
bgs_3 = $2c

; pieces of asterisk
bg_000 = $25
bg_001 = $26
bg_002 = $27
bg_003 = $28
bg_004 = $35
bg_005 = $36
bg_006 = $37
bg_007 = $38
bg_008 = $41
bg_009 = $42
bg_010 = $43
bg_011 = $45
bg_012 = $46
bg_013 = $47
bg_014 = $48
bg_015 = $4a
bg_016 = $4b
bg_017 = $4c
bg_018 = $51
bg_019 = $52
bg_020 = $53
bg_021 = $54
bg_022 = $55
bg_023 = $56
bg_024 = $57
bg_025 = $58
bg_026 = $59
bg_027 = $5a
bg_028 = $5b
bg_029 = $5c
bg_030 = $61
bg_031 = $62
bg_032 = $63
bg_033 = $64
bg_034 = $65
bg_035 = $66
bg_036 = $67
bg_037 = $68
bg_038 = $69
bg_039 = $6a
bg_040 = $6b
bg_041 = $6c
bg_042 = $71
bg_043 = $72
bg_044 = $73
bg_045 = $74
bg_046 = $75
bg_047 = $76
bg_048 = $77
bg_049 = $78
bg_050 = $79
bg_051 = $7a
bg_052 = $7b
bg_053 = $7c
bg_054 = $84
bg_055 = $85
bg_056 = $86
bg_057 = $87
bg_058 = $88
bg_059 = $89
bg_060 = $91
bg_061 = $92
bg_062 = $93
bg_063 = $94
bg_064 = $95
bg_065 = $96
bg_066 = $97
bg_067 = $98
bg_068 = $99
bg_069 = $9a
bg_070 = $9b
bg_071 = $9c
bg_072 = $a1
bg_073 = $a2
bg_074 = $a3
bg_075 = $a4
bg_076 = $a5
bg_077 = $a6
bg_078 = $a7
bg_079 = $a8
bg_080 = $a9
bg_081 = $aa
bg_082 = $ab
bg_083 = $ac
bg_084 = $b1
bg_085 = $b2
bg_086 = $b3
bg_087 = $b4
bg_088 = $b5
bg_089 = $b6
bg_090 = $b7
bg_091 = $b8
bg_092 = $b9
bg_093 = $ba
bg_094 = $bb
bg_095 = $bc
bg_096 = $c1
bg_097 = $c2
bg_098 = $c3
bg_099 = $c5
bg_100 = $c6
bg_101 = $c7
bg_102 = $c8
bg_103 = $ca
bg_104 = $cb
bg_105 = $cc
bg_106 = $d5
bg_107 = $d6
bg_108 = $d7
bg_109 = $d8
bg_110 = $e5
bg_111 = $e6
bg_112 = $e7
bg_113 = $e8

main:
  jsr load_palettes
  jsr clear_bg
  ; jsr clear_attr
  jsr draw_asterisk
  jsr place_blood_drops

  lda #%00011110 ; enable sprites, enable background
  sta $2001

  lda #%10010000 ;enable NMI, sprites from Pattern 0, background from Pattern 1
  sta $2000

  jmp forever

load_palettes:
  lda $2002 ; read PPU status to reset the latch
  lda #$3f
  sta $2006 ; write the hi byte
  lda #$00
  sta $2006 ; write the low byte
  ldx #$00
load_palettes_loop:
  lda main_palette, x  ; load palette byte
  sta $2007       ; write to ppu
  inx             ; move to next byte
  cpx #$20
  bne load_palettes_loop  ; if x = $20, we copied all 32 bytes, so we're done
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
  cpx #$08 ; only do it 8x
  bne @create_drip_loop

  ; set velocity to 1
  lda #1
  sta drip_velocity

  lda #%00010000 ; enable sprites
  sta $2001
  rts

clear_bg:
  lda $2002             ; read PPU status to reset the high/low latch
  lda #$20
  sta $2006             ; write the high byte of $2000 address
  lda #$00
  sta $2006             ; write the low byte of $2000 address

  lda #bgs_0
  ldy #30
@clear_bg_loop:
  ldx #32
  :
    sta $2007
    dex
    bne :-
  dey
  bne @clear_bg_loop

clear_attr:
  ldx #64
  lda #%00000000
@clear_attr_loop:
  sta $2007 
  dex
  bne @clear_attr_loop

  rts

; fill number of blocks as stored in X
blank_fill:
  lda #bgs_0
  :
  sta $2007
  dey
  bne :-
  rts

draw_asterisk:
  lda $2002             ; read PPU status to reset the high/low latch
  lda #$20
  sta $2006             ; write the high byte of $2000 address
  lda #$00
  sta $2006             ; write the low byte of $2000 address

  ldy #32
  jsr blank_fill
  ldy #32
  jsr blank_fill
  ldy #32
  jsr blank_fill

  ldx #0              ; start out at 0
@draw_asterisk_top_loop:
  lda asterisk_tiles_top, x
  sta $2007 ; write to PPU
  inx
  cpx #192
  bne @draw_asterisk_top_loop

  ldx #0              ; start out at 0
@draw_asterisk_bottom_loop:
  lda asterisk_tiles_bottom, x
  sta $2007 ; write to PPU
  inx
  cpx #224
  bne @draw_asterisk_bottom_loop
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
  lda asterisk_attrs ,x
  sta $2007
  inx
  cpx #10
  bne load_attr_loop

  rts ; return from draw_asterisk subroutine


forever:
  jmp forever

