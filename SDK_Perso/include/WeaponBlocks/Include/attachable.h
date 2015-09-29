#ifndef __ATTACHABLE_H__
#define __ATTACHABLE_H__

#include "WeaponBlocks.h"

class WEAPON_BLOCKS_API wAttachableImpl
{
protected:
	wAttachableImpl();

protected:
	void load_(const ed::string& model_name) const;

protected:
	mutable bool loaded_;
	mutable Math::Vec3d attach_pos_;
	mutable Math::Matrix3d attach_orient_;
};

// Загружает точку подвеса из модели
template <class T>
class WEAPON_BLOCKS_API wAttachableDescriptor : public wAttachableImpl
{
public:
	const Math::Vec3d& getConnectorPos() const { if (!loaded_) load_(reinterpret_cast<const T*>(this)->getModelName()); return attach_pos_; }
	const Math::Matrix3d& getConnectorOrientation() const { if (!loaded_) load_(reinterpret_cast<const T*>(this)->getModelName()); return attach_orient_; }
};

#endif
