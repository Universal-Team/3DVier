ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/ds_rules

ifneq ($(OS),Windows_NT)
BMP2DS	?= bmp2ds
else
BMP2DS	?= bmp2ds.exe
endif

GRAPHICS	:=	chars ui .

OUT			:=	../nitrofiles/graphics

INPUTS		:= $(foreach dir,$(GRAPHICS),$(addprefix $(dir)/,$(notdir $(wildcard $(dir)/*.bmp))))

TARGETS		:= $(INPUTS:.bmp=.gfx)

.PHONY	:	clean all directories

all	:	$(TARGETS)
	@echo graphics ...

%.gfx	:	%.bmp %.txt
	@mkdir -p $(OUT)/$(@D)
	@$(BMP2DS) $< $(OUT)/$@ -o $(shell cat $(word 2,$^))

%.gfx	:	%.bmp
	@mkdir -p $(OUT)/$(@D)
	@$(BMP2DS) $< $(OUT)/$@

clean	:
	@echo clean ...
	@rm -rf $(addprefix $(OUT)/,$(TARGETS))