.include "header.inc"

; We should be able to write our dotted line pattern to vram on startup.

main:
	ld a, $55
	ld ($8000), a
	jr main