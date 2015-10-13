//=============================================================================
//
// CResultクラス [CResult.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CRESULT_H_
#define _CRESULT_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../MAIN/main.h"
#include "../../PHASE/CPhase.h"
#include "../../MANAGER/CManager.h"

//*****************************************************************************
// 定数
//*****************************************************************************
// メニュー種類
enum RESULT_MENU{
	RESULT_MENU_RETRY,
	RESULT_MENU_CHARA_SELECT,
	RESULT_MENU_STAGE_SELECT,
	RESULT_MENU_MAX
};

// くるくるライトの数
static const int RESULT_CIRCLE_LIGHT_MAX = 100;

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;
class CSceneAnime;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CResult : public CPhase
{
public:
	// コンストラクタ
	CResult(void);

	// デストラクタ
	~CResult(void);
	
	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	//======================================================
	// クリエイト
	// 引数: フェーズモード、デバイス
	// 戻り値: 作ったやつのポインタ
	//======================================================
	static CResult* Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);

	// 非公開メンバ
private:
	//======================================================
	// 初期化
	// 引数: フェーズモード、デバイス
	//======================================================
	void Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);


	// BGの初期化
	void InitializeBG();
	// リモの勝者アニメーション初期化
	void InitializeWinAnimation();
	// リモの勝者アニメーション更新
	void UpdateWinAnimation();

	// 背景
	CScene2D* m_pBG;
	// ロゴ
	CScene2D* m_pLogo;
	// プッシュ文字
	CScene2D *m_pPush2D[PLAYER_MAX];
	// メニュー文字
	CScene2D *m_pMenuMoji[RESULT_MENU_MAX];
	// 選択メニュー
	CScene2D *m_pMenuBack;
	// プレイヤーアニメーション
	CSceneAnime *m_pPlayerAnime[PLAYER_MAX];
	// 勝った人の番号 引き分けなら-1 デフォルトも-1
	int m_nWinPlayerNum;
	// プレイヤーアニメーションのテクスチャ番号
	int m_nTextureIndex[PLAYER_MAX];
	// プレイヤアニメーションのテクスチャ変更するためのカウント
	int m_nTextureCount[PLAYER_MAX];
	// アニメーションから自動で遷移するカウント
	int m_nCount;
	// プッシュしたか
	bool m_bPush[PLAYER_MAX];
	// PUSHの点滅アニメーショ用カウント
	int m_nPushFlashCount[PLAYER_MAX];
	// PUSHの点滅アニメーショ用フラグ
	bool m_bPushFlash[PLAYER_MAX];
	// メニューのカーソル
	int m_nMenuCursol;
	// メニュー文字の拡大アニメーションのサイズ
	D3DXVECTOR2 m_vMojiSize;
	// そのフラグ
	bool m_bMojiScale;
	// CPUの数
	short m_nCpuNum;
	// 勝者に当てるライト
	CScene2D* m_pLight;
	// くるくるライト
	CScene2D* m_pCircleLight[RESULT_CIRCLE_LIGHT_MAX];
	// くるくるらいとの移動量
	D3DXVECTOR2 m_vCircleVelo[RESULT_CIRCLE_LIGHT_MAX];
	// くるくるライトの大きさ
	float m_vCircleSizeHalf[RESULT_CIRCLE_LIGHT_MAX];

};

#endif
//----EOF----