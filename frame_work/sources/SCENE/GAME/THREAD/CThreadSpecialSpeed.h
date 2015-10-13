//=============================================================================
//
// CThreadSpecialAttackクラス [CThreadSpecialAttack.h]
// Author : 佐藤　諒一
//
// 攻撃特化形態の糸
//
//=============================================================================
#ifndef _CTHREAD_SPECIAL_SPEED_H_
#define _CTHREAD_SPECIAL_SPEED_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../../RENDERER/CRenderer.h"
#include "CThreadBase.h"
#include "../PLAYER/CPlayer.h"

// 糸が伸び縮みする最大時間
static const int	GROW_THREAD_TIME = 60;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CThreadSpecialSpeed : public CThreadBase
{
	// 公開メンバ
public:
	
	CThreadSpecialSpeed(LPDIRECT3DDEVICE9 *pDevice, int priority = TYPE_PRIORITY_THREAD_OF_FOOTHOLD, OBJTYPE type = OBJTYPE_2D);
	~CThreadSpecialSpeed(void);

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
	static CThreadSpecialSpeed *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		short nPlayerNum,
		D3DXVECTOR3 pos,
		DIRECTION_PLAYER_FACING playerDirection);

	// デス関数
	void Death(void);

	// ライド関数
	void Ride(bool ride);
private:
	HRESULT Init(short nPlayerNum, D3DXVECTOR3 pos, DIRECTION_PLAYER_FACING playerDirection);

	// 糸伸びる処理
	void GrowThread(void);

	// フィールドアニメーション
	void FieldAnim(void);

	// 上下
	void UpDown(void);

	DIRECTION_PLAYER_FACING m_Direction;	// 方向
	float					m_fAddPower;	// 伸びる大きさ
	int						m_nAnimTimer;	// アニメーションのカウンター
	float					m_fDefaultWidth;
	float					m_fDestWidth;
	float					m_fDefaultHeight;
	float					m_fDestHeight;
	bool					m_bSetNavi;
};

#endif
//----EOF----