#include "stdafx.h"
#include "Pompe_MFX.h"
#include "Moteur_MFX.h"

using namespace cockpit;
using namespace cockpit::element;
using namespace cockpit::fluide;
using namespace cockpit::hydraulique;
using namespace cockpit::moteur;

Pompe_MFX::Pompe_MFX(MFXDevice *systeme_maitre, float temperature, float updateTime, etat_des_elements_de_base_mfx etat_element_pompe, float pression, type_element_mfx type_element_pompe) :
ElementHydraulique_MFX(systeme_maitre, temperature, pression, type_element_pompe, etat_element_pompe, updateTime)
{
	this->moteur	= nullptr;
	this->reservoir = nullptr;
	this->tuyau		= nullptr;
}
Pompe_MFX::~Pompe_MFX()
{
	panne();
	this->reservoir = nullptr;
	this->moteur	= nullptr;
	this->tuyau		= nullptr;
}

/* void lierMoteur(Moteur * moteur)
* Lier un moteur � la pompe.
*/
void Pompe_MFX::lierMoteur(Moteur_MFX * moteur){
	this->moteur = moteur;
}
/* void lierReservoir(fluide::ReservoirHuile_MFX * reservoir)
* Lier un r�servoir � la pompe.
*/
void Pompe_MFX::lierReservoir(ReservoirHuile_MFX * reservoir) {
	this->reservoir = reservoir;
}
/* void demarrage()
* D�marrage d'une pompe hydraulique.
*/
void Pompe_MFX::demarrage() {
	Debuggueur::log("Pompe_MFX::demarrage => 31");
	switch (getType()){
	case type_element_mfx::pompe_electrique_mfx:
		setEtatElementBase(etat_des_elements_de_base_mfx::etat_actif_mfx);
		Debuggueur::log("Puissance moteur > 4%");
		demarragePompe();
		tuyau->changementPression(getPression());
		break;
	case type_element_mfx::pompe_mfx:
		if (moteur != nullptr) {
			if (moteur->isOnOff() && (moteur->getPuissance() > 4)) {
				setEtatElementBase(etat_des_elements_de_base_mfx::etat_actif_mfx);
				Debuggueur::log("Puissance moteur > 4%");
				demarragePompe();
				tuyau->changementPression(getPression());
			}
		}
		break;
	}
}
/* void demarragePompe()
* D�marrage de la pompe.
* Lors du d�mmarrage de la pompe, la pression augmente quasi instantan�ment de 0 � 80 bar puis progressivement jusqu'a atteindre la pression nominal.
*
* Attention : Si la pression courante de la pompe est d�j� � la pression nominal, il ne faut pas l'augment�.
* Elle ne doit pas �tre sup�rieur � la pression nominal lors d'un fonctionnement normal.
*/
void Pompe_MFX::demarragePompe() {
	Debuggueur::log("Pompe_MFX::demarragePompe => 48");
	static bool demarrage_ok = false;
	if (pression_courante < getPressionNominal()) {
		Debuggueur::log("Pression en augmentation");
		if (pression_courante < 10.0f) {
			pression_courante += 10 / 5 * updateTime ;
		}
		else {
			pression_courante += (((getPressionNominal() / 5 ) - 10) * updateTime);
			if (!demarrage_ok && (pression_courante > 163)) {
				setEtatElementBase(etat_des_elements_de_base_mfx::etat_actif_mfx);
				demarrage_ok = true;
			}
			if (pression_courante >= getPressionNominal()) {
				pression_courante = getPressionNominal();
				demarrage_ok = false;
				actionEffectue();
			}
		}
	}
}
/* void arret()
* Proc�dure d'arr�t d'un �l�ment hydraulique.
*/
void Pompe_MFX::arret(){
	Debuggueur::log("Pompe_MFX::arret => 73");
	static bool pas_encore_arreter = true;
	if (pression_courante > 0.0f) {
		Debuggueur::log("Pression en diminution");
		pression_courante -= (getPressionNominal() / 2.0f * updateTime);
		if (pas_encore_arreter && (pression_courante < 163)) {
			setEtatElementBase(etat_des_elements_de_base_mfx::etat_inactif_mfx);
			pas_encore_arreter = false;
		}
		if (pression_courante <= 0.0f) {
			pression_courante = 0.0f;
			actionEffectue();
		}
	}
	tuyau->changementPression(getPression());
}

/* void update()
* Mise � jour de la pompe.
*/
void Pompe_MFX::update(){
	if (moteur != nullptr) {
		ElementHydraulique_MFX::update();
		Debuggueur::log("Pression : " + Utilitaire::convertFloatToString(getPression()));
	}
	else {
		if ((pression_courante != 0.0f) && ((action_a_execute == action_des_elements_mfx::action_demarrage_mfx) || (action_a_execute == action_des_elements_mfx::action_recuperation_mfx))) {
			action_a_execute = action_des_elements_mfx::action_panne_mfx;
		}
		switch (action_a_execute) {
		case action_des_elements_mfx::action_panne_mfx:
			panne();
			break;
		case action_des_elements_mfx::action_arret_mfx:
			arret();
			break;
		}
	}
}

/* void actionEffectue()
* Lorsque l'action demand� par l'utilisateur est terminer, toutes les fonctions d'actions font appel � cette fonction.
* Elle valide � l'�l�ment que l'action s'est terminer.
*/
void Pompe_MFX::actionEffectue() {
	action_demande = false;
}
/* void fuite()
* Demande � l'�l�ment ant�rieur de faire la simulation d'une fuite.
* Cette demande doit-�tre effectu� par un �l�ment pr�c�dent celui-ci qui serais en panne.
*/
void Pompe_MFX::fuite() {
	reservoir->fuite();
}
/* void panne()
* Proc�dure de mise en panne des �l�ments de base.
*/
void Pompe_MFX::panne() {
	setEtatElementBase(etat_des_elements_de_base_mfx::etat_panne_mfx);
	if (reservoir->getQuantite() > (reservoir->getReservoir() * 0.7f)) {
		reservoir->fuite();
		if (getPression() > (getPressionNominal() * 0.8f)) { setPression(getPression() * 0.8f); tuyau->changementPression(getPression()); }
	}
	else if (reservoir->getQuantite() > (reservoir->getReservoir() * 0.3)) {
		setPression(getPression() - 5.0f);
		tuyau->changementPression(getPression());
	}
	else {
		setPression(0.0f);
		tuyau->changementPression(getPression());
	}
	fuite();
}
/* void restauration(int ancienne_etat)
* Proc�dure de restauration des �l�ments de base.
* Remet l'�l�ment � l'ancienne �tat (avant �tat de panne).
*/
void Pompe_MFX::restauration(int ancienne_etat) {
	ElementDeBase_MFX::restauration(ancienne_etat);
}
/* void brancheApres(Tuyau_MFX * tuyau)
* Branch� un tuyau apr�s la pompe.
*/
void Pompe_MFX::brancheApres(Tuyau_MFX * tuyau) {
	this->tuyau = tuyau;
	this->tuyau->avantTuyau(static_cast<ElementHydraulique_MFX*>(this));
}
/* void changementPression(float nouvelle_pression)
* Comme les �l�ments hydraulique sont connect� les uns aux autres, lorsqu'il y a une modification de pression dans l'un d'entre eux, les autres sont aussi impacter.
* On donne la nouvelle pression en m�me temps.
*/
void Pompe_MFX::changementPression(float nouvelle_pression) {
	setPression(nouvelle_pression);
}