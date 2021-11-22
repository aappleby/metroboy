; Multiple checksum table handling

.define next_checksum    bss+0
.redefine bss            bss+2

; If PRINT_CHECKSUMS is defined, checksums are printed
; rather than compared.

; Initializes multiple checksum handler to use checksums
; table (defined by user).
; Preserved: BC, DE, HL
checksums_init:
     ld   a,<checksums
     ld   (next_checksum),a
     ld   a,>checksums
     ld   (next_checksum+1),a
     ret

; Compares current checksum with next checksum in
; list. Z if they match, NZ if not.
; Preserved: BC, DE, HL
checksums_compare:
.ifdef PRINT_CHECKSUMS
     lda  checksum+3
     push af
     lda  checksum+2
     push af
     lda  checksum+1
     push af
     lda  checksum+0
     push af
     
     ld   a,(next_checksum)
     inc  a
     ld   (next_checksum),a
     sub  <checksums+1
     and  $03
     ld   a,','
     jr   nz,+
     print_str newline,'.',"byte"
     ld   a,' '
+    call print_char

     pop  af
     call @print_byte
     pop  af
     call @print_byte
     pop  af
     call @print_byte
     ld   a,'$'
     call print_char
     pop  af
     call print_hex
     
     xor  a
     ret
     
@print_byte:
     push af
     ld   a,'$'
     call print_char
     pop  af
     call print_hex
     ld   a,','
     call print_char
     ret
.else
     
     push bc
     push de
     push hl
     ld   a,(next_checksum)
     ld   l,a
     ld   a,(next_checksum+1)
     ld   h,a
     ld   de,checksum
     ld   b,0
-    ld   a,(de)
     xor  (hl)
     or   b
     ld   b,a
     inc  hl
     inc  e
     ld   a,e
     cp   <(checksum+4)
     jr   nz,-
     ld   a,l
     ld   (next_checksum),a
     ld   a,h
     ld   (next_checksum+1),a
     
     ld   a,b
     cp   0
     pop  hl
     pop  de
     pop  bc
     ret
.endif
