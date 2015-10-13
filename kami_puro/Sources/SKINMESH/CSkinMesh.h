//=============================================================================
//
// スキンメッシュ処理 [CSkinMesh.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CSKINMESH_H_
#define _CSKINMESH_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../MAIN/main.h"
#include "CInheritanceHierarchy.h"

class CPlayer;
//*****************************************************************************
// マクロ
//*****************************************************************************
static const double DEFFAULT_ANIM_SPD = 0.01666;	// 基本のアニメーション速度
static const double DEFFAULT_CHANGE_ANIM_SPD = 0.15;// 基本のアニメーション変更速度
static const int	MAX_BONE_MATRIX = 52;

//*****************************************************************************
// 列挙体宣言
//*****************************************************************************
typedef enum
{
	SKIN_MESH_TYPE_TEST = 0,
	SKIN_MESH_TYPE_MAX
}SKIN_MESH_ANIM_MODEL;

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
// アニメーションの数だけ作る
typedef struct
{
	int nCallBackNum;			// コールバックする回数
	float* pCallBackTiming;		// コールバックするタイミング
}CALLBACK_TIMING;

//*****************************************************************************
// コールバック時に送りたいデータの塊
//*****************************************************************************
struct _CUSTOM_CALLBACK_DATA
{
	_CUSTOM_CALLBACK_DATA(void):nAnimationID(0){}
	int nAnimationID; //そのアニメーションの番号のデータ
};
typedef _CUSTOM_CALLBACK_DATA CUSTOM_CALLBACK_DATA;


//*****************************************************************************
// クラス宣言
//*****************************************************************************
class CSkinMesh
{
public:
	// コンストラクタ
	CSkinMesh(void);

	// デストラクタ
	~CSkinMesh(void);

	//================================================================================================
	// 初期化
	// 引数: デバイス、コールバックのタイミング構造体ポインタ、コールバック関数のハンドル、モデルタイプ
	//================================================================================================
	void Init(LPDIRECT3DDEVICE9 *pDevice, CALLBACK_TIMING* pCallBackTimimig, LPD3DXANIMATIONCALLBACKHANDLER pHandl, SKIN_MESH_ANIM_MODEL modelType);
	
	// 終了
	void Uninit(void);

	//================================================================================================
	// 更新
	// 引数: 座標、角度、スケール
	//================================================================================================
	void Update(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl);

	// 描画
	void Draw(CPlayer* pPlayer, int type);

	//================================================================================================
	// アニメーション変更関数
	// 引数: アニメーションID、ブレンディングの割合
	//================================================================================================
	void ChangeMotion(int nChangeAnimID,double dMoveRate);

	//================================================================================================
	// アニメーション速度アクセサ
	//================================================================================================
	double GetAnimSpd(void){return m_dAnimSpd;};
	void SetAnimSpd(double spd){m_dAnimSpd = spd;};

	//================================================================================================
	// デクラレーションゲット
	//================================================================================================
	LPDIRECT3DVERTEXDECLARATION9* GetDecl(void){ return &m_pDecl; };

private:
	//=================================================
	// コントローラーのトラックを増やす関数
	//=================================================
	LPD3DXANIMATIONCONTROLLER GetTruthController(void);

	//=================================================
	// コールバックアニメーションを作り出す
	// 引数: コールバックタイミング構造体のポインタ
	//=================================================
	void CreateCommpressAnimation(CALLBACK_TIMING* pCallBackTimimig);

	//=================================================
	// ボーン行列の個別割り当て
	// 引数: メッシュコンテナ情報、フレーム情報
	// 戻り値: 割り当てに成功したか
	//=================================================
	HRESULT AllocateBoneMatrix(LPD3DXMESHCONTAINER,LPD3DXFRAME p_FrameRoot);

	//=============================================================================
	// 全ボーン行列の割り当て
	// 同じ引数を入れているが、再帰の都合上そうしている
	// 引数: フレームのルートポインタ、フレームのルートポインタ
	// 戻り値: 割り当てに成功したか
	//=============================================================================
	HRESULT AllocateAllBoneMatrices(LPD3DXFRAME, LPD3DXFRAME p_FrameRoot);

	//=============================================================================
	// メッシュコンテナの描画
	// フレーム内のそれぞれのメッシュをレンダリングする
	// 引数: メッシュコンテナ情報、フレーム情報
	//=============================================================================
	VOID RenderMeshContainer(MYMESHCONTAINER*, MYFRAME*, CPlayer* pPlayer, int type);

	//=============================================================================
	// メッシュフレームの更新
	// フレームを更新する
	// 引数: フレーム情報、親の行列情報
	//============================================================================
	VOID UpdateFrameMatrices (LPD3DXFRAME , LPD3DXMATRIX );

	//=============================================================================
	// メッシュフレームの描画
	// フレームをレンダリングする
	// 引数: フレーム情報
	//============================================================================
	VOID DrawFrame(LPD3DXFRAME, CPlayer* pPlayer, int type);
	
	LPDIRECT3DDEVICE9				*m_pDevice;						// デバイス
	LPD3DXFRAME						m_pFrameRoot;					// フレームのルート
	LPD3DXANIMATIONCONTROLLER		m_pController;					// アニメーションのコントローラ
	CInheritanceHierarchy			m_InheritHierarchy;				// 継承階層構造クラス
	LPD3DXANIMATIONSET				*m_apAnimation;					// アニメーション情報
	DWORD							m_dwAnimType;					// アニメーション番号管理用
	LPD3DXCOMPRESSEDANIMATIONSET	*m_apAnimSetEx;					// コールバック機能付きのアニメーションセットID
	LPD3DXANIMATIONCALLBACKHANDLER	m_pCallBackHandler;				// アニメーションコールバックハンドラの多態ポインター
	CUSTOM_CALLBACK_DATA			*m_apCallBackDatas;				// アニメーションに送りたい情報の塊
	double							m_dAnimSpd;						// アニメーション速度
	int								m_nAnimType;					// アニメーションの種類
	LPDIRECT3DVERTEXDECLARATION9	m_pDecl;
	D3DXMATRIX						m_arrayWorldMtx[MAX_BONE_MATRIX];
};

#endif
//----EOF----