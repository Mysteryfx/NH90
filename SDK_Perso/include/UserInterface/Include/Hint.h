#pragma once

#include "Static.h"

//---------------------------------------------------------------------------
// Функции для работы с hint'om. Рекомендуется использовать их, а не работать 
// напрямую с классом Hint
class Hint;

USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR Hint*	getHint		();
USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR void	showHint	(const char* inText, Control* inControl);
USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR void	hideHint	(bool inForceHide);
USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR void	paintHint	();
USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR void	deleteHint	();

//---------------------------------------------------------------------------
struct USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR HintParameters : StaticParameters
{
public:
	HintParameters(int preset = 0);
	//Хранитель ресурсов
	RESOURCEKEEPER_DEFAULTRESOURCEFILE_H(HintParameters)
	virtual void serialize(Serializer &);

	typedef StaticParameters Base;	

	void read(const char* text, int size);

	// Параметры
	int			lifeTime;
	int			delayTime;

	Point		shiftFromCursor;
	Point		shiftFromBorder;
};

class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR Hint : public Static
{
public:
	typedef Static Base;
	typedef HintParameters Parameters;
private:
	DECLARE_MESSAGE_TABLE()
	
	void					thisArrange(const HintParameters& prms); 
	void					calculateCoordinates();
protected:
	// Параметры
	int						delayTime;
	unsigned int			lifeTime;
	Point					shiftFromCursor;
	Point					shiftFromBorder;

	int						birthTime; // время появления подсказки
	bool					needCalculateCoord;

	Control*				m_Control;

	void					onPaint();
public:
	//Конструкторы
							Hint(const Parameters& params);
							Hint();
	virtual					~Hint();

	// Отображение
	void					show(const char* text, Control* inControl);
	void					hide(bool inForceHide);

	// Параметры
	void					setDelayTime(int time)	{delayTime = time;}
	int						getDelayTime() const	{return delayTime;}
	void					setLifeTime(int time)	{lifeTime = time;}
	int						getLifeTime()			{return lifeTime;}

	// Оформление
	void					arrange(const Parameters& params)
							{ Base::arrange(params); thisArrange(params); }
	void					arrange(const ed::string& name) NAMEARRANGE(Parameters);

	friend void				paintHint();
};

