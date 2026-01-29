#pragma once

#include "GrpBase.h"

#include "../UserInterface/Locale_inc.h"

class CGraphicTexture : public CGraphicBase
{
public:
	virtual bool IsEmpty() const;

	int GetWidth() const;
	int GetHeight() const;

	void SetTextureStage(int stage) const;
#ifdef ENABLE_DX9
	LPDIRECT3DTEXTURE9 GetD3DTexture() const;
#else
	LPDIRECT3DTEXTURE8 GetD3DTexture() const;
#endif

	void DestroyDeviceObjects();

protected:
	CGraphicTexture();
	virtual	~CGraphicTexture();

	void Destroy();
	void Initialize();

protected:
	bool m_bEmpty;

	int m_width;
	int m_height;

#ifdef ENABLE_DX9
	LPDIRECT3DTEXTURE9 m_lpd3dTexture;
#else
	LPDIRECT3DTEXTURE8 m_lpd3dTexture;
#endif
};
