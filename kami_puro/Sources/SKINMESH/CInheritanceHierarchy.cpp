//=============================================================================
//
// CInheritanceHierarchyの処理 [CInheritanceHierarchy.cpp]
// Author : 野尻　尚希
//
//=============================================================================
#include "CInheritanceHierarchy.h"
#include "../CONST/const.h"
#include "../RENDERER/CRenderer.h"

//==============================================
// マクロ
//==============================================
static const D3DXCOLOR	COLOR_OF_NOT_EXIST_MATERIAL = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);	// マテリアルがなかった時の設定色

//==============================================================================
// フレームの作成(XFileの各パーツ単位のこと　腕とか足とかのパーツ作るイメージ)
//==============================================================================
HRESULT CInheritanceHierarchy::CreateFrame(LPCTSTR Name, LPD3DXFRAME *ppNewFrame)
{
	//フレーム用意
	MYFRAME *pFrame;

	//引数の新しいフレームへのポインタを初期化しとく
	*ppNewFrame = NULL;

	//フレームの確保を試みる
	pFrame = new MYFRAME;

	//フレームのメモリ確保できなかったら
	if (pFrame == NULL)
	{
		//スキンメッシュ用のメモリ不足を通達して終了
		return E_OUTOFMEMORY;
	}

	//今度は 名前のメモリ確保を試みる
	pFrame->Name= new TCHAR[lstrlen(Name) + 1];	// \0分の+1

	//メモリ不足なら失敗
	if (!pFrame->Name)
	{
		return E_FAIL;
	}

	//メモリ確保したフレーム内の名前欄に書き写しておく
	strcpy(pFrame->Name,Name);

	//次は行列を初期化する
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);

	// 行列の初期化
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

	//各種ポインタの初期化
	pFrame->pMeshContainer = NULL;	//メッシュコンテナ初期化
	pFrame->pFrameSibling = NULL;	//ぶらさがってるフレーム初期化
	pFrame->pFrameFirstChild = NULL;//フレームの子要素初期化

	//新しいフレームのポインタへのポインタを今色々やったフレームにしておく
	*ppNewFrame = pFrame;

	return S_OK;
}

//====================================================================================
// メッシュコンテナの作成(XFileの各パーツの集合)ようはスキンメッシュモデルをここで作る
//====================================================================================
HRESULT CInheritanceHierarchy::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA* pMeshData,
													CONST D3DXMATERIAL* pMaterials, CONST D3DXEFFECTINSTANCE* pEffectInstances,
													DWORD NumMaterials, CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, 
													LPD3DXMESHCONTAINER *ppMeshContainer )
{
	//========================================================================
	// コンテナ作成に当たって必要な変数を用意
	//========================================================================
	MYMESHCONTAINER *pMeshContainer = NULL;
	int iFacesAmount;
	DWORD iMaterial;
	LPDIRECT3DDEVICE9 pDevice = NULL;
	LPD3DXMESH pMesh = NULL;
	*ppMeshContainer = NULL;
	DWORD dwBoneNum=0;

	//========================================================================
	// コンテナを動的確保して値を入れていく作業へ
	//========================================================================
	pMesh = pMeshData->pMesh;
	pMeshContainer = new MYMESHCONTAINER;

	//メモリ不足のときは終了
	if (pMeshContainer == NULL)
	{
		return E_OUTOFMEMORY;
	}

	//作成に成功したら中身を0で初期化しておく
	ZeroMemory(pMeshContainer, sizeof(MYMESHCONTAINER));

	//メッシュコンテナとしてXFileでつけられてる名前を拾う
	pMeshContainer->Name=new TCHAR[lstrlen(Name) + 1];	// \0分の+1

	//名前がなかったらおかしいので終了
	if (!pMeshContainer->Name)
	{
		return E_FAIL;
	}

	//名前があるならそれにする
	strcpy(pMeshContainer->Name,Name);

	// デバイスゲット
	pMesh->GetDevice(&pDevice);

	// 面の数ゲット
	iFacesAmount = pMesh->GetNumFaces();

	pMeshContainer->MeshData.pMesh = pMesh;
	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	//通常メッシュの場合はこれが必要。スキンの場合、これをするとメモリリークになる。
	if (pSkinInfo == NULL)
	{
		pMesh->AddRef();
	}

	//メッシュのマテリアル設定
	pMeshContainer->NumMaterials = max(1, NumMaterials);
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	pMeshContainer->pAdjacency = new DWORD[iFacesAmount * NUM_POLYGON_CREATE_TRIANGLE];

	//隣接性情報またはマテリアルがなければ終了
	if( pMeshContainer->pAdjacency == NULL
	||	pMeshContainer->pMaterials == NULL )
	{
		return E_FAIL;
	}

	//情報をメッシュコンテナに流し込む
	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * iFacesAmount * NUM_POLYGON_CREATE_TRIANGLE);
	memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);

	//========================================================================
	// モデルのマテリアル情報
	//========================================================================
	if (NumMaterials > 0)
	{
		memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

		for (iMaterial = 0; iMaterial < NumMaterials; iMaterial++)
		{
			// マテリアルのディフーズ色を強制的に変更
			pMeshContainer->pMaterials[iMaterial].MatD3D.Diffuse = DEFAULT_POLYGON_COLOR;

			if (pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL)
			{
				TCHAR strTexturePath[MAX_PATH];
				strcpy( strTexturePath,TEX_FOLDER_PATH);
				strcat_s( strTexturePath, sizeof( char ) * MAX_LENGTH_FILE_PATH, pMeshContainer->pMaterials[iMaterial].pTextureFilename );
				if( FAILED( D3DXCreateTextureFromFile( pDevice, strTexturePath, 
														&pMeshContainer->ppTextures[iMaterial] ) ) )
				pMeshContainer->ppTextures[iMaterial] = NULL;
				pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;
			}
		}
	}
	else
	{
		pMeshContainer->pMaterials[0].pTextureFilename = NULL;
		memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = COLOR_OF_NOT_EXIST_MATERIAL.r;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = COLOR_OF_NOT_EXIST_MATERIAL.g;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = COLOR_OF_NOT_EXIST_MATERIAL.b;
		pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
	}

	//========================================================================
	//該当メッシュがスキン情報を持っている場合
	//========================================================================
	if (pSkinInfo != NULL)
	{
		pMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();
		dwBoneNum = pSkinInfo->GetNumBones();
		pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[dwBoneNum];

		for(DWORD i= 0; i < dwBoneNum; i++)
		{	
			memcpy(&pMeshContainer->pBoneOffsetMatrices[i],pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i),sizeof(D3DMATRIX));
		}
		if(FAILED(	pMeshContainer->pSkinInfo->ConvertToBlendedMesh(pMesh,
																	NULL,
																	pMeshContainer->pAdjacency,
																	NULL,
																	NULL,
																	NULL,
																	&pMeshContainer->dwWeight,
																	&pMeshContainer->dwBoneNum,
																	&pMeshContainer->pBoneBuffer,
																	&pMeshContainer->MeshData.pMesh)))
		{
			return E_FAIL;
		}

		// コンバート
		ConvertMesh(&pMeshContainer->MeshData.pMesh);
	}
	//ローカルに生成したメッシュコンテナーを呼び出し側にコピーする （コピーじゃないけど・・・）
	*ppMeshContainer = pMeshContainer;

	//参照カウンタを増やしたので減らす
	SAFE_RELEASE(pDevice);
	return S_OK;
}

