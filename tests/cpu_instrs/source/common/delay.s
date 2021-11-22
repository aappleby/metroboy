; Delays in cycles, milliseconds, etc.

; All routines are re-entrant (no global data). Routines never
; touch BC, DE, or HL registers. These ASSUME CPU is at normal
; speed. If running at double speed, msec/usec delays are half advertised.

; Delays n cycles, from 0 to 16777215
; Preserved: AF, BC, DE, HL
.macro delay ARGS n
     .if n < 0
          .printt "Delay must be >= 0"
          .fail
     .endif
     .if n > 16777215
          .printt "Delay must be < 16777216"
          .fail
     .endif
     delay_ n&$FFFF, n>>16
.endm

; Delays n clocks, from 0 to 16777216*4. Must be multiple of 4.
; Preserved: AF, BC, DE, HL
.macro delay_clocks ARGS n
     .if n # 4 != 0
          .printt "Delay must be a multiple of 4"
          .fail
     .endif
     delay_ (n/4)&$FFFF,(n/4)>>16
.endm

; Delays n microseconds (1/1000000 second)
; n can range from 0 to 4000 usec.
; Preserved: AF, BC, DE, HL
.macro delay_usec ARGS n
     .if n < 0
          .printt "Delay must be >= 0"
          .fail
     .endif
     .if n > 4000
          .printt "Delay must be <= 4000 usec"
          .fail
     .endif
     delay_ ((n * 1048576 + 500000) / 1000000)&$FFFF,((n * 1048576 + 500000) / 1000000)>>16
.endm

; Delays n milliseconds (1/1000 second)
; n can range from 0 to 10000 msec.
; Preserved: AF, BC, DE, HL
.macro delay_msec ARGS n
     .if n < 0
          .printt "Delay must be >= 0"
          .fail
     .endif
     .if n > 10000
          .printt "Delay must be <= 10000 msec"
          .fail
     .endif
     delay_ ((n * 1048576 + 500) / 1000)&$FFFF, ((n * 1048576 + 500) / 1000)>>16
.endm

     ; All the low/high quantities are to deal wla-dx's asinine
     ; restriction full expressions must evaluate to a 16-bit
     ; value. If the author ever rectifies this, all "high"
     ; arguments can be treated as zero and removed. Better yet,
     ; I'll just find an assembler that didn't crawl out of
     ; the sewer (this is one of too many bugs I've wasted
     ; hours working around).

     .define max_short_delay 28
     
     .macro delay_long_ ARGS n, high
          ; 0+ to avoid assembler treating as memory read
          ld   a,0+(((high<<16)+n) - 11) >> 16
          call delay_65536a_9_cycles_
          delay_nosave_ (((high<<16)+n) - 11)&$FFFF, 0
     .endm
     
     ; Doesn't save AF, allowing minimization of AF save/restore
     .macro delay_nosave_ ARGS n, high
          ; 65536+11     = maximum delay using delay_256a_9_cycles_
          ; 255+22  = maximum delay using delay_a_20_cycles
          ; 22      = minimum delay using delay_a_20_cycles
          .if high > 1
               delay_long_ n, high
          .else
               .if high*n > 11
                    delay_long_ n, high
               .else
                    .if (high*(255+22+1))|n > 255+22
                         ld   a,>(((high<<16)+n) - 11)
                         call delay_256a_9_cycles_
                         delay_nosave_ <(((high<<16)+n) - 11), 0
                    .else
                         .if n >= 22
                              ld   a,n - 22
                              call delay_a_20_cycles
                         .else
                              delay_short_ n
                         .endif
                    .endif
               .endif
          .endif
     .endm
     
     .macro delay_ ARGS low, high
          .if (high*(max_short_delay+1))|low > max_short_delay
               push af
               delay_nosave_ ((high<<16)+low - 7)&$FFFF, ((high<<16)+low - 7)>>16
               pop  af
          .else
               delay_short_ low
          .endif
     .endm


; Delays A cycles + overhead
; Preserved: BC, DE, HL
; Time: A+20 cycles (including CALL)
delay_a_20_cycles:
-    sub  5    ; 2
     jr   nc,- ;3/2 do multiples of 5
     rra       ; 1
     jr   nc,+ ;3/2 bit 0
+    adc  1    ; 2
     ret  nc   ;5/2 -1: 0 cycles
     ret  z    ;5/2  0: 2 cycles
     nop       ; 1   1: 4 cycles
     ret       ; 4 (thanks to dclxvi for original algorithm)

; Delays A*256 cycles + overhead
; Preserved: BC, DE, HL
; Time: A*256+12 cycles (including CALL)
delay_256a_12_cycles:
     or   a              ; 1
     ret  z              ; 5/2
delay_256a_9_cycles_:
-    delay 256-4
     dec  a              ; 1
     jr   nz,-           ;3/2
     ret                 ; 4

; Delays A*65536 cycles + overhead
; Preserved: BC, DE, HL
; Time: A*65536+12 cycles (including CALL)
delay_65536a_12_cycles:
     or   a              ; 1
     ret  z              ;5/2
delay_65536a_9_cycles_:
-    delay 65536-4
     dec  a              ; 1
     jr   nz,-           ;3/2
     ret                 ; 4

; Delays H*256+L cycles + overhead
; Preserved: AF, BC, DE, HL
; Time: H*256+L+51 cycles
delay_hl_51_cycles:
     push af
     ld   a,h
     call delay_256a_12_cycles
     ld   a,l
     call delay_a_20_cycles
     pop  af
     ret

     ; delay_short_ macro calls into these
     .ds max_short_delay-10,$00 ; NOP repeated several times
delay_unrolled_:
     ret

.macro delay_short_ ARGS n
     .if n < 0
          .fail
     .endif
     .if n > max_short_delay
          .fail
     .endif
     
     .if n == 1
          nop
     .endif
     .if n == 2
          nop
          nop
     .endif
     .if n == 3
          .byte $18,$00 ; JR +0
     .endif
     .if n == 4
          .byte $18,$00 ; JR +0
          nop
     .endif
     .if n == 5
          .byte $18,$00 ; JR +0
          nop
          nop
     .endif
     .if n == 6
          .byte $18,$00 ; JR +0
          .byte $18,$00 ; JR +0
     .endif
     .if n == 7
          push af
          pop  af
     .endif
     .if n == 8
          push af
          pop  af
          nop
     .endif
     .if n == 9
          push af
          pop  af
          nop
          nop
     .endif
     .if n >= 10
          call delay_unrolled_ + 10 - n
     .endif
.endm
