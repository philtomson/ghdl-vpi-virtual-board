#include "virtual_board.hh"

VirtualBoard::VirtualBoard() :
	m_window(nullptr)
{
}


VirtualBoard::~VirtualBoard()
{
	if (m_window)
		delete m_window;
}


