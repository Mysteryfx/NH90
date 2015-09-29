#ifndef _Font_h_
#define _Font_h_

#include "Renderer/RenderFont.h"
#include "rwrappers/renderUserAPI.h"
#include "renderer/Color.h"
#include <ed/string.h>
#include <ed/vector.h>
#include "box.h"

namespace Graphics
{

class Font : public RenderFont
{

	struct FontTC
    {
        int x,y;    // coords on the texture
        int dx,dy;  // size on the texture
        unsigned c;
        bool operator< (const FontTC& op)const
        {
            return c < op.c;
        }; 
    };

    typedef ed::vector<FontTC> fontTexCoord;
	fontTexCoord texCoords;

    ed::string m_fontName;
    float       m_fontSize;
    unsigned    m_fontFlags;
    int         LoadStep;

	render::Texture face;
	render::Shader sh;

	render::VertexBuffer vb;
	render::IndexBuffer ib;

	render::handle_t shp_Tex;
	render::handle_t shp0;
	render::handle_t shp1;
	render::handle_t shp2;
	render::handle_t shp3;
	render::handle_t shp4;
	render::handle_t shp5;
	render::handle_t shpAr;
	render::handle_t shpcolor;
    render::handle_t hMVP;
    render::ve_handle_t pos0SymanticHandle;

	render::stream_handle_t stream;
	render::handle_t tech;

    osg::Matrixf _world;
    osg::Matrixf _view;
    osg::Matrixf _proj;

	struct StringDesc
    {
        const char* str;
        POINT       sz;
        int         charsCount;

        StringDesc()
        { str = 0; sz.x = 0; sz.y = 0; charsCount = 0; }
    };

    typedef ed::vector<StringDesc> TextStrings;
    TextStrings  textStrings;
	POINT        textSize;

	bool MakeStrings(char* text);
	POINT GetCharSize( unsigned c );

    void drawInWorldSpace(const osg::Matrixf& pos);
	int DrawString(int xbeg, int ybeg, const POINT &strSize,
				   const RECT &clipRect,
				   const char *str, const Color &color, osg::Matrixf *m = 0);
					 //,FontVertex *pVertex, WORD *pIndex,int &vertex, int &index );
    void Draw( Box &textBox, RenderText * rtext, const Vector3 &textOrig);
    inline FontTC* getFontTC(unsigned c)
    {
        FontTC tc = {0, 0, 0, 0, c};
        fontTexCoord::iterator it = std::lower_bound(texCoords.begin(), texCoords.end(), tc);
        if(it == texCoords.end())
        {
            return 0;
        }
        else
        {
            return &(*it);
        }
    }

public:

	static Resource* Create(ResourceType type, const ed::string& className);

	Font();
	~Font();

	// установка параметры шрифта
	void SetFontName(const ed::string& fontName);
	void SetSize(float size);
	void SetFlags(unsigned flags);

	// получение параметров
	const ed::string& GetFontName();
	float GetSize();
	unsigned GetFlags();

	// получить габариты строки
    void GetTextExtent(const char       * text, size_t n ,Vector3& extent);

	// Вывести текст данным шрифтом
	void DrawRenderText(RenderText *rtext, bool bDontUseGlobalVP=false);
    void setParams( const osg::Matrixf &W, const osg::Matrixf &V, const osg::Matrixf &P);



	// интерфейс Resource
    bool  CreateByName(const ed::string& resname);
	const ed::string& ClassName();
	float GetLoadPriority();
	int   GetLoadStep();
	int   GetLoadStepCount();
	void  LoadToStep(int step);
	void  FreeToStep(int step);

    void serialize(Serializer& serializer) {}
};

}// namespace

#endif
