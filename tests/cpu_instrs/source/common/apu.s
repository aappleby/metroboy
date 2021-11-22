; Sound chip utilities

; Turns APU off
; Preserved: BC, DE, HL
sound_off:
     wreg NR52,0
     ret


; Turns APU on
; Preserved: BC, DE, HL
sound_on:
     wreg NR52,$80  ; power
     wreg NR51,$FF  ; mono
     wreg NR50,$77  ; volume
     ret


; Synchronizes to APU length counter within
; tens of clocks. Uses square 2 channel.
; Preserved: BC, DE, HL
sync_apu:
     wreg NR24,$00  ; disable length
     wreg NR21,$3E  ; length = 2 (in case of extra len clk)
     wreg NR22,$08  ; silent without disabling channel
     wreg NR24,$C0  ; start length
-    lda  NR52      ; wait for length to reach zero
     and  $02
     jr   nz,-
     ret


; Synchronizes to first square sweep within
; tens of clocks. Uses square 1 channel.
; Preserved: BC, DE, HL
sync_sweep:
     wreg NR10,$11  ; sweep period = 1, shift = 1
     wreg NR12,$08  ; silent without disabling channel
     wreg NR13,$FF  ; freq = $3FF
     wreg NR14,$83  ; start
-    lda  NR52
     and  $01
     jr   nz,-
     ret


; Copies 16-byte wave from (HL) to wave RAM
; Preserved: BC, DE
load_wave:
     push bc
     wreg NR30,$00  ; disable while writing
     ld   c,$30
-    ld   a,(hl+)
     ld   ($FF00+c),a
     inc  c
     bit  6,c
     jr   z,-
     pop  bc
     ret


; Makes short beep
; Preserved: BC, DE, HL
beep:
     xor  a         ; sound off
     sta  NR52
     dec  a
     sta  NR52      ; sound on
     sta  NR51      ; mono
     sta  NR50      ; volume
     wreg NR12,$F1  ; volume, envelope rate
     wreg NR14,$86  ; note on, pitch
     delay_msec 250
     ret


; Marks sound with bits of A encoded into volume
; Preserved: BC, DE, HL
mark_sound:
     push bc
     ld   c,a
     ld   b,8
     wreg NR10,0
     wreg NR11,$80
     wreg NR13,$F8
-    ld   a,$60
     rl   c
     jr   nc,+
     ld   a,$A0
+    sta  NR12
     wreg NR14,$87
     delay_usec 300
     wreg NR12,0
     delay_usec 100
     dec  b
     jr   nz,-
     pop  bc
     ret


; Fills wave RAM with A
; Preserved: BC, DE, HL
fill_wave:
     push bc
     ld   c,$30
-    ld   ($FF00+c),a
     inc  c
     bit  6,c
     jr   z,-
     pop  bc
     ret


; Gets current length counter value for
; channel with mask A into A. Length counter
; must be enabled for that channel.
; Preserved: BC, DE, HL
get_len_a:
     push bc
     ld   c,a
     ld   b,0
-    lda  NR52      ; 3
     and  c         ; 1
     jr   z,+       ; 2
     delay 4096-10
     inc  b         ; 1
     jr   nz,-      ; 3
+    ld   a,b
     pop  bc
     ret


; Synchronizes exactly to length clock. Next length clock
; occurs by 4079 clocks after this returns. Uses NR2x.
; Preserved: AF, BC, DE, HL
sync_length:
     push af
     push hl
     
     ld   hl,NR52
     wreg NR22,$08  ; silent without disabling channel
     wreg NR24,$40  ; avoids extra length clock on trigger
     wreg NR21,-2   ; length = 2, in case clock occurs immediately
     wreg NR24,$C0  ; start length
     
     ; Coarse sync
     ld   a,$02
-    and  (hl)
     jr   nz,-
     
     ; Fine sync. Slowly moves "forward" until
     ; length clock occurs just before reading NR52.
-    delay 4097-20
     wreg NR21,-1   ; 5
     wreg NR24,$C0  ; 5
     lda  NR52      ; 3
     delay 2        ; 2
     and  $02       ; 2
     jr   nz,-      ; 3
     
     pop  hl
     pop  af
     ret


; Delays n*4096 cycles
; Preserved: BC, DE, HL
.macro delay_frames ; n
     ld   a,\1
     call delay_frames_
.endm


; Delays A*4096+13 cycles (including CALL)
; Preserved: BC, DE, HL
delay_a_frames:
     or   a              ; 1
     jr   nz,+           ; 3
                         ; -1
     ret
delay_frames_:           ; delays 4096*A-2 cycles (including CALL)
     push af             ; 4
     ld   a,256-13-20-12 ; 2
     jr   ++             ; 3
+
-    push af             ; 4
     ld   a,256-13-20    ; 2
++   call delay_a_20_cycles
     delay 4096-256
     pop  af             ; 3
     dec  a              ; 1
     jr   nz,-           ; 3
                         ; -1
     ret


.macro test_chan_timing ; chan, iter
     ld   a,\1
     call print_dec
     call print_space
     ld   a,\2
-    push af
     test_chan 1<<\1, \1*5+NR10
     pop  af
     dec  a
     jr   nz,-
     call print_newline
.endm

.macro test_chans ARGS iter
     test_chan_timing 0,iter
     test_chan_timing 1,iter
     test_chan_timing 2,iter
     test_chan_timing 3,iter
.endm