//====================================================================================
// フレームの削除(XFileの各パーツ単位のこと　腕とか足とかのパーツ作るイメージ)
//====================================================================================
HRESULT CInheritanceHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	SAFE_DELETE_ARRAY(pFrameToFree->Name);

	// 始まり
	if(pFrameToFree->pFrameFirstChild)
	{
		DestroyFrame(pFrameToFree->pFrameFirstChild);
	}

	// つながり
	if(pFrameToFree->pFrameSibling)
	{
		DestroyFrame(pFrameToFree->pFrameSibling);
	}

	// 大枠
	if(pFrameToFree->pMeshContainer)
	{
		DestroyMeshContainer(pFrameToFree->pMeshContainer);
	}

	SAFE_DELETE(pFrameToFree);
	return S_OK;
}

//==============================================================================
// メッシュコンテナの削除
//==============================================================================
HRESULT CInheritanceHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	DWORD iMaterial;
	MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;

	SAFE_DELETE_ARRAY(pMeshContainer->Name);				// 確保した名前開放
	SAFE_RELEASE(pMeshContainer->pSkinInfo);				// スキン情報開放
	SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);			// 隣接情報開放
	SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);			// マテリアル情報開放

	SAFE_DELETE_ARRAY(pMeshContainer->ppBoneMatrix);		// ボーンのマトリクス開放
	
	// テクスチャーあるなら
	if (pMeshContainer->ppTextures != NULL)
	{
		// マテリアルの数分ループ
		for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			SAFE_RELEASE(pMeshContainer->ppTextures[iMaterial]);
		}
	}
	SAFE_DELETE_ARRAY(pMeshContainer->ppTextures);

	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);

	if(pMeshContainer->pBoneBuffer != NULL)
	{
		SAFE_RELEASE(pMeshContainer->pBoneBuffer);
		SAFE_DELETE_ARRAY(pMeshContainer->pBoneOffsetMatrices);
	}

	SAFE_DELETE(pMeshContainer);
	return S_OK;
}

//=============================================================================
// メッシュのコンバート関数
//============================================================================
void CInheritanceHierarchy::ConvertMesh(LPD3DXMESH* pMesh)
{
	LPDIRECT3DDEVICE9 *pDevice = CRenderer::GetDevice();
	// クローン作製
	D3DVERTEXELEMENT9 elements[] =
	{
		// 頂点ストリーム(パイプライン)番号, オフセット(頂点の型の先頭からのバイト数), データ型, DEFAULTでＯＫ, 使用用途, 使用用途が同じものを複数使うときに仕分ける番号
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
		{ 0, 24, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
		{ 0, 28, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()																					// 定義終了 絶対必要
	};
	LPD3DXMESH pOldMesh = *pMesh;
	pOldMesh->CloneMesh(D3DXMESH_MANAGED
		, elements
		, *pDevice
		, pMesh);
	pOldMesh->Release();
}

//----EOF----