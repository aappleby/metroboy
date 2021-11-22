; CPU speed manipulation.

; Switches to normal speed. No effect on DMG.
; Preserved: BC, DE, HL
cpu_norm:
     ; Do nothing if not CGB
     ld   a,(gb_id)
     and  gb_id_cgb
     ret  z
     
     lda  KEY1
     rlca
     ret  nc
     jr   cpu_speed_toggle


; Switches to double speed. No effect on DMG.
; Preserved: BC, DE, HL
cpu_fast:
     ; Do nothing if not CGB
     ld   a,(gb_id)
     and  gb_id_cgb
     ret  z
     
     lda  KEY1
     rlca
     ret  c
cpu_speed_toggle:
     di
     lda  IE
     push af
     xor  a
     sta  IE
     sta  IF
     wreg P1,$30
     wreg KEY1,1
     stop
     nop
     pop  af
     sta  IE
     ret


; Determines current CPU speed without using KEY1.
; A=1 if fast, 0 if normal. Always 0 on DMG.
; Preserved: BC, DE,HL
get_cpu_speed:
     push bc
     call sync_apu
     wreg NR14,$C0
     wreg NR11,-1
     wreg NR12,8
     wreg NR14,$C0
     ld   bc,-$262
-    inc  bc
     lda  NR52
     and  1
     jr   nz,-
     ld   a,0
     bit  7,b
     jr   nz,+
     inc  a
+    pop  bc
     ret
