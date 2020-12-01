TARGET  = virtual_board.vpi
BUILD   = build

SRC += src/led.cc
SRC += src/rgbled.cc
SRC += src/push_button.cc
SRC += src/switch.cc
SRC += src/segseven.cc
SRC += src/board_window.cc
SRC += src/main.cc

OBJS     = $(addprefix $(BUILD)/, $(notdir $(SRC:.cc=.o)))
OBJS    += $(BUILD)/resources.o
CFLAGS  += -W -Wall -O0 -g -Isrc `pkg-config --cflags gtkmm-3.0`
LDFLAGS += `pkg-config --libs gtkmm-3.0` -lrt
CXX      = g++

all: $(BUILD)/$(TARGET)

$(BUILD)/resources.c: src/resources.gresource.xml
	@mkdir -p $(@D)
	glib-compile-resources --internal --target=$@ --generate-source $<

$(BUILD)/resources.o: $(BUILD)/resources.c
	$(CXX) $(CFLAGS) -c $^ -o $@

$(BUILD)/%.o: src/%.cc
	@mkdir -p $(@D)
	$(CXX) $(CFLAGS) -c -o $@ $<

$(BUILD)/$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) $^ -o $@ $(LDFLAGS)

exec: $(BUILD)/$(TARGET)
	./$<

clean:
	@rm -rvf $(BUILD)

