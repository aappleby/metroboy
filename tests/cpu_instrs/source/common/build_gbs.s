; Build as GBS music file

.memoryMap
     defaultSlot 0
     slot 0 $3000 size $1000
     slot 1 $C000 size $1000
.endMe

.romBankSize $1000
.romBanks 2


;;;; GBS music file header

.byte "GBS"
.byte 1        ; vers
.byte 1        ; songs
.byte 1        ; first song
.word load_addr
.word reset
.word gbs_play
.word std_stack
.byte 0,0      ; timer
.ds $60,0
load_addr:

; WLA assumes we're building ROM and messes
; with bytes at the beginning, so skip them.
.ds $100,0


;;;; Shell
     
.include "runtime.s"

init_runtime:
     ld   a,$01     ; Identify as DMG hardware
     ld   (gb_id),a
     .ifdef TEST_NAME
          print_str TEST_NAME,newline,newline
     .endif
     ret

std_print:
     sta  SB
     wreg SC,$81
     delay 2304
     ret

post_exit:
     call play_byte
forever:
     wreg NR52,0    ; sound off
-    jp   -

.ifndef CUSTOM_RESET
     gbs_play:
.endif
console_flush:
console_normal:
console_inverse:
console_set_mode:
     ret

; Reports A in binary as high and low tones, with
; leading low tone for reference. Omits leading
; zeroes.
; Preserved: AF, BC, DE, HL
play_byte:
     push af
     push hl
     
     ; HL = (A << 1) | 1
     scf
     rla
     ld   l,a
     ld   h,0
     rl   h
     
     ; Shift left until next-to-top bit is 1
-    add  hl,hl
     bit  6,h
     jr   z,-
     
     ; Reset sound
     delay_msec 400
     wreg NR52,0    ; sound off
     wreg NR52,$80  ; sound on
     wreg NR51,$FF  ; mono
     wreg NR50,$77  ; volume
     
-    add  hl,hl

     ; Low or high pitch based on bit shifted out
     ; of HL
     ld   a,0
     jr   nc,+
     ld   a,$FF
+    sta  NR23
     
     ; Play short tone
     wreg NR21,$A0
     wreg NR22,$F0
     wreg NR24,$86
     delay_msec 75
     wreg NR22,0
     wreg NR23,$F8
     wreg NR24,$87
     delay_msec 200
     
     ; Loop until HL = $8000
     ld   a,h
     xor  $80
     or   l
     jr   nz,-
     
     pop  hl
     pop  af
     ret

.ends
