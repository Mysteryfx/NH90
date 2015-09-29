#pragma once

#include "Source.h"

namespace EagleFM { namespace Elec {

typedef enum type_Battery
{
	batt_Plumbum = 0,
	batt_NiCd,
	batt_AgZn,
	count_types,
};

//*************************************************************************

class FMBASE_API Battery : public Source
{
public:
	Battery(int cellsCount, double nominalCapacity, double constR_0);
	virtual ~Battery(void);

	// Perc - 0..1 rapport
	void SetNewCapacity(double capacity, double percents, double dt);
	void post_initialize();
	void simulate(double dt, double t_ambC);
	
	void SetTypeBattery(int type);
	
	double GetCurrentCapacity() const;
	double getNominalCapacity() const {return m_nominalCapacity;}

protected:
	void simulateLcl(double dt, double t_ambC);

	int m_CellsCount;				// Nombre de jarres de stockage

	// Configuration du type de batterie
	double m_constElement_max_EMF;	// banques EMF nominales
	double m_constEMF_Coeff;	    // Coefficient de EMF
	double m_constEMF_PowerCoeff;	// Le degré de coefficient EMF
	double m_constR_Coeff;		    // Coefficient de résistance interne
	double m_constR_PowerCoeff;		// Le degré de coefficient de résistance interne
	double m_constR_0;              // Résistance nominale

	// Capacité. Il peut être négatif!
	double m_capacity;              

	double m_overCHRG;              // Paramètre de la charge au-delà de la (charge)
	double m_offset;                // Facteur pour la formation de la substance à analyser. Le calcul de la courbe EMF
	double m_ocSpread;              // Facteur pour la formation de la substance à analyser. Le calcul de la courbe EMF
	// Coefficient EMF supplémentaire, qui est utilisé dans la batterie
	double kEMF_addCharge;

	double m_C;	                    // Facteur qui correspond à la capacité nominale
	double m_nominalCapacity;

	double EMF_addCharge;

private:
	void CalculateParameters(double charge, double dt);

};

} }
