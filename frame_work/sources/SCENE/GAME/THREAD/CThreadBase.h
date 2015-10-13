//=============================================================================
//
// CThreadBaseクラス [CThreadBase.h]
// Author : 塚本　俊彦
//
// 糸の基底クラス
//
//=============================================================================
#ifndef _CTHREADBASE_H_
#define _CTHREADBASE_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../CSCENE/CScene2D.h"
#include "../PLAYER/CPlayerManager.h"
#include "CThreadManager.h"
#include "../PLAYER/CPlayer.h"
#include "../../../MANAGER/CManager.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CThreadBase : public CScene2D
{
	// 公開メンバ
public:
	CThreadBase(LPDIRECT3DDEVICE9 *pDevice, int priority, OBJTYPE type = OBJTYPE_THREAD);
	virtual ~CThreadBase(void);

	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	short GetPlayerNum(void){ return m_nPlayerNum; };
	void SetPlayerFacing(DIRECTION_PLAYER_FACING playerFacing){ m_PlayerFaicing = playerFacing; }

	// 攻撃がヒットした時に呼び出す関数
	// 引数　この攻撃に当たったプレイヤーのプレイヤー番号
	virtual void HitPlayer(CPlayer* pPlayer);

	// 継承メンバ
protected:
	// 寿命をカウントする
	short m_nCount;
	// 寿命
	short m_nEndTime;
	// 当たり判定をする時間
	short m_nHitStartTime;
	short m_nHitEndTime;
	// この糸は誰のプレイヤー番号
	short m_nPlayerNum;
	// 当たり判定をするフラグ
	bool m_bHitFlg;
	// 糸の種類
	ThreadType m_ThreadType;

	// プレイヤーが糸発射時に向いていた方向
	DIRECTION_PLAYER_FACING m_PlayerFaicing;
	D3DXVECTOR3 m_vPlayerDirection;


	// 非公開メンバ
private:
	
};

#endif
//----EOF----