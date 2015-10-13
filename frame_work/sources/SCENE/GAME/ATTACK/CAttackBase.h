//=============================================================================
//
// CAttackBaseクラス [CAttackBase.h]
// Author : 塚本　俊彦
//
//=============================================================================
#ifndef _CATTACKBASE_H_
#define _CATTACKBASE_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../CSCENE/CScene2D.h"
#include "../PLAYER/CPlayerManager.h"
#include "CAttackManager.h"
#include "../../../MANAGER/CManager.h"

//=============================================================================
//	前方宣言
//=============================================================================
class CPlayer;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CAttackBase : public CScene2D
{
	// 公開メンバ
public:

	CAttackBase(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_ATTACK, OBJTYPE objType = OBJTYPE_ATTACK);
	virtual ~CAttackBase(void);

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void){};
	short GetPlayerNumber(void){ return m_nPlayerNum; };
	bool GetHitFlag(int playerID);

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
	// この攻撃は誰のかプレイヤー番号
	short m_nPlayerNum;
	// 当たり判定をするフラグ
	bool m_bHitFlg;
	// 当たったプレイヤー管理配列
	bool m_bHitPlayer[MAXIMUM_NUMBER_OF_PLAYER];
	// 攻撃の種類
	AttackType m_AttackType;

	// 非公開メンバ
private:
	
};

#endif
//----EOF----