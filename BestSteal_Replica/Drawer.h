#ifndef DRAWER_H_
#define DRAWER_H_

#include <d3d9.h>
#include <d3dx9.h>

#include "AppCommon.h"


namespace BestStealReplica {

class Drawer {
public:
	enum TextureType {
		MAP,
		CHARACTER
	};

	Drawer(HWND hWnd, IDirect3D9* pDirect3D, D3DPRESENT_PARAMETERS* d3dpp);
	bool CreateTexture(const char* filePath, TextureType textureType);
	void BeginDraw();
	void Draw(Vertices<DrawingVertex> vertices, TextureType textureType) const;
	void Draw(Vertices<DrawingVertex> vertices, TextureType textureType, UINT16 alpha) const;
	void EndDraw();
	void Release();

private:
	IDirect3DDevice9*	pD3Device;
	LPDIRECT3DTEXTURE9	pTexture[2];
	void Draw(Vertices<DrawingVertex> vertices, LPDIRECT3DTEXTURE9 pTexture, UINT16 alpha) const;
};

}

#endif