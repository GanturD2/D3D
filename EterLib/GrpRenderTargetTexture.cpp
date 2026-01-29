#include "StdAfx.h"
#include "../UserInterface/Locale_inc.h"

#ifdef ENABLE_RENDER_TARGET
#include "../EterBase/Stl.h"
#include "../EterBase/Utils.h"

#include "StateManager.h"
#include "GrpRenderTargetTexture.h"

CGraphicRenderTargetTexture::~CGraphicRenderTargetTexture() { Reset(); }
CGraphicRenderTargetTexture::CGraphicRenderTargetTexture() : m_d3dFormat{ D3DFMT_UNKNOWN }, m_depthStencilFormat{ D3DFMT_UNKNOWN }
{
	Initialize();

	//memset(&m_renderRect, 0, sizeof(m_renderRect));
	//memset(&m_renderBox, 0, sizeof(m_renderBox));

	m_renderRect.left = 0;
	m_renderRect.top = 0;
	m_renderRect.right = 0;
	m_renderRect.bottom = 0;
}

/*----------------------------
--------PUBLIC CLASS FUNCTIONS
-----------------------------*/

bool CGraphicRenderTargetTexture::Create(const int width, const int height, const D3DFORMAT texFormat, const D3DFORMAT depthFormat)
{
	Reset();

	m_height = height;
	m_width = width;

	if (!CreateRenderTexture(width, height, texFormat))
		return false;

	if (!CreateRenderDepthStencil(width, height, depthFormat))
		return false;

	return true;
}

void CGraphicRenderTargetTexture::CreateTextures()
{
	if (CreateRenderTexture(m_width, m_height, m_d3dFormat))
		CreateRenderDepthStencil(m_width, m_height, m_depthStencilFormat);
}

bool CGraphicRenderTargetTexture::CreateRenderTexture(const int width, const int height, const D3DFORMAT format)
{
	m_d3dFormat = format;

#ifdef ENABLE_DX9
	if (FAILED(ms_lpd3dDevice->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, m_d3dFormat, D3DPOOL_DEFAULT, &m_lpd3dRenderTexture, NULL)))
#else
	if (FAILED(ms_lpd3dDevice->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, m_d3dFormat, D3DPOOL_DEFAULT, &m_lpd3dRenderTexture)))
#endif
	{
		TraceError("RenderTarget texture failed");
		return false;
	}

	if (FAILED(m_lpd3dRenderTexture->GetSurfaceLevel(0, &m_lpd3dRenderTargetSurface)))
		return false;

	return true;
}

void CGraphicRenderTargetTexture::ReleaseTextures()
{
	SAFE_RELEASE(m_lpd3dRenderTexture);
	SAFE_RELEASE(m_lpd3dRenderTargetSurface);
	SAFE_RELEASE(m_lpd3dDepthSurface);
	SAFE_RELEASE(m_lpd3dDepthSurface);
	SAFE_RELEASE(m_lpd3dOriginalRenderTarget);
	SAFE_RELEASE(m_lpd3dOldDepthBufferSurface);

	//memset(&m_renderRect, 0, sizeof(m_renderRect));
	//memset(&m_renderBox, 0, sizeof(m_renderBox));

	m_renderRect.left = 0;
	m_renderRect.top = 0;
	m_renderRect.right = 0;
	m_renderRect.bottom = 0;
}

#ifdef ENABLE_DX9
LPDIRECT3DTEXTURE9 CGraphicRenderTargetTexture::GetD3DRenderTargetTexture() const
#else
LPDIRECT3DTEXTURE8 CGraphicRenderTargetTexture::GetRenderTargetTexture() const
#endif
{
	return m_lpd3dRenderTexture;
}

bool CGraphicRenderTargetTexture::CreateRenderDepthStencil(const int width, const int height, const D3DFORMAT format)
{
	m_depthStencilFormat = format;

#ifdef ENABLE_DX9
	return (ms_lpd3dDevice->CreateDepthStencilSurface(width, height, m_depthStencilFormat, D3DMULTISAMPLE_NONE, 0, FALSE, &m_lpd3dDepthSurface, NULL)) == D3D_OK;
#else
	return (ms_lpd3dDevice->CreateDepthStencilSurface(width, height, m_depthStencilFormat, D3DMULTISAMPLE_NONE, &m_lpd3dDepthSurface)) == D3D_OK;
#endif
}

