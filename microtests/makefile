ALL_GB = $(patsubst %.s,build/%.gb, $(wildcard *.s))

help:
	@echo "make (dmg|ags)"

ags: MODEL = AGS
ags: $(ALL_GB)

dmg: MODEL = DMG
dmg: $(ALL_GB)

build/%.o: %.s
	@echo compiling $<
	@wla-gb -D$(MODEL) -o $@ $< 

build/%.gb: build/%.o
	@echo linking $@
	@printf "[objects]\n%s\n" $< > temp.link
	@wlalink -S temp.link $@
	@rm $<
	@rm temp.link

clean:
	@rm build/*.gb
	@rm build/*.sym

.PHONY: all clean
