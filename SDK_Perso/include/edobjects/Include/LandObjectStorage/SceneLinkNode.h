#pragma once

#include <ed/list.h>

#include "Registry\Registered.h"

namespace BinarySceneAndTree
{
	enum LinkNodeType
	{
		LNT_OBJECT = 0,			// По id
		LNT_AIRFIELD = 1,		// Аэродром
	};
	// Список для составления LinkNode
	struct LinkKeyNode
	{
		// Тип элемента
		LinkNodeType type;
		// Имя элемента (мб NULL)
		ed::string name;
		// Ид. элемента (мб -1)
		ObjectID id;

		LinkKeyNode(){}
		LinkKeyNode(ObjectID id)
		{
			this->type = LNT_OBJECT;
			this->id = id;
		}
		LinkKeyNode(LinkNodeType type, const char* name)
		{
			this->type = type;
			this->name = name;
		}
		bool operator <(const LinkKeyNode& arg) const
		{
			if( this->type < arg.type) return true;
			if( this->type > arg.type) return false;
			if( this->type==LNT_OBJECT)
			{
				return this->id < arg.id;
			}
			else
			{
				return strcmp(this->name.c_str(), arg.name.c_str())<0;
			}
		}
	};
	struct LinkDecl
	{
		LinkKeyNode from;
		LinkKeyNode to;
	};
	typedef ed::list<LinkDecl> tag_LinkDeclList;
}