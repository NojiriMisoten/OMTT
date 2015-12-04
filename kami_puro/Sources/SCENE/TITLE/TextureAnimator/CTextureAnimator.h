//=============================================================================
//
// �e�N�X�`���A�j���[�V�������� [CTextureAnimator.h]
// Author : ��{ �F��
//
//=============================================================================
#ifndef _CEFFECT_HOLDER_H_
#define _CEFFECT_HOLDER_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../MathAnimation/math_animation.h"
//*****************************************************************************
// �񋓑�
//*****************************************************************************
// �e�N�X�`���̎��
typedef enum
{
	TEXTURE_ANIMATION_TYPE_LOOP,
	TEXTURE_ANIMATION_TYPE_NORML
}TEXTURE_ANIMATION_TYPE;

//*****************************************************************************
// �N���X�錾
//*****************************************************************************
class CTextureAnimator
{
public:
	// �R���X�g���N�^
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

	// �f�X�g���N�^
	~CTextureAnimator(void){};

	void Update(void);

	// �I��
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
	// �N���G�C�g
	//============================================================
	static CTextureAnimator *Create(int maxFrameX, int maxFrameY,CScene2D* myscene, int animationSpd = 60, TEXTURE_ANIMATION_TYPE animType = TEXTURE_ANIMATION_TYPE_LOOP);

private:
	//============================================================
	// ������
	// ����:�f�o�C�X
	// �߂�l:����
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