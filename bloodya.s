;
; iNES header
;

.segment "HEADER"

INES_MAPPER = 0 ; 0 = NROM
INES_MIRROR = 1 ; 0 = horizontal mirroring, 1 = vertical mirroring
INES_SRAM   = 0 ; 1 = battery backed SRAM at $6000-7FFF

; constants
OFFSCREEN     = $ef ; offscreen Y coordinate
MAX_VELOCITY  = 10  ; max velocity for a drip
DRIP_CHANGE_1 = 90  ; first breakpoint for color change
DRIP_CHANGE_2 = 120 ; second breakpoint for color change
DRIP_END      = 150 ; the Y of the end of the fall
DRIP_COUNT    = 8   ; number of drips we have

; convenience constants
WIDTH_TILES = 32
HEIGHT_TILES = 30

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

  .byte $36,$07,$17,$27 ; drip palette
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

big_anus:
big_anus_top:
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

big_anus_bottom:
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

sm_anus:
sm_anus_top:
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte sm_anus_0, sm_anus_1, sm_anus_2, sm_anus_3, sm_anus_4, sm_anus_5, sm_anus_6, sm_anus_7, sm_anus_8, sm_anus_9
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte sm_anus_10, sm_anus_11, sm_anus_12, sm_anus_13, sm_anus_14, sm_anus_15, sm_anus_16, sm_anus_17, sm_anus_18, sm_anus_19
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte sm_anus_20, sm_anus_21, sm_anus_22, sm_anus_23, sm_anus_24, sm_anus_25, sm_anus_26, sm_anus_27, sm_anus_28, sm_anus_29
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte sm_anus_30, sm_anus_31, sm_anus_32, sm_anus_33, sm_anus_34, sm_anus_35, sm_anus_36, sm_anus_37, sm_anus_38, sm_anus_39
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte sm_anus_40, sm_anus_41, sm_anus_42, sm_anus_43, sm_anus_44, sm_anus_45, sm_anus_46, sm_anus_47, sm_anus_48, sm_anus_49
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte sm_anus_50, sm_anus_51, sm_anus_52, sm_anus_53, sm_anus_54, sm_anus_55, sm_anus_56, sm_anus_57, sm_anus_58, sm_anus_59
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0

sm_anus_bottom:
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte sm_anus_60, sm_anus_61, sm_anus_62, sm_anus_63, sm_anus_64, sm_anus_65, sm_anus_66, sm_anus_67, sm_anus_68, sm_anus_69
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte sm_anus_70, sm_anus_71, sm_anus_72, sm_anus_73, sm_anus_74, sm_anus_75, sm_anus_76, sm_anus_77, sm_anus_78, sm_anus_79
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte sm_anus_80, sm_anus_81, sm_anus_82, sm_anus_83, sm_anus_84, sm_anus_85, sm_anus_86, sm_anus_87, sm_anus_88, sm_anus_89
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte sm_anus_90, sm_anus_91, sm_anus_92, sm_anus_93, sm_anus_94, sm_anus_95, sm_anus_96, sm_anus_97, sm_anus_98, sm_anus_99
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0
  .byte sm_anus_100, sm_anus_101, sm_anus_102, sm_anus_103, sm_anus_104, sm_anus_105, sm_anus_106, sm_anus_107, sm_anus_108, sm_anus_109
  .byte solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0, solids_0


blurb_tiles_1:
  .byte chr_w, chr_h, chr_a, chr_t, solids_0, chr_i, chr_s, solids_0, chr_a, solids_0, chr_b, chr_l, chr_o, chr_o, chr_d, chr_y
blurb_tiles_2:
  .byte chr_a, chr_n, chr_e, chr_s, solids_0, chr_d, chr_o, chr_i, chr_n, chr_g, solids_0, chr_h, chr_e, chr_r, chr_e, chr_qm

anus_attrs:
  .byte %00000000, %00000000, %00000000, %00000000, %00000000, %00000000, %00000000, %00000000
  .byte %00000000, %00000000, %00000000, %00000000, %00000000, %00000000, %00000000, %00000000


.segment "ZEROPAGE"
controller_1:       .res 1 ; state of controller 1 (is A pressed?)
temp:               .res 1 ; temporary variable
drip_velocity:      .res 8 ; each drip's velocity
last_drop_appeared: .res 8 ; a timer for when the last drop appeared
nmi_lock:           .res 1 ; set to 1 to prevent nmi reentry
nmi_latch:          .res 1 ; throttles animation speed.

.segment "BSS"
; nmt_update: .res 256 ; nametable update entry buffer for PPU update
; palette:    .res 32  ; palette buffer for PPU update

.segment "OAM"
oam: .res 256        ; sprite OAM data to be uploaded by DMA

.segment "CODE"
irq:
  rti

nmi:
  ; save registers
	pha
	txa
	pha
	tya
	pha
  ; prevent nmi reentry
  lda nmi_lock
  bne @nmi_end ; bail if we're in nmi still

  inc nmi_lock

  ; why am I doing this again?
  lda #$00
  sta $2003 ; set low byte of ram address
  lda #$02
  sta $4014 ; set the high byte of ram address

  ; if controller_1 has A pressed, then clench
  ; otherwise, don't clench
  lda controller_1
  beq @not_clenched

@is_clenched:
  jsr scroll_sm_anus
  jmp @nmi_end

@not_clenched:
  jsr scroll_big_anus

@nmi_end:
  ; jsr enable_rendering
  dec nmi_lock ; free up nmi lock

  ; restore registers and stuff
	pla
	tay
	pla
	tax
	pla

  inc nmi_latch

  rti

