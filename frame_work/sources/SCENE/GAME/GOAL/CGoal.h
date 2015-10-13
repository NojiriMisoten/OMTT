//=============================================================================
//
// CGoal�N���X [CGoal.h]
// Author : �˖{�@�r�F
//
// �󕨂������ɉ^��
//
//=============================================================================
#ifndef _CGOAL_H_
#define _CGOAL_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../../MAIN/main.h"
#include "../../CSCENE/CScene2D.h"

//=============================================================================
// �萔
//=============================================================================

enum GOAL_STATE{
	// �Ȃ�
	GOAL_STATE_NONE = 0,
	// �S�[���ɕ󔠎������v���C���������i1�t���[���̂ݒʂ�j
	GOAL_STATE_ARRIVE,
	// �]�����P�i�K��
	GOAL_STATE_TRANS_FIRST,
	// �]�����Q�i�K��
	GOAL_STATE_TRANS_SECOND,
	// �]�����R�i�K��
	GOAL_STATE_TRANS_THIRD,
	// �]���I��
	GOAL_STATE_TRANSED,
};

//*****************************************************************************
// �O����`
//*****************************************************************************
class CGame;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CGoal : public CScene2D
{
	// ���J�����o
public:

	CGoal(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_GOAL, OBJTYPE m_objType = OBJTYPE_GOAL);
	~CGoal(void);

	void Init(void);
	void Uninit(void);
	void Update(void);

	//=======================================================================
	// �N���G�C�g�֐�
	// ����:�f�o�C�X�A���W�A���A�����A�e�N�X�`���̎��
	// �߂�l:�쐬�����A�h���X
	//=======================================================================
	static CGoal *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos,
		short nPlayerNum,
		CGame *pGame);

	// �v���C���ԍ��̃Q�b�^�[
	short GetPlayerNum(){
		return m_nPlayerNum;
	}
	// �]�����ɃZ�b�g����
	void SetTrans();
	// �]��������Ȃ��ƃZ�b�g����
	void SetNoTrans();

	// ����J�����o
private:
	// �v���C���[�ԍ�
	short m_nPlayerNum;
	// �]���X�e�[�g
	GOAL_STATE m_State;
	// �]������J�E���g
	int m_nCount;
	// �]�����t���O
	bool m_bTransFlg;
	// �Q�[���C���X�^���X
	CGame *m_pGame;
};

#endif
//----EOF----