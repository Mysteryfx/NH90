#ifndef viMovingObject_H
#define viMovingObject_H

#include "edObjects.h"
#include "viObjectNode.h"
#include "MovingObjects\MovingHandle.h"

class viSearch;

// объекты, хранящиеся в MovingObjectStorage
class EDOBJECTS_API viMovingObject : 
	public viObjectNode, 
	public MovingHandle,
	public LinkHost
{
public:
	viMovingObject(viFlag type = viUnknown, const Vector3& _p = Vector3());
	virtual ~viMovingObject();

	virtual float GetSize();

	// Скорость
	virtual	float	ScalarVelocity() const;
	virtual const cVector&	VectorVelocity() const;
	virtual const cVector&	VectorAngular() const;

	virtual void serialize(Serializer &);

private: // FORBIDDEN
        viMovingObject(const viMovingObject&);
    void operator = (const viMovingObject&);
};

#endif // viMovingObject_H