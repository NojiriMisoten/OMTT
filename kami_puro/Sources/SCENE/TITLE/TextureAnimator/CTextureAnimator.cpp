//=============================================================================
//
// テクスチャアニメーション処理 [CTextureAnimator.cpp]
// Author : 坂本 友希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CTextureAnimator.h"

//=============================================================================
// クリエイト
//=============================================================================
CTextureAnimator *CTextureAnimator::Create(int maxFrameX, int maxFrameY,CScene2D* myscene, int animationSpd, TEXTURE_ANIMATION_TYPE animType)
{
	// 作成
	CTextureAnimator* p = new CTextureAnimator(maxFrameX, maxFrameY, myscene, animationSpd, animType);

	// 初期化
	p->Init();

	return p;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CTextureAnimator::Init(void)
{
	SetAnim(MaxFrameX, NowFrameX, MaxFrameY, NowFrameY, myScene);
	return S_OK; 
}

//=============================================================================
// 更新処理
//=============================================================================
void CTextureAnimator::Update(void)
{
	if (isPlay)
	{
		FrameCnt++;
		if (FrameCnt >= AnimationSpd)
		{
			FrameCnt = 0;
			NowFrameX++;
			if (NowFrameX > MaxFrameX)
			{
				NowFrameX = 1;

				NowFrameY++;

				if (NowFrameY > MaxFrameY)
				{
					switch (AnimType)
					{
					case TEXTURE_ANIMATION_TYPE_NORML:
						Stop();
						return;
						break;
					case TEXTURE_ANIMATION_TYPE_LOOP:
						Stop();
						Play();
						return;
						break;
					}
				}
			}
			SetAnim(MaxFrameX,NowFrameX, MaxFrameY, NowFrameY, myScene);
		}
	}
}