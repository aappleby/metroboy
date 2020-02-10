
.gbheader
	name "microtest"
	nintendologo
.endgb

.memorymap
  defaultslot 0 
  slot 0 start $0000 size $4000
.endme
.rombanksize $4000
.rombanks 2

.bank 0 slot 0

.org $100
  jp $100
