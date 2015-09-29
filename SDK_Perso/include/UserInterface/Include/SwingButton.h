#ifndef USERINTERFACE_SWINGBUTTON_H
#define USERINTERFACE_SWINGBUTTON_H

#include "./Static.h"

struct USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR SwingButtonParameters : public StaticParameters
{
public:
	SwingButtonParameters(int preset = 0);
	//Хранитель ресурсов
	RESOURCEKEEPER_DEFAULTRESOURCEFILE_H(SwingButtonParameters)
	virtual void serialize(Serializer &);

	typedef StaticParameters Base;

	void		read(const char*, int);

	int			switchTag;
	int			switchPress, switchUnpress;
};

class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR SwingButton : public Static
{
	DECLARE_MESSAGE_TABLE()

public:
	typedef Static Base;
	typedef SwingButtonParameters Parameters;

private:
	Control*		m_switch;
	int				m_switchTag;
	int				m_switchPress, m_switchUnpress;

	void			thisArrange(const SwingButtonParameters& params);
	void			init();	
protected:
	void			onCreate();
	void			onLButtonDown(int keys, int x, int y);
	void			onLButtonDblclk(int keys, int x, int y);
	void			onLButtonUp(int keys, int x, int y);
	void			onActivate(int state, Control* previous);
	void			onStyleChanging(bool deselect, int newStyle);
public:
	SwingButton(const Parameters& params) : Base(params)
	{
		init();
		thisArrange(params);
	}
	void arrange(const Parameters& params)
	{
		Base::arrange(params); 
		thisArrange(params);
	}
	void arrange(const ed::string& name) NAMEARRANGE(Parameters)

	virtual int		findFace();

}; // class SwingButton


#endif /* USERINTERFACE_SWINGBUTTON_H */
