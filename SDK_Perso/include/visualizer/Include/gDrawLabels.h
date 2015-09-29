//	gDrawLabels.h ***************************************************************

#pragma once

#include "Visualizer.h"
#include "viObjectNode.h"
#include "Renderer\RenderText.h"
#include <ed/string.h>
#include "wMovingObject.h"
//#include "ILabels.h"
#include "wsType.h"
/* Поведение маркеров
1. ...Mission->Options->Player->ExternalLabels, если false, то метки не рисуются вообще,
	иначе всё определяется конфигом
2. Структура LabelParams (globalLabels->getParams) определяет параметры появления
    и внешний вид всех типов маркеров (первоначально при загрузке инициализируется
	из файла Config/View/View.cfg)
*/

struct /*INTER_API*/ LabelParams
{
	// Какие типы маркеров показывать
	bool labelAirOn;
	bool labelGroundOn;
	bool labelNavyOn;
	bool labelMissileOn;

	// Как будет выглядеть метка на экране
	typedef ed::map<float, ed::string> fmtByDist;
	fmtByDist labelFormat[5];		// labelFormat[type.Level1()]

	// цвета меток
	Graphics::Color colorNeutrals;
	Graphics::Color colorAllies;
	Graphics::Color colorEnemies;
};

struct LabelData
{
	ed::string unitName;
	ed::string pilotName;
	ObjectID objId;
	bool isVisible;
	wModelTime lastCheckedTime;
};

class VISUALIZER_API gLabels//: public ILabels
{
	Graphics::RenderText  text;	// Параметры вывода текста.
		
	int	isLabelAll;		// Все метки.
	int	isLabelAir;		// Самолеты.
	int	isLabelGround;	// Наземные ракеты.
	int	isLabelNavy;	// Корабли.
	int	isLabelMissile;	// Летящие ракеты.
	int	isPilotName;	// Вывод имени пилота.
	LabelParams params; // Видимость отдельных полей метки и 
						// интервалы дальностей в котором отрисовываються метки
	
	typedef ed::map<ObjectID, LabelData> LabelsDataCache;
	LabelsDataCache labelCache;
	ed::vector <MovingObject *> labels;
	ed::vector<Vector3> pnts;
public:
	gLabels();
	virtual ~gLabels();

	static void setFont(const char*name, unsigned size);
	// ILabel Implementation
	virtual void setParams(LabelParams);
	virtual LabelParams getParams();
	
	bool IsNursLabeled(const wsType& type);
	bool IsWeaponLabeled(const wsType& type);
	bool IsLabeled(const wsType& type);
	
	void AddLabels(viObjectNode *obj);
	void Draw();

	void     All_OnOff(void);
	void     Air_OnOff(void);
	void  Ground_OnOff(void);
	void    Navy_OnOff(void);
	void Missile_OnOff(void);
	void IsPilotName(int PilotName);
	void ParseConfig(void);

    void Off()
    {
        isLabelAll		= 0;
        isLabelAir		= 0;
        isLabelGround	= 0;
        isLabelNavy		= 0;
        isLabelMissile	= 0;
    }

	void reset();
	void reset(ObjectID);
};

extern VISUALIZER_API gLabels *globalLabels;

