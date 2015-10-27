//=============================================================================
//
//	CCommandChartUIクラスの宣言(CCommandChartUI.h)
//
//	Author : 佐藤　諒一
//
//=============================================================================
#ifndef __CCOMMANDCHARTUI_H__
#define __CCOMMNADCHARTUI_H__

//-----------------------------------------------------------------------------
//	インクルード
//-----------------------------------------------------------------------------
#include "../../../MAIN/main.h"
#include "../../../BASE_OBJECT/CScene2D.h"

//-----------------------------------------------------------------------------
//	マクロ定義
//-----------------------------------------------------------------------------
static const int MAX_BUTTON_INFO = 4;

// ボタンの種類
typedef enum
{
	BUTTON_TYPE_NONE = 0,
	BUTTON_TYPE_1,
	BUTTON_TYPE_2,
	BUTTON_TYPE_3,
	BUTTON_TYPE_4,
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
	void Init(BUTTON_TYPE ButtonType, D3DXVECTOR3 pos);

	// 更新
	void Update(void);

	// 終了
	void Uninit(void);

	// 描画
	void Draw(void);

	// 生成
	// 戻り値：これのポインタ
	// 引数　：デバイス、ボタンの種類、座標
	static CCommandChartUI* Create(LPDIRECT3DDEVICE9 *pDevice, BUTTON_TYPE ButtonType, D3DXVECTOR3 pos);

	// 入力判断フラグの状態を返す
	// 戻り値：入力されている(true)入力されていない(false)
	bool GetInputFlag(void){ return m_isInputCommand; }

	// 入力判断フラグの状態を変更する
	// 引数：入力されている(true)入力されていない(false)
	void SetInputFlag(bool isInputCommand){ m_isInputCommand = isInputCommand; }

	// 自分がどのボタンなのかを返す
	// 戻り値：自分の種類
	BUTTON_TYPE GetButtonType(void){ return m_ButtonType; }
	
private:
	// 自分がどのボタンか
	BUTTON_TYPE m_ButtonType;
	// 入力されたコマンドとして保持するかどうかのフラグ
	bool m_isInputCommand;
};

#endif

//EOF