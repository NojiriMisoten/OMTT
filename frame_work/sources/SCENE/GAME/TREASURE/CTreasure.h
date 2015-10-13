//=============================================================================
//
// CTreasure�N���X [CTreasure.h]
// Author : �˖{�@�r�F
//
// �D�������󕨁i���j
//
//=============================================================================
#ifndef _CTREASURE_H_
#define _CTREASURE_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../../MAIN/main.h"
#include "../../CSCENE/CSceneAnime.h"

//=============================================================================
// �萔
//=============================================================================
enum TreasureState{
	TREASURE_STATE_OWNER_NONE,
	TREASURE_STATE_OWNED,
	TREASURE_STATE_JUMPING,
	TREASURE_STATE_MAX,
};

//=============================================================================
// �O���錾
//=============================================================================
class CSceneAnime;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTreasure : public CSceneAnime
{
	// ���J�����o
public:

	CTreasure(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_TRESURE, OBJTYPE m_objType = OBJTYPE_TREASURE);
	~CTreasure(void);

	void Uninit(void);
	void Update(void);

	//=======================================================================
	// �N���G�C�g�֐�
	// ����:�f�o�C�X�A���W�A���A�����A�e�N�X�`���̎��
	// �߂�l:�쐬�����A�h���X
	//=======================================================================
	static CTreasure *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos);

	// �v���C�����󕨂𗎂Ƃ��Ƃ��̏���
	// �e�N�X�`���Ƒ傫�������Z�b�g����
	void Reset(D3DXVECTOR3 pos);

	// �v���C�����󕨂��E�����Ƃ�
	// �e�N�X�`���Ƒ傫�����Z�b�g����
	void SetIcon();

	// �󔠂������Ă���v���C���[��ID���Z�b�g
	void SetOwnerPlayerNumber(short sPlayerNumber){ 
		m_sOwnerPlayer = sPlayerNumber; 
		m_TreasureState = TREASURE_STATE_OWNED;
	}

	// �󔠂������Ă���v���C���[��ID���Q�b�g
	short GetOwnerPlayerNumber(void){ return m_sOwnerPlayer; }

	// �󔠂̏�Ԃ��擾
	TreasureState GetTreasureState(void){ return m_TreasureState; }

	// �󔠂̏�Ԃ��Z�b�g
	void SetTreasureState(TreasureState State){ m_TreasureState = State; }

	// �󕨂̏ꏊ���Z�b�g �G�t�F�N�g�Ƃ����ꏏ�ɃZ�b�g����
	void SetPos(D3DXVECTOR3 pos);

	// �󕨂𗎂Ƃ�����
	void SetFall();

	// ����J�����o
private:

	// ������
	void Init(D3DXVECTOR3 pos);

	// ����̏��
	TreasureState m_TreasureState;
	short m_sOwnerPlayer;

	// �G�t�F�N�g
	CSceneAnime *m_pKira;
	CSceneAnime *m_pLight;

	// �A�j���[�V�����p�ړ���
	D3DXVECTOR3 m_vVelocity;
	// jump���n�߂Ă���̃J�E���g
	int m_nJumpCount;

};

#endif
//----EOF----