//=============================================================================
//
// CLogoクラス [CLogo.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CLOGO_H_
#define _CLOGO_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../../BASE_OBJECT/CScene2D.h"
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CLogo : public CScene2D
{
public:
	//=======================================================================
	// コンストラクタ
	// 引数:デバイス、プライオリティ、オブジェタイプ
	//=======================================================================
	CLogo(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objType = OBJTYPE_2D);

	// デストラクタ
	~CLogo();

	//=======================================================================
	// 初期化関数
	// 引数:座標、幅、高さ、テクスチャの種類、（以下必要なし）（横分割数、縦分割数）
	// 戻り値:作成したアドレス
	//=======================================================================
	HRESULT Init(D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType, int wblock = 1, int hblock = 1);

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	// 描画
	void DrawUIRender(void);

	//=======================================================================
	// クリエイト関数
	// 引数:デバイス、座標、幅、高さ、テクスチャの種類
	// 戻り値:作成したアドレス
	//=======================================================================
	static CLogo* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType);

	void StartFadeIN(int maxFadeInFrame);
	void StartFadeOUT(int maxFadeInFrame);
	void SetGrayScalColor(D3DXVECTOR3 color) { m_GrayScal = color; };
private:
	void UpdateFade(void);
	
	float	m_Alpha;
	bool	m_isFade;
	float	m_FadeTime;
	float	m_FadeSpd;
	D3DXVECTOR3 m_GrayScal;
	bool	m_isFadeIN;
};
#endif
//----EOF----