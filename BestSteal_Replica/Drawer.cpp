#define _USE_MATH_DEFINES
#include <math.h>
#include <d3dx9tex.h>

#include "Drawer.h"

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)


namespace BestStealReplica {
namespace {
struct CUSTOMVERTEX {
	float           x, y, z;                //位置情報
	float           rhw;                    //頂点変換値
	DWORD           color;                  //頂点カラー
	float           tu, tv;                 //テクスチャ座標
};
}

/* Constructor / Destructor ------------------------------------------------------------------------- */
Drawer::Drawer(HWND hWnd, IDirect3D9* pDirect3D, D3DPRESENT_PARAMETERS* pD3dpp) {
	pDirect3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		pD3dpp,
		&this->pD3Device);
}


/* Static Public Functions -------------------------------------------------------------------------- */
UINT16 Drawer::GetAlphaOnBlinking(int time) {
	double rad = time * 18 * M_PI / 180;
	double alpha = 0xFF * abs(sin(rad));
	return (UINT16)alpha;
}


/* Public Functions  -------------------------------------------------------------------------------- */
bool Drawer::CreateTexture(const TCHAR* filePath, TextureType textureType) {
	if (FAILED(D3DXCreateTextureFromFileEx(
		this->pD3Device,
		filePath,
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,
		D3DX_FILTER_NONE,
		D3DCOLOR_ARGB(255, 0, 255, 0),
		NULL,
		NULL,
		&this->pTexture[textureType]))) {
		return false;
	}
	return true;
}

void Drawer::BeginDraw() const {
	if (!this->pD3Device) return;

	//描画方法の設定
	this->pD3Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	this->pD3Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);  //SRCの設定
	this->pD3Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	this->pD3Device->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);

	this->pD3Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	this->pD3Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	this->pD3Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	this->pD3Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	this->pD3Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	this->pD3Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	//画面の消去
	this->pD3Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x00, 0x00, 0x00), 1.0, 0);
	//描画の開始
	this->pD3Device->BeginScene();

	// 頂点フォーマットの設定
	this->pD3Device->SetFVF(D3DFVF_CUSTOMVERTEX);
}

void Drawer::Draw(Vertices<DrawingVertex> vertices, TextureType textureType) const {
	Draw(vertices, this->pTexture[textureType], 0xFF);
}

void Drawer::Draw(Vertices<DrawingVertex> vertices, TextureType textureType, UINT16 alpha) const {
	Draw(vertices, this->pTexture[textureType], alpha);
}

void Drawer::Blackout() const {
	this->pD3Device->BeginScene();
	this->pD3Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x00, 0x00, 0x00), 1.0, 0);
	EndDraw();
}

void Drawer::EndDraw() const {
	//描画の終了
	this->pD3Device->EndScene();

	//表示
	this->pD3Device->Present(NULL, NULL, NULL, NULL);
}

void Drawer::Release() const {
	this->pD3Device->Release();
	this->pTexture[0]->Release();
	this->pTexture[1]->Release();
}


/* Private Functions  ------------------------------------------------------------------------------- */
void Drawer::Draw(Vertices<DrawingVertex> vertices, LPDIRECT3DTEXTURE9 pTexture, UINT16 alpha) const {
	if ((vertices.topLeft.x < 0 && vertices.topLeft.y < 0 && vertices.bottomRight.x < 0 && vertices.bottomRight.y < 0)
			|| (vertices.topLeft.x > AppCommon::GetWindowWidth() && vertices.topLeft.y > AppCommon::GetWindowHeight() 
				&& vertices.bottomRight.x > AppCommon::GetWindowWidth() && vertices.bottomRight.y > AppCommon::GetWindowHeight())) {
		// 描画範囲外
		return;
	}
	
	CUSTOMVERTEX customVertex[4];
	for (int i = 0; i < 4; ++i) {
		customVertex[i].z = 0.5f;
		customVertex[i].rhw = 1.0f;
		customVertex[i].color = D3DCOLOR_ARGB(alpha, 255, 255, 255);
	}

	customVertex[0].x = (float)vertices.topLeft.x;
	customVertex[0].y = (float)vertices.topLeft.y;
	customVertex[0].tu = vertices.topLeft.tu;
	customVertex[0].tv = vertices.topLeft.tv;

	customVertex[1].x = (float)vertices.bottomRight.x;
	customVertex[1].y = (float)vertices.topLeft.y;
	customVertex[1].tu = vertices.bottomRight.tu;
	customVertex[1].tv = vertices.topLeft.tv;

	customVertex[2].x = (float)vertices.bottomRight.x;
	customVertex[2].y = (float)vertices.bottomRight.y;
	customVertex[2].tu = vertices.bottomRight.tu;
	customVertex[2].tv = vertices.bottomRight.tv;

	customVertex[3].x = (float)vertices.topLeft.x;
	customVertex[3].y = (float)vertices.bottomRight.y;
	customVertex[3].tu = vertices.topLeft.tu;
	customVertex[3].tv = vertices.bottomRight.tv;

	this->pD3Device->SetTexture(0, pTexture);
	this->pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, customVertex, sizeof(CUSTOMVERTEX));
}

}