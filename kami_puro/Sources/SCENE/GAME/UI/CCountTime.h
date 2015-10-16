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

private:
	// ������
	void Init(D3DXVECTOR2 &pos, int time);

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

	// �f�o�C�X
	LPDIRECT3DDEVICE9 *m_pD3DDevice;
};

#endif
//----EOF----