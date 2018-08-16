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
  .byte $17,$17,$17,$17
  .byte $31,$35,$36,$37
  .byte $31,$39,$3A,$3B
  .byte $00,$00,$00,$00
  .byte $36,$05,$06,$07 ;; background
  .byte $01,$02,$38,$3C
  .byte $01,$1C,$15,$14
  .byte $01,$02,$38,$3C

.segment "ZEROPAGE"
  drip_velocity: .res 10 ; each drip's velocity
  last_drop_appeared: .res 1 ; a timer for when the last drop appeared

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

  ; we only want to animate the drop if last_drop_appeared is 0
  lda last_drop_appeared
  cmp #$00
  beq drip_drops

  ; if the last_drop_appeared is not zero, let's decrement by one
  dec last_drop_appeared

  lda last_drop_appeared
  cmp #$00
  bne end

  ; we just reached 0 on the last dropper
  ; so let's place the sprite back live
  lda #$10
  sta $0200

drip_drops:
  ; let's start trying to animate the blood going down
  lda $0200
  clc
  adc drip_velocity  ; add drip velocity to sprite[0].y 
  sta $0200

  lda drip_velocity
  cmp #$10 ; if it's equal to 10, skip increasing the thing
  beq :+
  inc drip_velocity ;increase drip velocity

:
  ; if drip is off-screen
  lda $0200 ; read in the y coordinate
  clc
  sbc #$ef ; check if it's off screen
  bcc end   ; carry flag not set, so it's not off-screen

  ; it is off screen
  ; lda #$10 ; set the default location for the drip
  ; sta $0200 ; set the y coord
  lda #$01 
  sta drip_velocity  ; reset the drip velocity to 1
  lda #$20
  sta last_drop_appeared

end:

  rti

