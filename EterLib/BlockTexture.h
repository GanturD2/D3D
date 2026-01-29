#pragma once

#include "GrpBase.h"

#include "../UserInterface/Locale_inc.h"

class CGraphicDib;

class CBlockTexture : public CGraphicBase
{
public:
	CBlockTexture();
	virtual ~CBlockTexture();

	bool Create(CGraphicDib * pDIB, const RECT & c_rRect, uint32_t dwWidth, uint32_t dwHeight);
	void SetClipRect(const RECT & c_rRect);
	void Render(int ix, int iy);
	void InvalidateRect(const RECT & c_rsrcRect);

protected:
	CGraphicDib * m_pDIB;
	RECT m_rect;
	RECT m_clipRect;
	BOOL m_bClipEnable;
	uint32_t m_dwWidth;
	uint32_t m_dwHeight;
#ifdef ENABLE_DX9
	LPDIRECT3DTEXTURE9 m_lpd3dTexture;
#else
	LPDIRECT3DTEXTURE8 m_lpd3dTexture;
#endif
};
