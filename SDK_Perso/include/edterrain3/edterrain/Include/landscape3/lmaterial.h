#ifndef __LANDSCAPE3_LMATERIAL_H
#define __LANDSCAPE3_LMATERIAL_H
#include "../landscape3_config.h"

#include <ed/map.h>
#include <stdint.h>
#include <ed/string.h>
#include "ed_string.h"

namespace material3
{

// Материал
struct EDTERRAIN_API Material
{
	// псевдоним материала
	ed::string matalias;
	// Семантика
	ed::string semantic;

	void setMat(const char* text)
	{
		matalias = text;
	}
	void setSem(const char* text)
	{
		semantic = text;
	}
	// Параметр материала
	struct Parameter
	{
		enum Type
		{
			NONE   = 0,
			STRING = 1,
			INT	   = 2
		};
		Type type;
		ed::string text;
		Parameter() : text(" ")
		{
			type = NONE;
			text[0] = 0;
		}
		Parameter(Type type, const ed::string& text) : type(type), text(text) 
		{
			if( this->text.empty())
				this->text = " ";
		}

		void Invalidate()
		{
			type = NONE;
		}
		bool IsValide()
		{
			return type == STRING;
		}
		// В строку
		EDTERRAIN_API int ToString(
		    char* buffer,
		    int bufferlen
		);
		//
		void setString(const char* text)
		{
			this->text = text;
			type = STRING;
			if( this->text.empty())
				this->text = " ";
		}
		bool operator <(const Parameter& arg) const
		{
			if((int)type<(int)arg.type)
				return true;
			if((int)type>(int)arg.type)
				return false;

			int res;
			res = strcasecmp(text.c_str(), arg.text.c_str());
			if(res<0) return true;
			if(res>0) return false;
			return false;
		}
	};
public:
	ed::map<ed::string, Parameter> params;

	// для текстурных массивов: Список имен текстур. Суда ссылается GeometryItem::SubItem::textureindex
	//	ed::vector<ed::string> texturearray;

	Material()
	{
		setMat("");
		setSem("");
	}
	Material(char* matalias, char* semantic)
	{
		setMat(matalias);
		setSem(semantic);
	}
	bool operator <(const Material& arg) const;

	bool operator ==(const Material& arg) const
	{
		if(*this<arg || arg<*this) return false;
		return true;
	}
	bool operator !=(const Material& arg) const
	{
		if(*this<arg || arg<*this) return true;
		return false;
	}

	// copy
	void CopyFrom(const Material& arg);

	// Параметр по имени
	Parameter* getParameter(const char* param_name);

	// Добавить параметры
	void addParam(const char* param_name, const char* value);

	// Добавить параметры
	void AddParameters(ed::map<ed::string, ed::string> &params);
};

}

#endif
