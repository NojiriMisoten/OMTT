//=============================================================================
//
// CAttackSpecialSpeedクラス [CAttackSpecialSpeed.h]
// Author : 佐藤　諒一
//
// スピード特化型
//
//=============================================================================
#ifndef _CATTACKSPECIALSPEED_H_
#define _CATTACKSPECIALSPEED_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../../RENDERER/CRenderer.h"
#include "CAttackBase.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CPlayer;

// 寿命
static const short ATTACK_SPEED_END_TIME = 10;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CAttackSpecialSpeed : public CAttackBase
{
	// 公開メンバ
public:

	CAttackSpecialSpeed(LPDIRECT3DDEVICE9 *pDevice);
	~CAttackSpecialSpeed(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	void HitPlayer(CPlayer* pPlayer);

	//=======================================================================
	// クリエイト関数
	// 引数:デバイス、プレイヤ番号、座標
	// 戻り値:作成したアドレス
	//=======================================================================
	static CAttackSpecialSpeed *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		short nPlayerNum,
		D3DXVECTOR3 pos,
		D3DXVECTOR3 velocity);

	// 非公開メンバ
private:
	D3DXVECTOR3 m_vVelocity;
};

#endif
//----EOF----