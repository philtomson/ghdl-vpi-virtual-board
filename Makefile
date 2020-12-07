TARGET  = virtual_board.vpi
BUILD   = build

SRC += src/led.cc
SRC += src/rgbled.cc
SRC += src/segseven.cc
SRC += src/push_button.cc
SRC += src/switch.cc
SRC += src/board_window.cc
SRC += src/virtual_board.cc
SRC += src/VBMessage.cc
SRC += src/Instance.cc
SRC += src/InspectorWindow.cc
SRC += src/vpi.cc

OBJS     = $(addprefix $(BUILD)/, $(notdir $(SRC:.cc=.o)))
OBJS    += $(BUILD)/resources.o
CFLAGS  += -W -Wall -Wno-write-strings -O2 -Isrc `pkg-config --cflags gtkmm-3.0`
LDFLAGS += `pkg-config --libs gtkmm-3.0` -lm -lrt
GHDL     = ghdl
#GHDL     = /opt/ghdl-0.35-mcode/bin/ghdl
#GHDL     = /opt/ghdl-0.36-mcode/bin/ghdl
#GHDL     = /opt/ghdl-0.37-mcode/bin/ghdl
#GHDL     = /opt/ghdl-0.37.0-mcode/bin/ghdl
#GHDL     = /opt/ghdl-nightly-mcode/bin/ghdl
CXX      = g++
GHDLCXX  = $(GHDL) --vpi-compile $(CXX)
GHDLLD   = $(GHDL) --vpi-link $(CXX)

#EXEC_TOP_UNIT = example_direct
EXEC_TOP_UNIT = example_mux

## In VPI we should free the callback handels that we do not need (to cancel the callback) 
## to prevent memory leeks.
## However with GHDL < nightly, freeing the callback handle raises
## an exception when the callback is called
GHDL_VERSION = $(shell $(GHDL) -v | grep -o -e '^GHDL\s\+[0-9\.]\+' | grep -o -e '[0-9\.]\+')
ifneq (,$(filter $(GHDL_VERSION), 0.33 0.34 0.35))
$(error Please use a GHDL with version >= 0.36)
endif
ifneq (,$(filter $(GHDL_VERSION), 0.36 0.37 0.37.0))
CFLAGS += -DVPI_DO_NOT_FREE_CALLBACK_HANDLES
endif


all: $(BUILD)/$(TARGET)

$(BUILD)/resources.c: src/resources.gresource.xml
	@mkdir -p $(@D)
	glib-compile-resources --internal --target=$@ --generate-source $<

$(BUILD)/resources.o: $(BUILD)/resources.c
	$(CXX) $(CFLAGS) -c $^ -o $@

$(BUILD)/%.o: src/%.cc
	@mkdir -p $(@D)
	$(GHDLCXX) $(CFLAGS) -c -o $@ $<

$(BUILD)/$(TARGET): $(OBJS)
	$(GHDLLD) $(CFLAGS) $^ -o $@ $(LDFLAGS)

exec: $(BUILD)/$(TARGET)
	$(GHDL) -a app.vhdl
	$(GHDL) -e $(EXEC_TOP_UNIT)
	$(GHDL) -r $(EXEC_TOP_UNIT) --vpi=./$(BUILD)/$(TARGET)

debug:  $(BUILD)/$(TARGET)
	$(GHDL) -a app.vhdl
	$(GHDL) -e $(EXEC_TOP_UNIT)
	gdbtui --args $(GHDL) -r $(EXEC_TOP_UNIT) --vpi=./$(BUILD)/$(TARGET)

clean:
	@rm -rvf $(BUILD)
	@rm -vf work-*.cf
	@rm -vf *.o
	@rm -vf test
	@rm -vf *.vcd
	@rm -vf trace.txt

