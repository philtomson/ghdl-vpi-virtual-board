#ifndef VB_MESSAGE_HH
#define VB_MESSAGE_HH

class VBMessage {
public:
	typedef enum {
		NONE = 0,
		SWITCH_0,
		SWITCH_1,
		SWITCH_2,
		SWITCH_3,
		SWITCH_4,
		SWITCH_5,
		SWITCH_6,
		SWITCH_7,
		SWITCH_8,
		SWITCH_9,
		SWITCH_10,
		SWITCH_11,
		SWITCH_12,
		SWITCH_13,
		SWITCH_14,
		SWITCH_15,
		PUSH_BUTTON_RSTN,
		PUSH_BUTTON_CENTER,
		PUSH_BUTTON_UP,
		PUSH_BUTTON_DOWN,
		PUSH_BUTTON_RIGHT,
		PUSH_BUTTON_LEFT,
		LED_0,
		LED_1,
		LED_2,
		LED_3,
		LED_4,
		LED_5,
		LED_6,
		LED_7,
		LED_8,
		LED_9,
		LED_10,
		LED_11,
		LED_12,
		LED_13,
		LED_14,
		LED_15,
		RGB_LED_0,
		RGB_LED_1,
		DISPLAY_0,
		DISPLAY_1,
		DISPLAY_2,
		DISPLAY_3,
		DISPLAY_4,
		DISPLAY_5,
		DISPLAY_6,
		DISPLAY_7
	} IO_name_t;

	typedef enum {
		MSG_NONE = 0,
		MSG_EXIT,
		MSG_RUN,
		MSG_RUN_N, // integer
		MSG_STOP,
		MSG_SET_FREQ, // integer
		MSG_UPDATE_SIGNALS,
		MSG_IO_CHANGED, // IO_name_t, integer
		MSG_CLOCK,
		MSG_STOPPED,
		MSG_RUNNING,
		MSG_SIGNALS_UPDATED,
		MSG_GUI_STARTED
	} message_type_t;

private:
	message_type_t m_type;
	unsigned int   m_value;
	IO_name_t      m_io_name;

public:
	VBMessage();
	VBMessage(const VBMessage& other);
	VBMessage(message_type_t type);
	VBMessage(message_type_t type, unsigned int value);
	VBMessage(message_type_t type, unsigned int value, IO_name_t io_name);

	message_type_t type() const;
	unsigned int   value() const;
	IO_name_t      io_name() const;

	static VBMessage exit();
	static VBMessage run();
	static VBMessage run_n(unsigned int n);
	static VBMessage stop();
	static VBMessage set_freq(unsigned int n);
	static VBMessage update_signals();
	static VBMessage io_changed(IO_name_t io_name, unsigned int value);
	static VBMessage clock();
	static VBMessage stopped();
	static VBMessage running();
	static VBMessage signals_updated();
	static VBMessage gui_started();
};

#endif /* VB_MESSAGE_HH */
