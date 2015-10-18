//=============================================================================
//
// CStaminaBar�N���X [CStaminaBar.h]
// Author : �˖{�r�F
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
	static CCrowdBar *Create(
		D3DXVECTOR2 &pos,
		float height,
		LPDIRECT3DDEVICE9 *pDevice);

	void Add(float value);

	// �o�[�̒��_��^�񒆂ɏW�߂�
	void Reset();

	// �o�[�̒��_��[�����ɖ߂�
	void Replace(float ratio);

private:
	// ������
	void Init(D3DXVECTOR2 &pos, float height);

	D3DXVECTOR2 m_Pos;

	// ���݂̃o�[�̒l
	float m_Value;
	// ���݂̃o�[�̒l�ő�l
	float m_ValueMax;
	// �o�[���A�j���[�V�����Ńx�[�X���_��ς��鎞�p�̒l
	float mValueBase;
	// �o�[�̒l�im_Value�j�P������̃s�N�Z����
	float m_WidthOneValue;

	// ��̃o�[
	CScene2D *m_pBarLeft;
	CScene2D *m_pBarRight;
};

#endif
//----EOF----