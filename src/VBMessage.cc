#include "VBMessage.hh"


VBMessage::VBMessage() :
	m_type(VBMessage::MSG_NONE),
	m_value(0),
	m_io_name(VBMessage::NONE)
{
}


VBMessage::VBMessage(const VBMessage& other) :
	m_type(other.m_type),
	m_value(other.m_value),
	m_io_name(other.m_io_name)
{
}


VBMessage::VBMessage(VBMessage::message_type_t type) :
	m_type(type),
	m_value(0),
	m_io_name(VBMessage::NONE)
{
}


VBMessage::VBMessage(VBMessage::message_type_t type, unsigned int value) :
	m_type(type),
	m_value(value),
	m_io_name(VBMessage::NONE)
{
}


VBMessage::VBMessage(VBMessage::message_type_t type, unsigned int value, VBMessage::IO_name_t io_name) :
	m_type(type),
	m_value(value),
	m_io_name(io_name)
{
}


VBMessage::message_type_t VBMessage::type() const
{
	return m_type;
}


unsigned int VBMessage::value() const
{
	return m_value;
}


VBMessage::IO_name_t VBMessage::io_name() const
{
	return m_io_name;
}


VBMessage VBMessage::exit()
{
	VBMessage msg(VBMessage::MSG_EXIT);
	return msg;
}


VBMessage VBMessage::run()
{
	VBMessage msg(VBMessage::MSG_RUN);
	return msg;
}


VBMessage VBMessage::run_n(unsigned int n)
{
	VBMessage msg(VBMessage::MSG_RUN_N, n);
	return msg;
}


VBMessage VBMessage::stop()
{
	VBMessage msg(VBMessage::MSG_STOP);
	return msg;
}


VBMessage VBMessage::set_freq(unsigned int n)
{
	VBMessage msg(VBMessage::MSG_SET_FREQ, n);
	return msg;
}


VBMessage VBMessage::update_signals()
{
	VBMessage msg(VBMessage::MSG_UPDATE_SIGNALS);
	return msg;
}


VBMessage VBMessage::io_changed(VBMessage::IO_name_t io_name, unsigned int value)
{
	VBMessage msg(VBMessage::MSG_IO_CHANGED, value, io_name);
	return msg;
}


VBMessage VBMessage::clock()
{
	VBMessage msg(VBMessage::MSG_CLOCK);
	return msg;
}


VBMessage VBMessage::stopped()
{
	VBMessage msg(VBMessage::MSG_STOPPED);
	return msg;
}


VBMessage VBMessage::running()
{
	VBMessage msg(VBMessage::MSG_RUNNING);
	return msg;
}


VBMessage VBMessage::signals_updated()
{
	VBMessage msg(VBMessage::MSG_SIGNALS_UPDATED);
	return msg;
}


VBMessage VBMessage::gui_started()
{
	VBMessage msg(VBMessage::MSG_GUI_STARTED);
	return msg;
}


