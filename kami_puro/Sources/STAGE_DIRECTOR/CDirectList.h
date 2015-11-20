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

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CDirectList
{
public:
	// コンストラクタ
	CDirectList( CManager *pManager );

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

	// 演出の配列
	CDirect			*m_pDirectList[DIR_MAX];

	/*ここにいっぱい*/
	CDirectChop		*m_pDirectChop;
	CDirectElbow	*m_pDirectElbow;
	CDirectLariat	*m_pDirectLariat;
};

#endif
//----EOF----