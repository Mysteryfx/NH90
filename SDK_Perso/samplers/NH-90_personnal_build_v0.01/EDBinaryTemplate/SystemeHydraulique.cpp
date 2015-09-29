#include "stdafx.h"
#include "SystemeHydraulique.h"

#include "SystemeElectrique.h"

#include "ccMainPanel.h"
#include "WorldManager.h"

using namespace cockpit;
using namespace cockpit::NH_90;
using namespace cockpit::hydraulique;
using namespace cockpit::moteur;
using namespace cockpit::fluide;
using namespace cockpit::element;

REGISTER_WORLD_FACTORY_EX(new WorldFactory<cockpit::NH_90::SystemeHydraulique>, SystemeHydraulique);

static const float updateTime = 0.2f;

SystemeHydraulique::SystemeHydraulique() : MFXDevice(type_de_systeme::systeme_hydraulique_mfx),
	pompe1(this, 20, updateTime, etat_des_elements_de_base_mfx::etat_inactif_mfx),
	pompe2(this, 20, updateTime, etat_des_elements_de_base_mfx::etat_inactif_mfx),
	pompe4(this, 20, updateTime, etat_des_elements_de_base_mfx::etat_inactif_mfx),
	epompe(this, 20, updateTime, etat_des_elements_de_base_mfx::etat_inactif_mfx, 1.0f, type_element_mfx::pompe_electrique_mfx),
	hyd1_tuyau1(this, updateTime), hyd1_tuyau2(this, updateTime), hyd1_tuyau3(this, updateTime),
	hyd2_tuyau1(this, updateTime), hyd2_tuyau2(this, updateTime), hyd2_tuyau3(this, updateTime),
	sov1(this, updateTime), sov2(this, updateTime), trsov(this, updateTime), utilsov(this, updateTime),

	reservoir1(this, type_reservoir_huile_mfx::hydraulique_mfx), reservoir2(this, type_reservoir_huile_mfx::hydraulique_mfx),
	mgb(this), ragb(this)
{
	electricite = false;

	mgb.setPuissance(0.0f);
	ragb.setPuissance(0.0f);

	epompe.brancheApres(&hyd1_tuyau1); epompe.lierMoteur(&mgb); epompe.lierReservoir(&reservoir1);
	pompe1.brancheApres(&hyd1_tuyau1); pompe1.lierMoteur(&mgb); pompe1.lierReservoir(&reservoir1);
	pompe2.brancheApres(&hyd2_tuyau1); pompe2.lierMoteur(&mgb); pompe2.lierReservoir(&reservoir2);
	pompe4.brancheApres(&hyd2_tuyau1); pompe4.lierMoteur(&ragb); pompe4.lierReservoir(&reservoir2);

	sov1.avantRobinet(&hyd1_tuyau1); sov1.apresRobinet(&hyd1_tuyau2);
	sov2.avantRobinet(&hyd2_tuyau1); sov2.apresRobinet(&hyd2_tuyau3);

	trsov.avantRobinet(&hyd1_tuyau1); trsov.apresRobinet(&hyd1_tuyau3);
	utilsov.avantRobinet(&hyd2_tuyau1); utilsov.apresRobinet(&hyd2_tuyau2);
}

SystemeHydraulique::~SystemeHydraulique(){}

