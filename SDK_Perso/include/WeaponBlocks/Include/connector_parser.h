#ifndef __CONNECTOR_PARSER_H__
#define __CONNECTOR_PARSER_H__

#include "weapon_exception.h"

SIMPLE_SUBEXCEPTION(wConnectorParserException, wWeaponException);

#include "Math/Vector.h"
#include "Math/Rotation3.h"

namespace Graphics { class Model; }

class wConnectorParser
{
public:
	wConnectorParser(const ed::string& model_name);

	// Эта операция не легче, чем getConnectorPos!!!
	Math::Vec3d getConnectorPos(const ed::string& connector_name);
	void getConnectorInfo(const ed::string& connector_name, Math::Vec3d& pos, Math::Matrix3d& rot);

protected:
	Graphics::Model* model_;
};

#endif
