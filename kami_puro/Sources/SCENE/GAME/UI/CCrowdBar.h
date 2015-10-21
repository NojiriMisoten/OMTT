//=============================================================================
//
// CStaminaBar�N���X [CStaminaBar.h]
// Author : �˖{�r�F
//
//
// �y�T�v�z
// ��o�[�𐶐����āA�|���S���̒��_�����E�Ɉړ������Ă���
// 
// �y�g�����z
// Add�Ɂ{�|�F�X�l������Ɗϋq�o�[�����E�Ɉړ�����
// 
// �y�ύX�ł��鐔�l�z
// �E�e�N�X�`��
// �E�o�[�̑��x�i�������x�A���̉����x�j
// �E�o�[�̐F�i�E��1P���A����2P���j
// 
//=============================================================================
#ifndef _CCROWNDBAR_H_
#define _CCROWNDBAR_H_
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
class CCrowdBar
{
public:

	CCrowdBar(LPDIRECT3DDEVICE9 *pDevice);
	~CCrowdBar(void);

	void Update();
	void Uninit();
	void DrawUI();

	// �쐬
	// �����AY���W�A���[�̍��W�A�E�[�̍��W�A�f�o�C�X
	static CCrowdBar *Create(
		float height,
		float posCenterY,
		float posLeft,
		float posRight,
		LPDIRECT3DDEVICE9 *pDevice);

	// �o�[�𑝌�������
	void Add(float value);

	// �J�n�A�j���[�V����������@������
	// �I������܂ł̃J�E���g(���t���[���A�j���[�V�������邩)
	void StartAnimation(int endCount);

private:
	// ������
	void Init(float height,
		float posCenterY,
		float posLeft,
		float posRight,
		LPDIRECT3DDEVICE9 *pDevice);

	void Init();

	// �o�[�𓮂����X�V
	void UpdateBarMove();

	// ���[�ƉE�[�̍��W�i�����Ȃ��j
	float m_PosLeftX;
	float m_PosRightX;
	// �^�񒆂̍��W�i�����Ȃ��j
	float m_PosCenterX;
	// �Z�b�g������W��Dest
	float m_PosDestX;
	// ���݂̋��ڂ̍��W
	float m_PosCurrentX;
	// �Z�b�g������W�̈ړ���
	float m_PosVel;
	// �Z�b�g������W������Ƃ��t���O
	bool m_isPosMove;

	// ���݂̃o�[�̒l
	float m_Value;
	// ���݂̃o�[�̒l�ő�l
	float m_ValueMax;
	// �o�[�̒l�im_Value�j�P������̃s�N�Z����
	float m_WidthOneValue;

	// ��̃o�[
	CScene2D *m_pBarLeft;
	CScene2D *m_pBarRight;

	// �f�o�C�X
	LPDIRECT3DDEVICE9 *m_pD3DDevice;

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
	float m_AnimeEasingTimer;

};

#endif
//----EOF----