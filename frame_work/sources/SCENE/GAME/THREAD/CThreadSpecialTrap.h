//=============================================================================
//
// CThreadSpecialAttackクラス [CThreadSpecialAttack.h]
// Author : 塚本俊彦
//
//=============================================================================
#ifndef _CTHREAD_SPECIAL_TRAP_H_
#define _CTHREAD_SPECIAL_TRAP_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../../RENDERER/CRenderer.h"
#include "CThreadBase.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CSceneAnime;

// 当たり判定の始まる時間
static const short THREAD_TRAP_HIT_START_TIME = 30;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CThreadSpecialTrap : public CThreadBase
{
	// 公開メンバ
public:
	// プライオリティとオブジェタイプはテスト
	CThreadSpecialTrap(
		LPDIRECT3DDEVICE9 *pDevice,
		int priority = TYPE_PRIORITY_ATTACK,
		OBJTYPE type = OBJTYPE_THREAD);

	~CThreadSpecialTrap(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// プレイヤに当たった時に呼ばれる関数
	void HitPlayer(CPlayer* pPlayer);

	//=======================================================================
	// クリエイト関数
	// 引数:デバイス、プレイヤ番号、座標
	// 戻り値:作成したアドレス
	//=======================================================================
	static CThreadSpecialTrap *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		short nPlayerNum,
		D3DXVECTOR3 pos);

	// 非公開メンバ
private:
	CSceneAnime *m_pBulletAnime;

};

#endif
//----EOF----