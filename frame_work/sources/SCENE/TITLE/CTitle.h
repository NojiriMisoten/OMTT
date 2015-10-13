//=============================================================================
//
// CTitle�N���X [CTitle.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CTITLE_H_
#define _CTITLE_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../MAIN/main.h"
#include "../../PHASE/CPhase.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTitle : public CPhase
{
public:
	// �R���X�g���N�^
	CTitle(void);

	// �f�X�g���N�^
	~CTitle(void);

	//=================================================================
	// �������֐�
	// ����: �V�[���̃��[�h, �f�o�C�X
	//=================================================================
	void Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	//=================================================================
	// �N���G�C�g�֐�
	// ����: �V�[���̃��[�h, �f�o�C�X
	// �߂�l: ���̃V�[���̃A�h���X
	//=================================================================
	static CTitle* Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);

private:
	// PUSH START�����Ƃ��ɌĂ΂��
	void PushStart();

	// �K�v�ȊG
	CScene2D* m_pBackGround;
	CScene2D* m_pLogo;
	CScene2D* m_pPushStart;
	CScene2D* m_pMenuBack;
	CScene2D* m_pCursol;

	// ���݂̃J�[�\���̈ʒu
	int m_nCursol;
	// �^�C�g���V�[���J�n����PUSH START���o��܂ł̃J�E���g
	int m_nCount;
	// PUSHSTART�����������ǂ���
	bool m_bPushed;
	// PUSHSTART�_�ŃJ�E���g
	int m_nPushStartCount;
	// PUSHSTART�_�Ńt���O
	bool m_bPushStartFlg;
	// �Ղ�Ղ�J�E���g
	int m_nPuruCount;
};

#endif
//----EOF----