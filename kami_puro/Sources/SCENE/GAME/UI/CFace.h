//=============================================================================
//
// CFace�N���X [CFace.h]
// Author : �˖{�r�F
//
//=============================================================================
#ifndef _CFACE_H_
#define _CFACE_H_
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
class CFace : public CScene2D
{
public:

	CFace(LPDIRECT3DDEVICE9 *pDevice);
	~CFace(void);

	void Update();
	void Uninit();
	void DrawUI();

	// �쐬
	static CFace *Create(
		D3DXVECTOR2 posLeft,
		D3DXVECTOR2 sizeLeft,
		D3DXVECTOR2 posRight,
		D3DXVECTOR2 sizeRight,
		LPDIRECT3DDEVICE9 *pDevice);

private:
	// ������
	void Init(
		D3DXVECTOR2 posLeft,
		D3DXVECTOR2 sizeLeft,
		D3DXVECTOR2 posRight,
		D3DXVECTOR2 sizeRight);

	// ���W
	D3DXVECTOR2 m_PosLeft;
	D3DXVECTOR2 m_PosRight;

	// �����̊�
	CScene2D *m_FaceLeft;
	// �����̊�̘g
	CScene2D *m_BackLeft;
	// �E���̊�
	CScene2D *m_FaceRight;
	// �E���̊�̘g
	CScene2D *m_BackRight;


};

#endif
//----EOF----