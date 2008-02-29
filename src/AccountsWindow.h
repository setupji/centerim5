/*
 * Copyright (C) 2007 by Mark Pustjens <pustjens@dds.nl>
 *
 * This file is part of CenterIM.
 *
 * CenterIM is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * CenterIM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * */

#ifndef __ACCOUNTSWINDOW_H__
#define __ACCOUNTSWINDOW_H__

#include "Log.h"
#include "Conf.h"

#include <cppconsui/Window.h>
#include <cppconsui/ListBox.h>
#include <cppconsui/HorizontalListBox.h>
#include <cppconsui/Panel.h>
#include <cppconsui/HorizontalLine.h>

class AccountsWindow
: public Window
{
	public:
		AccountsWindow();

	protected:

	private:
		~AccountsWindow();

		void Populate(void);

		void Add(void);
		void Edit(void);
		void Delete(void);

		Log *log;
		Conf *conf;

		ListBox *accounts;
		HorizontalListBox *menu;
		HorizontalLine *line;
		Panel *border;
};

#endif /* __ACCOUNTSWINDOW_H__ */