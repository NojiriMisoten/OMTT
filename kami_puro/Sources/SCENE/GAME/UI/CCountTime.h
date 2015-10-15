//=============================================================================
//
// CStaminaBar�N���X [CStaminaBar.h]
// Author : �˖{�r�F
//
// �V�[���͌p�����邯�ǎ��ۂɂ͂QD�������Ă������`��
// ���銴���I
// 
//=============================================================================
#ifndef _CCOUNTTIME_H_
#define _CCOUNTTIME_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../BASE_OBJECT/CScene2D.h"
//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CCountTime : public CScene2D
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
		D3DXVECTOR2 pos, int time,
		LPDIRECT3DDEVICE9 *pDevice);

private:
	// ������
	void Init(D3DXVECTOR2 pos, int time);

	// ���Ԃ���2D�̃e�N�X�`���ɔ��f������
	void Set(int time);

	// �^�C��
	int m_Time;

	// ���Ԃ�}�邽�߂̃J�E���g
	int m_TimeCount;

	// ��̈ʂ̌�
	CScene2D *m_Figure1st;
	// �\�̈ʂ̌�
	CScene2D *m_Figure2nd;

};

#endif
//----EOF----