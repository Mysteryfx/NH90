#pragma once

#include "ExpImpSpec.h" 

//------------------------------------------------------------------------------
struct USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR HotKey
{
	enum
	{
		ALT_KEY		= 1,
		CTRL_KEY	= 2,
		SHIFT_KEY	= 4
	};

	int				VirtKey;
	int				ModifyKey;
public:
	HotKey(int inModifyKey, int inVirtKey) : VirtKey(inVirtKey), ModifyKey(inModifyKey)
	{
	}
	friend bool operator < (const HotKey& in_a, const HotKey& in_b)
	{
		if (in_a.VirtKey != in_b.VirtKey) return in_a.VirtKey < in_b.VirtKey;
		else
			return in_a.ModifyKey < in_b.ModifyKey;
	}
};
//------------------------------------------------------------------------------
class Control;
struct CommandAndHandler
{
	int				Command;
	Control*		Object;
public:
	CommandAndHandler(int inCommand, Control* inHandler) : Command(inCommand), Object(inHandler)
	{
	}
};
//------------------------------------------------------------------------------
typedef ed::map<HotKey, CommandAndHandler>	CommandAndHandlerTable;
class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR AccelerateTable
{
	static CommandAndHandlerTable		m_Table;

	static bool		detachOne(const Control* inHandler);
public:
	static void		attach(const HotKey& inKey, int inCommand, Control* inHandler);
	static void		detach(const HotKey& inKey); 
	static void		detach(const Control* inHandler); 

	static bool		process(int inVirtKey, int inKeyData);

	static void		clear();
};
//------------------------------------------------------------------------------