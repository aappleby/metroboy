ALL_GB_SOURCE = $(wildcard *.s)
ALL_GB_OBJ    = $(patsubst %.s,%.o, $(ALL_GB_SOURCE))
ALL_GB_BINARY = $(patsubst %.s,%.gb, $(ALL_GB_SOURCE))

DMG_GB_BINARY = $(addprefix build/dmg/, $(ALL_GB_BINARY))
AGS_GB_BINARY = $(addprefix build/ags/, $(ALL_GB_BINARY))

all: $(AGS_GB_BINARY) $(DMG_GB_BINARY)

build/dmg/%.o : %.s
	@echo compiling $<
	@wla-gb -DDMG -o $@ $< 	

build/dmg/%.gb: build/dmg/%.o
	@echo linking $@
	@printf "[objects]\n%s\n" $< > temp.link
	@wlalink -S temp.link $@
	@rm $<
	@rm temp.link

build/ags/%.o : %.s
	@echo compiling $<
	@wla-gb -DAGS -o $@ $< 	

build/ags/%.gb: build/ags/%.o
	@echo linking $@
	@printf "[objects]\n%s\n" $< > temp.link
	@wlalink -S temp.link $@
	@rm $<
	@rm temp.link

clean:
	@rm build/ags/*
	@rm build/dmg/*

.PHONY: all clean
