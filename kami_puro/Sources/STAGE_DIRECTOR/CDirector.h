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
#include "../SCENE/GAME/PLAYER/CPlayerManager.h"

//*****************************************************************************
// マクロ
//*****************************************************************************

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// アニメーション
typedef enum
{
	DIR_SMALL_CHOP = 0,
	DIR_SMALL_ELBOW,
	DIR_SMALL_LARIAT,
	DIR_BIG_ROLLING,
	DIR_BIG_SHOULDER,
	DIR_BIG_DROPKICK,
	DIR_THROW_SLAP,
	DIR_THROW_BACKDROP,
	DIR_THROW_STUNNER,
	DIR_FINISHER,
	DIR_ROPE,
	DIR_ROPE_LARIAT,
	DIR_ROPE_DROPKICK,
	DIR_ROPE_STUNNER,
	DIR_BATTLE_INTRO,
	DIR_BATTLE_RESULT,
	DIR_MAX
}DIRECT_ID;

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CManager;
class CGame;
class CDirectList;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CDirector
{
public:
	typedef struct{
		DIRECT_ID	directingID;
		PLAYER_ID	playerID;
	}DIRECT_PLAYER;

	// コンストラクタ
	CDirector( CManager *pManager, CGame *pGame );

	// デストラクタ
	~CDirector( void );

	// 初期化
	void Init( void );

	// 終了
	void Uninit( void );

	// 更新
	void Update( void );
	
	// ディレクターに再生する演出を強制的に送る
	void SendDirector( DIRECT_ID directingID, PLAYER_ID playerID );

	// ディレクターに再生する演出を送る
	int Direct( DIRECT_ID directingID, PLAYER_ID playerID );
	
	// 現在再生中かどうかをゲット、-1なら再生していない
	DIRECT_PLAYER GetIsDirecting( void ) { return m_CurDirect; };

	// 現在再生中かどうかをセット、-1なら再生していない
	DIRECT_PLAYER SetIsDirecting( DIRECT_PLAYER directing ) { m_CurDirect = directing; };

	// 再生終了
	void SetEndDirecting( void );

	// デフォルトのカメラ設定
	void SetDefaultCamera( void );

private:

	CManager		*m_pManager;
	CGame			*m_pGame;
	CDirectList		*m_pDirectList;
	DIRECT_PLAYER	m_CueDirect;			// 再生予約
	DIRECT_PLAYER	m_CurDirect;			// 再生中
};

#endif
//----EOF----