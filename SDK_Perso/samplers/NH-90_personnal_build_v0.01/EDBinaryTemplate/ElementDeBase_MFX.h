#pragma once
#include "Debuggueur.h"

namespace cockpit{
	class MFXDevice;
	namespace element {

		typedef enum etat_des_elements_de_base_mfx
		{
			etat_panne_mfx		= 0,
			etat_actif_mfx		= 1,
			etat_inactif_mfx	= 2,
		};
		typedef enum type_element_mfx{
			mecanique_mfx,
			bouton_mfx,
			verrou_mfx,
			voyant_mfx,
			bouton_deux_position_mfx,
			bouton_deux_position_electrique_mfx,
			bouton_trois_position_mfx,
			bouton_trois_position_electrique_mfx,
			moteur_mfx,
			rotor_mfx,

			electrique_mfx,
			batterie_mfx,
			alternateur_mfx,
			transforedresseur_mfx,
			generateur_secour_mfx,
			mfd_mfx,
			multiprise_mfx,

			fluide_mfx,
			reservoir_mfx,

			hydraulique_mfx,
			pompe_mfx,
			pompe_electrique_mfx,
			robinet_sov_mfx,
			tuyau_hydraulique_mfx,
			verin_hydraulique_mfx,

		};
		typedef enum action_des_elements_mfx {
			action_demarrage_mfx,
			action_arret_mfx,
			action_panne_mfx,
			action_restauration_mfx,
			action_recuperation_mfx,
		};

		class ElementDeBase_MFX
		{
		public:
			/* ElementDeBase_MFX(type_element_mfx type_element_base, etat_des_elements_de_base_mfx etat_element_base, float updateTime = 0.2f)
			* Pour créer un élément de base, il faut :
			*				- type_element_mfx type_element_base, le type de l'élément;
			*				- etat_des_elements_de_base_mfx etat_element_base, l'état de l'élément de base (par défaut : etat_inactif_mfx).
			*				- float updateTime, le temps entre chaque mise à jour.
			*/
			ElementDeBase_MFX(MFXDevice *systeme_maitre, type_element_mfx type_element_base, etat_des_elements_de_base_mfx etat_element_base = etat_des_elements_de_base_mfx::etat_inactif_mfx, float updateTime = 0.2f);
			ElementDeBase_MFX();
			~ElementDeBase_MFX();

			/* type_element_mfx getType()
			* Donne le type de l'élément.
			*/
			type_element_mfx getType();
			/* etat_des_elements_de_base_mfx getEtat()
			* Permet de récupérer l'état de l'élément.
			*/
			etat_des_elements_de_base_mfx getEtat() { return this->getEtatElementBase(); }

			/* bool ElementElectriqueDeBase::isAlive() :
			* Retourne true si l'état n'est pas etat_panne.
			*/
			virtual bool isAlive();

			/* bool isOnOff() :
			* Retourne true si l'élément est actif.
			*/
			virtual bool isOnOff();
			/* void setOnOff(bool status) :
			* Si l'état n'est pas etat_panne :
			*		- Affecte la valeur etat_actif si status true,
			*		- Affecte la valeur etat_stand_by si status false.
			*/
			virtual void setOnOff(bool status);
			/* void switchOnOff() :
			* Active ou désactive l'élément en fonction de son état actuelle (voir isOnOff() - voir setOnOff(bool status)).
			*/
			virtual void switchOnOff();

			/* void actionEffectue()
			* Lorsque l'action demandé par l'utilisateur est terminer, toutes les fonctions d'actions font appel à cette fonction.
			* Elle valide à l'élément que l'action s'est terminer.
			*/
			virtual void actionEffectue();

			/* void userAction(action_des_elements_mfx action_a_execute)
			* Utilisé lorsque l'action est demandé par l'utilisateur.
			* Mise à jour de la variable action_demande à la valeur vrai.
			* L'action se fera dans la fonction update();
			*/
			virtual void userAction(action_des_elements_mfx action_a_execute);
			/* void update();
			* Mets à jours l'élément.
			*/
			virtual void update();
			/* void demarrage()
			* Procédure de démarrage des éléments de base.
			*/
			virtual void demarrage();
			/* void arret()
			* Procédure d'arrêt des éléments de base.
			*/
			virtual void arret();
			/* void restauration(int ancienne_etat)
			* Procédure de restauration des éléments de base.
			* Remet l'élément à l'ancienne état (avant état de panne).
			*/
			virtual void restauration(int ancienne_etat);
			/* void recuperation()
			* Procédure de récupération des éléments de base.
			*/
			virtual void recuperation() = 0;
			/* void panne()
			* Procédure de mise en panne des éléments de base.
			*/
			virtual void panne();
			/* action_des_elements_mfx getAction()
			* Donne la dernière action faîtes (peut-être l'action en cours).
			*/
			action_des_elements_mfx getAction() { return this->action_a_execute; }
			/* bool actionEncours()
			* Dis si l'élément est en train de faire une action quelcquonque.
			*/
			bool actionEncours() { return this->action_demande; }

		protected:
			etat_des_elements_de_base_mfx etat_element_base;	//Etat de base de l'élément.
			type_element_mfx type_element_base;					//Type de l'élément (voir énumération type_element_base_mfx).
			action_des_elements_mfx action_a_execute;			//Action à exécuté.
			bool action_demande;								//Une action à été demandé.
			float updateTime;									//Temps entre chaque mise_a_jour. Utilisé comme timer pour les actions.

			/* void setType(type_element_mfx type_element_base)
			* Modifie le type de l'élément.
			*/
			void setType(type_element_mfx type_element_base);
			/* etat_des_elements_de_base_mfx setEtatElementBase();
			* Donne l'état de l'élément.
			*/
			etat_des_elements_de_base_mfx getEtatElementBase();
			/* void etat_element_base(etat_des_elements_de_base_mfx etat_element_base);
			* Modifie l'état de l'élement.
			*/
			void setEtatElementBase(etat_des_elements_de_base_mfx etat_element_base);
			/* void changementEtat()
			* Fonction qui prévient le système maître que l'état de l'élément à changer.
			*/
			void changementEtat();

		private:
			MFXDevice *systeme_maitre;							//Système maître à qui l'on renvoie une information lorsque l'élément change d'état.

		};
	}
}
