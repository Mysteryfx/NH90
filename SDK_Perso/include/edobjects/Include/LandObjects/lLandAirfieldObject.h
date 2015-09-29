#ifndef LLAND_AIRFIELD_OBJECT_H_
#define LLAND_AIRFIELD_OBJECT_H_

#include "lCustomLandObject.h"

#define IID_lLandAirfieldObject Common::identify<lLandAirfieldObject>()

class lLandAirfieldObject : public lCustomLandObject{
public:
	/// Description of runway.
	struct Runway{
		enum{
			LOW_GLIDE_PATH=0,
			SLIGHTLY_LOW_GLIDE_PATH,
			ON_LOWER_GLIDE_PATH,
			ON_UPPER_GLIDE_PATH,
			SLIGHTLY_HIGH_GLIDE_PATH,
			HIGH_GLIDE_PATH,

			LAST_PATH
		};

		enum Side{
			NONE = 0,
			LEFT,
			RIGHT,
			CENTER
		};

		unsigned int number;
		Side side;

		int handle; /// holds handle to control runway lights

		double glidePaths[LAST_PATH]; /// holds angles of glide paths

		Graphics::ModelConnector *connector; /// holds connector to touch down zone of runway
	};

	typedef ed::vector<Runway> Runways;
protected:
	/// Holds time from start of given interval.
	wModelTime _oldT;
	/// Holds time from previous call of method Parse.
	wModelTime _prevT;

	float _animTime;

	/// holds list of connectors corresponding to touch down zones on runways.
	ed::vector<Runway> _runways;

	bool _startSnowdrift; /// Holds status of snowdrift.
	wModelTime _snowdriftTime; /// Holds time from snowdrift start.
	wModelTime _snowdriftDuration; /// Holds time from snowdrift start.
	static const wModelTime MAX_SNOWDRIFT_DURATION; /// How long snowdrift exists in sec.
	static const wModelTime SNOWDRIFT_ATTENUATION; /// How long snowdrift appears/attenuates in sec.
	static const float SNOWDRIFT_FREQ; /// Holds how often snowdrift will appear.

	Vector3 _windDirection;	/// Holds direction of wind.
	float _windSpeed;		/// Holds wind speed.
	float _precipitation;	/// Holds strength of precipitation.

	/// Fills _windDirection, _windSpeed, _precipitation.
	void getWeather();

	static lLandAirfieldObject* CreateObject();

	/// Loads model using LoadToStep.
	void loadModel();

	/// Sets weather effect of airfield.
	void controlWeather();

	/// Sets states of PAPIs/VASIs.
	void controlALS(const Position3 &pos);
public:
	EDOBJECTS_API static lLandAirfieldObject* CreateObject(Graphics::Model *model, const cPosition& _pos);
	EDOBJECTS_API static lLandAirfieldObject* CreateObject(const char *modelname, const cPosition& _pos);

	EDOBJECTS_API lLandAirfieldObject();
	EDOBJECTS_API ~lLandAirfieldObject();
	
	// from lCustomLandObject and viObjectShape
	EDOBJECTS_API virtual void SetModel(Graphics::Model *_model);
	EDOBJECTS_API virtual void SetModel(const ed::string &filename);

	EDOBJECTS_API void setRunways(const Runways &gladePaths);

	/// Turns on/off lights on airfield.
	EDOBJECTS_API void turnOnLights(bool on);

	/// Needed for AirfieldInfoProvider.
	EDOBJECTS_API void Parse(Graphics::ModelParser& parser);

	/// I force \param status to viAirfield.
	EDOBJECTS_API virtual void Init(wShape *_type, const cPosition& _pos, wModelTime t, viFlag status, void* context);

	// from viObjectShape
	EDOBJECTS_API void Parse(Graphics::ModelParser& parser, const Position3 &pos, Graphics::effectState* effects);
};

#endif
