#ifndef USERINTERFACE_SWITCH_WAFER_H
#define USERINTERFACE_SWITCH_WAFER_H

#include <ed/vector.h>
#include "ui_Control.h"
#include "UI/Selector.h"

struct SwitchWaferParameters: public ControlParameters
{
public:
	SwitchWaferParameters(int preset = 0);
	RESOURCEKEEPER_DEFAULTRESOURCEFILE_H(SwitchWaferParameters)
	virtual void serialize(Serializer &);

	typedef ControlParameters Base;
	
	void	read(const char*, int);

	int		m_min_distance;
};

class SwitchStatic;
typedef ed::vector<SwitchStatic*>	SwitchStaticPtrVector;

class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR SwitchWafer : public Control, public UI::Selector
{
	DECLARE_MESSAGE_TABLE()
public:
	typedef Control Base;
	typedef SwitchWaferParameters Parameters;

public:
	SwitchWafer(const Parameters&);

	void			arrange(const Parameters& params) { Base::arrange(params); thisArrange(params); }
	void			registerSwitchStatic(SwitchStatic* switch_static);
	void			unregisterSwitchStatic(SwitchStatic* switch_static);

	/* UI::Selector:: */
	virtual bool	setPosition(int);
	virtual int		getPosition() const;

	virtual bool	setPositionRange(int);
	virtual int		getPositionRange() const;

	virtual void	allowPosition(bool, int, int);

protected:
	void			onLButtonDown(int keys, int x, int y);
	void			onLButtonDblclk(int keys, int x, int y);
		
private:
	int				m_pos;
	int				m_numpos;
	int				m_dir;
	int				m_min_distance;

	SwitchStaticPtrVector	m_linked_statics;

	void			thisArrange(const Parameters&);
	int				getDistance(int x, int y, Control* linked_static);
};


#endif /* USERINTERFACE_SWITCH_WAFER_H */
