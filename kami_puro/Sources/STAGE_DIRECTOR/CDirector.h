//=============================================================================
//
// CDirectorクラス [CDirector.h]
// Author : 池島　大樹
//
//=============================================================================
#ifndef _CDIRECTOR_H_
#define _CDIRECTOR_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../MAIN/main.h"

//*****************************************************************************
// マクロ
//*****************************************************************************

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// アニメーション
typedef enum
{
	ATTACK_SMALL_CHOP = 0,
	ATTACK_SMALL_ELBOW,
	ATTACK_SMALL_LARIAT,
	ATTACK_BIG_ROLLING,
	ATTACK_BIG_SHOLDER,
	ATTACK_BIG_DROPKICK,
	ATTACK_THROW_SLAP,
	ATTACK_THROW_BACKDROP,
	ATTACK_THROW_STUNNER,
	ATTACK_FINISH_BODYPRESS,
	ATTACK_FINSH_ATOMICBUSTER,
	ATTACK_MAX
}ATTACK_LIST;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CDirector
{
public:
	// コンストラクタ
	CDirector( void );

	// デストラクタ
	~CDirector( void );

	// 初期化
	void Init( void );

	// 終了
	void Uninit( void );

	// 更新
	void Update( void );
	
	// ディレクターに再生する演出を送る
	void SendDirector( ATTACK_LIST attackID, int playerID );
	
	// 現在再生中かどうかをゲット
	bool GetIsDirecting( void ) { return m_IsDirecting; };

private:
	bool			m_IsDirecting;			// 現在再生中かどうか
	ATTACK_LIST		m_CurAttackID;			// 現在再生中のAttackID
	int				m_CurPlayerID;			// 現在再生中のPlayerID
};

#endif
//----EOF----