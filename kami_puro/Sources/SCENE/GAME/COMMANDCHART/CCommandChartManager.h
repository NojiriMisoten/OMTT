//=============================================================================
//
//	CCommandChartManagerクラスの宣言(CCommandChartManager.h)
//
//	Author : 佐藤　諒一
//
//=============================================================================
#ifndef __CCOMMANDCHARTMANAGER_H__
#define __CCOMMANDCHARTMANAGER_H__

//-----------------------------------------------------------------------------
// インクルード
//-----------------------------------------------------------------------------
#include "../../../MAIN/main.h"
#include "CCommandChart.h"


//-----------------------------------------------------------------------------
//	マクロ
//-----------------------------------------------------------------------------
static const int MAX_PLAYER_NUM = 2;

//-----------------------------------------------------------------------------
//	前方宣言
//-----------------------------------------------------------------------------

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

	// コマンドチャートの取得
	// 引数：プレイヤーID
	// 戻り値：コマンドチャートの構成した技のタイプ
	COMMAND_TYPE GetCommandChartTechnic(int nID){ return m_pCommandChart[nID]->UseTechnic(); }

	// コマンド入力の変更
	// 引数：コマンド入力可能にするのならばtrue不可にするのならばfalse
	void SetInputCommandChart(bool isInput){ m_pCommandChart[MY_ID_1]->SetCommandInputFlag(isInput);
											 m_pCommandChart[MY_ID_2]->SetCommandInputFlag(isInput);}

	// コマンドチャートのリセット
	void ResetCommandChart(void){ //m_pCommandChart[MY_ID_1]->ResetCommand();
								  //m_pCommandChart[MY_ID_2]->ResetCommand();
								}

private:
	// コマンドチャートのアドレス格納用のポインタ
	CCommandChart* m_pCommandChart[MAX_PLAYER_NUM];
};

#endif

// EOF