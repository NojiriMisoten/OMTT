//=============================================================================
//
// CStaminaBar�N���X [CStaminaBar.h]
// Author : �˖{�r�F
//
//=============================================================================
#ifndef _CSTAMINABAR_H_
#define _CSTAMINABAR_H_
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
class CStaminaBar : public CScene2D
{
public:
	// �o�[�̍��W���Œ肵�ē�������
	enum POSITIONBASE{
		// �E�����Œ�
		POSITIONBASE_RIGHT,
		// �������Œ�
		POSITIONBASE_LEFT,
	};

	CStaminaBar(LPDIRECT3DDEVICE9 *pDevice);
	~CStaminaBar(void);

	void Update();
	void Uninit();
	void DrawUI();

	// �쐬
	// ���[���W,�E�[���W,�o�[�̕ϓ����ɂǂ��瑤���Œ肷�邩
	static CStaminaBar *Create(
		D3DXVECTOR2 &leftPos,
		D3DXVECTOR2 &rightPos,
		POSITIONBASE positionBase,
		LPDIRECT3DDEVICE9 *pDevice);

	// �X�^�~�i���g��
	// �g���ʂ��Z�b�g���� �X�^�~�i������Ȃ������ꍇfalse
	bool UseStamina(float value);

private:
	// ������
	void Init(
		D3DXVECTOR2 &leftPos,
		D3DXVECTOR2 &rightPos,
		POSITIONBASE positionBase);

	// �X�^�~�i�̎�����
	void AutoHeal(float value);

	// �o�[�̒[�̍��W
	D3DXVECTOR2 m_PosLeft;
	D3DXVECTOR2 m_PosRight;
	// �o�[�̂ǂ�����Œ肵�ē�������
	POSITIONBASE m_PositionBase;
	// ���݂̃o�[�̒l�i�X�^�~�i�j
	float m_Value;
	// ���݂̃o�[�̒l�i�X�^�~�i�j�ő�l
	float m_ValueMax;
	// �X�^�~�i�̒l�im_Value�j�P������̃s�N�Z����
	float m_WidthOneValue;
};

#endif
//----EOF----