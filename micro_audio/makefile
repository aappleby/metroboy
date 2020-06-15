ALL_GB_SOURCE = $(wildcard *.s)
ALL_GB_OBJ    = $(patsubst %.s,%.o, $(ALL_GB_SOURCE))
ALL_GB_BINARY = $(patsubst %.s,%.gb, $(ALL_GB_SOURCE))
ALL_GB_LINK   = $(patsubst %.s,%.link, $(ALL_GB_SOURCE))

DMG_GB_LINK = $(addprefix build/dmg/, $(ALL_GB_LINK))

DMG_GB_BINARY = $(addprefix build/dmg/, $(ALL_GB_BINARY))
AGS_GB_BINARY = $(addprefix build/ags/, $(ALL_GB_BINARY))

all: $(DMG_GB_BINARY)

#all: $(AGS_GB_BINARY) $(DMG_GB_BINARY)

#all: $(AGS_GB_BINARY)

dmg_link: $(DMG_GB_LINK)

build/dmg/%.link: %.s
	@echo $@
	@echo [objects] > $@
	@echo $(patsubst %.link,%.o, $(notdir $@)) >> $@

build/dmg/%.gb: %.s
	@echo $@
	@wla-gb -DDMG -o build/temp.o $<
	@wlalink -S temp.link $@

build/ags/%.gb: %.s
	@echo $@
	@wla-gb -DAGS -o build/temp.o $<
	@wlalink -S temp.link $@

clean:
	@rm -f build/ags/*
	@rm -f build/dmg/*
	@rm -f build/temp.o

.PHONY: all clean
