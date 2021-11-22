; Main printing routine that checksums and
; prints to output device

; Character that does equivalent of print_newline
.define newline 10

; Prints char without updating checksum
; Preserved: BC, DE, HL
.define print_char_nocrc bss
.redefine bss            bss+3


; Initializes printing. HL = print routine
init_printing:
     ld   a,l
     ld   (print_char_nocrc+1),a
     ld   a,h
     ld   (print_char_nocrc+2),a
     jr   show_printing


; Hides/shows further printing
; Preserved: BC, DE, HL
hide_printing:
     ld   a,$C9     ; RET
     jr   +
show_printing:
     ld   a,$C3     ; JP (nn)
+    ld   (print_char_nocrc),a
     ret


; Prints character and updates checksum UNLESS
; it's a newline.
; Preserved: AF, BC, DE, HL
print_char:
     push af
     cp   newline
     call nz,update_crc
     call print_char_nocrc
     pop  af
     ret


; Prints space. Does NOT update checksum.
; Preserved: AF, BC, DE, HL
print_space:
     push af
     ld   a,' '
     call print_char_nocrc
     pop  af
     ret


; Advances to next line. Does NOT update checksum.
; Preserved: AF, BC, DE, HL
print_newline:
     push af
     ld   a,newline
     call print_char_nocrc
     pop  af
     ret


; Prints immediate string
; Preserved: AF, BC, DE, HL
.macro print_str ; string,string2
     push hl
     call print_str_
     .byte \1
     .if NARGS > 1
          .byte \2
     .endif
     .if NARGS > 2
          .byte \3
     .endif
     .byte 0
     pop  hl
.endm

print_str_:
     pop  hl
     call print_str_hl
     jp   hl


; Prints zero-terminated string pointed to by HL.
; On return, HL points to byte AFTER zero terminator.
; Preserved: AF, BC, DE
print_str_hl:
     push af
     jr   +
-    call print_char
+    ldi  a,(hl)
     or   a
     jr   nz,-
     pop  af
     ret
