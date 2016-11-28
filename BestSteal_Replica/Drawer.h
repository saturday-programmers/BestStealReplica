#ifndef DRAWER_H_
#define DRAWER_H_

#include <d3d9.h>
#include <d3dx9.h>

#include "AppCommon.h"


namespace BestStealReplica {

class Drawer {
public:
	/* Enums -------------------------------------------------------------------------------------------- */
	enum TextureType {
		MAP,
		CHARACTER
	};

	/* Constructor / Destructor ------------------------------------------------------------------------- */
	Drawer(HWND hWnd, IDirect3D9* pDirect3D, D3DPRESENT_PARAMETERS* pD3dpp);

	/* Static Functions --------------------------------------------------------------------------------- */
	static UINT16 GetAlphaOnBlinking(int time);

	/* Functions ---------------------------------------------------------------------------------------- */
	bool CreateTexture(const TCHAR* filePath, TextureType textureType);
	void BeginDraw() const;
	void Draw(Vertices<DrawingVertex> vertices, TextureType textureType) const;
	void Draw(Vertices<DrawingVertex> vertices, TextureType textureType, UINT16 alpha) const;
	void Blackout() const;
	void EndDraw() const;
	void Release() const;

private:
	/* Variables ---------------------------------------------------------------------------------------- */
	IDirect3DDevice9*	pD3Device;
	LPDIRECT3DTEXTURE9	pTexture[2];

	/* Functions ---------------------------------------------------------------------------------------- */
	void Draw(Vertices<DrawingVertex> vertices, LPDIRECT3DTEXTURE9 pTexture, UINT16 alpha) const;
};

}

#endif