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
			* Pour créer une pompe, il faut :
			*			- float temperature, la température de la pompe;
			*			- float updateTime, temps entre chaque mmise à jour;
			*			- type_element_mfx type_element_pompe, le type de pompe (par défaut : pompe_mfx);
			*			- etat_des_elements_de_base_mfx etat_element_pompe, l'état de la pompe;
			*			- float pression, la pression de la pompe (par défaut : 1.0f).
			*/
			Pompe_MFX(MFXDevice *systeme_maitre, float temperature, float updateTime, element::etat_des_elements_de_base_mfx etat_element_pompe = element::etat_des_elements_de_base_mfx::etat_inactif_mfx, float pression = 1.0f, element::type_element_mfx type_element_pompe = element::type_element_mfx::pompe_mfx);
			~Pompe_MFX();

			/* void lierMoteur(moteur::Moteur * moteur)
			* Lier un moteur à la pompe.
			*/
			void lierMoteur(moteur::Moteur_MFX * moteur);
			/* void lierReservoir(fluide::ReservoirHuile_MFX * reservoir)
			* Lier un réservoir à la pompe.
			*/
			void lierReservoir(fluide::ReservoirHuile_MFX * reservoir);

			/* void actionEffectue()
			* Lorsque l'action demandé par l'utilisateur est terminer, toutes les fonctions d'actions font appel à cette fonction.
			* Elle valide à l'élément que l'action s'est terminer.
			*/
			virtual void actionEffectue();
			/* void brancheApres(Tuyau_MFX * tuyau)
			* Branché un tuyau après la pompe.
			*/
			void brancheApres(Tuyau_MFX * tuyau);
			
			/* void update()
			* Mise à jour de la pompe.
			* Pour qu'une pompe fonctionne correctement, il lui faut un moteur duquel elle tire sa puissance.
			* Nous allons donc ajouter des contrôles supplémentaires avant de procéder aux différentes actions.
			*/
			virtual void update() override;
			/* void demarrage()
			* Procédure de démarrage d'un élément hydraulique.
			*/
			virtual void demarrage();
			/* void arret()
			* Procédure d'arrêt d'un élément hydraulique.
			*/
			virtual void arret() override;
			/* void restauration(int ancienne_etat)
			* Procédure de restauration des éléments de base.
			* Remet l'élément à l'ancienne état (avant état de panne).
			*/
			virtual void restauration(int ancienne_etat);
			/* void recuperation()
			* Procédure de récupération des éléments de base.
			*/
			virtual void recuperation() {}
			/* void panne()
			* Procédure de mise en panne des éléments de base.
			*/
			virtual void panne();
			/* void fuite()
			* Demande à l'élément antérieur de faire la simulation d'une fuite.
			* Cette demande doit-être effectué par un élément précédent celui-ci qui serais en panne.
			*/
			virtual void fuite();
			/* void changementPression(float nouvelle_pression)
			* Comme les éléments hydraulique sont connecté les uns aux autres, lorsqu'il y a une modification de pression dans l'un d'entre eux, les autres sont aussi impacter.
			* On donne la nouvelle pression en même temps.
			*/
			virtual void changementPression(float nouvelle_pression);

		protected:
			Tuyau_MFX * tuyau;							//Tuyau branché après la pompe.
			fluide::ReservoirHuile_MFX * reservoir;		//Réservoir d'huile associé à la pompe.
			moteur::Moteur_MFX * moteur;				//Moteur, fournis de la puissance à la pompe. Si aucun moteur n'est actif, aucune pompe ne peut démarrer.

			/* void demarragePompe()
			* Démarrage de la pompe.
			* Lors du démmarrage de la pompe, la pression augmente quasi instantanément de 0 à 80 bar puis progressivement jusqu'a atteindre la pression nominal.
			*
			* Attention : Si la pression courante de la pompe est déjà à la pression nominal, il ne faut pas l'augmenté.
			* Elle ne doit pas être supérieur à la pression nominal lors d'un fonctionnement normal.
			*/
			virtual void demarragePompe();

		};
	}
}