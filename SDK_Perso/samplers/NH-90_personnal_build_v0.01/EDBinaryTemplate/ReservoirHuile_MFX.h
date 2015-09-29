#pragma once
#include "ElementFluide_MFX.h"

namespace cockpit {
	namespace fluide {
		typedef enum type_reservoir_huile_mfx{
			hydraulique_mfx,
			moteur_mfx,
		};
		class ReservoirHuile_MFX :
			public element::ElementFluide_MFX
		{
		public:
			/* ReservoirHuile_MFX(type_reservoir_huile_mfx type_reservoir, float reservoir, bool remplie)
			* Pour créer un réservoir d'huile, il faut :
			*			- type_reservoir_huile_mfx type_reservoir, le type de réservoir;
			*			- float reservoir, la capacité du réservoir;
			*			- bool remplie, si le réservoir est remplie (par défaut : vrai).
			*/
			ReservoirHuile_MFX(MFXDevice *systeme_maitre, type_reservoir_huile_mfx type_reservoir, float reservoir = 1000.0f, bool remplie = true);
			~ReservoirHuile_MFX();

			/* type_reservoir_huile_mfx getTypeReservoir()
			* Donne le type de réservoir.
			* Ce type est utilisé pour définir la consommation d'huile.
			*/
			type_reservoir_huile_mfx getTypeReservoir();
			/* void fuite()
			* Simule une perte d'huile dans le réservoir due à une panne du réservoir ou du système.
			*/
			void fuite() { ajusterQuantiteDe(-10.0f); }
			
			/* void update();
			* Mets à jours les informations de l'équipement.
			*/
			virtual void update();
			/* void demarrage()
			* Procédure de démarrage d'un élément hydraulique.
			*/
			virtual void demarrage() { setEtatElementBase(element::etat_des_elements_de_base_mfx::etat_actif_mfx); this->setQuantite(getReservoir()); }
			/* void arret()
			* Procédure d'arrêt d'un élément hydraulique.
			*/
			virtual void arret() { setEtatElementBase(element::etat_des_elements_de_base_mfx::etat_inactif_mfx); }
			/* void restauration(int ancienne_etat)
			* Procédure de restauration des éléments de base.
			* Remet l'élément à l'ancienne état (avant état de panne).
			*/
			virtual void restauration(int ancienne_etat) { setEtatElementBase(static_cast<element::etat_des_elements_de_base_mfx>(ancienne_etat)); }
			/* void recuperation()
			* Procédure de récupération des éléments de base.
			*/
			virtual void recuperation() { this->setQuantite(getQuantite() + 100.0f); if (getQuantite() >= getReservoir()) { setQuantite(getReservoir()); actionEffectue(); } }
			/* void panne()
			* Procédure de mise en panne des éléments de base.
			*/
			virtual void panne() { this->setEtatElementBase(element::etat_des_elements_de_base_mfx::etat_panne_mfx); fuite(); }

		protected:
			/* virtual float aQuantiteDe(float quantite_ajuster) = 0;
			* Force la définition de l'ajustement de la quantité de par la classe fille.
			* Un réservoir d'huile moteur se vide différemment d'un réservoir de fuel.
			*/
			virtual void aQuantiteDe(float quantite_ajuster);
			/* virtual float aQuantiteA(float quantite_voulue) = 0;
			* Force la définition de l'ajustement de la quantité à par la classe fille.
			* Un réservoir d'huile moteur se vide différemment d'un réservoir de fuel.
			*/
			virtual void aQuantiteA(float quantite_voulue);

		private:
			type_reservoir_huile_mfx type_reservoir_huile;			//Utilisé pour définir la consommation d'huile par l'élément lié.

		};
	}
}