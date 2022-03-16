SRCS=$(wildcard *.S)
OBJS=$(SRCS:.S=.o)
ELFS=$(SRCS:.S=.elf)
BINS=$(SRCS:.S=.text.vh) $(SRCS:.S=.data.vh)

all: ${BINS}

%.o: %.S test_macros.h riscv_test.h
	riscv64-unknown-elf-gcc -c -mabi=ilp32 -march=rv32i -o $@ $<

%.elf: %.o 
	riscv64-unknown-elf-ld -Tlink.ld -melf32lriscv $< -o $@

%.text.bin: %.elf
	riscv64-unknown-elf-objcopy -O binary $< $@ -j .init -j .text --change-addresses -0x400000

%.data.bin: %.elf
	riscv64-unknown-elf-objcopy -O binary $< $@ -j .data -j .eh_frame -j .fini_array -j .init_array --change-addresses -0x80000000

%.vh: %.bin
	srec_cat $< -binary -byte-swap 4 -O $@ -VMem 32

clean:
	rm -f ${OBJS} ${ELFS} ${BINS}
