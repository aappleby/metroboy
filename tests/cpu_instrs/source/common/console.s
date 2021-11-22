; Scrolling text console

; Console is 20x18 characters. Buffers lines, so
; output doesn't appear until a newline or flush.
; If scrolling isn't supported (i.e. SCY is treated
; as if always zero), the first 18 lines will
; still print properly). Also works properly if
; LY isn't supported (always reads back as the same
; value).

.define console_width 20

.define console_buf      bss+0
.define console_pos      bss+console_width
.define console_mode     bss+console_width+1
.define console_scroll   bss+console_width+2
.redefine bss            bss+console_width+3


; Waits for start of LCD blanking period
; Preserved: BC, DE, HL
console_wait_vbl:
     push bc
     
     ; Wait for start of vblank, with
     ; timeout in case LY doesn't work
     ; or LCD is disabled.
     ld   bc,-1250
-    inc  bc
     ld   a,b
     or   c
     jr   z,@timeout
     lda  LY
     cp   144
     jr   nz,-
@timeout:
     
     pop  bc
     ret


; Initializes text console
console_init:
     call console_hide
     
     ; CGB-specific inits
     ld   a,(gb_id)
     and  gb_id_cgb
     call nz,@init_cgb
     
     ; Clear nametable
     ld   a,' '
     call @fill_nametable
     
     ; Load tiles
     ld   hl,TILES+$200
     ld   c,0
     call @load_tiles
     ld   hl,TILES+$A00
     ld   c,$FF
     call @load_tiles
     
     ; Init state
     ld   a,console_width
     ld   (console_pos),a
     ld   a,0
     ld   (console_mode),a
     ld   a,-8
     ld   (console_scroll),a
     call console_scroll_up_
     jr   console_show

@fill_nametable:
     ld   hl,BGMAP0
     ld   b,4
-    ld   (hl),a
     inc  l
     jr   nz,-
     inc  h
     dec  b
     jr   nz,-
     ret

@init_cgb:
     ; Clear palette
     wreg $FF68,$80
     ld   b,16
-    wreg $FF69,$FF
     wreg $FF69,$7F
     wreg $FF69,$00
     wreg $FF69,$00
     wreg $FF69,$00
     wreg $FF69,$00
     wreg $FF69,$00
     wreg $FF69,$00
     dec  b
     jr   nz,-
     
     ; Clear attributes
     ld   a,1
     ld   (VBK),a
     ld   a,0
     call @fill_nametable
     
     ld   a,0
     ld   (VBK),a
     ret

@load_tiles:
     ld   de,ASCII
     ld   b,96
--   push bc
     ld   b,8
-    ld   a,(de)
     inc  de
     xor  c
     ldi  (hl),a
     ldi  (hl),a
     dec  b
     jr   nz,-
     pop  bc
     dec  b
     jr   nz,--
     ret


; Shows console display
; Preserved: AF, BC, DE, HL
console_show:
     push af
     
     ; Enable LCD
     call console_wait_vbl
     wreg LCDC,$91
     wreg SCX,0
     wreg BGP,$E4
     
     jp   console_apply_scroll_


; Hides console display by turning LCD off
; Preserved: AF, BC, DE, HL
console_hide:
     push af
     
     ; LCD off
     call console_wait_vbl
     wreg LCDC,$11
     
     pop  af
     ret


; Changes to normal text mode
; Preserved: BC, DE, HL
console_normal:
     xor  a
     jr   console_set_mode

; Changes to inverse text mode
; Preserved: BC, DE, HL
console_inverse:
     ld   a,$80

; Changes console mode to A.
; 0: Normal, $80: Inverse
; Preserved: BC, DE, HL
console_set_mode:
     and  $80
     ld   (console_mode),a
     ret


; Prints char A to console. Will not appear until
; a newline or flush occurs.
; Preserved: AF, BC, DE, HL
console_print:
     push af
     
     cp   10
     jr   z,console_newline_
     
     push hl
     push af
     ld   hl,console_pos
     ldi  a,(hl)
     cp   <console_buf
     jr   nz,@not_at_end
     
     ; Newline if at end of current line. If this
     ; were done after writing to buffer, calling
     ; console_newline would print extra newline.
     ; Doing it before eliminates this.
     
     ; Ignore any spaces at end of line
     pop  af
     cp   ' '
     jr   z,@ignore_space
     call console_newline
     push af
     
@not_at_end:
     pop  af
     or   (hl)      ; apply current attributes
     dec  l         ; hl = console_pos
     dec  (hl)      ; console_pos = console_pos - 1
     ld   l,(hl)    ; hl = position in buffer
     ld   (hl),a
     
@ignore_space
     pop  hl
     pop  af
     ret


; Displays current line and starts new one
; Preserved: AF, BC, DE, HL
console_newline:
     push af
console_newline_:
     call console_wait_vbl
     call console_flush_
     call console_scroll_up_
     call console_flush_
     jp   console_apply_scroll_


console_scroll_up_:
     push bc
     push hl
     
     ; Scroll up 8 pixels
     ld   a,(console_scroll)
     add  8
     ld   (console_scroll),a
     
     ; Start new clear line
     ld   a,' '
     ld   hl,console_buf + console_width - 1
     ld   b,console_width
-    ldd  (hl),a
     dec  b
     jr   nz,-
     ld   a,<(console_buf + console_width)
     ld   (console_pos),a
     
     pop  hl
     pop  bc
     ret


; Displays current line's contents without scrolling.
; Preserved: A, BC, DE, HL
console_flush:
     push af
     call console_wait_vbl
     call console_flush_
console_apply_scroll_:
     ld   a,(console_scroll)
     sub  136
     sta  SCY
     pop  af
     ret
     
console_flush_:
     push de
     push hl
     
     ; Address of row in nametable
     ld   a,(console_scroll)
     ld   l,a
     ld   h,(>BGMAP0) >> 2
     add  hl,hl
     add  hl,hl
     
     ; Copy line
     ld   de,console_buf + console_width
-    dec  e
     ld   a,(de)
     ldi  (hl),a
     ld   a,e
     cp   <console_buf
     jr   nz,-
     
     pop  hl
     pop  de
     ret


ASCII:
     .incbin "console.bin"
