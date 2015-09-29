#pragma once

#include "BalanceFM/BFMStructs.h"
namespace EagleFM
{
namespace BalanceFM
{

class THelicopter;
class TShassy abstract
{
protected:
	THelicopter*	m_parent;
	TShassy(THelicopter* parent) : m_parent(parent){}
public:
	virtual int Run(TStateVector* vec1) = 0;
	virtual void SetShassysHeights(SVec surf_point_w, SVec surf_norm_w){};
	virtual ~TShassy(){};
public:
	SVec F;//вектор сил на выход в связной системе координат
	SVec M;//вектор моментов на выход в связной системе координат
	double  _brake_pos;
	int Err;//если ощибка 1 иначе 0 (не работает)
};


}
}
