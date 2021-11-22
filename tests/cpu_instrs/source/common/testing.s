; Diagnostic and testing utilities

.define result      bss+0
.define test_name   bss+1
.redefine bss       bss+3


; Sets test code and optional error text
; Preserved: AF, BC, DE, HL
.macro set_test ; code[,text[,text2]]
     push hl
     call set_test_
     jr   @set_test\@
     .byte \1
     .if NARGS > 1
          .byte \2
     .endif
     .if NARGS > 2
          .byte \3
     .endif
     .byte 0
@set_test\@:
     pop  hl
.endm

set_test_:
     pop  hl
     push hl
     push af
     inc  hl
     inc  hl
     ldi  a,(hl)
     ld   (result),a
     ld   a,l
     ld   (test_name),a
     ld   a,h
     ld   (test_name+1),a
     pop  af
     ret


; Initializes testing module
init_testing:
     set_test $FF
     call init_crc
     ret


; Reports "Passed", then exits with code 0
tests_passed:
     call print_newline
     print_str "Passed"
     ld   a,0
     jp   exit


; Reports "Done" if set_test has never been used,
; "Passed" if set_test 0 was last used, or
; failure if set_test n was last used.
tests_done:
     ld   a,(result)
     inc  a
     jr   z,+
     dec  a
     jr   z,tests_passed
     jr   test_failed
+    print_str "Done"
     ld   a,0
     jp   exit


; Reports current error text and exits with result code
test_failed:
     ld   a,(test_name)
     ld   l,a
     ld   a,(test_name+1)
     ld   h,a
     ld   a,(hl)
     or   a
     jr   z,+
     call print_newline
     call print_str_hl
     call print_newline
+    
     ld   a,(result)
     cp   1         ; if a = 0 then a = 1
     adc  0
     jp   exit


; Prints checksum as 8-character hex value
; Preserved: AF, BC, DE, HL
print_crc:
     push af
     
     ; Must read checksum entirely before printing,
     ; since printing updates it.
     lda  checksum
     cpl
     push af
     
     lda  checksum+1
     cpl
     push af
     
     lda  checksum+2
     cpl
     push af
     
     lda  checksum+3
     cpl
     
     call print_hex
     pop  af
     call print_hex
     pop  af
     call print_hex
     pop  af
     call print_a
     
     pop  af
     ret


; If checksum doesn't match expected, reports failed test.
; Passing 0 just prints checksum. Clears checksum afterwards.
.macro check_crc ARGS crc
     .if crc == 0
          call show_printing
          call print_newline
          call print_crc
     .else
          ld   bc,(crc >> 16) ~ $FFFF
          ld   de,(crc & $FFFF) ~ $FFFF
          call check_crc_
     .endif
.endm

check_crc_:
     lda  checksum+0
     cp   e
     jr   nz,+
     
     lda  checksum+1
     cp   d
     jr   nz,+
     
     lda  checksum+2
     cp   c
     jr   nz,+
     
     lda  checksum+3
     cp   b
     jr   nz,+
     
     jp   reset_crc
     
+    call print_crc
     jp   test_failed


; Updates checksum with bytes from addr to addr+size-1
.macro checksum_mem ARGS addr,size
     ld   hl,addr
     ld   bc,size
     call checksum_mem_
.endm

checksum_mem_:
-    ldi  a,(hl)
     call update_crc
     dec  bc
     ld   a,b
     or   c
     jr   nz,-
     ret
