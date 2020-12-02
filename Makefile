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
SRC += src/vpi.cc

OBJS     = $(addprefix $(BUILD)/, $(notdir $(SRC:.cc=.o)))
OBJS    += $(BUILD)/resources.o
CFLAGS  += -W -Wall -O2 -Isrc `pkg-config --cflags gtkmm-3.0`
LDFLAGS += `pkg-config --libs gtkmm-3.0` -lrt
GHDL     = ghdl
CXX      = g++
GHDLCXX  = $(GHDL) --vpi-compile $(CXX)
GHDLLD   = $(GHDL) --vpi-link $(CXX)


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
	$(GHDL) -e test
	$(GHDL) -r test --vpi=./$(BUILD)/$(TARGET)

clean:
	@rm -rvf $(BUILD)
	@rm -vf work-*.cf
	@rm -vf *.o
	@rm -vf test

