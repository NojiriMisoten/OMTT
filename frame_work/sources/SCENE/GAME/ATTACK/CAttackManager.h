//=============================================================================
//
// CAttacjManagerクラス [CAttacjManager.h]
// Author : 塚本　俊彦
//
// 攻撃マネージャー
//
//=============================================================================
#ifndef _CATTACKMANAGER_H_
#define _CATTACKMANAGER_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../../RENDERER/CRenderer.h"

//=============================================================================
// 定数
//=============================================================================
enum AttackType{
	ATTACK_TYPE_NORMAL,
	ATTACK_TYPE_ATTACK,			// 攻撃力特化型
	ATTACK_TYPE_ATTACK_THREAD,	// 攻撃力特化型(糸攻撃)
	ATTACK_TYPE_SPEED,			// スピード特化型
	ATTACK_TYPE_TRAP,			// 罠作成型
	ATTACK_TYPE_MAX
};

//=============================================================================
// 前方宣言
//=============================================================================
class CAttack;
class CPlayerManager;
class CAttackNormal;
class CAttackSpecialAttack;
class CAttackSpecialSpeed;
class CAttackSpecialTrap;
class CGame;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CAttackManager
{
	// 公開メンバ
public:
	CAttackManager(LPDIRECT3DDEVICE9 *pDevice);//, CPlayerManager* pPlayerManager);
	CAttackManager(LPDIRECT3DDEVICE9 *pDevice , CGame* pGame);//, CPlayerManager* pPlayerManager);
	~CAttackManager(void);

	void Init(void);
	void Uninit(void);
	void Update(void);

	// 攻撃を作る関数(攻撃タイプ,プレイヤ番号,座標,移動量（向きベクトル）)
	void CreateAttack(
		AttackType type,
		short sPlayerNum,
		D3DXVECTOR3 pos,
		D3DXVECTOR3 velocity);

	// 非公開メンバ
private:
	LPDIRECT3DDEVICE9*			m_pDevice;
	CGame*						m_pGame;
	//CPlayerManager*				m_pPlayerManager;		// プレイヤーマネージャのポインタ
	CAttackNormal*			m_apAttackNormal[20];	// 通常攻撃のポインタを入れておく配列
	//CAttackSpecialAttack*		m_apAttackAttack[12];	// 攻撃特化攻撃のポインタを入れておく配列
	//CAttackSpecialSpeed*		m_apAttackSpeed[4];		// 移動特化の攻撃のポインタを入れておく配列
	//CAttackSpecialTrap*		m_apAttackTrap[12];		// 妨害特化の攻撃のポインタを入れておく配列
};

#endif
//----EOF----