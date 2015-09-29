#pragma once

#include "FMElectricSystems/Items/Switch.h"
#include "FMElectricSystems/Items/Lamp.h"
#include "Utilities/ccPanelGauge.h"

namespace Graphics
{ 
	class Material;
}

namespace plane_lights
{ 
	class woLA_LightCollection;
}

namespace cockpit
{
class ccMainPanel;

class radiance_controller
{ 
	class radiance_entry
	{ 
	public:
		radiance_entry():material(0),color(1.0,1.0,1.0){}
		void update(float value);

		Graphics::Material   *material;
		ed::string           material_name;
		Vector3               color;
	};
	ed::vector<radiance_entry> radiance_;
public:
	radiance_controller(){};
	void read_radiance_from_state(Lua::Config & config);
	void update_radiance(float val);
};

class COCKPITBASE_API external_light_controller
{
public:
	external_light_controller() :
		target_collection(0),
		target_child_inside_collection(0),
		dimmed_brightness_mode(false),
		dimmed_brightness(0.5),
		normal_brightness(1),
		lamp(28.0)
		{}

	void connect(EagleFM::Elec::ItemBase& Input);
	void set_only_power(bool value);
		
	void update(double dt);
	void set_target(plane_lights::woLA_LightCollection* collection, unsigned child_inside);
	void set_dimmed_brightness_mode(bool val) {dimmed_brightness_mode = val;}

	void set_normal_brightness(double value) {normal_brightness = value;}
	void set_dimmed_brightness(double value) {dimmed_brightness = value;}

	void setElecConsumParams(const EagleFM::Elec::ConsumerParams& params);
	void setElecConsumParamsFromState(Lua::Config& config, const char* table);

	void setElecLampParams(const EagleFM::Elec::LampParams& params);
	void setElecLampParamsFromState(Lua::Config& config, const char* table);

private:
	EagleFM::Elec::Switch       switcher;
	EagleFM::Elec::Lamp         lamp;

	bool                        dimmed_brightness_mode;
	double                      dimmed_brightness;
	double                      normal_brightness;
	plane_lights::woLA_LightCollection* target_collection;
	unsigned                    target_child_inside_collection;
};

class COCKPITBASE_API light_arg_controller : public ccPanelGauge
{
public:
	light_arg_controller() : lamp(28.0)
	{
		brightness              = 0;
		smooth                  = false;
		use_electric_power      = true;
	}

	double get_brightness() const;
	void update(cockpit::ccMainPanel *panel);
	void connect(EagleFM::Elec::ItemBase& Input);
	void set(float value);
	void set_only_power(bool value);
	void set_only_brightness(float value);
	void read_from_state(Lua::Config & config);

	void setElecConsumParams(const EagleFM::Elec::ConsumerParams& params);
	void setElecConsumParamsFromState(Lua::Config& config, const char* table);

private:
	EagleFM::Elec::Switch       switcher;
	EagleFM::Elec::Lamp			lamp;

	double                      brightness;
	bool                        smooth;
	bool                        use_electric_power;
	radiance_controller         radiance_;
};  

class COCKPITBASE_API flood_light_controller : public ccPanelGauge
{
	class flood_light_entry : public ccPanelEffect
	{ 
	public:
		flood_light_entry() : color(1.0,1.0,1.0) {}
		void  set_brightness(float value);
		Vector3          color;
	};
	ed::vector<flood_light_entry*> light_sources;

	radiance_controller             radiance_;
public:
	flood_light_controller() : brightness(0),
		lamp(28.0)
	{}

	~flood_light_controller();

	void update      (cockpit::ccMainPanel *panel);
	void onAddTo     (cockpit::ccMainPanel *panel);
	void onRemoveFrom(cockpit::ccMainPanel *panel);

	void connect(EagleFM::Elec::ItemBase& Input);
	void set(float value);
		
	void read_from_state(Lua::Config & config);

	virtual double get_brightness() const
	{ 
		if (lamp.isPowered())
			return brightness;

		return 0;
	}

	void setElecConsumParams(const EagleFM::Elec::ConsumerParams& params);
	void setElecConsumParamsFromState(Lua::Config& config, const char* table);

protected:
	EagleFM::Elec::Switch       switcher;
	EagleFM::Elec::Lamp			lamp;

	double                      brightness;
}; 

//common ambient light for simplified light scheme
class avBasicLightSystem;
class COCKPITBASE_API ambient_controller : public ccPanelEffect
{ 
public:
	ambient_controller() : parent(0), sun(0) {}
	void  set_parent  (avBasicLightSystem * p_) { parent = p_;}
	void  update      (cockpit::ccMainPanel *panel);
	void  read_from_state(Lua::Config & config);
	float get_sun_luminance() const;
private:
	avBasicLightSystem*               parent;
	mutable Graphics::VolumeEffect*   sun;
	Vector3                           flood_light_color;
};

}

