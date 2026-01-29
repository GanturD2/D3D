#pragma once

#include "GrpBase.h"

#include "../UserInterface/Locale_inc.h"

class CPixelShader : public CGraphicBase
{
public:
	CPixelShader();
	virtual ~CPixelShader();

	void Destroy();
	bool CreateFromDiskFile(const char * c_szFileName);

	void Set();

protected:
	void Initialize();

protected:
#ifdef ENABLE_DX9
	LPDIRECT3DPIXELSHADER9 m_handle;
#else
	DWORD m_handle;
#endif
};
