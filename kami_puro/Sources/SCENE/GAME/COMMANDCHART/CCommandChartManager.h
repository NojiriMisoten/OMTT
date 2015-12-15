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
#include "../PLAYER/CPlayerManager.h"

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
	CCommandChartManager(CPlayerManager* pPlayerManager);

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
	static CCommandChartManager* Create(LPDIRECT3DDEVICE9* device, CPlayerManager* pPlayerManager);

	// コマンドチャートの取得
	// 引数：プレイヤーID
	// 戻り値：コマンドチャートの構成した技のタイプ
	COMMAND_TYPE GetCommandChartTechnic(int nID){ return m_pCommandChart[nID]->UseTechnic(); }

	// コマンド入力の変更
	// 引数：コマンド入力可能にするのならばtrue不可にするのならばfalse
	void SetInputCommandChart(bool isInput){ m_pCommandChart[PLAYER_1]->SetCommandInputFlag(isInput);
											 m_pCommandChart[PLAYER_2]->SetCommandInputFlag(isInput);}

	// モードをセット
	void SetCommandChartMode(int ID, CCommandChart::MODE_COMMAND_CHART mode);
	// モードのゲット
	CCommandChart::MODE_COMMAND_CHART GetCommandChartMode(int ID) { return m_pCommandChart[ID]->GetCommandChartMode(); };


	bool GetCanUseFinishSkill(PLAYER_ID id){ return m_pPlayerManager->GetUseFinishFlag(id); };
	bool GetCanUseRopeSkill(PLAYER_ID id){ return m_pPlayerManager->GetRopeFlag(id); };

	void SetInputCommandChart(int playerID, bool isInput) { m_pCommandChart[playerID]->SetCommandInputFlag(isInput); };

	//================================================================
	// 始動コマンドだけの状態になる
	// 技出した後バトルモード継続ならこっち
	//================================================================
	void ResetCommandList(int playerID);

	//================================================================
	// 位置やテクスチャなどの何回も呼べる初期化
	// MODE_APPEARにセットする前に呼んでほしい
	//================================================================
	void ResetAllCommand(int playerID);

	// ロープ時のオープンアニメ初期設定関数
	void StartOpenAnimeForRope(int playerID);
private:
	// コマンドチャートのアドレス格納用のポインタ
	CCommandChart* m_pCommandChart[MAX_PLAYER_NUM];

	CPlayerManager	*m_pPlayerManager;
};

#endif

// EOF