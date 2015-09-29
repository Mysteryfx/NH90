#ifndef ED_INTER_EFFECT_CONNECTOR_TYPE_H
#define ED_INTER_EFFECT_CONNECTOR_TYPE_H

//типы коннекторов модели для рождения эффектов
//TODO: добавить остальные типы коннекторов из art/docs/коннекторы/коннекторы.doc, и допилить их загрузку
enum EffectConnectorType
{
	GEAR_CENTER,
	GEAR_RIGHT,
	GEAR_LEFT,		

	WINGTIP_LEFT,
	WINGTIP_RIGHT,

	AFTERBURN_LEFT,
	AFTERBURN_RIGHT,

	INVERSE_LEFT,
	INVERSE_RIGHT,

	FUEL_JETTISON_LEFT,
	FUEL_JETTISON_RIGHT,

	EXHAUST_1,
	EXHAUST_2,
	EXHAUST_3,
	EXHAUST_4,
	EXHAUST_5,
	EXHAUST_6,
	EXHAUST_7,
	EXHAUST_8,
	EXHAUST_9,
	EXHAUST_10,
	EXHAUST_11,
	EXHAUST_12,
		
	CT_VALUES_COUNT,	//количество элементов в массиве, руками не трогать
	CT_CUSTOM		//положение эффекта будет задано явно через параметр функции
};

#endif
