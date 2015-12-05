//=============================================================================
//
//	CCommandChartUIクラスの宣言(CCommandChartUI.h)
//
//	Author : 佐藤　諒一
//
//=============================================================================
#ifndef __CCOMMANDCHARTUI_H__
#define __CCOMMANDCHARTUI_H__

//-----------------------------------------------------------------------------
//	インクルード
//-----------------------------------------------------------------------------
#include "../../../MAIN/main.h"
#include "../../../BASE_OBJECT/CScene2D.h"

//-----------------------------------------------------------------------------
//	マクロ定義
//-----------------------------------------------------------------------------
//static const int MAX_BUTTON_INFO = 4;
static const float COMMAND_POLYGON_WIDTH = 30.0f * (SCREEN_HEIGHT * 0.5f / 150.f);		// コマンドのポリゴンの横幅
static const float COMMAND_POLYGON_HEIGHT = 30.0f * (SCREEN_HEIGHT * 0.5f / 150.f);		// コマンドのポリゴンの高さ

// ボタンの種類
typedef enum
{
	BUTTON_TYPE_NONE = 0,
	BUTTON_TYPE_1,		// 右上
	BUTTON_TYPE_2,		// 右下
	BUTTON_TYPE_3,		// 左上
	BUTTON_TYPE_4,		// 左下
	BUTTON_TYPE_5,		// 左右上方向同時押し
	BUTTON_TYPE_6,		// 左右下方向同時押し
	BUTTON_TYPE_MAX
}BUTTON_TYPE;

//-----------------------------------------------------------------------------
//	クラス定義
//-----------------------------------------------------------------------------
class CCommandChartUI : public CScene2D
{
public:
	// コンストラクタ
	// 引数：デバイス
	CCommandChartUI(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objType = OBJTYPE_2D);

	// デストラクタ
	~CCommandChartUI();

	// 初期化
	// 引数：ボタンのタイプ、座標
	void Init(BUTTON_TYPE ButtonType, D3DXVECTOR3 pos, TEXTURE_TYPE Texture);

	// 更新
	void Update(void);

	// 終了
	void Uninit(void);

	// 描画
	void Draw(void);

	// 生成
	// 戻り値：これのポインタ
	// 引数　：デバイス、ボタンの種類、座標
	static CCommandChartUI* Create(LPDIRECT3DDEVICE9 *pDevice, BUTTON_TYPE ButtonType, D3DXVECTOR3 pos, TEXTURE_TYPE Texture);

	// 入力判断フラグの状態を返す
	// 戻り値：入力されている(true)入力されていない(false)
	bool GetInputFlag(void){ return m_isInputCommand; }

	// 入力判断フラグの状態を変更する
	// 引数：入力されている(true)入力されていない(false)
	void SetInputFlag(bool isInputCommand){ m_isInputCommand = isInputCommand; }

	// 自分がどのボタンなのかを返す
	// 戻り値：自分の種類
	BUTTON_TYPE GetButtonType(void){ return m_ButtonType; }

	// 移動させるための目標座標の設定
	// 引数：目標の座標
	void SetDestPos(D3DXVECTOR3 DestPos){ m_DestPos = DestPos; m_isMoveX = true; m_isMoveY = true; }

	// 移動用関数
	void Move(void);

	// 入力された時のコマンドのボタンのテクスチャをずらす
	// 引数：ボタンの種類, 押されてるやつか
	void InputUIUVChange(BUTTON_TYPE ButtonType, bool isPush);

	// コマンド入力終了の時の処理
	void EndOfInputCommand(void);

	// コマンド入力開始時の処理
	// 引数：座標
	void RestartOfInputCommand(D3DXVECTOR3 pos);

	//********************************************
	//    2015/12/4
	//    佐藤　追記
	//********************************************
	// コマンドが入力候補だったら後ろにポリゴン描画
	void CandidateInputBackPolygonDraw(void){ m_pBackPolygon->SetDrawFlag(true); }

	// コマンドが入力候補で無くなったら後ろのポリゴンを消す
	void CandidateInputBackPolygonVanish(void){ m_pBackPolygon->SetDrawFlag(false); }
	//********************************************
	//    2015/12/4
	//    佐藤　追記
	//********************************************


private:
	// 自分がどのボタンか
	BUTTON_TYPE m_ButtonType;
	// 最終的な表示座標
	D3DXVECTOR3 m_DestPos;
	// 色
	D3DXCOLOR m_Color;
	// 入力されたコマンドとして保持するかどうかのフラグ
	bool m_isInputCommand;
	// x方向の移動を行うかどうかのフラグ
	bool m_isMoveX;
	// y方向の移動を行うかどうかのフラグ
	bool m_isMoveY;

	//********************************************
	//    2015/12/4
	//    佐藤　追記
	//********************************************
	// コマンドの背景に描画するポリゴン
	CScene2D* m_pBackPolygon;
	//********************************************
	//    2015/12/4
	//    佐藤　追記
	//********************************************

};

#endif

//EOF