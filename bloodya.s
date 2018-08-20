;
; iNES header
;

.segment "HEADER"

INES_MAPPER = 0 ; 0 = NROM
INES_MIRROR = 1 ; 0 = horizontal mirroring, 1 = vertical mirroring
INES_SRAM   = 0 ; 1 = battery backed SRAM at $6000-7FFF

.byte 'N', 'E', 'S', $1A ; ID
.byte $02 ; 16k PRG bank count
.byte $01 ; 8k CHR bank count
.byte INES_MIRROR | (INES_SRAM << 1) | ((INES_MAPPER & $f) << 4)
.byte (INES_MAPPER & %11110000)
.byte $0, $0, $0, $0, $0, $0, $0, $0 ; padding

;
; CHR ROM
;

.segment "TILES"
.incbin "chr1.chr"

; vectors placed at top 6 bytes of memory area
;

.segment "VECTORS"
.word nmi
.word reset
.word irq

;
; reset routine
;

.segment "CODE"
reset:
  sei       ; disable IRQs
  cld       ; disable decimal mode
  ldx #$40
  stx $4017 ; disable APU frame IRQ
  ldx #$FF
  txs       ;set up stack
  inx       ;now X = 0 (255 + 1). we're gonna write $00 to several memory addresses
  stx $2000 ; disable NMI
  stx $2001 ; disable rendering
  stx $4010 ; disable DMC IRQs

  jsr vblankwait

  ldx #$00 ; init x
: ; loop over and reset memory
  lda #$00
  sta $0000, x
  sta $0100, x
  sta $0200, x
  sta $0300, x
  sta $0400, x
  sta $0500, x
  sta $0600, x
  sta $0700, x
  inx
  bne :-

  ; place all sprites offscreen at Y=255
  lda #255
  ldx #0
:
  sta oam, X
  inx
  inx
  inx
  inx
  bne :-

  jsr vblankwait

  ; ok, we can start the program
  jmp main

vblankwait:
  bit $2002
  bpl vblankwait
  rts

.include "main.inc"

.segment "RODATA"

main_palette:
  .byte $17,$08,$26,$36 ;; main palette
  .byte $31,$35,$36,$37
  .byte $31,$39,$3A,$0f
  .byte $00,$00,$00,$00
  .byte $36,$05,$39,$37
  .byte $01,$02,$38,$3C
  .byte $01,$1C,$15,$14
  .byte $01,$02,$38,$3C

drip_positions:
  .byte $35,$00,%00000000,$70
  .byte $42,$00,%00000000,$75
  .byte $38,$00,%00000000,$80
  .byte $32,$00,%00000000,$88

  .byte $40,$00,%00000000,$82
  .byte $44,$00,%00000000,$78
  .byte $46,$00,%00000000,$89
  .byte $30,$00,%00000000,$80

drip_starting_timing:
  .byte 00,43,97,117
  .byte 159,193,211,240

asterisk_tiles:
asterisk_tiles_top:
  .byte bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0
  .byte bgs_0, bgs_0, bgs_0, bgs_0, bg_000, bg_001, bg_002, bg_003, bgs_0, bgs_0, bgs_0, bgs_0
  .byte bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0
  .byte bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0
  .byte bgs_0, bgs_0, bgs_0, bgs_0, bg_004, bg_005, bg_006, bg_007, bgs_0, bgs_0, bgs_0, bgs_0
  .byte bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0
  .byte bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0
  .byte bg_008, bg_009, bg_010, bgs_0, bg_011, bg_012, bg_013, bg_014, bgs_0, bg_015, bg_016, bg_017
  .byte bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0
  .byte bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0
  .byte bg_018, bg_019, bg_020, bg_021, bg_022, bg_023, bg_024, bg_025, bg_026, bg_027, bg_028, bg_029
  .byte bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0
  .byte bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0
  .byte bg_030, bg_031, bg_032, bg_033, bg_034, bg_035, bg_036, bg_037, bg_038, bg_039, bg_040, bg_041
  .byte bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0
  .byte bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0
  .byte bg_042, bg_043, bg_044, bg_045, bg_046, bg_047, bg_048, bg_049, bg_050, bg_051, bg_052, bg_053
  .byte bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0

