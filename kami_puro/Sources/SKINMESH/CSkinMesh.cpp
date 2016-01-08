//=============================================================================
//
// スキンメッシュ処理 [SkinMesh.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//==============================================
// インクルード
//==============================================
#include "CSkinMesh.h"
#include "../CONST/const.h"
#include "../SCENE/GAME/PLAYER/CPlayer.h"
//==============================================
// マクロ
//==============================================
static const float COMMPRESS_COEFFICIENT = 0.5f;							// コンプレスの係数

// スキンメッシュのパス
static const char *SKIN_MODEL_PATH[SKIN_MESH_TYPE_MAX] =
{
	"../data/MODEL/SKIN_MODEL/Player02.x",
	"../data/MODEL/SKIN_MODEL/Player02_b.x",
};
//=============================================================================
// コンストラクタ
//=============================================================================
CSkinMesh::CSkinMesh(void)
{
	m_pFrameRoot = NULL;
	m_pController = NULL;
	m_apAnimation = NULL;
	m_dwAnimType = 0;
	m_apAnimSetEx = NULL;
	m_pCallBackHandler = NULL;
	m_dAnimSpd = DEFFAULT_ANIM_SPD;
	m_pDevice = NULL;
	m_nAnimType = 0;
	m_AnimTime = 0;
	m_pTexture = NULL;
	m_isCulcBone = true;
}

//=============================================================================
// デストラクタ
//=============================================================================
CSkinMesh::~CSkinMesh(void)
{
}

//=============================================================================
// ボーン行列の個別割り当て
// 引数   : メッシュコンテナ情報
// 引数2  : フレーム情報
// 戻り値 : 割り当てに成功したか
//=============================================================================
HRESULT CSkinMesh::AllocateBoneMatrix(LPD3DXMESHCONTAINER pMeshContainerBase ,LPD3DXFRAME p_FrameRoot)
{
	MYFRAME *pFrame = NULL;
	DWORD dwBoneNum = 0;		// ボーンの数

	// ベースを入れる
	MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;

	if (pMeshContainer->pSkinInfo == NULL)
	{
		return S_OK;
	}

	// ボーンの数取得
	dwBoneNum = pMeshContainer->pSkinInfo->GetNumBones();

	// ボーンの数分マトリクス作成
	pMeshContainer->ppBoneMatrix = new D3DXMATRIX*[dwBoneNum];
	// ボーン分ループ
	for (DWORD i = 0; i < dwBoneNum; ++i)
	{
		// コンテナの中にその名前のものがあるか
		pFrame = (MYFRAME*)D3DXFrameFind(p_FrameRoot, pMeshContainer->pSkinInfo->GetBoneName(i));
		if (pFrame == NULL)
		{
			return E_FAIL;
		}

		// マトリクスを合成
		pMeshContainer->ppBoneMatrix[i] = &pFrame->CombinedTransformationMatrix;
	}

	return S_OK;
}

