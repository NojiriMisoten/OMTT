//=============================================================================
//
// CThreadSpecialAttack�N���X [CThreadSpecialAttack.h]
// Author : �˖{�r�F
//
//=============================================================================
#ifndef _CTHREAD_SPECIAL_TRAP_H_
#define _CTHREAD_SPECIAL_TRAP_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../../RENDERER/CRenderer.h"
#include "CThreadBase.h"

//=============================================================================
// �O���錾
//=============================================================================
class CSceneAnime;

// �����蔻��̎n�܂鎞��
static const short THREAD_TRAP_HIT_START_TIME = 30;
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CThreadSpecialTrap : public CThreadBase
{
	// ���J�����o
public:
	// �v���C�I���e�B�ƃI�u�W�F�^�C�v�̓e�X�g
	CThreadSpecialTrap(
		LPDIRECT3DDEVICE9 *pDevice,
		int priority = TYPE_PRIORITY_ATTACK,
		OBJTYPE type = OBJTYPE_THREAD);

	~CThreadSpecialTrap(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �v���C���ɓ����������ɌĂ΂��֐�
	void HitPlayer(CPlayer* pPlayer);

	//=======================================================================
	// �N���G�C�g�֐�
	// ����:�f�o�C�X�A�v���C���ԍ��A���W
	// �߂�l:�쐬�����A�h���X
	//=======================================================================
	static CThreadSpecialTrap *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		short nPlayerNum,
		D3DXVECTOR3 pos);

	// ����J�����o
private:
	CSceneAnime *m_pBulletAnime;

};

#endif
//----EOF----