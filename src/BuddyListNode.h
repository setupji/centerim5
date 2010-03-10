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

#ifndef _BUDDYLISTNODE_H__
#define _BUDDYLISTNODE_H__

#include "Log.h"
#include "Conf.h"

#include <cppconsui/Button.h>
#include <cppconsui/TreeView.h>

#include <libpurple/blist.h>

class BuddyListNode
: public Button
{
	public:
		BuddyListNode(TreeView& parent, PurpleBlistNode *node);
		virtual ~BuddyListNode();

		virtual void Draw(void);

		static BuddyListNode* CreateNode(TreeView& parent, PurpleBlistNode *node);

		virtual void Update(void) =0;

		virtual void GiveFocus(void);
		virtual void TakeFocus(void);

		/* Actions for keybindings */
		virtual void OnActivate(void) =0;

		BuddyListNode* GetParent(void);

		TreeView::NodeReference ref;

	protected:
		PurpleBlistNode *node;

		TreeView *treeview;

		Conf *conf;

	private:
};

class BuddyListBuddy
: public BuddyListNode
{
	public:
		BuddyListBuddy(TreeView& parent, PurpleBlistNode *node);

		virtual void Draw(void);
		virtual void Update(void);

		virtual void OnActivate(void);

	protected:
		PurpleBuddy *buddy;

	private:
};

class BuddyListChat
: public BuddyListNode
{
	public:
		BuddyListChat(TreeView& parent, PurpleBlistNode *node);

		virtual void Draw(void);
		virtual void Update(void);

		virtual void OnActivate(void);

	protected:
		PurpleChat *chat;

	private:
};

class BuddyListContact
: public BuddyListNode
{
	public:
		BuddyListContact(TreeView& parent, PurpleBlistNode *node);

		virtual void Draw(void);
		virtual void Update(void);

		virtual void OnActivate(void);

	protected:
		PurpleContact *contact;

	private:
};

class BuddyListGroup
: public BuddyListNode
{
	public:
		BuddyListGroup(TreeView& parent, PurpleBlistNode *node);

		virtual void Draw(void);
		virtual void Update(void);

		virtual void OnActivate(void);

	protected:
		PurpleGroup *group;

	private:
};

#endif /* _BUDDYLISTNODE_H__ */