void SystemeHydraulique::initialize(unsigned char ID, const ed::string& Name, const ed::string& script_name) {
	avDevice::initialize(ID, Name, script_name);

	listen_command_range(this, 3001, 3011);

	make_default_activity(updateTime);
}
void SystemeHydraulique::SetCommand(int command, float value) {
	switch (command) {
	case 3001:
		epompe.switchOnOff();
		this->verificationPompe();
		break;
	case 3003:
		if (value < 0.0f) {
			if (!sov2.estOuvert()) {
				sov2.ouvrir();
			}
			if (sov1.estOuvert()) {
				sov1.fermer();
			}
		}
		else if (value > 0.0f) {
			if (!sov1.estOuvert()) {
				sov1.ouvrir();
			}
			if (sov2.estOuvert()) {
				sov2.fermer();
			}
		}
		else {
			if (!sov1.estOuvert()) {
				sov1.ouvrir();
			}
			if (!sov2.estOuvert()) {
				sov2.ouvrir();
			}
		}
		this->verificationSov();
		break;
	case 3005:
		if (value < 0.0f) {
			if (utilsov.isOnOff()) {
				utilsov.setOnOff(false);
			}
		}
		else if (value > 0.0f) {
			if (!utilsov.isOnOff()) {
				utilsov.setOnOff(true);
			}
		}
		else {
			if ((!pompe2.isOnOff() || !pompe4.isOnOff()) && utilsov.isOnOff()) {
				utilsov.setOnOff(false);
			}
		}
		//pompe1.switchOnOff();
		//pompe2.switchOnOff();
		//pompe4.switchOnOff();
		break;
	case 3006:
		sov1.switchOnOff();
		break;
	case 3007:
		sov2.switchOnOff();
		break;
	case 3008:
		break;
	case 3009:
		break;
	case 3010:
		break;
	case 3011:
		break;
	}
}
void SystemeHydraulique::update() {
	if (get_context()->devices.get("SYSTEMEELECTRIQUE")->is_active()) {
		electriciteChange(static_cast<SystemeElectrique*>(get_context()->devices.get("SYSTEMEELECTRIQUE"))->isOnOff());
	}

	reservoir1.update();
	reservoir2.update();

	epompe.update();
	pompe1.update();
	pompe2.update();
	pompe4.update();

	hyd1_tuyau1.update();
	hyd2_tuyau1.update();

	sov1.update();
	sov2.update();
	trsov.update();
	utilsov.update();

	hyd1_tuyau2.update();
	hyd1_tuyau3.update();
	hyd2_tuyau2.update();
	hyd2_tuyau3.update();
}
bool SystemeHydraulique::checkAnimationConditions(int arg_num, int command, float& value) {
	switch (command) {
	case 3001:
		//Pompe Electrique On/Off
		if (get_context()->panel->GetArgument(arg_cache_epompe) < 1.0f) { //Vérification que le cache est bien enlevé.
			return false;
		}
		return true;
	case 3002:
		//Cache de la pompe Electrique
		if (value < 1.0f) {
			value = 1.0f;
		}
		return true;
	case 3003:
		//Switch SOV
		if (get_context()->panel->GetArgument(arg_cache_sov) < 1.0f) {
			return false;
		}
		if ((value < 0.0f) && (value != -1.0f)) {
			value = -1.0f;
		}
		else if ((value > 0.0f) && (value != 1.0f)) {
			value = 1.0f;
		}
		else if (value != 0.0f) {
			value = 0.0f;
		}
		return true;
	case 3004:
		//Cache SOV
		if (get_context()->panel->GetArgument(arg_switch_sov) != 0.0f) {
			return false;
		}
		else {
			if ((value < 1.0f) && (get_context()->panel->GetArgument(arg_cache_sov) != 0.0f)) {
				value = 0.0f;
			}
			else if ((value > 0.0f) && (get_context()->panel->GetArgument(arg_cache_sov) != 1.0f)) {
				value = 1.0f;
			}
			return true;
		}
	case 3005:
		//UtilSOV switch
		if (get_context()->panel->GetArgument(arg_cache_util) != 1.0f) {
			return false;
		}
		else {
			if ((value < 0.0f) && (value != -1.0f)) {
				value = -1.0f;
			}
			else if ((value > 0.0f) && (value = 1.0f)) {
				value = 1.0f;
			}
			else if (value != 0.0f){
				value = 0.0f;
			}
			return true;
		}
	case 3006:
		//Cache UtilSOV
		if (get_context()->panel->GetArgument(arg_switch_util) < 0.0f) {
			return false;
		}
		if ((value < 1.0f) && (get_context()->panel->GetArgument(arg_cache_util) != 0.0f)) {
			value = 0.0f;
		}
		else if ((value > 0.0f) && (get_context()->panel->GetArgument(arg_cache_sov) != 1.0f)) {
			value = 1.0f;
		}
		return true;
	default:
		return true;
	}
}

/* void electriciteChange(bool electricite)
* Le système électrique informe le système hydraulique que son état évolue.
*/
void SystemeHydraulique::electriciteChange(bool electricite) {
	if (this->electricite != electricite) {
		if (electricite) {
			electriciteOn();
		}
		else {
			electriciteOff();
		}
	}
}
/* void electriciteOn()
* Le système hydraulique conserve l'information selon laquelle le système électrique est on.
*/
void SystemeHydraulique::electriciteOn() {
	electricite = true;
	verifieTout();
}
/* void electriciteOff()
* Le système hydraulique conserve l'information selon laquelle le système électrique est off.
*/
void SystemeHydraulique::electriciteOff() {
	electricite = false;
	verifieTout();
}