asterisk_tiles_bottom:
  .byte bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0
  .byte bgs_0, bgs_0, bgs_0, bg_054, bg_055, bg_056, bg_057, bg_058, bg_059, bgs_0, bgs_0, bgs_0
  .byte bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0
  .byte bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0
  .byte bg_060, bg_061, bg_062, bg_063, bg_064, bg_065, bg_066, bg_067, bg_068, bg_069, bg_070, bg_071
  .byte bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0
  .byte bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0
  .byte bg_072, bg_073, bg_074, bg_075, bg_076, bg_077, bg_078, bg_079, bg_080, bg_081, bg_082, bg_083
  .byte bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0
  .byte bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0
  .byte bg_084, bg_085, bg_086, bg_087, bg_088, bg_089, bg_090, bg_091, bg_092, bg_093, bg_094, bg_095
  .byte bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0
  .byte bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0
  .byte bg_096, bg_097, bg_098, bgs_0, bg_099, bg_100, bg_101, bg_102, bgs_0, bg_103, bg_104, bg_105
  .byte bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0
  .byte bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0
  .byte bgs_0, bgs_0, bgs_0, bgs_0, bg_106, bg_107, bg_108, bg_109, bgs_0, bgs_0, bgs_0, bgs_0
  .byte bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0
  .byte bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0
  .byte bgs_0, bgs_0, bgs_0, bgs_0, bg_110, bg_111, bg_112, bg_113, bgs_0, bgs_0, bgs_0, bgs_0
  .byte bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0, bgs_0

blurb_tiles_1:
  .byte chr_w, chr_h, chr_a, chr_t, bgs_0, chr_i, chr_s, bgs_0, chr_a, bgs_0, chr_b, chr_l, chr_o, chr_o, chr_d, chr_y
blurb_tiles_2:
  .byte chr_a, chr_n, chr_e, chr_s, bgs_0, chr_d, chr_o, chr_i, chr_n, chr_g, bgs_0, chr_h, chr_e, chr_r, chr_e, chr_qm

asterisk_attrs:
  .byte %00000000, %00000000, %00000000, %00000000, %00000000, %00000000, %00000000, %00000000
  .byte %00000000, %00000000, %00000000, %00000000, %00000000, %00000000, %00000000, %00000000


.segment "ZEROPAGE"
drip_velocity: .res 8 ; each drip's velocity
last_drop_appeared: .res 8 ; a timer for when the last drop appeared
temp:           .res 1 ; temporary variable

.segment "BSS"
; nmt_update: .res 256 ; nametable update entry buffer for PPU update
; palette:    .res 32  ; palette buffer for PPU update

.segment "OAM"
oam: .res 256        ; sprite OAM data to be uploaded by DMA

.segment "CODE"
irq:
  rti

nmi:
  lda #$00
  sta $2003 ; set low byte of ram address
  lda #$02
  sta $4014 ; set the high byte of ram address

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
  beq animate_drop

  dec last_drop_appeared, x ; decrement the value

  lda last_drop_appeared, x ; load it so we can check it
  cmp #$00 ; check if it's zero
  bne next_drop

  ; the thing hit zero, so we need to initialize it before we start animating
  lda drip_positions, y
  sta $0200, y

  jmp animate_drop

animate_drop:
  lda $0200, y
  clc
  adc drip_velocity, x
  sta $0200, y

  lda drip_velocity, x
  cmp #$10 ; if it's equal to 10, skip increasing the thing
  beq :+ ; skip incrementing
  inc drip_velocity, x ;increase drip velocity

:
  ; if drip is off-screen
  lda $0200, y ; read in the y coordinate
  clc
  sbc #$ef ; check if it's off screen
  bcc next_drop ; carry flag not set, so it's not off-screen

  ; it is off screen
  lda #$01 
  sta drip_velocity, x  ; reset the drip velocity to 1
  lda #$20
  sta last_drop_appeared, x

  cpx #$08
  beq end

next_drop:
  inx
  iny
  iny
  iny
  iny

  cpx #$08 ; check if we're at the 8th sprite
  beq end  ; end our thing if so.

  jmp drip_loop

end:

  lda #$00
  sta $2005
  sta $2005

  rti

;
; subroutines and stuff
;


; ppu_address_tile: use with rendering off, sets memory address to tile at X/Y, ready for a $2007 write
;   Y =  0- 31 nametable $2000
;   Y = 32- 63 nametable $2400
;   Y = 64- 95 nametable $2800
;   Y = 96-127 nametable $2C00
ppu_address_tile:
	lda $2002 ; reset latch
	tya
	lsr
	lsr
	lsr
	ora #$20 ; high bits of Y + $20
	sta $2006
	tya
	asl
	asl
	asl
	asl
	asl
	sta temp
	txa
	ora temp
	sta $2006 ; low bits of Y + X
	rts
