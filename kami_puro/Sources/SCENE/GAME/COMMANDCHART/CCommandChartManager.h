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

//-----------------------------------------------------------------------------
//	前方宣言
//-----------------------------------------------------------------------------
class CCommandChart;

//-----------------------------------------------------------------------------
//	クラス定義
//-----------------------------------------------------------------------------
class CCommandChartManger
{
public:
	// コンストラクタ
	CCommandChartManger();

	// デストラクタ
	~CCommandChartManger();

	// 初期化
	// 引数：デバイス
	static void Init(LPDIRECT3DDEVICE9* device);

	// 更新
	static void Update(void);

	// 描画
	static void Draw(void);

	// 終了
	static void Uninit(void);

	// 生成
	// 引数：デバイス
	static void Create(LPDIRECT3DDEVICE9* device);

private:
	// コマンドチャートのアドレス格納用のポインタ
	static CCommandChart* m_pCommandChart;
};

#endif

// EOF