/* void verificationSov()
* Vérification des SOV. Signifie qu'une sov à changé d'état.
*/
void SystemeHydraulique::verificationSov() {
	if (sov1.estOuvert() && sov1.isOnOff()) {
		mfd_ecran(arg_mfd_hyd_sov1, 1.0f);
	}
	else {
		mfd_ecran(arg_mfd_hyd_sov1, 0.0f);
	}
	if (sov2.estOuvert() && sov2.isOnOff()) {
		mfd_ecran(arg_mfd_hyd_sov2, 1.0f);
	}
	else {
		mfd_ecran(arg_mfd_hyd_sov2, 0.0f);
	}
	if (utilsov.estOuvert() && utilsov.isOnOff()) {
		mfd_ecran(arg_mfd_hyd_utsov, 1.0f);
	}
	else {
		mfd_ecran(arg_mfd_hyd_utsov, 0.0f);
	}
	if (trsov.estOuvert() && utilsov.isOnOff()) {
		mfd_ecran(arg_mfd_hyd_trsov, 1.0f);
	}
	else {
		mfd_ecran(arg_mfd_hyd_trsov, 0.0f);
	}
}
/* void verificationTuyau()
* Vérification des tuyaux. Signifie q'un tuyau à changé d'état.
*/
void SystemeHydraulique::verificationTuyau(){
	//Circuit Hydraulique 1
	if (hyd1_tuyau1.isOnOff()) { //Tuyau après les pompes 1 et électrique.
		mfd_ecran(arg_mfd_hyd_c12, 1.0f);
		mfd_ecran(arg_mfd_hyd_trans1, 1.0f);
		mfd_ecran(arg_mfd_hyd_pression1, 1.0f);
	}
	else {
		mfd_ecran(arg_mfd_hyd_c12, 0.0f);
		mfd_ecran(arg_mfd_hyd_trans1, 0.0f);
		mfd_ecran(arg_mfd_hyd_pression1, 0.0f);
	}
	if (hyd1_tuyau2.isOnOff()) { //Tuyau après la SOV1
		//Petite particularité, si la UtilSOV est fermé ou inactive, l'affichage pour lee train d'attérissage peut-être en emergency.
		if (utilsov.estOuvert() && utilsov.isOnOff()) { 
			mfd_ecran(arg_mfd_hyd_c11, 1.0f);
		}
		else {
			mfd_ecran(arg_mfd_hyd_c11, 0.5f);
		}
		mfd_ecran(arg_mfd_hyd_trans2, 1.0f);
		mfd_ecran(arg_mfd_hyd_fcp1, 1.0f);
	}
	else {
		mfd_ecran(arg_mfd_hyd_c11, 0.0f);
		mfd_ecran(arg_mfd_hyd_trans2, 0.0f);
		mfd_ecran(arg_mfd_hyd_fcp1, 0.0f);
	}
	if (hyd1_tuyau3.isOnOff()) { //Tuyau après la TRSOV
		mfd_ecran(arg_mfd_hyd_t1, 1.0f);
		mfd_ecran(arg_mfd_hyd_trp, 1.0f);
		mfd_ecran(arg_mfd_hyd_trans3, 1.0f);
	}
	else {
		mfd_ecran(arg_mfd_hyd_t1, 0.0f);
		mfd_ecran(arg_mfd_hyd_trp, 0.0f);
		mfd_ecran(arg_mfd_hyd_trans3, 0.0f);
	}
	//Circuit hydraulique 2
	if (hyd2_tuyau1.isOnOff()) { //Tuyau après les pompes 2 et 4.
		mfd_ecran(arg_mfd_hyd_c21, 1.0f);
		mfd_ecran(arg_mfd_hyd_pression2, 1.0f);
		mfd_ecran(arg_mfd_hyd_trans4, 1.0f);
	}
	else {
		mfd_ecran(arg_mfd_hyd_c21, 0.0f);
		mfd_ecran(arg_mfd_hyd_pression2, 0.0f);
		mfd_ecran(arg_mfd_hyd_trans4, 0.0f);
	}
	if (hyd2_tuyau2.isOnOff()) { //Tuyau après UtilSOV
		mfd_ecran(arg_mfd_hyd_c22, 1.0f);
		mfd_ecran(arg_mfd_hyd_trans5, 1.0f);
	}
	else {
		mfd_ecran(arg_mfd_hyd_c22, 0.0f);
		mfd_ecran(arg_mfd_hyd_trans5, 0.0f);
	}
	if (hyd2_tuyau3.isOnOff()) { //Tuyau après SOV2
		mfd_ecran(arg_mfd_hyd_t2, 1.0f);
		mfd_ecran(arg_mfd_hyd_fcp2, 1.0f);
		mfd_ecran(arg_mfd_hyd_trans6, 1.0f);
	}
	else {
		mfd_ecran(arg_mfd_hyd_t2, 0.0f);
		mfd_ecran(arg_mfd_hyd_fcp2, 0.0f);
		mfd_ecran(arg_mfd_hyd_trans6, 0.0f);
	}
}
/* void verificationPompe()
* Vérification des pompes. Signifie q'une pompe à changé d'état.
*/
void SystemeHydraulique::verificationPompe(){
	if (epompe.getPression() > ElementHydraulique_MFX::PRESSIONMARCHE) {
		mfd_ecran(arg_mfd_hyd_epompe, 1.0f);
		mfd_ecran(arg_mfd_hyd_epompe_pression, 1.0f);
	}
	else {
		mfd_ecran(arg_mfd_hyd_epompe, 0.0f);
		mfd_ecran(arg_mfd_hyd_epompe_pression, 0.0f);
	}
	if (pompe1.getPression() > ElementHydraulique_MFX::PRESSIONMARCHE) {
		mfd_ecran(arg_mfd_hyd_pompe1, 1.0f);
		mfd_ecran(arg_mfd_hyd_pompe1_pression, 1.0f);
	}
	else {
		mfd_ecran(arg_mfd_hyd_pompe1, 0.0f);
		mfd_ecran(arg_mfd_hyd_pompe1_pression, 0.0f);
	}
	if (pompe2.getPression() > ElementHydraulique_MFX::PRESSIONMARCHE) {
		mfd_ecran(arg_mfd_hyd_pompe2, 1.0f);
		mfd_ecran(arg_mfd_hyd_pompe2_pression, 1.0f);
	}
	else {
		mfd_ecran(arg_mfd_hyd_pompe2, 0.0f);
		mfd_ecran(arg_mfd_hyd_pompe2_pression, 0.0f);
	}
	if (pompe4.getPression() > ElementHydraulique_MFX::PRESSIONMARCHE) {
		mfd_ecran(arg_mfd_hyd_pompe4, 1.0f);
		mfd_ecran(arg_mfd_hyd_pompe4_pression, 1.0f);
	}
	else {
		mfd_ecran(arg_mfd_hyd_pompe4, 0.0f);
		mfd_ecran(arg_mfd_hyd_pompe4_pression, 0.0f);
	}
}
/* void verifieTout()
* Vérification de tous les éléments.
*/
void SystemeHydraulique::verifieTout(){
	verificationPompe();
	verificationSov();
	verificationTuyau();

	verificationVoyant();
}
/* void verificationVoyant()
* Vérification des voyants.
*/
void SystemeHydraulique::verificationVoyant() {

}
/* void elementAChangerEtat(element::type_element_mfx type)
* Un élément informe le système qu'il à changer d'état.
* On retrouve les éléments à vérifié par le type de celui-ci.
*/
void SystemeHydraulique::elementAChangerEtat(type_element_mfx type){
	switch (type) {
	case type_element_mfx::pompe_mfx:
	case type_element_mfx::pompe_electrique_mfx:
		verificationPompe();
		break;
	case type_element_mfx::robinet_sov_mfx:
		verificationSov();
		break;
	case type_element_mfx::tuyau_hydraulique_mfx:
		verificationTuyau();
		break;
	}
}

/* void mfd_ecran(int argument, float valeur)
* Effectue l'affichage dans le mfd correspondant.
*/
void SystemeHydraulique::mfd_ecran(int argument, float valeur){
	if (static_cast<SystemeElectrique*>(get_context()->devices.get("SYSTEMEELECTRIQUE"))->isOnOff()) {
		get_context()->panel->SetArgument(argument, valeur);
	}
}