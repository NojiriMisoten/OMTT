//=============================================================================
//
// スキンメッシュ処理 [CInheritanceHierarchy.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CINHERITANCEHEIRARCHY_H_
#define _CINHERITANCEHEIRARCHY_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../MAIN/main.h"

//*****************************************************************************
// スキンメッシュ用構造体
//*****************************************************************************
// 派生フレーム構造体（それぞれのメッシュ用の最終ワールド行列を追加する）
struct MYFRAME: public D3DXFRAME
{	
	D3DXMATRIX CombinedTransformationMatrix;	//行列を合成するための行列
};

//*****************************************************************************
// 派生メッシュコンテナー構造体
//*****************************************************************************
struct MYMESHCONTAINER: public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9*	ppTextures;				// コンテナーがテクスチャを複数持てるようにするポインターのポインターを追加する
	DWORD				dwWeight;				// 重みの個数（重みとは頂点への影響）
	DWORD				dwBoneNum;				// ボーンの数
	LPD3DXBUFFER		pBoneBuffer;			// ボーンテーブル
	D3DXMATRIX**		ppBoneMatrix;			// 全てのボーンのワールド行列の先頭ポインター
	D3DXMATRIX*			pBoneOffsetMatrices;	// フレームとしてのボーンのワールド行列のポインター
};

//*****************************************************************************
// クラス宣言
//*****************************************************************************
class CInheritanceHierarchy: public ID3DXAllocateHierarchy
{
public:
	CInheritanceHierarchy(){}
	STDMETHOD(CreateFrame)			(THIS_ LPCSTR  , LPD3DXFRAME *);
	STDMETHOD(CreateMeshContainer)	(THIS_ LPCTSTR , CONST D3DXMESHDATA* ,CONST D3DXMATERIAL* , CONST D3DXEFFECTINSTANCE* , DWORD , CONST DWORD *, LPD3DXSKININFO , LPD3DXMESHCONTAINER *);
	STDMETHOD(DestroyFrame)			(THIS_ LPD3DXFRAME );			//削除用 しないとメモリリーク
	STDMETHOD(DestroyMeshContainer)	(THIS_ LPD3DXMESHCONTAINER );	//削除用 しないとメモリリーク

private:
	//=============================================================================
	// メッシュのコンバート関数
	//============================================================================
	void ConvertMesh(LPD3DXMESH* pMesh);
};

#endif