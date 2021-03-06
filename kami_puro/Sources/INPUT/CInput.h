//=============================================================================
//
// CInputクラス [CInput.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CINPUT_H_
#define _CINPUT_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../MAIN/main.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CInput
{
public:
	// コンストラクタ
	CInput(void);

	// デストラクタ
	virtual ~CInput(void);

	//===================================================
	// 初期化
	// 引数: インスタンス、ウィンドウハンドル
	// 戻り値: 成否
	//===================================================
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);

	// 終了
	virtual void Uninit(void);

	// 更新
	virtual void Update(void) = 0;

protected:
	static LPDIRECTINPUT8 m_pDInput;	// ダイレクトインプットのポインタ
	LPDIRECTINPUTDEVICE8 m_pDIDevice;	// デバイス
};

#endif
//---------EOF--------