// EDBinaryTemplate.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include "HUD.h"
#include "WorldManager.h"
#include "ed_log.h"
#include "wTime.h"
#include "ccMainPanel.h"


#include "Indicators/ccIndication.h"
#include "Indicators/ceStringPoly.h"
#include "Renderer/RenderText.h"
#include "IView.h"


using namespace cockpit;
using namespace cockpit::Samples;
//register factory for newly defined device


class hardcodedText : public ceStringPoly
{
public:
	hardcodedText(const char * uniqueName)
	{
		name = uniqueName;
	}
	void set_stringdefs(const ceFontStringDefinition & new_defs)
	{
		defs = new_defs;
	}
	void set_formats(const ed::vector<ed::string> & fts)
	{
		formats = fts;
	}
	void set_init_pos(const Vector3 & v) 
	{
		init_pos = v;
	}
	void set_init_rot(const Vector3 & v) 
	{
		init_rot = v;
	}
	void add_controller (const 	element_controller & ec)
	{
		element_controllers.push_back(ec);
	}
};

class hardcodedMesh : public ceTexPoly
{
public:
	hardcodedMesh(const char * uniqueName)
	{
		name = uniqueName;
	}

	void set_geometry(const vectorarray & new_vertices,
					  const vectorarray & new_texcoords,
					  const indexarray  & new_indices)
	{
		verts   =  new_vertices;
		indices =  new_indices;
		statesTexCoords.resize(1);
		statesTexCoords[0]			 = new_texcoords;
		state						 = 0;
		direct_assignment_tex_coords = true;
		CustomGeometry * cgeom		 = robj.geometry->QueryCustomGeometry();

		cgeom->SetVertices(verts);
		cgeom->SetIndices(indices);
		cgeom->SetTexCoords(statesTexCoords[0]);
	}

	void add_controller (const 	element_controller & ec)
	{
		element_controllers.push_back(ec);
	}
};

class hardcodedPage: public ccIndicatorPage
{
public:
	hardcodedPage(ccIndicator * p)
	{
		setSize(p->GetHalfWidth(),p->GetHalfHeight());
		setViewDistance(0.6);
		setParent(p);
	}

	void init_done()
	{
		distribute_elements();
	}
};



REGISTER_WORLD_FACTORY_EX(new WorldFactory<cockpit::Samples::HUD_Indicator>,	   HUD_Indicator);
//////////////////////////////////////////////////////////////////////////
HUD_Indicator::HUD_Indicator()
{
	time		  = -100;
	next_page	  = 0;
	make_default_activity(0.1);
	render_target_current = 0; // assign to render target 0 -  "mfd0"   render target surface in graphics engine
}



void HUD_Indicator::initialize( avDevice* parent, unsigned char id , const ed::string& script )
{
	ccIndicator::initialize(parent,id,script);
#if 0  //HARDCODED PAGE CREATION WITHOUT LUA , IF SOMEONE INTERESTED 
	ccLuaElementSet new_page;
	if (hardcodedPage * new_page_subset = new hardcodedPage(this))
	{
		ceFontStringDefinition fnt1;
			/*
			if (alignment == "RightBottom")
			{
				defs.hor_just   = RenderText::tjUpper;
				defs.ver_just   = RenderText::tjLower;
			}
			else if (alignment == "LeftTop")
			{
				defs.hor_just   = RenderText::tjLower;
				defs.ver_just   = RenderText::tjUpper;
			}
			else if (alignment == "RightTop")
			{
				defs.hor_just   = RenderText::tjUpper;
				defs.ver_just   = RenderText::tjUpper;
			}
			else if (alignment == "LeftCenter")
			{
				defs.hor_just   = RenderText::tjLower;
				defs.ver_just   = RenderText::tjCenter;
			}
			else if (alignment == "RightCenter")
			{
				defs.hor_just   = RenderText::tjUpper;
				defs.ver_just   = RenderText::tjCenter;
			}
			else if (alignment == "CenterBottom")
			{
				defs.hor_just   = RenderText::tjCenter;
				defs.ver_just   = RenderText::tjLower;
			}
			else if (alignment == "CenterTop")
			{
				defs.hor_just   = RenderText::tjCenter;
				defs.ver_just   = RenderText::tjUpper;
			}
			else if (alignment == "CenterCenter")
			{
				defs.hor_just   = RenderText::tjCenter;
				defs.ver_just   = RenderText::tjCenter;
			}
			else //alignment   == "LeftBottom"
			{
				defs.hor_just   = RenderText::tjLower;
				defs.ver_just   = RenderText::tjLower;
			}
			*/
		fnt1.font_height = 0.01;
		fnt1.font_width  = 0.007;
		fnt1.hor_just    = RenderText::tjCenter;
		fnt1.ver_just    = RenderText::tjUpper;
		fnt1.inter_char_spacing = 0;
		fnt1.inter_rows_spacing = 0;

		if (hardcodedText * text_element = new hardcodedText("TEXT_OBJECT_01"))
		{
			text_element->setparent(new_page_subset);
			text_element->set_stringdefs(fnt1);
			text_element->set_string("SOME VALUE");

			new_page_subset->addElem(text_element);
		}
		new_page_subset->init_done();
		new_page.push_back(new_page_subset);
	}
	PageMap[3] = new_page;  //register it like page 3
#endif
}


