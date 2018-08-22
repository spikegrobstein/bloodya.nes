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
  .byte $17,$08,$26,$25 ;; main palette
  .byte $17,$08,$18,$28
  .byte $31,$39,$3A,$0f
  .byte $00,$00,$00,$00

  .byte $36,$07,$26,$37
  .byte $01,$02,$38,$3C
  .byte $01,$1C,$15,$14
  .byte $01,$02,$38,$3C

drip_positions:
  .byte $35,$00,%00000000,$70
  .byte $42,$01,%00000000,$75
  .byte $38,$02,%00000000,$80
  .byte $32,$01,%00000000,$88

  .byte $40,$02,%00000000,$82
  .byte $44,$00,%00000000,$78
  .byte $46,$01,%00000000,$89
  .byte $30,$00,%00000000,$80

drip_starting_timing:
  .byte 00,43,97,117
  .byte 159,193,211,240

asterisk_tiles:
asterisk_tiles_top:
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte big_anus_0, big_anus_1, big_anus_2, big_anus_3, big_anus_4, big_anus_5, big_anus_6, big_anus_7, big_anus_8, big_anus_9, big_anus_10, big_anus_11
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte big_anus_12, big_anus_13, big_anus_14, big_anus_15, big_anus_16, big_anus_17, big_anus_18, big_anus_19, big_anus_20, big_anus_21, big_anus_22, big_anus_23
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte big_anus_24, big_anus_25, big_anus_26, big_anus_27, big_anus_28, big_anus_29, big_anus_30, big_anus_31, big_anus_32, big_anus_33, big_anus_34, big_anus_35
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte big_anus_36, big_anus_37, big_anus_38, big_anus_39, big_anus_40, big_anus_41, big_anus_42, big_anus_43, big_anus_44, big_anus_45, big_anus_46, big_anus_47
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte big_anus_48, big_anus_49, big_anus_50, big_anus_51, big_anus_52, big_anus_53, big_anus_54, big_anus_55, big_anus_56, big_anus_57, big_anus_58, big_anus_59
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte big_anus_60, big_anus_61, big_anus_62, big_anus_63, big_anus_64, big_anus_65, big_anus_66, big_anus_67, big_anus_68, big_anus_69, big_anus_70, big_anus_71
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0

asterisk_tiles_bottom:
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte big_anus_72, big_anus_73, big_anus_74, big_anus_75, big_anus_76, big_anus_77, big_anus_78, big_anus_79, big_anus_80, big_anus_81, big_anus_82, big_anus_83
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte big_anus_84, big_anus_85, big_anus_86, big_anus_87, big_anus_88, big_anus_89, big_anus_90, big_anus_91, big_anus_92, big_anus_93, big_anus_94, big_anus_95
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte big_anus_96, big_anus_97, big_anus_98, big_anus_99, big_anus_100, big_anus_101, big_anus_102, big_anus_103, big_anus_104, big_anus_105, big_anus_106, big_anus_107
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte big_anus_108, big_anus_109, big_anus_110, big_anus_111, big_anus_112, big_anus_113, big_anus_114, big_anus_115, big_anus_116, big_anus_117, big_anus_118, big_anus_119
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte big_anus_120, big_anus_121, big_anus_122, big_anus_123, big_anus_124, big_anus_125, big_anus_126, big_anus_127, big_anus_128, big_anus_129, big_anus_130, big_anus_131
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte big_anus_132, big_anus_133, big_anus_134, big_anus_135, big_anus_136, big_anus_137, big_anus_138, big_anus_139, big_anus_140, big_anus_141, big_anus_142, big_anus_143
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte big_anus_144, big_anus_145, big_anus_146, big_anus_147, big_anus_148, big_anus_149, big_anus_150, big_anus_151, big_anus_152, big_anus_153, big_anus_154, big_anus_155
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0

blurb_tiles_1:
  .byte chr_w, chr_h, chr_a, chr_t, solids_0, chr_i, chr_s, solids_0, chr_a, solids_0, chr_b, chr_l, chr_o, chr_o, chr_d, chr_y
blurb_tiles_2:
  .byte chr_a, chr_n, chr_e, chr_s, solids_0, chr_d, chr_o, chr_i, chr_n, chr_g, solids_0, chr_h, chr_e, chr_r, chr_e, chr_qm

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
  lda #0
  sta $0201, y

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
  bcc update_drop_colors; carry flag not set, so it's not off-screen

  ; it is off screen
  lda #$01 
  sta drip_velocity, x  ; reset the drip velocity to 1
  lda #$20
  sta last_drop_appeared, x

update_drop_colors:
  ; update the drop colors basd on their y coordinate
  ; this is to simulate the fading as they fall
  lda $0200, y
  clc
  sbc #150
  bcc :+

  lda $0201, y
  sta temp
  inc temp
  lda temp
  sta $0201, y

:
  lda $0200, y
  clc
  sbc #170
  bcc :+

  lda $0201, y
  sta temp
  inc temp
  lda temp
  sta $0201, y

:
  cpx #$08 ; we did all 8
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

