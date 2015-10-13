//=============================================================================
//
// アニメーションクラス [CAnimation.h]
// Author : 塚本　俊彦
//
// 数字のカウントダウンに使ってる。
//
//=============================================================================
#ifndef _CANIMATION_H_
#define _CANIMATION_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../../MAIN/main.h"
#include "../../CSCENE/CScene.h"
#include "../../CSCENE/CScene2D.h"

//*****************************************************************************
// 定数
//*****************************************************************************
// 重力加速度
static const float GRAVITY_SCALE = 2.0f;
// 床に当たった時の反射量
static const float REFLECT_SCALE = 0.34f;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CAnimation : public CScene2D
{
public:
	//======================================================
	// コンストラクタ
	// 引数: デバイス、プライオリティ
	//======================================================
	CAnimation(LPDIRECT3DDEVICE9 *pDevice, int nPriority, OBJTYPE m_objType = OBJTYPE_2D);

	// デストラクタ
	~CAnimation();

	//======================================================
	// 初期化
	// 引数: 座標、幅、高さ、テクスチャータイプ、テクスチャ横分割数、テクスチャ縦分割数
	// 戻り値: 成否
	//======================================================
	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int sepalateX,int sepalateY);
	
	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	// 描画
	void Draw(void);
	
	//======================================================
	// アニメーション番号設定
	// 引数: アニメーション番号,反転するか
	//======================================================
	void SetIndex(int nIdx);
	void SetIndex(int nIdx, bool reverse);

	//======================================================
	// 描画するか
	// 引数: フラグ
	//======================================================
	void SetDrawFlag(bool flag){m_bDraw = flag;};

	// 落ちるセット
	void SetFall(D3DXVECTOR3 pos, float fFloorHeight);
	// 落ないセット
	void SetNonFall(D3DXVECTOR3 pos);
	// フェードアウトセット
	void SetFadeOut(float alpha, float fFadeSpeed);
	// フェードアウトしないセット
	void SetNonFadeOut();
	// フェードインセット
	void SetFadeIn(float alpha, float fFadeSpeed);
	// フェードインしないセット
	void SetNonFadeIn();


	//======================================================
	// クリエイト関数
	// 引数: デバイス、座標、幅、高さ、テクスチャータイプ、テクスチャ横分割数、テクスチャ縦分割数
	// 戻り値: 作ったやつのアドレス
	//======================================================
	static CAnimation* Create(LPDIRECT3DDEVICE9 *pDevice, int nPriority, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int sepalateX, int sepalateY);

protected:
	// ループなし更新
	void Update_Normal(void);

	// ループあり更新
	void Update_Loop(void);
	
	LPDIRECT3DDEVICE9	*m_pD3DDevice;			// デバイスオブジェクト(描画に必要)
	float				m_fLeft;				// UV値(左)
	float				m_fRight;				// UV値(右)
	float				m_fTop;					// UV値(上)
	float				m_fBottom;				// UV値(下)
	int					m_nWidth;				// 横幅
	int					m_nHeight;				// 縦幅
	int					m_nSepalateNumX;		// 画像分割数(横)・・・ex:横に５個並んでるなら５
	int					m_nSepalateNumY;		// 画像分割数(縦)
	int					m_nIdx;					// 分割番号
	int					m_nMaxIdx;				// idxの最大値
	bool				m_bDraw;				// 描画するか

	// 落ちるアニメ―ション　デフォルトOFF
	bool				m_bFall;				// 落下るフラグ
	float				m_fGravity;				// 重力
	float				m_fFloor;				// 落ちる時の床の位置
	// フェードアニメ―ション　デフォルトOFF
	bool				m_bFadeOut;				// フェードアウトするフラグ
	bool				m_bFadeIn;				// フェードインするフラグ
	float				m_fAlpha;				// 2Dのアルファ値
	float				m_fFadeSpeed;			// フェードするスピード
};
#endif
//----EOF----