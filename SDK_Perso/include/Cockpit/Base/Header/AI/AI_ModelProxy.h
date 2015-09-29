#pragma once

class IwoLA;

namespace gunner_AI
{
	class ModelArgProxy
	{
	public:
		static float get_value(IwoLA* la, int arg, char pylon = -1	/*-1 get from LA model*/);
		static void set_value(IwoLA* la, int arg, float value, char pylon = -1	/*-1 set to LA model*/);
	};
}