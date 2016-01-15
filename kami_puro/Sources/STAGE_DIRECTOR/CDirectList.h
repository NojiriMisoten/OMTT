//=============================================================================
//
// CDirectListクラス [CDirectList.h]
// Author : 池島　大樹
//
//=============================================================================
#ifndef _CDIRECTLIST_H_
#define _CDIRECTLIST_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../MAIN/main.h"
#include "../MANAGER/CManager.h"
#include "CDirector.h"

//*****************************************************************************
// マクロ
//*****************************************************************************

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// 前方定義
//*****************************************************************************
class CDirect;
class CDirectChop;
class CDirectElbow;
class CDirectLariat;
class CDirectRolling;
class CDirectShoulder;
class CDirectDropkick;
class CDirectSlap;
class CDirectBackdrop;
class CDirectStunner;
class CDirectFinisher;
class CDirectRope;
class CDirectLariatRope;
class CDirectDropkickRope;
class CDirectStunnerRope;
class CDirectBattleIntro;
class CDirectBattleResult;

class CGame;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CDirectList
{
public:
	// コンストラクタ
	CDirectList( CManager *pManager, CGame *pGame );

	// デストラクタ
	~CDirectList( void );

	// 初期化
	void Init( void );

	// 終了
	void Uninit( void );

	// 終了
	void Update( void );

	// ディレクトＩＤからポインタを取得
	CDirect *GetDirectList( DIRECT_ID dirID ) { return m_pDirectList[dirID]; };

private:
	CManager		*m_pManager;
	CGame			*m_pGame;

	// 演出の配列
	CDirect			*m_pDirectList[DIR_MAX];

	/*ここにいっぱい*/
	CDirectChop			*m_pDirectChop;
	CDirectElbow		*m_pDirectElbow;
	CDirectLariat		*m_pDirectLariat;
	CDirectRolling		*m_pDirectRolling;
	CDirectShoulder		*m_pDirectShoulder;
	CDirectDropkick		*m_pDirectDropkick;
	CDirectSlap			*m_pDirectSlap;
	CDirectBackdrop		*m_pDirectBackdrop;
	CDirectStunner		*m_pDirectStunner;
	CDirectFinisher		*m_pDirectFinisher;
	CDirectRope			*m_pDirectRope;
	CDirectLariatRope	*m_pDirectLariatRope;
	CDirectDropkickRope	*m_pDirectDropkickRope;
	CDirectStunnerRope	*m_pDirectStunnerRope;
	CDirectBattleIntro	*m_pDirectBattleIntro;
	CDirectBattleResult	*m_pDirectBattleResult;
};

#endif
//----EOF----