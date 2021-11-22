; Fast table-based CRC-32

.define crc_tables  (bss+$FF)&$FF00 ; 256-byte aligned
.redefine bss       crc_tables+$400


; Initializes fast CRC tables and resets checksum.
; Time: 47 msec
init_crc_fast:
     ld   l,0
@next:
     xor  a
     ld   c,a
     ld   d,a
     ld   e,l
     
     ld   h,8
-    rra
     rr   c
     rr   d
     rr   e
     jr   nc,+
     xor  $ED
     ld   b,a
     ld   a,c
     xor  $B8
     ld   c,a
     ld   a,d
     xor  $83
     ld   d,a
     ld   a,e
     xor  $20
     ld   e,a
     ld   a,b
     
+    dec  h
     jr   nz,-
     
     ld   h,>crc_tables
     ld   (hl),e
     inc  h
     ld   (hl),d
     inc  h
     ld   (hl),c
     inc  h
     ld   (hl),a
     
     inc  l
     jr   nz,@next
     
     jp   init_crc


; Faster version of update_crc
; Preserved: BC, DE
; Time: 50 cycles (including CALL)
update_crc_fast:

; Fastest inline macro version of update_crc_fast
; Time: 40 cycles
; Size: 28 bytes
.macro update_crc_fast
     ld   l,a            ; 1
     lda  checksum       ; 3
     xor  l              ; 1
     ld   l,a            ; 1
     ld   h,>crc_tables  ; 2
     
     lda  checksum+1     ; 3
     xor  (hl)           ; 2
     inc  h              ; 1
     sta  checksum       ; 3
     
     lda  checksum+2     ; 3
     xor  (hl)           ; 2
     inc  h              ; 1
     sta  checksum+1     ; 3
     
     lda  checksum+3     ; 3
     xor  (hl)           ; 2
     inc  h              ; 1
     sta  checksum+2     ; 3
     
     ld   a,(hl)         ; 2
     sta  checksum+3     ; 3
.endm
     update_crc_fast
     ret
