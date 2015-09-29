#include "stdafx.h"

#include "Avionics/Radio/avRadioGUI.h"
#include "Lua/Config.h"

void cockpit::loadInterphone(Lua::Config & config, ICommandDialogsPanel::Interphone & interphone)
{
	config.get("displayName", &interphone.displayName);
}

inline void rangeFromState(Lua::Config & src, ICommandDialogsPanel::Radio::FrequencyRange & frequencyRange)
{
	src.get("min", &frequencyRange.min);
	src.get("max", &frequencyRange.max);
}

void cockpit::loadRadio(Lua::Config & config, ICommandDialogsPanel::Radio & radio)
{
	if(config.open("range"))
	{
		radio.frequencyRanges.resize(1);
		rangeFromState(config, radio.frequencyRanges[0]);
		config.pop();
	}
	else if(config.open("ranges"))
	{
		int rangeIndex = 1;
		while(config.open(rangeIndex))
		{
			radio.frequencyRanges.push_back(ICommandDialogsPanel::Radio::FrequencyRange());
			ICommandDialogsPanel::Radio::FrequencyRange & frequencyRange = radio.frequencyRanges.back();
			rangeFromState(config, frequencyRange);
			config.pop();
			rangeIndex++;
		}
		config.pop();
	}
	else if(config.open("channels"))
	{
		int channelIndex = 1;
		float freq;
		while(config.get(channelIndex, &freq))
		{
			radio.channels.push_back(freq);
			channelIndex++;
		}
		config.pop();
	}
	config.get("AM", &radio.AM);
	config.get("FM", &radio.FM);
	config.get("displayName", &radio.displayName);
}