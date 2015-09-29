#ifndef __USERINTERFACE_RESPARSER_H__
#define __USERINTERFACE_RESPARSER_H__

#include <ed/string.h>
#include <ed/sstream.h>
#include <ed/list.h>
#include <ed/vector.h>

#include "ui_Point.h"
#include "ExpImpSpec.h"

class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR ResParser
{
public:
	ResParser(const char* text, int textSize);
	void setAvailableChars(const char* chars) { m_availableChars = chars; }

	/**
	 * Get next command and prepare to read argument list
	 * @retval false no more commands available
	 */
	bool nextCommand(ed::string& cmd);

	bool get(bool&);	// Read bool argument, retval false if there is no bool
	bool get(int&);		// Read int argument, retval false if there is no int
	bool get(long&);	// Read long argument, retval false if there is no int
	bool get(DWORD&);	// Read long argument, retval false if there is no int
	bool get(float&);	// Read float argument, retval false if there is no float
	bool get(double&);	// Read double argument, retval false if there is no double
	bool get(ed::string&);	// Read string 
	bool get(Point& point);	// Read coordinates

	template<class T>		
	bool get(ed::list<T>& lst)	// Read list of values
	{
		T val;
		while (get(val))
			lst.insert(lst.end(), val);
		return lst.size() > 0;
	}

	template<class T>
	bool get(ed::vector<T>& vec, bool append = false)	// Read vector of values
	{
		T val;
		if (append)	
		{
			while (get(val))	// read values and append it to the end
				vec.push_back(val);
			return vec.size() > 0;
		}
		else
		{
			for (int i = 0; i < (int)vec.size(); ++i)
			{
				if (get(val)) vec[i] = val;
				else
					return false;
			}
		}
		return true;
	}

	int getPos() const;	// Get current read position
	void setPos(int);	// Set current read position
private:
	bool				isAvailableChar(char chr);
	bool				nextArg();

	ed::istringstream	m_input;
	int					m_inputSize;
	bool				argList;

	ed::string			m_availableChars;
};
//-------------------------------------------------------------------------------
template <class T>
void Update(const ed::string& inCommand, const ed::string& inCtrlName, 
			T Object, ResParser& ioParser)
{
	if (inCommand == inCtrlName)
	{
		ed::string aFileName;
		if (ioParser.get(aFileName))
		{
			*Object = *(Object->resourceKeeper().getResource(aFileName, Object->m_preset));
		}
	}
	else if (inCommand == (ed::string("update") + inCtrlName))
	{
		ed::string aFileName;
		if (ioParser.get(aFileName))
		{
			Object->resourceKeeper().updateResource(Object, aFileName);
		}
	}
}
//-------------------------------------------------------------------------------


#endif /* __USERINTERFACE_RESPARSER_H__ */
