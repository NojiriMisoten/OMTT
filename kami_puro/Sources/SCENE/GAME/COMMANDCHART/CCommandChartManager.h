//=============================================================================
//
//	CCommandChartManagerクラスの宣言(CCommandChartManager.h)
//
//	Author : 佐藤　諒一
//
//=============================================================================
#ifndef __CCOMMANDCHARTMANAGER_H__
#define __CCOMMNADCHARTMANAGER_H__

//-----------------------------------------------------------------------------
// インクルード
//-----------------------------------------------------------------------------
#include "../../../MAIN/main.h"
//#include "CCommandChart.h"

//-----------------------------------------------------------------------------
//	マクロ
//-----------------------------------------------------------------------------
static const int MAX_PLAYER_NUM = 2;

//-----------------------------------------------------------------------------
//	前方宣言
//-----------------------------------------------------------------------------
class CCommandChart;

//-----------------------------------------------------------------------------
//	クラス定義
//-----------------------------------------------------------------------------
class CCommandChartManager
{
public:
	// コンストラクタ
	CCommandChartManager();

	// デストラクタ
	~CCommandChartManager();

	// 初期化
	// 引数：デバイス
	void Init(LPDIRECT3DDEVICE9* device);

	// 更新
	void Update(void);

	// 描画
	void Draw(void);

	// 終了
	void Uninit(void);

	// 生成
	// 引数：デバイス
	static CCommandChartManager* Create(LPDIRECT3DDEVICE9* device);

	// 最終的に繰り出す技を調べて返す
	// 戻り値：繰り出す技のタイプ
//	COMMAND_TYPE UseTechnic( int playerNum ) { return m_pCommandChart[playerNum]->UseTechnic(); };

private:
	// コマンドチャートのアドレス格納用のポインタ
	CCommandChart* m_pCommandChart[MAX_PLAYER_NUM];
};

#endif

// EOF