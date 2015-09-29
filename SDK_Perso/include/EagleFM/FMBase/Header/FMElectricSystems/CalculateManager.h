#ifndef __Elec_Calculate_Manager__ 
#define __Elec_Calculate_Manager__ 

#include "Items/ItemBase.h"
#include "Alglib/Base/ap.h"

namespace EagleFM { namespace Elec {

class CalcNode;
class Source;

typedef std::vector<CalcNode*> NodesVec;
typedef std::vector<ItemBase*> ItemVec;
typedef std::vector<Source*> SourceVec;
typedef std::vector<bool> BoolVec;

class FMBASE_API CalculateManager
{
public:
	CalculateManager();
	virtual ~CalculateManager();

	void calculate(double dt);
	
	void createTopology(int nodesN_, int branchesN_);
	void createDefaultTopology(int branchesN_);
	void setBranchSign(int node, int branch, int sign);
	void addNode(CalcNode* in, int node);
	void addBranch(ItemBase* in, int branch);
	void setVoltageEps(double valIn) {voltageEps = valIn;}

protected:
	double getBranchConductance(int branch) const;
	double getBranchEMF(int branch) const;

private:
	ap::real_2d_array A;          // Матрица соединений (узловая матрица)
	ap::real_1d_array Jdot;       // Вектор токов источников тока
	ap::real_1d_array Edot;       // Вектор ЭДС источников напряжения
	ap::real_1d_array Jydot;      // Вектор узловых токов
	ap::real_2d_array Y;          // Матрица проводимостей ветвей (диагональная)
	ap::real_2d_array Yy;         // Матрица узловых проводимостей
	ap::real_1d_array Psidot;     // Матрица узловых потенциалов (решение)
	ap::real_1d_array PsidotPrev; // Матрица узловых потенциалов на предыдущем шаге
	
	ap::real_1d_array workVec1;   // Промежуточный вектор;
	ap::real_1d_array workVec2;   // Промежуточный вектор;
	ap::real_2d_array workMtx;    // Промежуточная матрица;

	NodesVec nodes;
	ItemVec ordinaryBranches;
	SourceVec sourceBranches;
	BoolVec branchesTypes;

	int nodesN, branchesN;

	bool firstRun;
	bool initialized;
	double voltageEps;
};

} }

#endif // __Elec_Calculate_Manager__ 
