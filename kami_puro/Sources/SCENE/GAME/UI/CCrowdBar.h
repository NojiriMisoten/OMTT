//=============================================================================
//
// CStaminaBar�N���X [CStaminaBar.h]
// Author : �˖{�r�F
//
// �y�g�����z
// �o�[�̍��E�̒[�̍��W�ƃo�[�ϓ����ɂǂ�����Œ肷�邩��
// �ݒ肵�ăN���G�C�g����B
// �o�[�̕��i�����j��cpp�Œ萔��������B
//
// �X�^�~�i���g������UseStamina()�Ŏg���X�^�~�i�ʂ��Z�b�g
// �����񕜂�cpp�Œ萔�ݒ肵�Ă���
//
//=============================================================================
#ifndef _CCROWNDBAR_H_
#define _CCROWNDBAR_H_
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
class CCrowdBar : public CScene2D
{
public:

	CCrowdBar(LPDIRECT3DDEVICE9 *pDevice);
	~CCrowdBar(void);

	void Update();
	void Uninit();
	void DrawUI();

	// �쐬
	// ���[���W,�E�[���W,
	static CCrowdBar *Create(
		D3DXVECTOR2 pos,
		LPDIRECT3DDEVICE9 *pDevice);

	void Add(float value);

private:
	// ������
	void Init(D3DXVECTOR2 pos);

	D3DXVECTOR2 m_Pos;

	// ���݂̃o�[�̒l
	float m_Value;
	// ���݂̃o�[�̒l�ő�l
	float m_ValueMax;
	// �o�[�̒l�im_Value�j�P������̃s�N�Z����
	float m_WidthOneValue;
};

#endif
//----EOF----