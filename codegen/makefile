ALL_GB_SOURCE = $(wildcard *.s)
ALL_GB_BINARY = $(patsubst %.s,%.gb, $(ALL_GB_SOURCE))
DMG_GB_BINARY = $(addprefix build/dmg/, $(ALL_GB_BINARY))

all: $(DMG_GB_BINARY)

build/dmg/%.gb: %.s
	@echo $<
	@wla-gb -DDMG -o build/temp.o $<
	@wlalink -S build/temp.link $@

clean:
	@rm build/dmg/*

.PHONY: all clean