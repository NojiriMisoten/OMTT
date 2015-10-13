//=============================================================================
//
// CThreadBase�N���X [CThreadBase.h]
// Author : �˖{�@�r�F
//
// ���̊��N���X
//
//=============================================================================
#ifndef _CTHREADBASE_H_
#define _CTHREADBASE_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../CSCENE/CScene2D.h"
#include "../PLAYER/CPlayerManager.h"
#include "CThreadManager.h"
#include "../PLAYER/CPlayer.h"
#include "../../../MANAGER/CManager.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CThreadBase : public CScene2D
{
	// ���J�����o
public:
	CThreadBase(LPDIRECT3DDEVICE9 *pDevice, int priority, OBJTYPE type = OBJTYPE_THREAD);
	virtual ~CThreadBase(void);

	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	short GetPlayerNum(void){ return m_nPlayerNum; };
	void SetPlayerFacing(DIRECTION_PLAYER_FACING playerFacing){ m_PlayerFaicing = playerFacing; }

	// �U�����q�b�g�������ɌĂяo���֐�
	// �����@���̍U���ɓ��������v���C���[�̃v���C���[�ԍ�
	virtual void HitPlayer(CPlayer* pPlayer);

	// �p�������o
protected:
	// �������J�E���g����
	short m_nCount;
	// ����
	short m_nEndTime;
	// �����蔻������鎞��
	short m_nHitStartTime;
	short m_nHitEndTime;
	// ���̎��͒N�̃v���C���[�ԍ�
	short m_nPlayerNum;
	// �����蔻�������t���O
	bool m_bHitFlg;
	// ���̎��
	ThreadType m_ThreadType;

	// �v���C���[�������ˎ��Ɍ����Ă�������
	DIRECTION_PLAYER_FACING m_PlayerFaicing;
	D3DXVECTOR3 m_vPlayerDirection;


	// ����J�����o
private:
	
};

#endif
//----EOF----