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
			* Pour cr�er un r�servoir d'huile, il faut :
			*			- type_reservoir_huile_mfx type_reservoir, le type de r�servoir;
			*			- float reservoir, la capacit� du r�servoir;
			*			- bool remplie, si le r�servoir est remplie (par d�faut : vrai).
			*/
			ReservoirHuile_MFX(MFXDevice *systeme_maitre, type_reservoir_huile_mfx type_reservoir, float reservoir = 1000.0f, bool remplie = true);
			~ReservoirHuile_MFX();

			/* type_reservoir_huile_mfx getTypeReservoir()
			* Donne le type de r�servoir.
			* Ce type est utilis� pour d�finir la consommation d'huile.
			*/
			type_reservoir_huile_mfx getTypeReservoir();
			/* void fuite()
			* Simule une perte d'huile dans le r�servoir due � une panne du r�servoir ou du syst�me.
			*/
			void fuite() { ajusterQuantiteDe(-10.0f); }
			
			/* void update();
			* Mets � jours les informations de l'�quipement.
			*/
			virtual void update();
			/* void demarrage()
			* Proc�dure de d�marrage d'un �l�ment hydraulique.
			*/
			virtual void demarrage() { setEtatElementBase(element::etat_des_elements_de_base_mfx::etat_actif_mfx); this->setQuantite(getReservoir()); }
			/* void arret()
			* Proc�dure d'arr�t d'un �l�ment hydraulique.
			*/
			virtual void arret() { setEtatElementBase(element::etat_des_elements_de_base_mfx::etat_inactif_mfx); }
			/* void restauration(int ancienne_etat)
			* Proc�dure de restauration des �l�ments de base.
			* Remet l'�l�ment � l'ancienne �tat (avant �tat de panne).
			*/
			virtual void restauration(int ancienne_etat) { setEtatElementBase(static_cast<element::etat_des_elements_de_base_mfx>(ancienne_etat)); }
			/* void recuperation()
			* Proc�dure de r�cup�ration des �l�ments de base.
			*/
			virtual void recuperation() { this->setQuantite(getQuantite() + 100.0f); if (getQuantite() >= getReservoir()) { setQuantite(getReservoir()); actionEffectue(); } }
			/* void panne()
			* Proc�dure de mise en panne des �l�ments de base.
			*/
			virtual void panne() { this->setEtatElementBase(element::etat_des_elements_de_base_mfx::etat_panne_mfx); fuite(); }

		protected:
			/* virtual float aQuantiteDe(float quantite_ajuster) = 0;
			* Force la d�finition de l'ajustement de la quantit� de par la classe fille.
			* Un r�servoir d'huile moteur se vide diff�remment d'un r�servoir de fuel.
			*/
			virtual void aQuantiteDe(float quantite_ajuster);
			/* virtual float aQuantiteA(float quantite_voulue) = 0;
			* Force la d�finition de l'ajustement de la quantit� � par la classe fille.
			* Un r�servoir d'huile moteur se vide diff�remment d'un r�servoir de fuel.
			*/
			virtual void aQuantiteA(float quantite_voulue);

		private:
			type_reservoir_huile_mfx type_reservoir_huile;			//Utilis� pour d�finir la consommation d'huile par l'�l�ment li�.

		};
	}
}