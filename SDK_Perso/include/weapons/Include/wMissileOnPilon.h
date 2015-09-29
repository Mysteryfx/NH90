//	file wMissileOnPilon.h

#pragma once

#include "viObjectNode.h"

//class wStowage;
class wBarrel;

#define IID_IMissileOnPilon Common::identify<wMissileOnPilon>() 

class wMissileOnPilon : public viObjectNode
{
public:

    wMissileOnPilon();
	virtual ~wMissileOnPilon();

    virtual void				setLauncher(viObjectNode* theParent = 0, wBarrel* theBarrel = 0, unsigned char objName_ = 0);

	virtual cPosition&			Position(wModelTime t);
	const Graphics::DParamList& GetDrawArguments() { return dparams; }

	virtual void				serialize(Serializer &);
/*
    virtual void Parse(
        Graphics::ModelParser& parser,
        const Position3& pos,
        Graphics::effectState* effects=0);
*/
protected:
	
	cPosition					pos;
	Graphics::DParamList		dparams;	
	unsigned char				objName;
	wBarrel*	                barrel;
};

