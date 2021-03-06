//=============================================================================
//
// CSkillクラス [CSkill.h]
// Author : 池島　大樹
//
//=============================================================================
#ifndef _CSKILL_H_
#define _CSKILL_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../MAIN/main.h"
#include "../MANAGER/CManager.h"

//*****************************************************************************
// マクロ
//*****************************************************************************

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSkill
{
public:
	// コンストラクタ
	CSkill( CManager *pManager );

	// デストラクタ
	~CSkill( void );

	// 初期化
	virtual void Init( void );

	// 終了
	virtual void Uninit( void );

	// 更新
	virtual void Update( void );

private:
	CManager *m_pManager;
	int m_FrameCount;				// 技の経過フレーム数

};

#endif
//----EOF----