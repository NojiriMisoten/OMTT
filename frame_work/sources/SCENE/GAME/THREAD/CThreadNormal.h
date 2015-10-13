//=============================================================================
//
// CThreadNormal�N���X [CThreadNoaml.h]
// Author : �˖{�@�r�F
//
// ���ʎ�
//
//=============================================================================
#ifndef _CTHREADNORMAL_H_
#define _CTHREADNORMAL_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../../RENDERER/CRenderer.h"
#include "CThreadBase.h"

//=============================================================================
// �O���錾
//=============================================================================
class CSceneAnime;

// ����
static const short THREAD_NORMAL_END_TIME = 30;
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CThreadNormal : public CThreadBase
{
	// ���J�����o
public:
	// �v���C�I���e�B�ƃI�u�W�F�^�C�v�̓e�X�g
	CThreadNormal(
		LPDIRECT3DDEVICE9 *pDevice,
		int priority = TYPE_PRIORITY_ATTACK,
		OBJTYPE type = OBJTYPE_THREAD);

	~CThreadNormal(void);

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
	static CThreadNormal *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		short nPlayerNum,
		D3DXVECTOR3 pos,
		D3DXVECTOR3 direction);

	// ����J�����o
private:
	CSceneAnime *m_pBulletAnime;
};

#endif
//----EOF----