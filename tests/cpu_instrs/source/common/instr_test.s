; Framework for CPU instruction tests

; Calls test_instr with each instruction copied
; to instr, with a JP instr_done after it.
; Verifies checksum after testing instruction and
; prints opcode if it's wrong.

.include "checksums.s"
.include "cpu_speed.s"
.include "apu.s"
.include "crc_fast.s"

.define instr $DEF8
.define rp_temp (instr-4)

.define temp   bss

; Sets SP to word at addr
; Preserved: BC, DE
.macro ldsp ; addr
     ld   a,(\1)
     ld   l,a
     ld   a,((\1)+1)
     ld   h,a
     ld   sp,hl
.endm

main:
     call cpu_fast
     call init_crc_fast
     call checksums_init
     set_test 0
     
     ld   hl,instrs
-    ; Copy instruction
     ld   a,(hl+)
     ld   (instr),a
     ld   a,(hl+)
     ld   (instr+1),a
     ld   a,(hl+)
     ld   (instr+2),a
     push hl

     ; Put JP instr_done after it
     ld   a,$C3
     ld   (instr+3),a
     ld   a,<instr_done
     ld   (instr+4),a
     ld   a,>instr_done
     ld   (instr+5),a
     
     call reset_crc
     call test_instr
     
     call checksums_compare
     jr   z,passed
     
     set_test 1
     ld   a,(instr)
     call print_a
     cp   $CB
     jr   nz,+
     ld   a,(instr+1)
     call print_a
+    

passed:
     ; Next instruction
     pop  hl
     ld   a,l
     cp   <instrs_end
     jr   nz,-
     ld   a,h
     cp   >instrs_end
     jr   nz,-
     
     jp   tests_done


; Updates checksum with AF, BC, DE, and HL
checksum_af_bc_de_hl:
     push hl
     
     push af
     update_crc_fast
     pop  hl
     ld   a,l
     update_crc_fast
     
     ld   a,b
     update_crc_fast
     ld   a,c
     update_crc_fast
     
     ld   a,d
     update_crc_fast
     ld   a,e
     update_crc_fast
     
     pop  de
     ld   a,d
     update_crc_fast
     ld   a,e
     update_crc_fast
     ret
