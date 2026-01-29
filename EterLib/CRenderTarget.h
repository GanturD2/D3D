#pragma once
#include "../UserInterface/Locale_inc.h"

#ifdef ENABLE_RENDER_TARGET
#include <memory>
#include "GrpRenderTargetTexture.h"

class CInstanceBase;
class CGraphicImageInstance;

class CRenderTarget
{
	using TCharacterInstanceMap = std::map<DWORD, CInstanceBase*>;
	
	public:
		CRenderTarget(DWORD width, DWORD height);
		~CRenderTarget();

		void SetVisibility(bool isShow);
		void RenderTexture() const;
		void SetRenderingRect(RECT* rect) const;

		void SelectModel(DWORD index);
		void SelectModelNPC(DWORD index);
#if defined(ENABLE_ACCE_COSTUME_SYSTEM) || defined (ENABLE_AURA_SYSTEM)
		void SelectModelPC(const DWORD race, DWORD index, DWORD weapon, DWORD armor, DWORD hair, DWORD acce, DWORD aura);
#else
		void SelectModelPC(const DWORD race, DWORD index, DWORD weapon, DWORD armor, DWORD hair);
#endif
		bool ItemMatchesRace(DWORD race, DWORD vnum);
		DWORD RaceMatchesAntiflag(DWORD byRace, DWORD dwItemVnum);

		bool CreateBackground(const char* imgPath, DWORD width, DWORD height);
		void RenderBackground() const;
		void UpdateModel();
		void DeformModel() const;
		void RenderModel() const;
		void SetZoom(bool bZoom) noexcept;

		void SetMotion(DWORD motion);
		void SetRotation(bool rotation);
		void ChangeArmor(DWORD vnum);
		void ChangeWeapon(DWORD vnum);
		void ChangeHair(DWORD vnum);
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
		void ChangeAcce(DWORD vnum);
#endif
#ifdef ENABLE_AURA_SYSTEM
		void ChangeAura(DWORD vnum);
#endif

		void CreateTextures() const;
		void ReleaseTextures() const;
	
	private:
		std::unique_ptr<CInstanceBase> m_pModel; 
		std::unique_ptr<CGraphicImageInstance> m_background;
		std::unique_ptr<CGraphicRenderTargetTexture> m_renderTargetTexture;
		float m_modelRotation;
		float m_fEyeY;
		float m_fTargetY;
		float m_fTargetHeight;
		float m_fZoomY;
		bool m_visible;
};
#endif
