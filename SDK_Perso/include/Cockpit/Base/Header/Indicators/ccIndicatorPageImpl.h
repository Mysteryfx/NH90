#ifndef ccIndicatorPage_H
#define ccIndicatorPage_H
#include "CockpitBase.h"
#include <ed/string.h>
#include <ed/map.h>
#include <ed/unordered_map.h>
#include <ed/vector.h>
#include "Position.h"

struct lua_State;

namespace Graphics
{
    class RenderParser;
}

namespace cockpit
{
typedef enum ccIndication_types
{
    ccCOMMON,
    ccCOLLIMATOR,
    ccHELMET_SIGHT
};
class Element;
class ccDrawable;

typedef ed::unordered_map<ed::string, Element*> ccElementMap;
typedef ed::vector<Element*>                     ccElementList;

class COCKPITBASE_API ccIndicatorPage
{
public:
	// constructor/destructor
	ccIndicatorPage();
	virtual ~ccIndicatorPage();

	virtual void initialize(const ed::string & str, bool is_it_chunk = false);
	// draw 
	virtual void draw(Graphics::RenderParser& parser ,unsigned char level);

	// add element
	virtual void addElem(Element*);

	// update information from the plane
	virtual void update();
    virtual void update(unsigned char level);

	virtual void  setSize(float _xSize, float _ySize) {xSize=_xSize; ySize=_ySize;};
	virtual void  setViewDistance(float X) {viewDistance = X;};

	virtual int   getType()  const;

	virtual void  setCurrentVertexScaleModifier(char type,float value = 0);
    virtual float getCurrentVertexScaleModifier() const {return CurrentVertexScaleModifier;};

	virtual void setParent(ccDrawable* ptr){parent = ptr;};
	virtual ccDrawable* getparent() const {return parent;};	

	ccElementMap&         get_SortedPageElems()         {return SortedPageElems;};
	ccElementList&        get_PageElems()		        {return PageElems;};
	const  ccElementList& get_PageElems() const         {return PageElems;};

    void                  log(ed::string & out);
protected:
	ccElementMap   SortedPageElems;
	ccElementList  PageElems;
	//Сведения о геометрии плашки
	float          xSize;
	float          ySize;
	float          viewDistance;  // расстояние от точки взгляда  до плоскости изображения
	float          CurrentVertexScaleModifier;
	ccDrawable*    parent;
	virtual void		reg_in_script(lua_State *Ls);
    virtual void		reg_in_reusable_state(lua_State *Ls);
	virtual void		distribute_elements();
	static int          l_push_guid_string(lua_State *L);
	static ed::string  create_guid();
};

}

#endif // ccIndicatorPage_H
