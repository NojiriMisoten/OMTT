//=============================================================================
//
// テクスチャアニメーション処理 [CTextureAnimator.h]
// Author : 坂本 友希
//
//=============================================================================
#ifndef _CEFFECT_HOLDER_H_
#define _CEFFECT_HOLDER_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../MathAnimation/math_animation.h"
//*****************************************************************************
// 列挙体
//*****************************************************************************
// テクスチャの種類
typedef enum
{
	TEXTURE_ANIMATION_TYPE_LOOP,
	TEXTURE_ANIMATION_TYPE_NORML
}TEXTURE_ANIMATION_TYPE;

//*****************************************************************************
// クラス宣言
//*****************************************************************************
class CTextureAnimator
{
public:
	// コンストラクタ
	CTextureAnimator(int maxFrameX, int maxFrameY,CScene2D* myscene, int animationSpd, TEXTURE_ANIMATION_TYPE animType)
	{
		MaxFrameX = maxFrameX;
		MaxFrameY = maxFrameY;
		myScene = myscene;
		AnimationSpd = animationSpd;
		AnimType = animType;
		NowFrameX = 1;
		NowFrameY = 1;
		FrameCnt = 0;

	};

	// デストラクタ
	~CTextureAnimator(void){};

	void Update(void);

	// 終了
	void Uninit(void);

	void Play(void)
	{
		isPlay = true;
	}

	void Stop(void)
	{
		isPlay = false;
		NowFrameX=1;
		NowFrameY=1;
		FrameCnt = 0;
	}

	void Pause(void)
	{
		isPlay = true;
		FrameCnt = 0;
	}

	void SetAnimSpd(int value)
	{
		AnimationSpd = value;
	}

	//============================================================
	// クリエイト
	//============================================================
	static CTextureAnimator *Create(int maxFrameX, int maxFrameY,CScene2D* myscene, int animationSpd = 60, TEXTURE_ANIMATION_TYPE animType = TEXTURE_ANIMATION_TYPE_LOOP);

private:
	//============================================================
	// 初期化
	// 引数:デバイス
	// 戻り値:成否
	//============================================================
	HRESULT Init(void);

	int MaxFrameX;
	int MaxFrameY;

	int NowFrameX;
	int NowFrameY;

	int AnimationSpd;

	TEXTURE_ANIMATION_TYPE AnimType;

	CScene2D* myScene;

	bool isPlay;

	int FrameCnt;

};

#endif
//----EOF----