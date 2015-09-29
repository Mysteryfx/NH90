#include "stdafx.h"
#include "RobinetSOV_MFX.h"

using namespace cockpit;
using namespace cockpit::hydraulique;
using namespace cockpit::element;

RobinetSOV_MFX::RobinetSOV_MFX(MFXDevice *systeme_maitre, float updateTime, float temperature, float pression, etat_des_elements_de_base_mfx etat_robinet) :
	ElementHydraulique_MFX(systeme_maitre, temperature, pression, type_element_mfx::robinet_sov_mfx, etat_robinet, updateTime) 
{
	brancheApres = nullptr;
	brancheAvant = nullptr;
}
RobinetSOV_MFX::~RobinetSOV_MFX(){
	brancheApres = nullptr;
	brancheAvant = nullptr;
	ouvert = false;
}


/* void avantRobinet(Tuyau_MFX *tuyau)
* Attaché un tuyau avant le robinet.
* Le robinet transfert la pression venant de ce tuyau au tuyau attaché après.
*/
void RobinetSOV_MFX::avantRobinet(Tuyau_MFX *tuyau){
	brancheAvant = tuyau;
	brancheAvant->apresTuyau(static_cast<ElementHydraulique_MFX*>(this));
}
/* void apresRobinet(Tuyau_MFX *tuyau)
* Attaché un tuyau après le robinet.
* Le robinet transfert donc la pression venant ddu tuyau avant à celui attaché après.
*/
void RobinetSOV_MFX::apresRobinet(Tuyau_MFX *tuyau){
	brancheApres = tuyau;
	brancheApres->avantTuyau(static_cast<ElementHydraulique_MFX*>(this));
}

/* void demarrage()
* Lors du démarrage d'une pompe hydraulique quelcquonque.
*/
void RobinetSOV_MFX::demarrage(){
	//Debuggueur::log("RobinetSOV_MFX::demarrage => 41");
	setPression(brancheAvant->getPression());
	//Debuggueur::log("setPression effectue ; this->pression : " + Utilitaire::convertFloatToString(getPression()));
	brancheApres->demarrage();
	actionEffectue();
}
/* void arret()
* Procédure d'arrêt d'un élément hydraulique.
*/
void RobinetSOV_MFX::arret(){
	setPression(0.0f);
	brancheApres->arret();
	actionEffectue();
}
/* void restauration(int ancienne_etat)
* Procédure de restauration des éléments de base.
* Remet l'élément à l'ancienne état (avant état de panne).
*/
void RobinetSOV_MFX::restauration(int ancienne_etat){
	ElementDeBase_MFX::restauration(ancienne_etat);
}
/* void panne()
* Procédure de mise en panne des éléments de base.
*/
void RobinetSOV_MFX::panne(){
	setEtatElementBase(etat_des_elements_de_base_mfx::etat_panne_mfx);
	if ((brancheAvant->getPression() == getPressionNominal()) && (getPression() > (getPressionNominal() * 0.5f))) {
		setPression(getPression() - 5.0f);
	}
	else if (brancheAvant->getPression() != getPressionNominal()) {
		setPression(getPression() - 5.0f);
	}
	fuite();
}
/* void fuite()
* Demande à l'élément antérieur de faire la simulation d'une fuite.
* Cette demande doit-être effectué par un élément précédent celui-ci qui serais en panne.
*/
void RobinetSOV_MFX::fuite(){
	if (ouvert || (getEtatElementBase() == etat_des_elements_de_base_mfx::etat_panne_mfx)) {
		brancheAvant->fuite();
	}
}
/* void setPression(float pression)
* Modifie la pression de l'élément.
*/
void RobinetSOV_MFX::setPression(float pression){
	if (pression < 0.0f) {
		pression = 0.0f;
	}
	ElementHydraulique_MFX::setPression(pression);
	if (getPression() >= ElementHydraulique_MFX::PRESSIONMARCHE) {
		setEtatElementBase(etat_des_elements_de_base_mfx::etat_actif_mfx);
	}
	else {
		setEtatElementBase(etat_des_elements_de_base_mfx::etat_inactif_mfx);
	}
	if (brancheApres != nullptr) {
		brancheApres->changementPression(pression);
	}
}