//=============================================================================
//
// CPlayerManagerクラス [CPlayerManager.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CPLAYERMANAGER_H_
#define _CPLAYERMANAGER_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../MAIN/main.h"
#include "../../../SKINMESH/CSkinMesh.h"
#include "CPlayer.h"

//*****************************************************************************
// 列挙体
//*****************************************************************************
typedef enum
{
	PLAYER_1 = 0,
	PLAYER_2,
	PLAYER_MAX
}PLAYER_ID;

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CManager;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CPlayerManager
{
public:
	// コンストラクタ
	CPlayerManager(CManager* pManager);

	// デストラクタ
	~CPlayerManager(void);

	// 作成
	void CreatePlayer(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, SKIN_MESH_ANIM_MODEL type);

	// 終了
	void Uninit(void);

	D3DXVECTOR3& GetPlayerPos(PLAYER_ID ID);

	int GetPlayerHP( PLAYER_ID ID );

	CPlayer::PLAYER_ANIM_TYPE GetPlayerState( PLAYER_ID ID );

	//==================================================
	// アニメーション変更
	// moveRateは現在と次のアニメーションの混ぜる比率
	// ０で補間無でパッとかわる
	//==================================================
	void SetAnimType(PLAYER_ID ID, int type, double moveRate = 0.0);

	// アニメーションを時間の状態に指定
	void SetAnimMortionOfTime(PLAYER_ID ID, int percent);

	// アニメーション速度セット
	void SetAnimSpd(PLAYER_ID ID, double spd);

	// ダメージ処理
	void TakeDamage( PLAYER_ID ID, int damage ) { m_pPlayer[ID]->TakeDamage( damage ); };

	// 回復処理
	void TakeHeal( PLAYER_ID ID, int heal ) { m_pPlayer[ID]->TakeHeal( heal ); };

	// FINISH技使用可能フラグアクセサ
	bool GetUseFinishFlag(PLAYER_ID ID);
	void SetUseFinishFlag(PLAYER_ID ID, bool flag);

	// ロープフラグアクセサ
	bool GetRopeFlag(PLAYER_ID ID);
	void SetRopeFlag(PLAYER_ID ID, bool flag);

private:
	static const int PLAYER_NUM = 2;

	CPlayer*	m_pPlayer[PLAYER_MAX];
	CManager*	m_pManager;
};

#endif
//----EOF----