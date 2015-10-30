//=============================================================================
//
// CHpBar�N���X [CHpBar.h]
// Author : �˖{�r�F
//
// �y�T�v�z
// �����N���X��CBarBase���Ă̂������āA����łS�̃o�[�𐶐��B
// ���E�ɂ̗�/�Ԃ̃o�[�������Ă���B
// 
// �y�g�����z
// �����̃o�[�i1P��HP�j�𑝌�����Ȃ灨AddLeft�ASubLeft
// �E���̃o�[�i2P��HP�j�𑝌�����Ȃ灨AddRight�ASubRight
// ���ĂԁB�����͂ǂ��������̐���
// �v���X�ƃ}�C�i�X�ł��낢��ς�邩�番�����B���匾��Ȃ��ŁB
//
// �y�ύX�ł��鐔�l�z
// �E�e�N�X�`��
// �E�Ԃ��o�[�����炷�܂ł̊Ԋu
// �E�o�[�̑��x
// �E�o�[�̐F�i�����A�݂ǂ�j
//
//=============================================================================
#ifndef _CHPBAR_H_
#define _CHPBAR_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../RENDERER/CRenderer.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CHpBar
{
public:
	CHpBar(LPDIRECT3DDEVICE9 *pDevice);
	~CHpBar(void);

	void Update();
	void Uninit();
	void DrawUI();

	// �쐬
	// Y���W�A�����o�[�̍��E��X���W�A�E���o�[�̍��EX���W
	static CHpBar *Create(
		float height,
		float posCenterY,
		float posLeftBarLeftX,
		float posLeftBarRightX,
		float posRightBarLeftX,
		float posRightBarRightX,
		LPDIRECT3DDEVICE9 *pDevice);

	// ���T�C�h��HP�������̒l�ŉ��Z
	void AddLeft(float value);
	// ���T�C�h��HP�������̒l�Ō��Z
	void SubLeft(float value);
	// �E�T�C�h��HP�������̒l�ŉ��Z
	void AddRight(float value);
	// �E�T�C�h��HP�������̒l�Ō��Z
	void SubRight(float value);

	// �J�n�A�j���[�V����������@������
	// �I������܂ł̃J�E���g(���t���[���A�j���[�V�������邩)
	void StartAnimation(int endCount);

private:
	enum BarInfo{
		BAR_RED_L,
		BAR_RED_R,
		BAR_GREEN_L,
		BAR_GREEN_R,
		BAR_MAX
	};

	// ����łS�̃o�[�����
	class CBarBase{
	public:
		// ���݂̒l
		float m_Value;
		// �����̍��W
		float m_PosLeft;
		// �E���̍��W
		float m_PosRight;
		// ��Ԃ���ۂ�start�l
		float m_PosEasingStart;
		// ��Ԃ���ۂ�end�l
		float m_PosEasingEnd;
		// ��Ԃ���Ƃ��Ɏg���^�C�}�i0~1�j
		float m_TimerEasing;
		// �o�[��\������|���S��
		CScene2D *m_p2D;
	};

	// ������
	void Init(
		float height,
		float posCenterY,
		float posLeftBarLeftX,
		float posLeftBarRightX,
		float posRightBarLeftX,
		float posRightBarRightX);

	// �e�l�̏������@�J�n�A�j���V�����̌�ŌĂ�
	void Init();

	// �o�[�̃C���X�^���X
	CBarBase m_pBar[BAR_MAX];

	// ���݂̃o�[�̒l�ő�l
	float m_ValueMax;
	// 1value������̃s�N�Z��
	float m_WidthOneValue;
	// �_���[�W���󂯂Ă���̃J�E���g�@�Ԃ��o�[�Ɏg��
	int m_RedResetCountLeft;
	int m_RedResetCountRight;
	// �Ԃ��o�[��ύX���邩�t���O
	bool m_isRedResetLeft;
	bool m_isRedResetRight;
	// �Ԃ��o�[�̐��`��Ԃ�����t���O �X�V�𕪂���������
	bool m_isRedEasingLeft;
	bool m_isRedEasingRight;

	// �g
	CScene2D *m_pFrameLeft;
	CScene2D *m_pFrameRight;

	//-------------------------------------
	// �A�j���[�V�����p
	void UpdateAnime();
	// �J�n�A�j�������邽�߂̃J�E���g
	int m_AnimeCount;
	// �J�n�A�j���̏I���@�t���[����
	int m_AnimeCountMax;
	// �J�n�A�j�����Ă���t���O
	bool m_isAnime;
	// �J�n�A�j���[�V�����p�̕ۊǃ^�C��
	float m_AnimeEasingOneFrame;

	// �J�n�A�j���[�V�������̕�Ԃ̃^�C�}
	float m_AnimeTimerEasing;

	// �f�o�C�X
	LPDIRECT3DDEVICE9 *m_pD3DDevice;
};

#endif
//----EOF----