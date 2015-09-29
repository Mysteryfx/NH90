#ifndef COOLING_RADIATOR_H
#define COOLING_RADIATOR_H

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include "Base.h"

namespace EagleFM
{
	struct FluidState;

	class FMBASE_API Radiator
	{
	public:
		struct Tube
		{
			Tube()
				: length(0.254), diam(0.004), b(3.2), f(0.7)
			{}

			double length;
			double diam;
			double b;
			double f;
		}; 

		struct State
		{
			State()
			{
				reset();
			}
			void reset();

			double gasVelocity;	//скорость набегающего потока  газа
			double tempRadBody;	//средняя температура радиатора
			double deltaT;		//подогрев газа после радиатора
			double pressureAmb;
			double temperatureAmb;
			double fluidIntakeT;
			double fluidOutletT; 
			double scoopRelPos;		//отн. позиция совка (0...1, абс - отношение площадей входа и выхода в туннель)
			double tempBoiling;		//температура кипения
			double effectiveness;
			bool   boilingOn;
			bool   hermetic;
		};

		Radiator(const FluidState* fs);

		void init(double i_heatCapacity, double i_sEffective, 
			double i_dzeta, double i_liquidK, double i_gas_c_p, 
			const Tube& i_tube, bool thermostat);


		void initScoopLims(double posMin, double posMax)
		{
			if(posMin >= posMax || posMin < 0 || posMax > 1)
				return;
			scoopPosMin = __max(0.01, posMin);
			scoopPosMax = posMax;
		}

		void initTemperature(double t)
		{
			state.tempRadBody = t;
			state.fluidIntakeT = t;
			state.fluidOutletT = t;
		}

		void initTemperature(double tb, double tin, double tout)
		{
			state.tempRadBody = tb;
			state.fluidIntakeT = tin;
			state.fluidOutletT = tout;
		}

		void		reset();

		void		simulate(double dt);

		Tube&		getTube()	{ return tube; }
		State&		getState()	{ return state; }
        const FluidState* getFluidState () const { return fstate; }

		double		getRadF()	const { return f_rad; }
		double		getDrag()	const { return drag; }

	private:

		Tube		tube;
		State		state;
		const		FluidState*	fstate;

		double		heatCapacity;	//теплоемкость
		double		sEffective;		//эффективная площадь

		double		dzeta;			//Коэффициент потерь напора в канале капота: 2-4 установка в крыле и под мотором
		//1-2 для лобовой установки, 4-6 для подфюзеляжной
		double		liquidK;		//коэффициент для разных жидкостей
		double		gas_c_p;		//теплоемкость газа

		double		f_rad;

		double		p_k;			//Коэффициент теплопередачи от жидкости в воздух

		double		drag;
		double		gasFlow;
		double		pressureDrop;

		double		scoopPosMin;
		double		scoopPosMax;

		bool		thermostatFlag;
	};


}

#endif
