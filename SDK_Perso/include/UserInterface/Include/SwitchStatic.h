#ifndef UI_SWITCH_STATIC_H
#define UI_SWITCH_STATIC_H

#include "Static.h"

struct SwitchStaticParameters: public StaticParameters
{
public:
	SwitchStaticParameters(int preset = 0);
	RESOURCEKEEPER_DEFAULTRESOURCEFILE_H(SwitchStaticParameters)
	virtual void serialize(Serializer &);

	typedef StaticParameters Base;

	void	read(const char*, int);

	int		switchTag;
	int		switchState;
};

class SwitchWafer;
class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR SwitchStatic : public Static {
	DECLARE_MESSAGE_TABLE()

public:
	typedef Static Base;
	typedef SwitchStaticParameters Parameters ;

private:
	SwitchWafer*		m_switch;
	int					m_switchTag;
	int					m_switchState;

	void				thisArrange(const Parameters& params);
	void				init();	
protected:
	void				onCreate();
	void				onClose();
	void				onLButtonDown(int keys, int x, int y);
public:
	SwitchStatic(const Parameters& params) : Base(params) { init(); thisArrange(params); }
	void	arrange(const Parameters& params) { Base::arrange(params); thisArrange(params); }

	int		getSwitchState() { return m_switchState;}
}; 

#endif /* UI_SWITCH_STATIC_H */
