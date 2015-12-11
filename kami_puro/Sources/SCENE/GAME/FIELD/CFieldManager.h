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
class CSceneX;
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

	// �`�悵�Ȃ�
	void SetImvisible(void);
	// �`�悷��
	void SetVisible(void);

	CRopeManager* GetRopeManager( void ) { return m_pRopeManager; };
private:
	// �f�o�C�X�̕ۑ�
	LPDIRECT3DDEVICE9 *m_pDevice;
	// �}�l�[�W���[
	CManager *m_pManager;

	// ���[�v�Ǘ�
	CRopeManager *m_pRopeManager;

	// �����O
	CSceneX *m_pRing;
	// �֎q
	CSceneX *m_pFrontChair;
	CSceneX *m_pOverChair;
	CSceneX *m_pLeftChair;
	CSceneX *m_pRightChair;
	// �X�e�[�W
	CSceneX *m_pStage;


};

#endif
//----EOF----