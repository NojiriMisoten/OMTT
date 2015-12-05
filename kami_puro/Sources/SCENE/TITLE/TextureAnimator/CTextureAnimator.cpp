//=============================================================================
//
// �e�N�X�`���A�j���[�V�������� [CTextureAnimator.cpp]
// Author : ��{ �F��
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CTextureAnimator.h"

//=============================================================================
// �N���G�C�g
//=============================================================================
CTextureAnimator *CTextureAnimator::Create(int maxFrameX, int maxFrameY,CScene2D* myscene, int animationSpd, TEXTURE_ANIMATION_TYPE animType)
{
	// �쐬
	CTextureAnimator* p = new CTextureAnimator(maxFrameX, maxFrameY, myscene, animationSpd, animType);

	// ������
	p->Init();

	return p;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CTextureAnimator::Init(void)
{
	SetAnim(MaxFrameX, NowFrameX, MaxFrameY, NowFrameY, myScene);
	return S_OK; 
}

//=============================================================================
// �X�V����
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