//=============================================================================
//
// CThreadSpecialAttackクラス [CThreadSpecialAttack.h]
// Author : 佐藤　諒一
//
// 攻撃特化形態の糸
//
//=============================================================================
#ifndef _CTHREAD_SPECIAL_ATTACK_H_
#define _CTHREAD_SPECIAL_ATTACK_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../../RENDERER/CRenderer.h"
#include "../ATTACK/CAttackBase.h"

//*****************************************************************************
//	前方宣言
//*****************************************************************************
class CPlayer;

// 寿命
static const short THREAD_ATTACK_END_TIME = 120;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CThreadSpecialAttack : public CAttackBase
{
	// 公開メンバ
public:
	// プライオリティとオブジェタイプはテスト
	CThreadSpecialAttack(LPDIRECT3DDEVICE9 *pDevice, int priority = TYPE_PRIORITY_THREAD_OF_FOOTHOLD, OBJTYPE type = OBJTYPE_ATTACK);
	~CThreadSpecialAttack(void);

	// 初期化
	//  引数　　座標、プレイヤー番号
	HRESULT Init(D3DXVECTOR3 pos, short playerNumber, D3DXVECTOR3 velocity);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 攻撃がヒットした時に呼び出す関数
	// 引数　この攻撃に当たったプレイヤーのプレイヤー番号
	void HitPlayer(CPlayer* pPlayer);

	//=======================================================================
	// クリエイト関数
	// 引数:デバイス、プレイヤ番号、座標
	// 戻り値:作成したアドレス
	//=======================================================================
	static CThreadSpecialAttack *Create(
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