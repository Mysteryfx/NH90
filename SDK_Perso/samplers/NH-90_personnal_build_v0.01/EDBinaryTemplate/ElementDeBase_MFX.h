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
			* Pour cr�er un �l�ment de base, il faut :
			*				- type_element_mfx type_element_base, le type de l'�l�ment;
			*				- etat_des_elements_de_base_mfx etat_element_base, l'�tat de l'�l�ment de base (par d�faut : etat_inactif_mfx).
			*				- float updateTime, le temps entre chaque mise � jour.
			*/
			ElementDeBase_MFX(MFXDevice *systeme_maitre, type_element_mfx type_element_base, etat_des_elements_de_base_mfx etat_element_base = etat_des_elements_de_base_mfx::etat_inactif_mfx, float updateTime = 0.2f);
			ElementDeBase_MFX();
			~ElementDeBase_MFX();

			/* type_element_mfx getType()
			* Donne le type de l'�l�ment.
			*/
			type_element_mfx getType();
			/* etat_des_elements_de_base_mfx getEtat()
			* Permet de r�cup�rer l'�tat de l'�l�ment.
			*/
			etat_des_elements_de_base_mfx getEtat() { return this->getEtatElementBase(); }

			/* bool ElementElectriqueDeBase::isAlive() :
			* Retourne true si l'�tat n'est pas etat_panne.
			*/
			virtual bool isAlive();

			/* bool isOnOff() :
			* Retourne true si l'�l�ment est actif.
			*/
			virtual bool isOnOff();
			/* void setOnOff(bool status) :
			* Si l'�tat n'est pas etat_panne :
			*		- Affecte la valeur etat_actif si status true,
			*		- Affecte la valeur etat_stand_by si status false.
			*/
			virtual void setOnOff(bool status);
			/* void switchOnOff() :
			* Active ou d�sactive l'�l�ment en fonction de son �tat actuelle (voir isOnOff() - voir setOnOff(bool status)).
			*/
			virtual void switchOnOff();

			/* void actionEffectue()
			* Lorsque l'action demand� par l'utilisateur est terminer, toutes les fonctions d'actions font appel � cette fonction.
			* Elle valide � l'�l�ment que l'action s'est terminer.
			*/
			virtual void actionEffectue();

			/* void userAction(action_des_elements_mfx action_a_execute)
			* Utilis� lorsque l'action est demand� par l'utilisateur.
			* Mise � jour de la variable action_demande � la valeur vrai.
			* L'action se fera dans la fonction update();
			*/
			virtual void userAction(action_des_elements_mfx action_a_execute);
			/* void update();
			* Mets � jours l'�l�ment.
			*/
			virtual void update();
			/* void demarrage()
			* Proc�dure de d�marrage des �l�ments de base.
			*/
			virtual void demarrage();
			/* void arret()
			* Proc�dure d'arr�t des �l�ments de base.
			*/
			virtual void arret();
			/* void restauration(int ancienne_etat)
			* Proc�dure de restauration des �l�ments de base.
			* Remet l'�l�ment � l'ancienne �tat (avant �tat de panne).
			*/
			virtual void restauration(int ancienne_etat);
			/* void recuperation()
			* Proc�dure de r�cup�ration des �l�ments de base.
			*/
			virtual void recuperation() = 0;
			/* void panne()
			* Proc�dure de mise en panne des �l�ments de base.
			*/
			virtual void panne();
			/* action_des_elements_mfx getAction()
			* Donne la derni�re action fa�tes (peut-�tre l'action en cours).
			*/
			action_des_elements_mfx getAction() { return this->action_a_execute; }
			/* bool actionEncours()
			* Dis si l'�l�ment est en train de faire une action quelcquonque.
			*/
			bool actionEncours() { return this->action_demande; }

		protected:
			etat_des_elements_de_base_mfx etat_element_base;	//Etat de base de l'�l�ment.
			type_element_mfx type_element_base;					//Type de l'�l�ment (voir �num�ration type_element_base_mfx).
			action_des_elements_mfx action_a_execute;			//Action � ex�cut�.
			bool action_demande;								//Une action � �t� demand�.
			float updateTime;									//Temps entre chaque mise_a_jour. Utilis� comme timer pour les actions.

			/* void setType(type_element_mfx type_element_base)
			* Modifie le type de l'�l�ment.
			*/
			void setType(type_element_mfx type_element_base);
			/* etat_des_elements_de_base_mfx setEtatElementBase();
			* Donne l'�tat de l'�l�ment.
			*/
			etat_des_elements_de_base_mfx getEtatElementBase();
			/* void etat_element_base(etat_des_elements_de_base_mfx etat_element_base);
			* Modifie l'�tat de l'�lement.
			*/
			void setEtatElementBase(etat_des_elements_de_base_mfx etat_element_base);
			/* void changementEtat()
			* Fonction qui pr�vient le syst�me ma�tre que l'�tat de l'�l�ment � changer.
			*/
			void changementEtat();

		private:
			MFXDevice *systeme_maitre;							//Syst�me ma�tre � qui l'on renvoie une information lorsque l'�l�ment change d'�tat.

		};
	}
}
