play_beep:
  ; lda #%00000001
  ; sta $4015 ;enable Square 1 channel, disable others

  ; lda #%00010110
  ; sta $4015 ;enable Square 2, Triangle and DMC channels.  Disable Square 1 and Noise.

  ; lda #$00
  ; sta $4015 ;disable all channels

  ; lda #$0F
  ; sta $4015 ;enable Square 1, Square 2, Triangle and Noise channels.  Disable DMC.
           ;this is the most common usage.

  lda #APU_NOISE_MASK
  sta $4015 ;enable square 1

  lda #%10111111 ;Duty 10, Volume F
  sta $4000

  lda #$C9    ;0C9 is a C# in NTSC mode
  sta $4002
  lda #$00
  sta $4003

  rts
