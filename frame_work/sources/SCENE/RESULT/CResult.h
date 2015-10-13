//=============================================================================
//
// CResult�N���X [CResult.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CRESULT_H_
#define _CRESULT_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../MAIN/main.h"
#include "../../PHASE/CPhase.h"
#include "../../MANAGER/CManager.h"

//*****************************************************************************
// �萔
//*****************************************************************************
// ���j���[���
enum RESULT_MENU{
	RESULT_MENU_RETRY,
	RESULT_MENU_CHARA_SELECT,
	RESULT_MENU_STAGE_SELECT,
	RESULT_MENU_MAX
};

// ���邭�郉�C�g�̐�
static const int RESULT_CIRCLE_LIGHT_MAX = 100;

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;
class CSceneAnime;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CResult : public CPhase
{
public:
	// �R���X�g���N�^
	CResult(void);

	// �f�X�g���N�^
	~CResult(void);
	
	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	//======================================================
	// �N���G�C�g
	// ����: �t�F�[�Y���[�h�A�f�o�C�X
	// �߂�l: �������̃|�C���^
	//======================================================
	static CResult* Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);

	// ����J�����o
private:
	//======================================================
	// ������
	// ����: �t�F�[�Y���[�h�A�f�o�C�X
	//======================================================
	void Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);


	// BG�̏�����
	void InitializeBG();
	// �����̏��҃A�j���[�V����������
	void InitializeWinAnimation();
	// �����̏��҃A�j���[�V�����X�V
	void UpdateWinAnimation();

	// �w�i
	CScene2D* m_pBG;
	// ���S
	CScene2D* m_pLogo;
	// �v�b�V������
	CScene2D *m_pPush2D[PLAYER_MAX];
	// ���j���[����
	CScene2D *m_pMenuMoji[RESULT_MENU_MAX];
	// �I�����j���[
	CScene2D *m_pMenuBack;
	// �v���C���[�A�j���[�V����
	CSceneAnime *m_pPlayerAnime[PLAYER_MAX];
	// �������l�̔ԍ� ���������Ȃ�-1 �f�t�H���g��-1
	int m_nWinPlayerNum;
	// �v���C���[�A�j���[�V�����̃e�N�X�`���ԍ�
	int m_nTextureIndex[PLAYER_MAX];
	// �v���C���A�j���[�V�����̃e�N�X�`���ύX���邽�߂̃J�E���g
	int m_nTextureCount[PLAYER_MAX];
	// �A�j���[�V�������玩���őJ�ڂ���J�E���g
	int m_nCount;
	// �v�b�V��������
	bool m_bPush[PLAYER_MAX];
	// PUSH�̓_�ŃA�j���[�V���p�J�E���g
	int m_nPushFlashCount[PLAYER_MAX];
	// PUSH�̓_�ŃA�j���[�V���p�t���O
	bool m_bPushFlash[PLAYER_MAX];
	// ���j���[�̃J�[�\��
	int m_nMenuCursol;
	// ���j���[�����̊g��A�j���[�V�����̃T�C�Y
	D3DXVECTOR2 m_vMojiSize;
	// ���̃t���O
	bool m_bMojiScale;
	// CPU�̐�
	short m_nCpuNum;
	// ���҂ɓ��Ă郉�C�g
	CScene2D* m_pLight;
	// ���邭�郉�C�g
	CScene2D* m_pCircleLight[RESULT_CIRCLE_LIGHT_MAX];
	// ���邭��炢�Ƃ̈ړ���
	D3DXVECTOR2 m_vCircleVelo[RESULT_CIRCLE_LIGHT_MAX];
	// ���邭�郉�C�g�̑傫��
	float m_vCircleSizeHalf[RESULT_CIRCLE_LIGHT_MAX];

};

#endif
//----EOF----