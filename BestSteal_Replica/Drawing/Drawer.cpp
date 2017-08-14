#define _USE_MATH_DEFINES
#include <math.h>
#include <d3dx9tex.h>

#include "Drawer.h"
#include "Drawing/IDRawable.h"


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

	CUSTOMVERTEX() = default;
	CUSTOMVERTEX(const CUSTOMVERTEX&) = delete;
};


/* Constants ---------------------------------------------------------------------------------------- */
const TCHAR* MAP_CHIP_FILE_PATH = TEXT("image\\mapchip.png");
const TCHAR* CHARACTER_FILE_PATH = TEXT("image\\character.png");


/* Static Variables --------------------------------------------------------------------------------- */
IDirect3DDevice9* pD3Device = nullptr;
std::vector<const IDrawable*> pDrawables;
std::map<Drawing::TextureType, LPDIRECT3DTEXTURE9> textures;
}


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

	// テクスチャ設定
	std::vector<Drawing::TextureType> requiredTextureTypes;
	rDrawable.ConfigureTextureTypes(&requiredTextureTypes);
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
	for (UINT16 i = 0; i < pDrawables.size(); ++i) {
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
	if (rContext.rect.bottomRight.x < 0 || rContext.rect.bottomRight.y < 0
		|| rContext.rect.topLeft.x > AppCommon::GetWindowWidth() || rContext.rect.topLeft.y > AppCommon::GetWindowHeight()) {
		// 描画範囲外
		return;
	}

	CUSTOMVERTEX customVertexArr[4];
	for (auto& vertex : customVertexArr) {
		vertex.z = 0.5f;
		vertex.rhw = 1.0f;
		vertex.color = D3DCOLOR_ARGB(rContext.alpha, rContext.r, rContext.g, rContext.b);
	}

	customVertexArr[0].x = (float)rContext.rect.topLeft.x;
	customVertexArr[0].y = (float)rContext.rect.topLeft.y;

	customVertexArr[1].x = (float)rContext.rect.bottomRight.x;
	customVertexArr[1].y = (float)rContext.rect.topLeft.y;

	customVertexArr[2].x = (float)rContext.rect.bottomRight.x;
	customVertexArr[2].y = (float)rContext.rect.bottomRight.y;

	customVertexArr[3].x = (float)rContext.rect.topLeft.x;
	customVertexArr[3].y = (float)rContext.rect.bottomRight.y;

	if (rContext.textureType != TextureType::NONE) {
		customVertexArr[0].tu = rContext.rect.topLeft.tu;
		customVertexArr[0].tv = rContext.rect.topLeft.tv;
		customVertexArr[1].tu = rContext.rect.bottomRight.tu;
		customVertexArr[1].tv = rContext.rect.topLeft.tv;
		customVertexArr[2].tu = rContext.rect.bottomRight.tu;
		customVertexArr[2].tv = rContext.rect.bottomRight.tv;
		customVertexArr[3].tu = rContext.rect.topLeft.tu;
		customVertexArr[3].tv = rContext.rect.bottomRight.tv;
	}

	pD3Device->SetTexture(0, textures.at(rContext.textureType));
	pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, customVertexArr, sizeof(CUSTOMVERTEX));
}

void Drawer::EndDraw() {
	// 描画の終了
	pD3Device->EndScene();

	// 表示
	pD3Device->Present(NULL, NULL, NULL, NULL);
}

}
}
