//=============================================================================
//
//	CCommandChartクラスの宣言(CCommandChart.h)
//
//	Author : 佐藤　諒一
//
//=============================================================================
#ifndef __CCOMMANDCHART_H__
#define __CCOMMNADCHART_H__

//-----------------------------------------------------------------------------
//	インクルード
//-----------------------------------------------------------------------------
#include "../../../MAIN/main.h"
#include "../../../INPUT/CInputKeyboard.h"

//-----------------------------------------------------------------------------
//	マクロ定義
//-----------------------------------------------------------------------------
// コマンドの最大入力数
static const int MAX_COMMAND_KEEP = 6;
static const int MAX_NEXT_COMMAND_VIEW = 4;

// プレイヤー番号の列挙
typedef enum
{
	NO_PLAYER =0,
	PLAYER_NUMBER_1,
	PLAYER_NUMBER_2,
	MAX_PLAYER_NUM
}PLAYER_NUMBER;

//-----------------------------------------------------------------------------
//	前方宣言
//-----------------------------------------------------------------------------
class CCommandChartUI;

//-----------------------------------------------------------------------------
//	クラス定義
//-----------------------------------------------------------------------------
class CCommandChart
{
public:
	// コンストラクタ
	// 引数：デバイス、プレイヤー番号
	CCommandChart(LPDIRECT3DDEVICE9* pDevice, PLAYER_NUMBER PlayerNumber);

	// デストラクタ
	~CCommandChart();

	// 初期化
	void Init(void);

	// 更新
	void Update(void);

	// 描画
	void Draw(void);

	// 終了
	void Uninit(void);

	// 自身の生成
	// 引数：デバイスのポインタ、自分のプレイヤー番号
	// 戻り値：自身のポインタ、プレイヤー番号
	static CCommandChart* Create(LPDIRECT3DDEVICE9* pDevice, PLAYER_NUMBER PlayerNumber);

	// 次に入力する候補のコマンドの表示
	// 引数：何個目のコマンド表記か
	void CreateNextCommand(int nNumCommand);

	// 次に表示するコマンドを削除
	void DethNextCommand(void);

private:
	// デバイスの保持
	LPDIRECT3DDEVICE9* m_pD3DDevice;
	// 保持中のコマンド数
	int m_nKeepCommandNum;
	// 表示する入力されたコマンドUIの保持
	CCommandChartUI* m_apCommandUI[MAX_COMMAND_KEEP];
	// 次入力候補のコマンドUIの保持
	CCommandChartUI* m_apNextCommandUI[MAX_NEXT_COMMAND_VIEW];
	// 入力後のUIを表示するx座標
	float m_fPosX;
	// 表示しているコマンドを消すまでのカウント
	int m_nCommandDethCnt;
	// コマンド入力可能かどうかのフラグ
	bool m_isCommandInput;
	// コマンド保持用配列
	KEYBOARD_CODE m_aCommandKeep[MAX_COMMAND_KEEP];
	// 自分のプレイヤー番号
	PLAYER_NUMBER m_MyNumber;
};

#endif

//EOF