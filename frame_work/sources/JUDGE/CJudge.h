//=============================================================================
//
// CJudgeクラス [CJudge.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CJUDGE_H_
#define _CJUDGE_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../MAIN/main.h"
#include "../SCENE/GAME/PLAYER/CPlayerManager.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CJudgeManager;
class CScene2D;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CJudge
{
public:
	// OBB情報構造体
	typedef struct
	{
		D3DXVECTOR2 pos;		// 中心の座標
		D3DXVECTOR2 axisX;		// 分離軸
		D3DXVECTOR2 axisY;		// 分離軸
		float		lengthX;	// 中心からの距離
		float		lengthY;	// 中心からの距離
	}OBB_INFO;

	// コンストラクタ
	CJudge(CJudgeManager* pJudgeManager);

	// デストラクタ
	~CJudge(void);

	// フィールドとプレイヤーのあたり判定
	void ColiFieldxPlayer(void);

	// 宝とプレイヤーのあたり判定
	void ColiTreasurexPlayer(void);

	// 攻撃とプレイヤの当たり判定
	void ColiAttackxPlayer(void);

	// 糸とプレイヤの当たり判定
	void ColiThreadxPlayer(void);

	// フィールドと足場になる糸のあたり判定
	void ColiFieldxThreadOfFoothold(void);

	// ゴールとプレイヤの当たり判定
	void ColiGoalxPlayer(void);

	// フィールドと宝箱の当たり判定
	void ColiFieldxTreasure(void);

private:
	// 線分構造体
	struct Segment {
		D3DXVECTOR2 s; // 始点
		D3DXVECTOR2 v; // 方向ベクトル（線分の長さも担うので正規化しないように！）
	};
	//=========================================================================
	// 矩形と矩形のあたり判定
	// 引数:判定したいもののOBB情報
	// 戻り値：真偽
	//=========================================================================
	bool IsOBB(OBB_INFO& rectA, OBB_INFO& rectB);

	//=========================================================================
	// OBB情報作成関数
	// 引数:作成したOBB情報入れるやつ、座標、ロット、幅、高さ
	//=========================================================================
	void CreateOBBInfo(OBB_INFO* outOBB, D3DXVECTOR2* pos, float* rot, float* width, float* height);

	//=========================================================================
	// 線分と線分のあたり判定
	// 引数:線分1、線分2、(out)交点の座標
	// 戻り値：真偽
	//=========================================================================
	bool ColiRayxRay(Segment &seg1, Segment &seg2, D3DXVECTOR2* outPos);

	CJudgeManager*	m_pJudgeManager;
	CScene2D*		m_LastFieldColiPlayer[MAXIMUM_NUMBER_OF_PLAYER];	// プレイヤーが最後に当たってたフィールドポインタ
};

#endif
//----EOF----