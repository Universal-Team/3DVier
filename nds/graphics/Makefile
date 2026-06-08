ifeq ($(OS),Windows_NT)
BMP2DS	?= bmp2ds.exe
else
BMP2DS	?= bmp2ds
endif

GRAPHICS	:=	chars ui .

OUT			:=	../nitrofiles/graphics

INPUTS		:= $(foreach dir,$(GRAPHICS),$(addprefix $(dir)/,$(notdir $(wildcard $(dir)/*.bmp))))

TARGETS		:= $(INPUTS:.bmp=.gfx)

.PHONY	:	clean all directories

all	:	$(TARGETS)
	@echo "  GRAPHICS"

%.gfx	:	%.bmp %.txt
	@mkdir -p $(OUT)/$(@D)
	@$(BMP2DS) $< $(OUT)/$@ -o $(shell cat $(word 2,$^))

%.gfx	:	%.bmp
	@mkdir -p $(OUT)/$(@D)
	@$(BMP2DS) $< $(OUT)/$@

clean	:
	@echo "  CLEAN"
	@rm -rf $(addprefix $(OUT)/,$(TARGETS))