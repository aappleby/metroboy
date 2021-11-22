; Tests instructions that don't fit template

.include "shell.inc"

main:
     set_test 2,"JR negative"
     ld   a,0
     jp   jr_neg
     inc  a
-    inc  a
     inc  a
     cp   2
     jp   nz,test_failed
     jp   +
jr_neg:
     jr   -
+
     
     set_test 3,"JR positive"
     ld   a,0
     jr   +
     inc  a
+    inc  a
     inc  a
     cp   2
     jp   nz,test_failed
     
     
     set_test 4,"LD PC,HL"
     ld   hl,+
     ld   a,0
     ld   pc,hl
     inc  a
+    inc  a
     inc  a
     cp   2
     jp   nz,test_failed
     

     set_test 5,"POP AF"
     ld   bc,$1200
-    push bc
     pop  af
     push af
     pop  de
     ld   a,c
     and  $F0
     cp   e
     jp   nz,test_failed
     inc  b
     inc  c
     jr   nz,-
     
     
     set_test 6,"DAA"
     ; Test all combinations of A and flags (256*16 total)
     ld   de,0
-    push de
     pop  af
     daa
     
     push af
     call update_crc
     pop  hl
     ld   a,l
     call update_crc
     
     inc  d
     jr   nz,-
     
     ld   a,e
     add  $10
     ld   e,a
     jr   nz,-
     
     check_crc $6A9F8D8A
     
     jp   tests_passed
