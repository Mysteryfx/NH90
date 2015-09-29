#include "stdafx.h"
#include "DigitalClock.h"
#include "WorldManager.h"
#include "ed_log.h"
#include "wTime.h"
#include "ccMainPanel.h"
#include "Indicators/ccIndication.h"
#include "Indicators/ceStringPoly.h"
#include "Renderer/RenderText.h"

using namespace cockpit;
using namespace cockpit::Samples;

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
	void add_controller(const 	element_controller & ec)
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
		verts = new_vertices;
		indices = new_indices;
		statesTexCoords.resize(1);
		statesTexCoords[0] = new_texcoords;
		state = 0;
		direct_assignment_tex_coords = true;
		CustomGeometry * cgeom = robj.geometry->QueryCustomGeometry();

		cgeom->SetVertices(verts);
		cgeom->SetIndices(indices);
		cgeom->SetTexCoords(statesTexCoords[0]);
	}

	void add_controller(const 	element_controller & ec)
	{
		element_controllers.push_back(ec);
	}
};

class hardcodedPage : public ccIndicatorPage
{
public:
	hardcodedPage(ccIndicator * p)
	{
		setSize(p->GetHalfWidth(), p->GetHalfHeight());
		setViewDistance(0.6);
		setParent(p);
	}

	void init_done()
	{
		distribute_elements();
	}
};



REGISTER_WORLD_FACTORY_EX(new WorldFactory<cockpit::Samples::DigitalClock>, DigitalClock);

DigitalClock::DigitalClock()
{
	time = -100;
	next_page = 0;
	make_default_activity(0.1);
	render_target_current = 0; // assign to render target 0 -  "mfd0"   render target surface in graphics engine
}


DigitalClock::~DigitalClock()
{
}


void DigitalClock::initialize(avDevice* parent, unsigned char id, const ed::string& script) {
	ccIndicator::initialize(parent, id, script);

	ccLuaElementSet new_page;
	if (hardcodedPage * new_page_subset = new hardcodedPage(this))
	{
		ceFontStringDefinition fnt1;

		fnt1.font_height = 0.01;
		fnt1.font_width = 0.007;
		fnt1.hor_just = RenderText::tjCenter;
		fnt1.ver_just = RenderText::tjUpper;
		fnt1.inter_char_spacing = 0;
		fnt1.inter_rows_spacing = 0;

		if (hardcodedText * text_element = new hardcodedText("TEXT_OBJECT_01"))
		{
			text_element->setparent(new_page_subset);
			text_element->set_stringdefs(fnt1);
			text_element->set_string("SOME VALUE");
			//text_element->set_init_pos(Vector3(0.0f, 0.0f, 2.9f));

			new_page_subset->addElem(text_element);
		}
		new_page_subset->init_done();
		new_page.push_back(new_page_subset);
	}
	PageMap[0] = new_page;  //register it like page 4
}

void DigitalClock::create_elements_controllers_map()
{
	ccIndicator::create_elements_controllers_map();
	auto  & controllers = get_elements_controllers_ref();
	controllers["text_using_parameter"] = txt_pitch;
}

void DigitalClock::control()
{
	if (wTime::GetModelTime() - time > 4.0)//change page every four seconds 
	{
		time = wTime::GetModelTime();
		if (next_page > 3)
			next_page = 0;
		select_page(next_page);
		next_page++;
	}
}

void DigitalClock::select_page(unsigned page)
{
	auto it = PageMap.find(page);
	if (it != PageMap.end())
	{
		current_page = &it->second;
	}
	else
	{
		current_page = nullptr;
	}
}

void DigitalClock::update() {
	control();
}

void DigitalClock::txt_pitch(Element* elem, ccDrawable* parent, const DParamList& args) {
	ceStringPoly * txt = static_cast<ceStringPoly*>(elem);
	int minutes, secondes;
	std::string minute, seconde, texte;
	texte = *dynamic_cast<std::string*>(elem);
	minute = texte.substr(0,2);
	seconde = texte.substr(2, 2);
	minutes = atoi(minute.c_str());
	secondes = atoi(seconde.c_str());
	secondes++;
	if (secondes >= 60) {
		secondes = 0;
		minutes++;
	}
	if (minutes >= 60) {
		minutes++;
	}

	texte = std::to_string(minutes) + ":" + std::to_string(secondes);

	txt->set_strcat_by_predefined_format(cockpit::roundf(args[0]), texte);
}