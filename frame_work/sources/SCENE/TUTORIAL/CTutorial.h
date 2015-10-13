//=============================================================================
//
// CTutorial�N���X [CTutorial.h]
// Author : �˖{�r�F
//
//=============================================================================
#ifndef _CTUTORIAL_H_
#define _CTUTORIAL_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../MAIN/main.h"
#include "../../PHASE/CPhase.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;
class CSceneAnime;

//*****************************************************************************
// �萔
//*****************************************************************************
const short TUTORIAL_PAGE_MAX = 4;
const short TUTORIAL_PICT_MAX = 3;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTutorial : public CPhase
{
public:
	// �R���X�g���N�^
	CTutorial(void);

	// �f�X�g���N�^
	~CTutorial(void);

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
	static CTutorial* Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);

private:

	// ���A�j���[�V�����X�V
	void UpdateArrow();

	// �w�i
	CScene2D* m_pBackGround;
	// �`���[�g���A���̊G
	CScene2D* m_p2D[TUTORIAL_PICT_MAX];
	// �G�̈ړ���
	D3DXVECTOR3 m_vVelo;
	// ���E�ǂ����ɓ����Ă�̃t���O
	bool m_bRight;
	// �y�[�W��
	short m_nPage;
	// ��ʂ̉��ɂ���G�̓Y�����ԍ�
	short m_nPictNumLeft;
	short m_nPictNumCenter;
	short m_nPictNumRight;
	// ���̃A�j���[�V�����@�{���̓e�N�X�`���A�j���[�V�����ł�낤���Ǝv�������ǂ���ς�߂�
	CSceneAnime *m_pArrowRight;
	CSceneAnime *m_pArrowLeft;
	// ���̊g��t���O
	bool m_bArrowScale;
	// ���g��܂ł̃J�E���g
	int m_nArrowCount;
	// �y�[�W�\���̊G
	CScene2D *m_pPage2D;
};

#endif
//----EOF----