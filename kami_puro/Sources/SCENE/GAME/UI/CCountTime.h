//=============================================================================
//
// CStaminaBar�N���X [CStaminaBar.h]
// Author : �˖{�r�F
//
//=============================================================================
#ifndef _CCOUNTTIME_H_
#define _CCOUNTTIME_H_
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
class CCountTime
{
public:

	CCountTime(LPDIRECT3DDEVICE9 *pDevice);
	~CCountTime(void);

	void Update();
	void Uninit();
	void DrawUI();

	// �쐬
	// �J�E���g�̒��S���W
	static CCountTime* Create(
		D3DXVECTOR2 &pos, int time,
		LPDIRECT3DDEVICE9 *pDevice);

	// �J�n�A�j���[�V����������@������
	// �I������܂ł̃J�E���g(���t���[���A�j���[�V�������邩)
	void StartAnimation(int endCount);

private:
	// ������
	void Init(D3DXVECTOR2 &pos, int time);
	// ���Ԃ��J�E���g����X�V
	void UpdateTime();
	// �J�n�A�j���[�V����������X�V
	void UpdateAnime();
	// ���Ԃ���2D�̃e�N�X�`���ɔ��f������
	void Set(int time);

	// �^�C��
	int m_Time;
	// ���Ԃ�}�邽�߂̃J�E���g
	int m_TimeCount;
	// ��̈ʂ̌�
	CScene2D *m_pFigure1st;
	// �\�̈ʂ̌�
	CScene2D *m_pFigure2nd;

	// �J�n�A�j�������邽�߂̃J�E���g
	int m_AnimeCount;
	// �J�n�A�j���̏I���@�t���[����
	int m_AnimeCountMax;
	// �J�n�A�j�����Ă���t���O
	bool m_isAnime;
	// �J�n�A�j��1�t���[���ŕύX����A���t�@�l
	float m_AnimeOneFrameAlpha;
	D3DXCOLOR m_Anime2DColor;

	// �f�o�C�X
	LPDIRECT3DDEVICE9 *m_pD3DDevice;
};

#endif
//----EOF----