//=============================================================================
//
// CCharaSelectクラス [CCharaSelect.h]
// Author : 塚本俊彦
//
//=============================================================================
#ifndef _CCHARA_SELECT_H_
#define _CCHARA_SELECT_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../MAIN/main.h"
#include "../../PHASE/CPhase.h"
#include "../../MANAGER/CManager.h"
#include "../GAME/PLAYER/CPlayer.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;
class CSceneAnime;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CCharaSelect : public CPhase
{
public:
	// コンストラクタ
	CCharaSelect(void);

	// デストラクタ
	~CCharaSelect(void);

	//=================================================================
	// 初期化関数
	// 引数: シーンのモード, デバイス
	//=================================================================
	void Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	//=================================================================
	// クリエイト関数
	// 引数: シーンのモード, デバイス
	// 戻り値: このシーンのアドレス
	//=================================================================
	static CCharaSelect* Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);

private:
	// ボタン
	enum ButtonNumber{
		// この番号の時はボタンじゃない
		BUTTON_NONE = -1,
		// キャラを選択しているボタン
		BUTTON_CHARA,
		// 準備完了のボタン
		BUTTON_READY,
		// コンフィグのボタン
		BUTTON_CONFIG,
	};

	// BGの初期化　ロゴも初期化してる
	void InitializeBG();
	// BGの更新 アニメーションをする
	void UpdateBG();
	// 参戦
	void Join(int playerNum);
	// 不参戦
	void UnJoin(int playerNum);
	// PUSHの点滅更新
	void UpdatePushAnimation();
	// READYの点滅更新
	void UpdateReadyAnimation();
	// リモのアニメーション更新
	void UpdateRimoAnimation();
	// 入力で参戦する更新
	void UpdateInputJoin();
	// カーソルの更新　アニメーションや入力で移動など
	void UpdateCursol();
	// 入力でテクスチャアニメーション
	void UpdateInput();
	// リモ2Dにテクスチャのループをセットする関数
	void SetRimoTextureIndex(int index, int min, int max){
		m_nRimoTextureMin[index] = min;
		m_nRimoTextureMax[index] = max;
	}
	// 引数のposと当たっているボタン番号を返す indexはプレイヤ番号
	ButtonNumber CollisionButton(int playerNum, D3DXVECTOR3 vCursolPos);
	// 準備完了を押したとき
	void Ready(int playerNum);
	// 準備完了じゃない
	void UnReady(int playerNum);

	// 背景
	CScene2D* m_pBG;
	// 背景の移動量
	D3DXVECTOR3 m_vBgVelo;
	// 背景が移動している方向(0:左 1:上 2:右 3:下)
	short m_nBgDirection;
	// キャラクター選択って表示する
	CScene2D* m_pLogo;
	// プレイヤ背景
	CScene2D* m_pPlayerBG[PLAYER_MAX];

	// PUSH文字の背景
	CScene2D *m_pPush2DBG[PLAYER_MAX];
	// PUSH文字
	CScene2D *m_pPush2D[PLAYER_MAX];
	// PUSHしたか
	bool m_bPush[PLAYER_MAX];
	// PUSHの点滅アニメーショ用カウント
	int m_nPushFlashCount[PLAYER_MAX];
	// PUSHの点滅アニメーショ用フラグ
	bool m_bPushFlash[PLAYER_MAX];

	// リモ２Ｄ
	CSceneAnime *m_pRimo[PLAYER_MAX];
	// リモ２Ｄのテクスチャ番号
	int m_nRimoTextureIndex[PLAYER_MAX];
	// リモ２Ｄのテクスチャ変更するためのカウント
	int m_nRimoTextureCount[PLAYER_MAX];
	// リモ２Ｄのテクスチャループインデックスの番号
	int m_nRimoTextureMax[PLAYER_MAX];
	int m_nRimoTextureMin[PLAYER_MAX];

	// 参戦2D
	CScene2D *m_pJoin2D[PLAYER_MAX];

	// Startボタン
	CScene2D *m_pStart2D;

	// キーコンフィグボタン
	CScene2D *m_pConfig;

	// カーソル位置
	int m_nCursol[PLAYER_MAX];
	// カーソル2D
	CScene2D *m_pCursol2D[PLAYER_MAX];
	// カーソルの角度
	float m_fCursolRad[PLAYER_MAX];
	// カーソルアニメーションのフラグ
	bool m_bCursolFlg[PLAYER_MAX];

	// 準備完了フラグ
	bool m_bReady[PLAYER_MAX];
	// 準備完了2D
	CScene2D *m_pReady2D[PLAYER_MAX];
	// 準備完了の点滅アニメーショ用カウント
	int m_nReadyFlashCount[PLAYER_MAX];
	// 準備完了の点滅アニメーショ用フラグ
	bool m_bReadyFlash[PLAYER_MAX];

	// リモの変身状態
	PLAYER_MODE m_PlayerMode[PLAYER_MAX];

	// 全員準備完了から遷移までのカウント
	int m_nNextPhaseCount;
	// 全員準備完了から遷移するかの１回だけのフラグ
	bool m_bNextPhaseOnece;
	// 参加するプレイヤの人数
	short m_nPlayerManualNum;
	// 準備完了した人数
	short m_nPlayerReadyNum;

	// 前のシーンで参加していた人数
	short m_nPlayerJoinedNum;

};

#endif
//----EOF----