#ifndef __IFuelElement_h__
#define __IFuelElement_h__

#include "FMFuelSystem/FuelPipeline.h"

namespace EagleFM
{

//		ВИРТУАЛЬНАЯ ТОПЛИВНАЯ ЕДИНИЦА

class FMBASE_API IFuelElement
{
public:
	IFuelElement(double V = 1.0) { OutputLine.setVolumeMax(V); pInputLine = 0; }
	virtual ~IFuelElement() {}

	virtual void	simulate(double dt) {}

	void			setInputLine(FuelPipeline* pLine) { if(pLine) pInputLine = pLine; }
	FuelPipeline*	getOutputLine() { return &OutputLine; }


protected:
	FuelPipeline	OutputLine;		//выходная магистраль
	FuelPipeline	*pInputLine;	//входная (извне) магистраль	
};


//		ТОПЛИВНЫЕ УЗЛЫ

class FMBASE_API FuelInNode //Узел-смеситель (два входа, один выход)
{
public:
	FuelInNode(double V = 1.0) { OutputLine.setVolumeMax(V); pLeftInputLine = pRightInputLine = 0; setK_LeftToRight(0.5); }

	virtual ~FuelInNode() {}

	void			setK_LeftToRight(double K) { K_LeftToRight = __max(0.0, __min(K, 1.0)); }

	void			simulate(double dt);

	void			setLeftInputLine(FuelPipeline* pLine) { if(pLine) pLeftInputLine = pLine; }
	void			setRightInputLine(FuelPipeline* pLine) { if(pLine) pRightInputLine = pLine; }

	FuelPipeline*	getOutputLine() { return &OutputLine; }

protected:
	FuelPipeline	OutputLine;			//выходная магистраль
	FuelPipeline	*pLeftInputLine;	//левая входная магистраль	
	FuelPipeline	*pRightInputLine;	//правая входная магистраль	
	double			K_LeftToRight;		//к-т: отношение пропускной способности левого входа к правому
};

class FMBASE_API FuelOutNode //Узел-размеситель (один вход, два выхода)
{
public:
	FuelOutNode(double Vl = 1.0, double Vr = 1.0) { LeftOutputLine.setVolumeMax(Vl); RightOutputLine.setVolumeMax(Vr); pInputLine = 0; }

	virtual ~FuelOutNode() {}

	void			simulate(double dt);

	void			setInputLine(FuelPipeline* pLine) { if(pLine) pInputLine = pLine; }

	FuelPipeline*	getLeftOutputLine() { return &LeftOutputLine; }
	FuelPipeline*	getRightOutputLine() { return &RightOutputLine; }

protected:
	FuelPipeline	LeftOutputLine;		//левая выходная магистраль
	FuelPipeline	RightOutputLine;	//правая выходная магистраль
	FuelPipeline	*pInputLine;		//входная магистраль	
};

class FMBASE_API FuelCrossNode //управляемый узел - перекресток (два входа, два выхода, кран кольцевания)
{
public:
	FuelCrossNode(double Vl = 1.0, double Vr = 1.0, double Vin = 1.0); //объем выходных магистралей и внутренний объем
	
	virtual ~FuelCrossNode() {}

	void			init(double Vl, double Vr, double Vin);

	void			setK_LeftToRight(double K) { InNode.setK_LeftToRight(K); }


	void			simulate(double dt);

	void			setLeftInputLine(FuelPipeline* pLine);
	void			setRightInputLine(FuelPipeline* pLine); 

	FuelPipeline*	getLeftOutputLine() { return &LeftOutputLine; }
	FuelPipeline*	getRightOutputLine() { return &RightOutputLine; }
	
	bool			Bypass;		//true - закольцован

protected:
	FuelInNode		InNode;		//смеситель
	FuelOutNode		OutNode;	//размеситель

	FuelPipeline	*pLeftInputLine;	//левая входная магистраль	
	FuelPipeline	*pRightInputLine;	//правая входная магистраль	
	FuelPipeline	LeftOutputLine;		//левая выходная магистраль	
	FuelPipeline	RightOutputLine;	//правая выходная магистраль	
};


/*template<int N, int M> //ЕСЛИ ПОНАДОБИЦА! РЕАЛИЗОВАТЬ simulate()
class FMBASE_API IFuelNode //Узел (несколько входов, несколько выходов)
{
public:
	IFuelNode(double V = 1.0)
	{ 
		for(int i = 0; i < N; i++)
			OutputLine[i].setVolumeMax(V), 
		pInputLine = new FuelPipeline[M];
		for(int i = 0; i < M; i++)
			pInputLine[i] = 0;
	}

	virtual ~IFuelNode() {}

	void			simulate(double dt)
	{
		// -подсчитать кол-во входных магистралей с P+
		// -подсчитать кол-во выходных магистралей с P-
		// если P+ + P- == 0 - return
		// - если P- == 0 - заливать всем! считаем кому сколько надо находим общий требуемый объем, иначе считаем только кому надо
		// - если P+ == 0 - берем у всех! делим общий треб.объем на всех иначе  делим общий треб.объем на P+
		// - распределяем добычу, собираем излишки
		// - распределяем излишки по порядку (не по ровну), если остались излишки - раскидываем их по P+ или всем обратно
	}

	void			setInputLine(FuelPipeline* pLine, int Num) 
	{
		if(pLine && Num > 0 && Num < M) 
			pInputLine[Num] = pLine; 
	}
	FuelPipeline*	getOutputLine(int Num) 
	{
		if(Num > 0 && Num < N)
			return &OutputLine[Num]; 
		return 0;
	}

protected:
	FuelPipeline	OutputLine[N];	//массив выходных магистралей
	FuelPipeline	*pInputLine;	//массив входных (извне) магистраль	

};*/

}

#endif