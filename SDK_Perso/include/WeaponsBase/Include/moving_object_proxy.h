#ifndef __ENGINE_PROXY_H__
#define __ENGINE_PROXY_H__

#include <ed/string.h>

#include "WeaponsBase.h"

#include "Math/Position.h"
#include "wMovingObject.h"


class WEAPONSBASE_API wMovingObjectProxy : public MovingObject
{
public:
	wMovingObjectProxy();

	void setModel(const ed::string& str);
	void setPosition(wModelTime t, const Math::Pos3dd& pos, const Math::Vec3d& vel);
	void setParent(viMovingObject* parent);
	viMovingObject* getParent();

	void setMaxDrawArgIndex(int v);
	void setDrawArg(int index, double value);

	void setLocalBox(const Math::Vec3d& min, const Math::Vec3d& max);

	// Из MovingObject и viObject 
	// Из-за особенностей локона приходится перекрывать две функции для получения позиции
	virtual cPosition& Position(wModelTime t);
	virtual dVector GetPoint();
	virtual bool UseChildren(wModelTime t, viArgumentPurpose purpose) { return true; }

	virtual const gBox& LocalBox(wModelTime t);

	virtual const Graphics::DParamList& GetDrawArguments() { return drawArgs_; }

	virtual int Class();
protected:
	// GetPoint вызывается несколько раз за кадр - лучше кешировать
	dVector localPos_;
	dVector vel_;
	cPosition localOrient_;

	cPosition time_pos;

	viMovingObject* parent_;

	Graphics::DParamList drawArgs_;

	bool overrideBox_;
};

#endif
