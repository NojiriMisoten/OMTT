//=============================================================================
//
// CCrowdManagerクラス [CCrowdManager.h]
// Author : 塚本俊彦
//
// リングの前後にいる観客たち
// ３Ｄの板ポリ
//
//=============================================================================
#ifndef _CCROWDMANAGER_H_
#define _CCROWDMANAGER_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../RENDERER/CRenderer.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CManager;
class CCrowd;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CCrowdManager
{
public:
	// 上の客席の横の列数
	static const short UPPER_CHAIR_COL_MAX = 5;
	// 上の客席の行数
	static const short UPPER_CHAIR_ROW_MAX = 6;
	// 上の客席の横の列数
	static const short UPPER_SIDE_CHAIR_COL_MAX = 6;
	// 上の客席の行数
	static const short UPPER_SIDE_CHAIR_ROW_MAX = 3;
	// 下の客席の前の列数
	static const short LOWER_FRONT_CHAIR_COL_MAX = 2;
	// 下の客席の前以外の列数
	static const short LOWER_RIGHT_CHAIR_COL_MAX = 5;
	// 下の客席の行数
	static const short LOWER_CHAIR_ROW_MAX = 5;

	CCrowdManager(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager);
	~CCrowdManager();

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	// クリエイト
	static CCrowdManager* Create(
		LPDIRECT3DDEVICE9 *pDevice,
		CManager *pManager);

	// 描画しない
	void SetImvisible(void);
	// 描画する
	void SetVisible(void);

private:
	void Init();

	// 保存しときたいポインタ
	LPDIRECT3DDEVICE9 *m_pD3DDevice;
	CManager *m_pManager;

	// 2Fの客席 上から前、左、右
	CCrowd *m_pUpperFront[UPPER_CHAIR_COL_MAX][UPPER_CHAIR_ROW_MAX];
	CCrowd *m_pUpperLeft[UPPER_SIDE_CHAIR_COL_MAX][UPPER_SIDE_CHAIR_ROW_MAX];
	CCrowd *m_pUpperRight[UPPER_SIDE_CHAIR_COL_MAX][UPPER_SIDE_CHAIR_ROW_MAX];
	// 1Fの前の客席　左サイド
	CCrowd *m_pLowerFrontLeft[LOWER_FRONT_CHAIR_COL_MAX][LOWER_CHAIR_ROW_MAX];
	// 1Fの前の客席　右サイド
	CCrowd *m_pLowerFrontRight[LOWER_FRONT_CHAIR_COL_MAX][LOWER_CHAIR_ROW_MAX];
	// 1Fの右の客席
	CCrowd *m_pLowerRight[LOWER_RIGHT_CHAIR_COL_MAX][LOWER_CHAIR_ROW_MAX];
	// 1Fの左の客席
	CCrowd *m_pLowerLeft[LOWER_RIGHT_CHAIR_COL_MAX][LOWER_CHAIR_ROW_MAX];
	// 1Fの後ろの客席
	CCrowd *m_pLowerBack[LOWER_RIGHT_CHAIR_COL_MAX][LOWER_CHAIR_ROW_MAX];

};

#endif
//----EOF----