void HUD_Indicator::create_elements_controllers_map()
{
	auto  & controllers = get_elements_controllers_ref();
	controllers["set_origin_to_cockpit_shape"]  = set_origin_to_cockpit_shape;
	controllers["hud_pitch"]					= hud_pitch;
	controllers["hud_roll"]						= hud_roll;
	controllers["txt_pitch"]					= txt_pitch;
}

void HUD_Indicator::txt_pitch( Element * elem,ccDrawable * parent, const DParamList& args )
{
	ceStringPoly * txt = static_cast<ceStringPoly*>(elem);
	txt->set_strcat_by_predefined_format(cockpit::roundf(args[0]),57.3 * parent->get_context()->human_ptr->Tang());
}

void HUD_Indicator::hud_pitch( Element * elem,ccDrawable * parent, const DParamList& args )
{
	Position3 & pos = elem->get_position_ref();
	float scale = !args.empty() ? args[0] : 1.0f;
	pos.p  +=  pos.y * scale * parent->get_context()->human_ptr->Tang();
}

void HUD_Indicator::hud_roll( Element * elem,ccDrawable * parent, const DParamList& args )
{
	Position3 & pos = elem->get_position_ref();
	float scale = !args.empty() ? args[0] : 1.0f;
	pos.RotateX(scale * parent->get_context()->human_ptr->Roll());
}

void HUD_Indicator::control()
{
	if (wTime::GetModelTime() - time > 4.0)//change page every four seconds 
	{	
		time  = wTime::GetModelTime();
		if (next_page > 4)
			next_page = 0;
		select_page(next_page);
		next_page++;
	}
}

void HUD_Indicator::set_origin_to_cockpit_shape( Element * elem,ccDrawable * parent, const DParamList& args )
{
	Position3 & pos			   =  elem->get_position_ref();
	Position3   inv_parent_pos = ~parent->get_initpos();
	pos *= inv_parent_pos;
}

void HUD_Indicator::select_page( unsigned page )
{
	auto it  = PageMap.find(page);
	if  (it != PageMap.end())
	{
		current_page =  &it->second;
	}
	else
	{
		current_page = nullptr;
	}
}

static const dVector  camera_placement(0,-1.0,-4.0); // one meter down and four meters to the left from the aircraft center

const Graphics::Camera & HUD_Indicator::get_camera()
{
	Position3 pos = get_context()->human_ptr->Position();

	pos.p += camera_placement.x * pos.x +
			 camera_placement.y * pos.y +
			 camera_placement.z * pos.z;
	pos.RotateY(sin(wTime::GetModelTime()) * TO_RAD(30.0));
	pos.RotateZ(TO_RAD(-5.0));

	render_camera.setType(Camera::ctPerspective);
	render_camera.setPos(pos);
	render_camera.setHAngleAndAspect(TO_RAD(20.0),1.0);
	return render_camera;
}

dVector	HUD_Indicator::get_camera_point()	
{
	cPosition pos = get_context()->human_ptr->Position();
	dVector   hp  = get_context()->human_ptr->GetPoint();//double origin to remove jittering 
	hp	  += camera_placement.x * pos.x +
			 camera_placement.y * pos.y +
			 camera_placement.z * pos.z;
	return hp;
}

TVScreenCondition HUD_Indicator::get_screen_condition()
{
	render_screen_type = TV_TV;
	if (globalView->InCockpit() || render_target_always)
	{
		return render_screen_type;
	}
	else
	{
		return TV_Blank;
	}
}

void HUD_Indicator::PrepareParser()//for tv picture render , set brightness ansd contrast 
{
	ccIndicator::PrepareParser();
	static float brightness = 0.35;
	static float contrast   = 0;
	render_parser.set_inverted(false);
	render_parser.set_shader_constant(brightness,contrast,0);
}
