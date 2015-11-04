//=============================================================================
//
// CFieldManager�N���X [CFieldManager.h]
// Author : �˖{�r�F
//
//=============================================================================
#ifndef _CFIELDMANAGER_H_
#define _CFIELDMANAGER_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../MAIN/main.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CManager;
class CRopeManager;
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CFieldManager
{
public:
	CFieldManager(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager);
	~CFieldManager(void);

	// �쐬
	static CFieldManager *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		CManager *pManager);

	void Init();
	void Uninit();
	void Update();

private:
	// �f�o�C�X�̕ۑ�
	LPDIRECT3DDEVICE9 *m_pDevice;
	// �}�l�[�W���[
	CManager *m_pManager;

	// ���[�v�Ǘ�
	CRopeManager *m_pRopeManger;
};

#endif
//----EOF----