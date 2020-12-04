/* Copyright (C) 2020 Théotime Bollengier <theotime.bollengier@ensta-bretagne.fr>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MODULE_INSTANCE_HH
#define MODULE_INSTANCE_HH

#include <vector>
#include <string>
#include <vpi_user.h>


class ModuleNet {
public:
	std::string name;
	int         width;
	int         direction; // 0: none, 1: input, 2: output, 3: inout
	vpiHandle   handle;
	std::string value;

	ModuleNet();

	std::string to_s();
};


class ModuleInstance {
public:
	std::string name;
	std::vector<ModuleInstance> modules;
	std::vector<ModuleNet> nets;

	ModuleInstance();

	void print(std::string b = std::string(" "), int c = 0);
};


#endif /* MODULE_INSTANCE_HH */