void CGraphicRenderTargetTexture::SetRenderTarget()
{
	bool bSuccess = true;

#ifdef ENABLE_DX9
	ms_lpd3dDevice->GetRenderTarget(0, &m_lpd3dOriginalRenderTarget);
	ms_lpd3dDevice->GetDepthStencilSurface(&m_lpd3dOldDepthBufferSurface);
	//ms_lpd3dDevice->SetRenderTarget(0, m_lpd3dRenderTargetSurface);

	if (FAILED(ms_lpd3dDevice->SetRenderTarget(0, m_lpd3dRenderTargetSurface)))
	{
#ifdef _DEBUG
		TraceError("GrpRenderTargetTexture.cpp::SetRenderTarget : Unable to Set Render Target");
#endif _DEBUG

		Create(m_width, m_height, m_d3dFormat, m_depthStencilFormat);

#ifdef _DEBUG
		TraceError("GrpRenderTargetTexture.cpp::SetRenderTarget : Reset Render Target\n");
#endif _DEBUG

		bSuccess = false;
	}

	ms_lpd3dDevice->SetDepthStencilSurface(m_lpd3dDepthSurface);
#else
	ms_lpd3dDevice->GetRenderTarget(&m_lpd3dOriginalRenderTarget);
	ms_lpd3dDevice->GetDepthStencilSurface(&m_lpd3dOldDepthBufferSurface);
	ms_lpd3dDevice->SetRenderTarget(m_lpd3dRenderTargetSurface, m_lpd3dDepthSurface);
#endif
}

void CGraphicRenderTargetTexture::ResetRenderTarget()
{
#ifdef ENABLE_DX9
	ms_lpd3dDevice->SetRenderTarget(0, m_lpd3dOriginalRenderTarget);
	ms_lpd3dDevice->SetDepthStencilSurface(m_lpd3dOldDepthBufferSurface);
#else
	ms_lpd3dDevice->SetRenderTarget(m_lpd3dOriginalRenderTarget, m_lpd3dOldDepthBufferSurface);
#endif

	SAFE_RELEASE(m_lpd3dOriginalRenderTarget);
	SAFE_RELEASE(m_lpd3dOldDepthBufferSurface);
}

void CGraphicRenderTargetTexture::SetRenderingRect(RECT* rect)
{
	m_renderRect = *rect;
}

RECT* CGraphicRenderTargetTexture::GetRenderingRect()
{
	return &m_renderRect;
}

//void CGraphicRenderTargetTexture::SetRenderingBox(RECT* renderBox)
//{
//	m_renderBox = *renderBox;
//}
//
//RECT* CGraphicRenderTargetTexture::GetRenderingBox()
//{
//	return &m_renderBox;
//}

void CGraphicRenderTargetTexture::Clear()
{
	ms_lpd3dDevice->Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0xff, 0xff, 0xff), 1.0f, 0);	// Hellen Schatten

#ifdef _DEBUG
	bool bSuccess = true;

	if (FAILED(ms_lpd3dDevice->Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0xff, 0xff, 0xff), 1.0f, 0)))	// Hellen Schatten
	{
		TraceError("GrpRenderTargetTexture.cpp::SetRenderTarget : Unable to Clear Render Target\n");

		bSuccess = false;
	}
#endif _DEBUG
}