//=============================================================================
// 全ボーン行列の割り当て
// 同じ引数を入れているが、再帰の都合上そうしている
//=============================================================================
HRESULT CSkinMesh::AllocateAllBoneMatrices(LPD3DXFRAME pFrame ,LPD3DXFRAME p_FrameRoot)
{
	p_FrameRoot->pMeshContainer;
	// コンテナあるならボーン行列の個別割り当て
	if (pFrame->pMeshContainer != NULL)
	{
		if(FAILED(AllocateBoneMatrix(pFrame->pMeshContainer,p_FrameRoot)))
		{
			return E_FAIL;
		}
	}
	
	// つながりがあるならもう一回
	if (pFrame->pFrameSibling != NULL)
	{
		if(FAILED(AllocateAllBoneMatrices(pFrame->pFrameSibling,p_FrameRoot)))
		{
			return E_FAIL;
		}
	}

	// 子がいるならもう一回
	if (pFrame->pFrameFirstChild != NULL)
	{
		if(FAILED(AllocateAllBoneMatrices(pFrame->pFrameFirstChild,p_FrameRoot)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// メッシュコンテナの描画
// フレーム内のそれぞれのメッシュをレンダリングする
//=============================================================================
VOID CSkinMesh::RenderMeshContainer(MYMESHCONTAINER* pMeshContainer
									, MYFRAME* pFrame
									, CPlayer* pPlayer
									, int type)
{
	DWORD i,k,m;
	DWORD dwBoneIndex,dwBlendMatrixNum;
	DWORD dwPrevBoneID;
	LPD3DXBONECOMBINATION pBoneCombination;
	UINT iMatrixIndex;
	D3DXMATRIX mStack;

	//警告削除
	(void*)m;
	(void*)dwBoneIndex;

	DWORD boneNum = pMeshContainer->pSkinInfo->GetNumBones();
	
	//スキンメッシュの場合
	if(pMeshContainer->pSkinInfo != NULL)
	{	
		pBoneCombination = (LPD3DXBONECOMBINATION)pMeshContainer->pBoneBuffer->GetBufferPointer();
	
		dwPrevBoneID = UINT_MAX;
		for(i = 0; i < pMeshContainer->dwBoneNum; ++i)
		{
			dwBlendMatrixNum = 0;
			for (k = 0; k< pMeshContainer->dwWeight; k++)
			{
				if (pBoneCombination[i].BoneId[k] != UINT_MAX)
				{
					dwBlendMatrixNum = k;
				}
			}

			// 頂点に対しての重みづけ
			for (k = 0; k < boneNum; k++)
			{
				iMatrixIndex = pBoneCombination[i].BoneId[k];
				if (iMatrixIndex != UINT_MAX)
				{
					m_arrayWorldMtx[k] = pMeshContainer->pBoneOffsetMatrices[iMatrixIndex] * (*pMeshContainer->ppBoneMatrix[iMatrixIndex]);
				}
			}
			
			dwPrevBoneID = pBoneCombination[i].AttribId;

			// ワールドマトリクス適用
			pPlayer->SetWorldMtx(&m_arrayWorldMtx[0], (CPlayer::PLAYER_RENDERER_TYPE)type, dwBlendMatrixNum);

			m_pTexture = pMeshContainer->ppTextures;

			pPlayer->SetTextureForPS((CPlayer::PLAYER_RENDERER_TYPE)type, m_pTexture);

			// 描画
			pMeshContainer->MeshData.pMesh->DrawSubset(i);

			(*m_pDevice)->SetRenderState(D3DRS_VERTEXBLEND, 0);		// 書かないとXPで動かない
			//(*m_pDevice)->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, 0);
		}
	}

	//通常メッシュの場合
	else
	{
		return;
	}

	//D3DXMATRIX mtxRot, worldMtx;
	//D3DXMatrixIdentity(&mtxRot);
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	////================================================
	//// 位置と回転行列とスケールの計算
	//// D3DXMatrixMultiplyではfloatの無駄計算が多く
	//// 発生するためにこうしてる別に今回はmultiplyでも○
	////=================================================
	//worldMtx._11 = m_Scl.x * mtxRot._11;
	//worldMtx._12 = m_Scl.x * mtxRot._12;
	//worldMtx._13 = m_Scl.x * mtxRot._13;
	//worldMtx._14 = 0.0f;
	//
	//worldMtx._21 = m_Scl.y * mtxRot._21;
	//worldMtx._22 = m_Scl.y * mtxRot._22;
	//worldMtx._23 = m_Scl.y * mtxRot._23;
	//worldMtx._24 = 0.0f;
	//
	//worldMtx._31 = m_Scl.z * mtxRot._31;
	//worldMtx._32 = m_Scl.z * mtxRot._32;
	//worldMtx._33 = m_Scl.z * mtxRot._33;
	//worldMtx._34 = 0.0f;
	//
	//worldMtx._41 = m_Pos.x;
	//worldMtx._42 = m_Pos.y;
	//worldMtx._43 = m_Pos.z;
	//worldMtx._44 = 1.0f;
	//
	//// TODO
	//for (int i = 0; i < MAX_BONE_MATRIX; i++)
	//{
	//	m_arrayWorldMtx[i] = worldMtx;
	//}
}

//=============================================================================
// メッシュフレームの描画
// フレームをレンダリングする
//============================================================================
VOID CSkinMesh::DrawFrame(LPD3DXFRAME pFrameBase, CPlayer* pPlayer, int type)
{
	MYFRAME* pFrame=(MYFRAME*)pFrameBase;
	MYMESHCONTAINER* pMeshContainer = (MYMESHCONTAINER*)pFrame->pMeshContainer;

	// コンテナーがあるなら
	while(pMeshContainer != NULL)
	{
		RenderMeshContainer(pMeshContainer, pFrame, pPlayer, type);

		pMeshContainer = (MYMESHCONTAINER*)pMeshContainer->pNextMeshContainer;
	}

	// ぶら下がってるのあるなら
	if (pFrame->pFrameSibling != NULL)
	{
		DrawFrame(pFrame->pFrameSibling, pPlayer, type);
	}

	// 最初の子があるなら
	if (pFrame->pFrameFirstChild != NULL)
	{
		DrawFrame(pFrame->pFrameFirstChild, pPlayer, type);
	}
}

//=============================================================================
// メッシュフレームの更新
// フレームを更新する
//============================================================================
//フレーム内のメッシュ毎にワールド変換行列を更新する
VOID CSkinMesh::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	// 送られてきたフレーム情報入れる
	MYFRAME *pFrame = (MYFRAME*)pFrameBase;

	// 親のマトリクスがいるなら合成
	if (pParentMatrix != NULL)
	{
		D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	}

	// 居なかったらそのまま
	else
	{
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
	}

	// つながってるやついるならフレームマトリクス更新
	if (pFrame->pFrameSibling != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}

	// 子がいるならフレームマトリクス更新
	if (pFrame->pFrameFirstChild != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
	}
}

//==============================================
// 初期化
//==============================================
void CSkinMesh::Init(LPDIRECT3DDEVICE9 *pDevice, CALLBACK_TIMING* pCallBackTimimig, LPD3DXANIMATIONCALLBACKHANDLER pHandl, SKIN_MESH_ANIM_MODEL modelType)
{
	m_pDevice = pDevice;

	for (int i = 0; i < MAX_BONE_MATRIX; i++)
	{
		D3DXMatrixIdentity(&m_arrayWorldMtx[i]);
	}

	// 頂点要素配列を作る
	D3DVERTEXELEMENT9 elements[] =
	{
		// 頂点ストリーム(パイプライン)番号, オフセット(頂点の型の先頭からのバイト数), データ型, DEFAULTでＯＫ, 使用用途, 使用用途が同じものを複数使うときに仕分ける番号
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
		{ 0, 24, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
		{ 0, 28, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		D3DDECL_END()																					// 定義終了 絶対必要
	};
	// 頂点宣言したものを作る
	(*pDevice)->CreateVertexDeclaration(elements, &m_pDecl);

	//スキンメッシュに使うデータのメモリを初期化しておく
	ZeroMemory(&m_pFrameRoot	,sizeof(LPD3DXFRAME));
	ZeroMemory(&m_pController	,sizeof(LPD3DXANIMATIONCONTROLLER));

	if(FAILED(D3DXLoadMeshHierarchyFromX(
								SKIN_MODEL_PATH[modelType],							// ファイル名
								D3DXMESH_MANAGED,					// 管理用フラグ定数
								(*m_pDevice),						// デバイスへのポインタ
								&m_InheritHierarchy,				// 階層モデル構造体へのポインタ
								NULL,
								&m_pFrameRoot,
								&m_pController
								)))
	{
		MessageBox(NULL,"読み込みに失敗","警告",MB_OK);
	}
	//最後にAdvanceTime 中に呼び出すコールバックインスタンスを生成
	m_pCallBackHandler = pHandl;

	//すぐに正しいコントローラに変換する
	m_pController = GetTruthController();

	//ボーン行列の割り当て
	AllocateAllBoneMatrices(m_pFrameRoot,m_pFrameRoot);

	//各アニメーションデータの作成
	DWORD nAnimMax = m_pController->GetNumAnimationSets();	//アニメーションの個数をコントローラから回収
	m_apAnimation		= new LPD3DXANIMATIONSET			[ nAnimMax ];
	m_apAnimSetEx		= new LPD3DXCOMPRESSEDANIMATIONSET	[ nAnimMax ];
	m_apCallBackDatas	= new CUSTOM_CALLBACK_DATA			[ nAnimMax ];

	//--アニメーション数を元に各パラメータ代入--
	for(DWORD i=0; i<nAnimMax;i++)
	{
		m_pController->GetAnimationSet(i, &m_apAnimation[i]);
		m_apCallBackDatas[i].nAnimationID = i;
	}

	//コールバック付きアニメーションの再生
	CreateCommpressAnimation(pCallBackTimimig);

	//初期のモーションにセット
	m_pController->SetTrackAnimationSet(0, m_apAnimSetEx[0]);

	//==========================================
	// フレーム行列更新処理(初期の姿勢に設定しておく)
	//==========================================
	D3DXMATRIX IdentityMtx;
	D3DXMatrixIdentity(&IdentityMtx);
	UpdateFrameMatrices(m_pFrameRoot,&IdentityMtx);

	// アニメーション番号初期化
	m_dwAnimType = 0;
}

//==============================================
// 終了
//==============================================
void CSkinMesh::Uninit(void)
{
	SAFE_RELEASE(m_pController);
	SAFE_RELEASE(m_pDecl);

	delete [] m_apAnimation;
	delete [] m_apAnimSetEx;
	delete [] m_apCallBackDatas;
	
	//スキンメッシュ用のフレームをコールバックで解放
	//忘れるとやばいことになる
	m_InheritHierarchy.DestroyFrame( m_pFrameRoot);
}

//==============================================
// 更新
//==============================================
void CSkinMesh::Update(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl)
{
	D3DXMATRIX mtxRot, worldMtx;
	D3DXMatrixIdentity(&mtxRot);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	m_Pos = pos;
	m_Rot = rot;
	m_Scl = scl;
	//================================================
	// 位置と回転行列とスケールの計算
	// D3DXMatrixMultiplyではfloatの無駄計算が多く
	// 発生するためにこうしてる別に今回はmultiplyでも○
	//=================================================
	worldMtx._11 = scl.x * mtxRot._11;
	worldMtx._12 = scl.x * mtxRot._12;
	worldMtx._13 = scl.x * mtxRot._13;
	worldMtx._14 = 0.0f;

	worldMtx._21 = scl.y * mtxRot._21;
	worldMtx._22 = scl.y * mtxRot._22;
	worldMtx._23 = scl.y * mtxRot._23;
	worldMtx._24 = 0.0f;

	worldMtx._31 = scl.z * mtxRot._31;
	worldMtx._32 = scl.z * mtxRot._32;
	worldMtx._33 = scl.z * mtxRot._33;
	worldMtx._34 = 0.0f;

	worldMtx._41 = pos.x;
	worldMtx._42 = pos.y;
	worldMtx._43 = pos.z;
	worldMtx._44 = 1.0f;

	// TODO
	for (int i = 0; i < MAX_BONE_MATRIX; i++)
	{
		//D3DXMatrixIdentity(&m_arrayWorldMtx[i]);
		m_arrayWorldMtx[i] = worldMtx;
	}

	//==========================================
	// アニメーション更新処理  kDefaultSpan分進めている
	//==========================================
	const double kDefaultSpan = m_dAnimSpd;							// アニメーション速度　これを変数にするとアニメーション速度を動的に変えられる
	m_pController->AdvanceTime(kDefaultSpan,m_pCallBackHandler);	// AdvanceTimeがモーションを進行させている

	//==========================================
	// フレーム行列更新処理
	//==========================================
	UpdateFrameMatrices(m_pFrameRoot, &worldMtx);	// 親のフレームから再帰的に各フレームの位置を計算、描画していく

	m_AnimTime += GetAnimSpd();
	//if (m_AnimTime > 1.0)
	//{
	//	m_AnimTime = 0.0;
	//}
}

//==============================================
// 描画
//==============================================
void CSkinMesh::Draw(CPlayer* pPlayer, int type)
{
	DrawFrame(m_pFrameRoot, pPlayer, type);	// 親のフレームから再帰的に各フレームの位置を計算、描画していく
}

//==============================================
// アニメーション変更
//==============================================
void CSkinMesh::ChangeMotion(int nChangeAnimID,double dMoveRate)
{
	// 今と違うアニメーションへのチェンジなら
	if(m_nAnimType != nChangeAnimID)
	{
		// アニメーションの種類保存
		m_nAnimType = nChangeAnimID;

		//前回と異なるトラック位置を得る
		DWORD dwNewTrack = (m_dwAnimType == 0 ? 1 : 0);
		m_pController->SetTrackAnimationSet(dwNewTrack,m_apAnimSetEx[nChangeAnimID]);

		//キー情報のリセット
		m_pController->UnkeyAllTrackEvents(m_dwAnimType);
		m_pController->UnkeyAllTrackEvents(dwNewTrack);

		//今現在のトラック情報のデータをリセット
		m_pController->KeyTrackSpeed (m_dwAnimType ,0.0f, m_pController->GetTime() ,dMoveRate,D3DXTRANSITION_LINEAR);
		m_pController->KeyTrackWeight(m_dwAnimType ,0.0f, m_pController->GetTime() ,dMoveRate,D3DXTRANSITION_LINEAR);
		m_pController->KeyTrackEnable(m_dwAnimType ,FALSE, m_pController->GetTime() + dMoveRate);

		//これから遷移するデータを設定
		m_pController->KeyTrackSpeed (dwNewTrack, 1.0f, m_pController->GetTime(), dMoveRate, D3DXTRANSITION_LINEAR);
		m_pController->KeyTrackWeight(dwNewTrack, 1.0f, m_pController->GetTime(), dMoveRate, D3DXTRANSITION_LINEAR);
		m_pController->SetTrackEnable(dwNewTrack, TRUE);
		m_pController->KeyTrackPosition(dwNewTrack, 0.0f, m_pController->GetTime());

		//現在のトラックを更新
		m_dwAnimType = dwNewTrack;
	}
}

//==============================================================================
// 正しいアニメーションコントローラを取得する
//==============================================================================
LPD3DXANIMATIONCONTROLLER CSkinMesh::GetTruthController(void)
{
	//真実のコントローラを用意
	ID3DXAnimationController* newTruthController = NULL;

	//事前に正しいコントローラが回収できた場合のみ実行
	if(m_pController != NULL)
	{
		//クローンなのにこうしないと
		//正しいアニメーションコントローラ得られない
		m_pController->CloneAnimationController(m_pController->GetMaxNumAnimationOutputs()
												, m_pController->GetMaxNumAnimationSets()
												, m_pController->GetMaxNumTracks()
												, m_pController->GetMaxNumEvents()
												, &newTruthController);
	}

	//偽コントローラに真実のコントローラを代入しちゃうので、
	//前のコントローラを殺す でないとリリースリークを起こす
	m_pController->Release();
	m_pController = nullptr;

	//真実のコントローラを得る
	return newTruthController;
}

//==============================================================================
// コールバック呼び出し機能つきのモーションデータを作る  ******* 初期化で１回のみ呼ぶ(あくまで作るやつだから) ***********
//==============================================================================
void CSkinMesh::CreateCommpressAnimation(CALLBACK_TIMING* pCallBackTimimig)
{
	//===================================
	//初期設定
	//===================================

	//キーフレーム用のアニメーションセットを用意
	ID3DXKeyframedAnimationSet* keyFramedanimSet = NULL;
	
	//バッファに対して圧縮用のデータを用意
	ID3DXBuffer* compressedData = NULL;
	UINT max = m_pController->GetNumAnimationSets();
	int timing = 0;
	for(UINT animIndex = 0; animIndex < max; animIndex++)
	{
		//コールバック用の数とデータを用意
		//足音の分のキーフレームごとに必要なものと考える
		//↓アニメ中にコールバックする回数　モーション毎別もOK
		int numCallbacks = pCallBackTimimig[animIndex].nCallBackNum;
		D3DXKEY_CALLBACK* keys;										// 音を１アニメーションに３回鳴らすなら３個作る
		keys = new D3DXKEY_CALLBACK[numCallbacks];

		m_pController	->GetAnimationSet(animIndex, (ID3DXAnimationSet**)&keyFramedanimSet);
		keyFramedanimSet->Compress(D3DXCOMPRESS_DEFAULT, COMMPRESS_COEFFICIENT, NULL, &compressedData);
		timing = 0;
		for(timing = 0; timing < numCallbacks; timing++)
		{
			// コールバック用に元々の再生時間的なのを回収してる感じ
			double ticks = keyFramedanimSet->GetSourceTicksPerSecond();
			double period = keyFramedanimSet->GetPeriod();

			// コールバックが呼び出される時間を指定
			// 例えば↓のように/2.0fしてるのでモーションの50%くらいが再生されたときに発動する
			//keys[0].Time = float(period/2.0f*ticks);

			keys[timing].Time = float(period * pCallBackTimimig[animIndex].CallBackTiming * ticks);	// 1.0がＭＡＸ(アニメーションの時間を0.0～1.0とする)
			keys[timing].pCallbackData = (LPVOID)&m_apCallBackDatas[animIndex];	// m_apCallBackDatas[0]の０がアニメーション番号
		}
	
		// コールバックが設定された新たなアニメーションセットを作成
		D3DXCreateCompressedAnimationSet(keyFramedanimSet->GetName()
										, keyFramedanimSet->GetSourceTicksPerSecond()
										, keyFramedanimSet->GetPlaybackType()
										, compressedData,numCallbacks
										, &keys[0]
										, &m_apAnimSetEx[animIndex]);

		//圧縮データはもういらないので殺す
		compressedData->Release();
		keyFramedanimSet->Release();

		SAFE_DELETE_ARRAY(keys);
	}

	for(UINT animIndex = 0; animIndex < max; animIndex++)
	{
		//=============================================
		// 各設定を新規のアニメーションセットに反映
		//=============================================
		//通常のアニメーションセットを抹消
		m_pController->UnregisterAnimationSet(m_apAnimation[animIndex]);
	
		// コールバック付きのアニメセットを新たに登録する(差し替え)
		m_pController->RegisterAnimationSet(m_apAnimSetEx[animIndex]);
	}
}

//================================================================================================
// アニメーション指定時間の状態にする
//================================================================================================
void CSkinMesh::SetAnimMotion(double time)
{
	//assert(time >= 0.0 && time <= 1.0 && "セット時間が不正です");

	InitAnim();

	const double kDefaultSpan = time;
	m_AnimTime += time;
	m_pController->AdvanceTime(kDefaultSpan, m_pCallBackHandler);
}

//================================================================================================
// アニメーションの現在時刻ゲット
//================================================================================================
double CSkinMesh::GetAnimTime(void)
{
	return m_AnimTime;
}

//=============================================================================
// アニメーション初期化関数
//============================================================================
void CSkinMesh::InitAnim(void)
{
	//前回と異なるトラック位置を得る
	DWORD dwNewTrack = (m_dwAnimType == 0 ? 1 : 0);
	m_pController->SetTrackAnimationSet(dwNewTrack, m_apAnimSetEx[m_nAnimType]);

	//キー情報のリセット
	m_pController->UnkeyAllTrackEvents(m_dwAnimType);
	m_pController->UnkeyAllTrackEvents(dwNewTrack);

	//今現在のトラック情報のデータをリセット
	m_pController->KeyTrackSpeed(m_dwAnimType, 0.0f, m_pController->GetTime(), 0, D3DXTRANSITION_LINEAR);
	m_pController->KeyTrackWeight(m_dwAnimType, 0.0f, m_pController->GetTime(), 0, D3DXTRANSITION_LINEAR);
	m_pController->KeyTrackEnable(m_dwAnimType, FALSE, m_pController->GetTime());

	//これから遷移するデータを設定
	m_pController->KeyTrackSpeed(dwNewTrack, 1.0f, m_pController->GetTime(), 0, D3DXTRANSITION_LINEAR);
	m_pController->KeyTrackWeight(dwNewTrack, 1.0f, m_pController->GetTime(), 0, D3DXTRANSITION_LINEAR);
	m_pController->SetTrackEnable(dwNewTrack, TRUE);
	m_pController->KeyTrackPosition(dwNewTrack, 0.0f, m_pController->GetTime());

	//現在のトラックを更新
	m_dwAnimType = dwNewTrack;

	m_AnimTime = 0.0;
}
//----EOF----