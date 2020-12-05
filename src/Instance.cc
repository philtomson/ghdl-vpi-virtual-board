#include <iostream>
#include "Instance.hh"

ModuleNet::ModuleNet() :
	name(),
	width(0),
	direction(0),
	handle(nullptr),
	value(),
	value_changed(false)
{
}


std::string ModuleNet::to_s()
{
	std::string str = name + std::string("[") + std::to_string(width) + std::string("]");
	switch (direction) {
		case 1:
			str += std::string(" \e[32mInput\e[0m");
			break;
		case 2:
			str += std::string(" \e[31mOutput\e[0m");
			break;
		case 3:
			str += std::string(" \e[33mInOut\e[0m");
			break;
	}

	return str;
}


ModuleInstance::ModuleInstance() :
	name(),
	modules(),
	nets(),
	signal_liststore()
{
}


void ModuleInstance::print(std::string b, int c)
{
	const int nb_nets = nets.size();
	const int nb_mods = modules.size();
	const int nbmpnmo = nb_nets + nb_mods - 1;
	int i, j;
	std::string d(b);

	switch (c) {
		case 1:
			d += std::string("└─ ");
			break;
		case 2:
			d += std::string("├─ ");
			break;
	}
	std::cout << d << "\e[1;32mModule\e[0m " << name << std::endl;
	switch (c) {
		case 1:
			b += std::string("   ");
			break;
		case 2:
			b += std::string("│  ");
			break;
	}
	for (i = 0; i < nb_nets; i++) {
		std::string bb = std::string(b);
		if (i == nbmpnmo)
			bb += std::string("└─ ");
		else
			bb += std::string("├─ ");
		std::cout << bb << "\e[1;34mNet\e[0m " << nets[i].to_s() << std::endl;
	}
	for (i = 0, j = nb_nets; i < nb_mods; i++, j++) {
		modules[i].print(b, (j == nbmpnmo) ? 1 : 2);
	}
}


