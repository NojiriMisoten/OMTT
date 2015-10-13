//=============================================================================
//
// CThreadNormalクラス [CThreadNoaml.h]
// Author : 塚本　俊彦
//
// 普通糸
//
//=============================================================================
#ifndef _CTHREADNORMAL_H_
#define _CTHREADNORMAL_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../../RENDERER/CRenderer.h"
#include "CThreadBase.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CSceneAnime;

// 寿命
static const short THREAD_NORMAL_END_TIME = 30;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CThreadNormal : public CThreadBase
{
	// 公開メンバ
public:
	// プライオリティとオブジェタイプはテスト
	CThreadNormal(
		LPDIRECT3DDEVICE9 *pDevice,
		int priority = TYPE_PRIORITY_ATTACK,
		OBJTYPE type = OBJTYPE_THREAD);

	~CThreadNormal(void);

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
	static CThreadNormal *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		short nPlayerNum,
		D3DXVECTOR3 pos,
		D3DXVECTOR3 direction);

	// 非公開メンバ
private:
	CSceneAnime *m_pBulletAnime;
};

#endif
//----EOF----