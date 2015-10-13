//=============================================================================
//
// CThreadManager�N���X [CThreadManager.h]
// Author : �˖{�@�r�F
//
// ���}�l�[�W���[
//
//=============================================================================
#ifndef _CTHREADMANAGER_H_
#define _CTHREADMANAGER_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../../RENDERER/CRenderer.h"
#include "../PLAYER/CPlayer.h"

//=============================================================================
// �萔
//=============================================================================
enum ThreadType{
	THREAD_TYPE_NORMAL,
	THREAD_TYPE_ATTACK,		// �U�������`�Ԃ̎�
	THREAD_TYPE_SPEED,		// �ړ������`�Ԃ̎�
	THREAD_TYPE_TRAP,		// 㩓����`�Ԃ̎�
	THREAD_TYPE_MAX
};

//=============================================================================
// �O���錾
//=============================================================================
class CThread;
class CEffectManager;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CThreadManager
{
	// ���J�����o
public:
	CThreadManager(LPDIRECT3DDEVICE9 *pDevice);
	~CThreadManager(void);

	void Init(void);
	void Uninit(void);
	void Update(void);

	// �������֐�(���^�C�v,�v���C���ԍ�,���W)
	void CreateThread(ThreadType type, int nPlayerNum, D3DXVECTOR3 pos, DIRECTION_PLAYER_FACING playerFacing, CEffectManager *pEffectManager);

	// ����J�����o
private:
	LPDIRECT3DDEVICE9 *m_pDevice;
};

#endif
//----EOF----