//=============================================================================
//
// CCrowdManager�N���X [CCrowdManager.h]
// Author : �˖{�r�F
//
// �����O�̑O��ɂ���ϋq����
// �R�c�̔|��
//
//=============================================================================
#ifndef _CCROWDMANAGER_H_
#define _CCROWDMANAGER_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../RENDERER/CRenderer.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CManager;
class CCrowd;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CCrowdManager
{
public:
	CCrowdManager(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager);
	~CCrowdManager();

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �N���G�C�g
	static CCrowdManager* Create(
		LPDIRECT3DDEVICE9 *pDevice,
		CManager *pManager);

private:
	void Init();

	// �ۑ����Ƃ������|�C���^
	LPDIRECT3DDEVICE9 *m_pD3DDevice;
	CManager *m_pManager;

	// �ϋq�̑O��̔|��
	CCrowd *m_pCrowd3DFront;
	CCrowd *m_pCrowd3DBack;
};

#endif
//----EOF----