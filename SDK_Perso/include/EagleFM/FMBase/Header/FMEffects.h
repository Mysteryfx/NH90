#ifndef __FMEffects_h__
#define __FMEffects_h__

#include "Base.h"

#include <ed/list.h>

namespace EagleFM
{ 

class FMBASE_API EffectManager
{
public:
	EffectManager() { effects.clear(); }

	enum EffectType
	{
		ETV_ARG,			//дернуть аргумент рисования (номер и значение указаны в параметре)
		ETV_SPARK,			//искры - виз.
		ETV_DUST,			//пыль - виз.
		ETV_PIECES,			//обломки - виз.
		ETV_DUSTTAIL,		//пыльный сплошной след - виз.
		ETV_WHEELTRACK,		//след резины - виз.
		ETV_TOUCHDOWNSMOKE,	//дымок от касания - виз.
		ETV_PISTONEXHAUST,	//выхлоп-вспышка мотора - виз.
		ETV_HOT_START_EFFECT,	//эффект хот старта
		ETV_SPECIAL,		//индивидуальный - виз. (определяется параметрами)

		ETA_TOUCHDOWN,		//касание - звук
		ETA_TOUCHDOWNHARD,	//жесткая посадка - звук
		ETA_TOUCHDOWNBREAK,	//жесткая посадка с повреждением шасси - звук
		ETA_CONCRETESLIDING,//скольжение корпусом по бетону - звук
		ETA_GROUNDSLIDING,	//скольжение корпусом по земле - звук
		ETA_WHEELSLIDE,		//юз колеса - звук
		ETA_GEARPOSTLOCK,	//закрытие замка стойки шасси - звук
		ETA_BLADELIMBLOW,	//удар лопасти по упору
		ETA_FUELPUMPWORK,	//работа топливных насосов - звук
        ETA_FLAPSDRIVESACTIVE,//работа приводов закрылков - звук
		ETA_GEARDOORLOCK,	//закрытие/открытие замка створок шасси - звук
		ETA_PISTONEXHAUST,	//выхлоп-вспышка мотора - звук
		ETA_SPECIAL,		//индивидуальный - звук (определяется параметрами)

		ETS_EXIT			//завершение приложения - системный
	};

#pragma warning(disable : 4351) // new behavior: elements of array 'EagleFM::EffectManager::Effect::param' will be default initialized

	struct Effect
	{
		Effect(EffectType type) : type_(type), param() {}

		static const short max_size_ = 20;

		EffectType	type_;
		double		param[max_size_];

		static short get_max_size() { return max_size_; }
	};

#pragma warning(default : 4351)

	void			pushEffect(Effect eff) { effects.push_back(eff); }
	Effect			popEffect() { Effect effect = effects.front(); effects.pop_front(); return effect; }
	bool			getEmpty() const { return effects.empty(); }			
private:
	ed::list<Effect> effects;
};
}

#endif
