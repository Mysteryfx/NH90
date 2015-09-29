#ifndef DXGeometry_H
#define DXGeometry_H

#include "Renderer\Geometry.h"

namespace Graphics
{

// Интерфейс для геометрии DirectX
class DXGeometry : public Geometry {
protected:
	uint32_t FVF;
public:
	DXGeometry() {FVF=0;}

    inline uint32_t GetFVF() {return FVF;}

	virtual bool SupportsUnload(){return true;}
};

} // namespace Graphics

#endif // DXGeometry_H
