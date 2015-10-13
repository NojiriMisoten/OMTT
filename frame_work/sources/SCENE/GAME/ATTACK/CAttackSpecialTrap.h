//=============================================================================
//
// CAttackSpecialTrapクラス [CAttackSpecialTrap.h]
// Author : 佐藤　諒一
//
// 罠特化型
//
//=============================================================================
#ifndef _CATTACKSPECIALTRAP_H_
#define _CATTACKSPECIALTRAP_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../../RENDERER/CRenderer.h"
#include "CAttackBase.h"

//=============================================================================
//	前方宣言
//=============================================================================
class CPlayer;

// 寿命
static const short ATTACK_TRAP_END_TIME = 30;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CAttackSpecialTrap : public CAttackBase
{
	// 公開メンバ
public:

	CAttackSpecialTrap(LPDIRECT3DDEVICE9 *pDevice);
	~CAttackSpecialTrap(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	void HitPlayer(CPlayer* pPlayer);

	//=======================================================================
	// クリエイト関数
	// 引数:デバイス、プレイヤ番号、座標
	// 戻り値:作成したアドレス
	//=======================================================================
	static CAttackSpecialTrap *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		short nPlayerNum,
		D3DXVECTOR3 pos,
		D3DXVECTOR3 velocity);

	// 非公開メンバ
private:
	D3DXVECTOR3 m_vDirection;
};

#endif
//----EOF----