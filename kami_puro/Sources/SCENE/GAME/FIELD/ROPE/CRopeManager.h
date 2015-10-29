//=============================================================================
//
// CRopeManager�N���X [CRopeManager.h]
// Author : �˖{�r�F
//
//=============================================================================
#ifndef _CROPEMANAGER_H_
#define _CROPEMANAGER_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../RENDERER/CRenderer.h"


//*****************************************************************************
// �O���錾
//*****************************************************************************
class CRope;
class CManager;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CRopeManager
{
public:
	CRopeManager(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager);
	~CRopeManager(void);

	void Init();
	void Update();
	void Uninit();

	// �S���A�j���\�V�����X�^�[�g
	// 1�t���[���ň�������́A���t���[���������邩
	void Pull(float pullPower, int pullInterval);


private:

	// �f�o�C�X
	LPDIRECT3DDEVICE9 *m_pD3DDevice;
	// �}�l�[�W���[
	CManager *m_pManager;

	// ���[�v
	CRope *m_pRope;
};

#endif
//----EOF----