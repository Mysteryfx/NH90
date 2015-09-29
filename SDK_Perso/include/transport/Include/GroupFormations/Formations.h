#pragma  once

#include "GroupFormations/FormationColumn.h"
#include "GroupFormations/FormationWedge.h"
#include "GroupFormations/FormationRow.h"
#include "GroupFormations/FormationDiamond.h"
#include "GroupFormations/FormationEchelon.h"
#include "GroupFormations/FormationVee.h"
#include "GroupFormations/FormationFree.h"
#include "ed/unordered_map.h"

class FormationsFactory
{
public:
	static FormationsFactory& instance();

	Mem::Ptr<Formation> create(CarFormationType fType, bool isHuman);

private:
	FormationsFactory();
	~FormationsFactory();
	bool addFormation(CarFormationType fType, bool isHuman, Mem::Ptr<Formation>);

	ed::unordered_map<int, Mem::Ptr<Formation>> _baseFormations;
};