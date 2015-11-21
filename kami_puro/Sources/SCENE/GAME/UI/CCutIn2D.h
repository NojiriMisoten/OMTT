//=============================================================================
//
// CCutIn2D2D�N���X [CCutIn2D.h]
// Author : �˖{�r�F
//
//=============================================================================
#ifndef _CCUTIN2D_H_
#define _CCUTIN2D_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../RENDERER/CRenderer.h"
#include "../../../BASE_OBJECT/CScene2D.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CCutIn2D : public CScene2D
{
public:

	CCutIn2D(LPDIRECT3DDEVICE9 *pDevice);
	~CCutIn2D(void);

	void Update();
	void Uninit();
	void DrawNormalRender();

	// �����AY���W�A���[�̍��W�A�E�[�̍��W�A�f�o�C�X
	static CCutIn2D *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3& pos,
		float width, float height, TEXTURE_TYPE texType);

	// ���������̒���
	bool AddWhite(float f);

	// �����̃��Z�b�g
	void InitWhite()
	{
		m_WhiteColor = 0;
	}

private:
	// ������
	void Init(D3DXVECTOR3& pos,
		float width, float height, TEXTURE_TYPE texType);

	// ��������
	float m_WhiteColor;
};

#endif
//----EOF----