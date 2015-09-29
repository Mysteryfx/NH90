#pragma once
#include "ElementHydraulique_MFX.h"
#include "ReservoirHuile_MFX.h"
#include "Moteur_MFX.h"
#include "Tuyau_MFX.h"

namespace cockpit {
	namespace element{ class ElementDeBase_MFX; }
	namespace hydraulique {
		class Pompe_MFX :
			public element::ElementHydraulique_MFX
		{
		public:
			/* Pompe_MFX(float temperature, etat_des_elements_de_base_mfx etat_element_pompe, float pression, type_element_mfx type_element_pompe)
			* Pour cr�er une pompe, il faut :
			*			- float temperature, la temp�rature de la pompe;
			*			- float updateTime, temps entre chaque mmise � jour;
			*			- type_element_mfx type_element_pompe, le type de pompe (par d�faut : pompe_mfx);
			*			- etat_des_elements_de_base_mfx etat_element_pompe, l'�tat de la pompe;
			*			- float pression, la pression de la pompe (par d�faut : 1.0f).
			*/
			Pompe_MFX(MFXDevice *systeme_maitre, float temperature, float updateTime, element::etat_des_elements_de_base_mfx etat_element_pompe = element::etat_des_elements_de_base_mfx::etat_inactif_mfx, float pression = 1.0f, element::type_element_mfx type_element_pompe = element::type_element_mfx::pompe_mfx);
			~Pompe_MFX();

			/* void lierMoteur(moteur::Moteur * moteur)
			* Lier un moteur � la pompe.
			*/
			void lierMoteur(moteur::Moteur_MFX * moteur);
			/* void lierReservoir(fluide::ReservoirHuile_MFX * reservoir)
			* Lier un r�servoir � la pompe.
			*/
			void lierReservoir(fluide::ReservoirHuile_MFX * reservoir);

			/* void actionEffectue()
			* Lorsque l'action demand� par l'utilisateur est terminer, toutes les fonctions d'actions font appel � cette fonction.
			* Elle valide � l'�l�ment que l'action s'est terminer.
			*/
			virtual void actionEffectue();
			/* void brancheApres(Tuyau_MFX * tuyau)
			* Branch� un tuyau apr�s la pompe.
			*/
			void brancheApres(Tuyau_MFX * tuyau);
			
			/* void update()
			* Mise � jour de la pompe.
			* Pour qu'une pompe fonctionne correctement, il lui faut un moteur duquel elle tire sa puissance.
			* Nous allons donc ajouter des contr�les suppl�mentaires avant de proc�der aux diff�rentes actions.
			*/
			virtual void update() override;
			/* void demarrage()
			* Proc�dure de d�marrage d'un �l�ment hydraulique.
			*/
			virtual void demarrage();
			/* void arret()
			* Proc�dure d'arr�t d'un �l�ment hydraulique.
			*/
			virtual void arret() override;
			/* void restauration(int ancienne_etat)
			* Proc�dure de restauration des �l�ments de base.
			* Remet l'�l�ment � l'ancienne �tat (avant �tat de panne).
			*/
			virtual void restauration(int ancienne_etat);
			/* void recuperation()
			* Proc�dure de r�cup�ration des �l�ments de base.
			*/
			virtual void recuperation() {}
			/* void panne()
			* Proc�dure de mise en panne des �l�ments de base.
			*/
			virtual void panne();
			/* void fuite()
			* Demande � l'�l�ment ant�rieur de faire la simulation d'une fuite.
			* Cette demande doit-�tre effectu� par un �l�ment pr�c�dent celui-ci qui serais en panne.
			*/
			virtual void fuite();
			/* void changementPression(float nouvelle_pression)
			* Comme les �l�ments hydraulique sont connect� les uns aux autres, lorsqu'il y a une modification de pression dans l'un d'entre eux, les autres sont aussi impacter.
			* On donne la nouvelle pression en m�me temps.
			*/
			virtual void changementPression(float nouvelle_pression);

		protected:
			Tuyau_MFX * tuyau;							//Tuyau branch� apr�s la pompe.
			fluide::ReservoirHuile_MFX * reservoir;		//R�servoir d'huile associ� � la pompe.
			moteur::Moteur_MFX * moteur;				//Moteur, fournis de la puissance � la pompe. Si aucun moteur n'est actif, aucune pompe ne peut d�marrer.

			/* void demarragePompe()
			* D�marrage de la pompe.
			* Lors du d�mmarrage de la pompe, la pression augmente quasi instantan�ment de 0 � 80 bar puis progressivement jusqu'a atteindre la pression nominal.
			*
			* Attention : Si la pression courante de la pompe est d�j� � la pression nominal, il ne faut pas l'augment�.
			* Elle ne doit pas �tre sup�rieur � la pression nominal lors d'un fonctionnement normal.
			*/
			virtual void demarragePompe();

		};
	}
}