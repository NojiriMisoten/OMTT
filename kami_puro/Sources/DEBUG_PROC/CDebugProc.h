//=============================================================================
//
// デバッグ情報処理 [CDebugProc.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CDEBUGPROC_H_
#define _CDEBUGPROC_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../MAIN/main.h"

//*****************************************************************************
// クラス宣言
//*****************************************************************************
class CDebugProc
{
public:
	// コンストラクタ
	CDebugProc(void);

	// デストラクタ
	~CDebugProc(void);

	//==============================================
	// 初期化処理
	// 戻り値:成否
	//==============================================
	HRESULT Init(void);

	// 終了処理
	void Uninit(void);

	// 描画処理
	static void Draw(void);

	//=================================================
	// 登録処理
	// 引数:表示したい文字(%d等含む)、%d等に対応する変数
	//==================================================
	static void PrintL(const char *fmt, ...);
	static void PrintR( const char *fmt, ... );
	static void PrintU( const char *fmt, ... );
	static void PrintDL( const char *fmt, ... );
	static void PrintDR( const char *fmt, ... );

	//=================================================
	// 表示してるかのフラグセットット処理
	// 引数:フラグ
	//==================================================
	static void SetDrawDisp(bool bDisp){m_bDisp = bDisp;};

private:
	static const int LENGTH_STRING_BUFF = 1028; // 最大表示文字数

	static LPD3DXFONT m_pD3DXFONT;				// フォントへのポインタ
	static char m_aStrL[LENGTH_STRING_BUFF];	// デバッグ表示用の文字列のバッファ
	static char m_aStrR[LENGTH_STRING_BUFF];
	static char m_aStrU[LENGTH_STRING_BUFF];
	static char m_aStrDL[LENGTH_STRING_BUFF];
	static char m_aStrDR[LENGTH_STRING_BUFF];
	static bool m_bDisp;						// デバッグ表示ON/OFF
	static int	m_nCounter;						// 文字数カウンター
};

#endif
//----EOF----