void CGraphicRenderTargetTexture::Render() const
{
	//const float sx = static_cast<float>(m_renderRect.left) - 0.5f + static_cast<float>(m_renderBox.left);
	//const float sy = static_cast<float>(m_renderRect.top) - 0.5f + static_cast<float>(m_renderBox.top);
	//const float ex = static_cast<float>(m_renderRect.left) + (static_cast<float>(m_renderRect.right) - static_cast<float>(m_renderRect.left)) - 0.5f - static_cast<float>(m_renderBox.right);
	//const float ey = static_cast<float>(m_renderRect.top) + (static_cast<float>(m_renderRect.bottom) - static_cast<float>(m_renderRect.top)) - 0.5f - static_cast<float>(m_renderBox.bottom);

	//const float texReverseWidth = 1.0f / (static_cast<float>(m_renderRect.right) - static_cast<float>(m_renderRect.left));
	//const float texReverseHeight = 1.0f / (static_cast<float>(m_renderRect.bottom) - static_cast<float>(m_renderRect.top));

	//const float su = m_renderBox.left * texReverseWidth;
	//const float sv = m_renderBox.top * texReverseHeight;
	//const float eu = ((m_renderRect.right - m_renderRect.left) - m_renderBox.right) * texReverseWidth;
	//const float ev = ((m_renderRect.bottom - m_renderRect.top) - m_renderBox.bottom) * texReverseHeight;

	//TPDTVertex pVertices[4];
	//pVertices[0].position = TPosition(sx, sy, 0.0f);
	//pVertices[0].texCoord = TTextureCoordinate(su, sv);
	//pVertices[0].diffuse = 0xffffffff;

	//pVertices[1].position = TPosition(ex, sy, 0.0f);
	//pVertices[1].texCoord = TTextureCoordinate(eu, sv);
	//pVertices[1].diffuse = 0xffffffff;

	//pVertices[2].position = TPosition(sx, ey, 0.0f);
	//pVertices[2].texCoord = TTextureCoordinate(su, ev);
	//pVertices[2].diffuse = 0xffffffff;

	//pVertices[3].position = TPosition(ex, ey, 0.0f);
	//pVertices[3].texCoord = TTextureCoordinate(eu, ev);
	//pVertices[3].diffuse = 0xffffffff;

	D3DSURFACE_DESC desc;
	m_lpd3dRenderTargetSurface->GetDesc(&desc);

	const auto imgWidth = static_cast<float>(m_renderRect.right - m_renderRect.left);
	const auto imgHeight = static_cast<float>(m_renderRect.bottom - m_renderRect.top);

	constexpr auto su = 0.0f;
	constexpr auto sv = 0.0f;
	const auto eu = imgWidth / static_cast<float>(desc.Width);
	const auto ev = imgHeight / static_cast<float>(desc.Height);


	TPDTVertex pVertices[4];

	pVertices[0].position = TPosition(m_renderRect.left - 0.5f, m_renderRect.top - 0.5f, 0.0f);
	pVertices[0].texCoord = TTextureCoordinate(su, sv);
	pVertices[0].diffuse = 0xffffffff;


	pVertices[1].position = TPosition(m_renderRect.right - 0.5f, m_renderRect.top - 0.5f, 0.0f);
	pVertices[1].texCoord = TTextureCoordinate(eu, sv);
	pVertices[1].diffuse = 0xffffffff;

	pVertices[2].position = TPosition(m_renderRect.left - 0.5f, m_renderRect.bottom - 0.5f, 0.0f);
	pVertices[2].texCoord = TTextureCoordinate(su, ev);
	pVertices[2].diffuse = 0xffffffff;

	pVertices[3].position = TPosition(m_renderRect.right - 0.5f, m_renderRect.bottom - 0.5f, 0.0f);
	pVertices[3].texCoord = TTextureCoordinate(eu, ev);
	pVertices[3].diffuse = 0xffffffff;

	if (SetPDTStream(pVertices, 4))
	{
		CGraphicBase::SetDefaultIndexBuffer(CGraphicBase::DEFAULT_IB_FILL_RECT);

		STATEMANAGER.SetTexture(0, GetD3DRenderTargetTexture());
		STATEMANAGER.SetTexture(1, NULL);
#ifdef ENABLE_DX9
		STATEMANAGER.SetFVF(D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_DIFFUSE);
#else
		STATEMANAGER.SetVertexShader(D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_DIFFUSE);
#endif
		STATEMANAGER.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 4, 0, 2);
	}
	//STATEMANAGER.RestoreRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHABLENDENABLE);
	//STATEMANAGER.RestoreRenderState(D3DRENDERSTATETYPE::D3DRS_SRCBLEND);
}

/*----------------------------
------PROTECTED CLASS FUNCTIONS
-----------------------------*/

void CGraphicRenderTargetTexture::Reset()
{
	Destroy();
	//ReleaseTextures();

	m_d3dFormat = D3DFMT_UNKNOWN;
	m_depthStencilFormat = D3DFMT_UNKNOWN;
}
#endif
// FIXED!