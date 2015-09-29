#pragma once

#include "CockpitBase.h"
#include <ed/vector.h>
#include <ed/string.h>
#include <ed/map.h>
#include "cLinear.h"
#include "Vector4.h"
#include "GraphMath/box.h"


namespace Lua
{
    class Config;
};

namespace Graphics
{
	class Material;
};

namespace cockpit
{
struct ceFontCharDefinition
{
	double char_H_aspect;
	double char_W_aspect;
	double tex_min_x;
	double tex_max_x;
	double tex_min_y;
	double tex_max_y;
};

struct ceFontStringDefinition
{
	double font_height;
	double font_width;
	double inter_char_spacing;
	double inter_rows_spacing;
	unsigned char hor_just;
	unsigned char ver_just;
};

typedef ed::map<unsigned,ceFontCharDefinition> ceFontCharDefinitionMap;


class COCKPITBASE_API ceFont		
{
	bool	 direct_size_calculation; 
private:
    ceFontCharDefinitionMap charset;
    ceFontCharDefinition    default_def;
    ed::string             texture_name;
    void	 initialize(Lua::Config & config);
    void	 initialize(const ed::string & DXUnicodeFontData_name);
public:
	ceFont(void);
	virtual ~ceFont(void);
    static   ceFont * construct(Lua::Config & config,const Vector4& color,const ed::string& font_name);
	void     make_string(const ed::string & s,const ceFontStringDefinition &defs,Box *box, ed::vector<Vector3>		& vertex_buffer,
        																			        ed::vector<Vector3>        & texture_buffer,
																					        ed::vector<unsigned short> & index_buffer);
    const ceFontCharDefinition & get_char_definition(unsigned c) const;
    const ceFontCharDefinition & space() const { return get_char_definition(32); };
	Graphics::Material  *mat;
};

}
