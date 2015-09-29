#include "stdafx.h"
#include "Tuyau_MFX.h"
#include "Pompe_MFX.h"
#include "RobinetSOV_MFX.h"

using namespace cockpit;
using namespace cockpit::hydraulique;
using namespace cockpit::element;

Tuyau_MFX::Tuyau_MFX(MFXDevice *systeme_maitre, float updateTime, float temperature_courante, float pression_courante, etat_des_elements_de_base_mfx etat_element_hydraulique) :
	ElementHydraulique_MFX(systeme_maitre, temperature_courante, pression_courante, type_element_mfx::tuyau_hydraulique_mfx, etat_element_hydraulique, updateTime){}
Tuyau_MFX::~Tuyau_MFX() {
	setEtatElementBase(etat_des_elements_de_base_mfx::etat_panne_mfx);
	brancheApres.clear();
	brancheAvant.clear();
}

/* void setPression(float pression)
* Modifie la pression de l'�l�ment.
*/
void Tuyau_MFX::setPression(float pression){
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
	if (!brancheApres.empty()) {
		for (int i = 0; i < brancheApres.size(); i++) {
			brancheApres.at(i)->changementPression(getPression());
		}
	}
}
/* float getPressionAvant()
* Donne la plus haute pression des �l�ments branch� en amont du tuyau.
*/
float Tuyau_MFX::getPressionAvant() {
	switch (brancheAvant.at(0)->getType()) {
	case type_element_mfx::pompe_mfx:
	case type_element_mfx::pompe_electrique_mfx:
		return getPompePression();
	case type_element_mfx::robinet_sov_mfx:
		return static_cast<hydraulique::RobinetSOV_MFX*>(brancheAvant.at(0))->getPression();
	default:
		return 0.0f;
	}
}
float Tuyau_MFX::getPompePression() {
	hydraulique::Pompe_MFX *pompe1 = static_cast<hydraulique::Pompe_MFX*>(brancheAvant.at(0));
	hydraulique::Pompe_MFX *pompe2 = static_cast<hydraulique::Pompe_MFX*>(brancheAvant.at(1));
	return (pompe1->getPression() >= pompe2->getPression() ? pompe1->getPression() : pompe2->getPression());
}

/* void avantTuyau(element::ElementHydraulique_MFX *element)
* Attach� un �l�ment avant le tuyau.
* Le tuyau transfert la pression venant de cet �l�ment � l'�l�ment attach� apr�s.
*/
void Tuyau_MFX::avantTuyau(ElementHydraulique_MFX *element) {
	brancheAvant.push_back(element);
}
/* void apresTuyau(element::ElementHydraulique_MFX *element)
* Attach� un �l�ment apr�s le tuyau.
* Le tuyau transfert donc la pression venant de l'�l�ment attach� avant � celui attach� apr�s.
*/
void Tuyau_MFX::apresTuyau(ElementHydraulique_MFX *element) {
	brancheApres.push_back(element);
}
/* void coupurePression()
* Coupure de la pression dans le tuyau. Instruction venant d'un robinet SOV.
* Pression descendue � 0 et influence sur les �l�ments pr�c�dant.
*/
void Tuyau_MFX::coupurePression() {
	setPression(0.0f);
}
/* void ouverturePression()
* Ouverture de la pression dans le tuyau. Instruction venant d'un robinet SOV.
* Pression r�cup�rer depuis l'/les �l�ments ant�rieur.
*/
void Tuyau_MFX::ouverturePression() {
	setPression(getPressionAvant());
}

/* void demarrage()
* Lors du d�marrage d'une pompe hydraulique quelcquonque.
*/
void Tuyau_MFX::demarrage() {
	Debuggueur::log("Tuyau_MFX::demarrage => 33");
	if (!brancheAvant.empty()) {
		setPression(getPressionAvant());
	}
	actionEffectue();
}
/* void arret()
* Proc�dure d'arr�t d'un �l�ment hydraulique.
*/
void Tuyau_MFX::arret(){
	Debuggueur::log("Tuyau_MFX::arret => 55");
	setPression(0.0f);
	actionEffectue();
}
/* void restauration(int ancienne_etat)
* Proc�dure de restauration des �l�ments de base.
* Remet l'�l�ment � l'ancienne �tat (avant �tat de panne).
*/
void Tuyau_MFX::restauration(int ancienne_etat){
	ElementDeBase_MFX::restauration(ancienne_etat);
}
/* void panne()
* Proc�dure de mise en panne des �l�ments de base.
*/
void Tuyau_MFX::panne(){
	setEtatElementBase(etat_des_elements_de_base_mfx::etat_panne_mfx);
	if ((getPressionAvant() == getPressionNominal()) && (getPression() > (getPressionNominal() * 0.5f))) {
		setPression(getPression() - 5.0f);
	}
	else if (getPressionAvant() != getPressionNominal()) {
		setPression(getPression() - 5.0f);
	}
	fuite();
}
/* void fuite()
* Demande � l'�l�ment ant�rieur de faire la simulation d'une fuite.
* Cette demande doit-�tre effectu� par un �l�ment pr�c�dent celui-ci qui serais en panne.
*/
void Tuyau_MFX::fuite() {
	for (int i = 0; i <= brancheAvant.size(); i++) {
		brancheAvant.at(i)->fuite();
	}
}
/* void changementPression(float nouvelle_pression)
* Comme les �l�ments hydraulique sont connect� les uns aux autres, lorsqu'il y a une modification de pression dans l'un d'entre eux, les autres sont aussi impacter.
* On donne la nouvelle pression en m�me temps.
*/
void Tuyau_MFX::changementPression(float nouvelle_pression) {
	setPression(nouvelle_pression);
}