#ifndef viObjectNode_H
#define viObjectNode_H

#include <ed/vector.h>

#include "viObjectShape.h"

class EDOBJECTS_API viObjectNode : public viObjectShape
{
private:
	int parent_index;
	viObjectNode *parent;

	typedef ed::vector<viObjectNode *> ChildrenList;
	ChildrenList children;
	
	int AddChild(viObjectNode *child);
	void RemoveChild(int i);
protected:
	virtual void  OnParentDeath()
	{
		parent = nullptr;
	}
public:
	viObjectNode(viFlag type = viUnknown, viObjectNode *_parent = 0);
	virtual ~viObjectNode();

	// брать ли в расчет детей (для рисования, столкновений и т.д.)
	virtual bool UseChildren(wModelTime t, viArgumentPurpose purpose);

	//from ModelInstance
	/// Don't work.
	virtual const Position3& GetPosition();


	viObjectNode *Parent() const;
	int ChildCount() const;	// возвращает вместе с нулевыми детьми
	viObjectNode *Child(int i) const;
	void ChangeParent(viObjectNode *_parent);

    virtual void serialize(Serializer &);
};

#endif // viObjectNode_H
