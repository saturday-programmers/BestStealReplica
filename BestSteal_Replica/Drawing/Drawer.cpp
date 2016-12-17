#define _USE_MATH_DEFINES
#include <math.h>
#include <d3dx9tex.h>

#include "Drawer.h"
#include "../Drawing/IDRawable.h"


#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

namespace BestStealReplica {
namespace Drawing {

namespace {
/* Structs ------------------------------------------------------------------------------------------ */
struct CUSTOMVERTEX {
	float           x, y, z;                //位置情報
	float           rhw;                    //頂点変換値
	DWORD           color;                  //頂点カラー
	float           tu, tv;                 //テクスチャ座標
};

/* Static Variables --------------------------------------------------------------------------------- */
IDirect3DDevice9* pD3Device = nullptr;
std::vector<const IDrawable*> pDrawables;
std::map<Drawing::TextureType, LPDIRECT3DTEXTURE9> textures;
}


/* Constants ---------------------------------------------------------------------------------------- */
const TCHAR* Drawer::MAP_CHIP_FILE_PATH = TEXT("image\\mapchip.png");
const TCHAR* Drawer::CHARACTER_FILE_PATH = TEXT("image\\character.png");


/* Static Public Functions -------------------------------------------------------------------------- */
void Drawer::Create(HWND hWnd, IDirect3D9* pDirect3D, D3DPRESENT_PARAMETERS* pD3dpp) {
	pDirect3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		pD3dpp,
		&pD3Device);
}

void Drawer::Release() {
	pD3Device->Release();
	for (auto& texture : textures) {
		texture.second->Release();
	}
}

UINT16 Drawer::GetAlphaOnBlinking(int time) {
	double rad = time * 18 * M_PI / 180;
	double alpha = 0xFF * abs(sin(rad));
	return (UINT16)alpha;
}

bool Drawer::AddDrawable(const IDrawable& rDrawable) {
	bool ret = true;
	pDrawables.push_back(&rDrawable);
	std::vector<Drawing::TextureType> requiredTextureTypes = rDrawable.GetTextureTypes();
	for (auto& tex : requiredTextureTypes) {
		// ロードされていないテクスチャのみ処理
		if (textures.count(tex) == 0) {
			ret = CreateTexture(tex);
			if (!ret) {
				break;
			}
		}
	}
	return ret;
}

void Drawer::Draw() {
	// 描画データ生成
	DataTable<DrawingContext> contextTable;
	contextTable.resize(pDrawables.size());
	for (int i = 0; i < (int)pDrawables.size(); ++i) {
		pDrawables[i]->CreateDrawingContexts(&contextTable[i]);
	}

	// 描画
	BeginDraw();
	for (auto& contexts : contextTable) {
		for (auto& context : contexts) {
			Draw(context);
		}
	}
	EndDraw();
}

void Drawer::Blackout() {
	pD3Device->BeginScene();
	pD3Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x00, 0x00, 0x00), 1.0, 0);
	EndDraw();
}


/* Private Functions  ------------------------------------------------------------------------------- */
bool Drawer::CreateTexture(Drawing::TextureType textureType) {
	const TCHAR* filePath = nullptr;
	switch (textureType) {
		case Drawing::TextureType::MAP:
			filePath = MAP_CHIP_FILE_PATH;
			break;
		case Drawing::TextureType::CHARACTER:
			filePath = CHARACTER_FILE_PATH;
			break;
	}

	if (FAILED(D3DXCreateTextureFromFileEx(
		pD3Device,
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
		&textures[textureType]))) {
		return false;
	}
	return true;
}

void Drawer::BeginDraw() {
	if (!pD3Device) return;

	// 描画方法の設定
	pD3Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	pD3Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);  //SRCの設定
	pD3Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pD3Device->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);

	pD3Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pD3Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	pD3Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	pD3Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pD3Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	pD3Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	// 画面の消去
	pD3Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x00, 0x00, 0x00), 1.0, 0);
	// 描画の開始
	pD3Device->BeginScene();

	// 頂点フォーマットの設定
	pD3Device->SetFVF(D3DFVF_CUSTOMVERTEX);
}

void Drawer::Draw(const DrawingContext& rContext) {
	if ((rContext.vertices.topLeft.x < 0 && rContext.vertices.topLeft.y < 0 && rContext.vertices.bottomRight.x < 0 && rContext.vertices.bottomRight.y < 0)
		|| (rContext.vertices.topLeft.x > AppCommon::GetWindowWidth() && rContext.vertices.topLeft.y > AppCommon::GetWindowHeight()
		&& rContext.vertices.bottomRight.x > AppCommon::GetWindowWidth() && rContext.vertices.bottomRight.y > AppCommon::GetWindowHeight())) {
		// 描画範囲外
		return;
	}

	CUSTOMVERTEX customVertex[4];
	for (auto& vertex : customVertex) {
		vertex.z = 0.5f;
		vertex.rhw = 1.0f;
		vertex.color = D3DCOLOR_ARGB(rContext.alpha, 255, 255, 255);
	}

	customVertex[0].x = (float)rContext.vertices.topLeft.x;
	customVertex[0].y = (float)rContext.vertices.topLeft.y;
	customVertex[0].tu = rContext.vertices.topLeft.tu;
	customVertex[0].tv = rContext.vertices.topLeft.tv;

	customVertex[1].x = (float)rContext.vertices.bottomRight.x;
	customVertex[1].y = (float)rContext.vertices.topLeft.y;
	customVertex[1].tu = rContext.vertices.bottomRight.tu;
	customVertex[1].tv = rContext.vertices.topLeft.tv;

	customVertex[2].x = (float)rContext.vertices.bottomRight.x;
	customVertex[2].y = (float)rContext.vertices.bottomRight.y;
	customVertex[2].tu = rContext.vertices.bottomRight.tu;
	customVertex[2].tv = rContext.vertices.bottomRight.tv;

	customVertex[3].x = (float)rContext.vertices.topLeft.x;
	customVertex[3].y = (float)rContext.vertices.bottomRight.y;
	customVertex[3].tu = rContext.vertices.topLeft.tu;
	customVertex[3].tv = rContext.vertices.bottomRight.tv;

	pD3Device->SetTexture(0, textures.at(rContext.textureType));
	pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, customVertex, sizeof(CUSTOMVERTEX));
}

void Drawer::EndDraw() {
	// 描画の終了
	pD3Device->EndScene();

	// 表示
	pD3Device->Present(NULL, NULL, NULL, NULL);
}

}
}
