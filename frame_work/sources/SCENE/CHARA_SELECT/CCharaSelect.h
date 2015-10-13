//=============================================================================
//
// CCharaSelect�N���X [CCharaSelect.h]
// Author : �˖{�r�F
//
//=============================================================================
#ifndef _CCHARA_SELECT_H_
#define _CCHARA_SELECT_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../MAIN/main.h"
#include "../../PHASE/CPhase.h"
#include "../../MANAGER/CManager.h"
#include "../GAME/PLAYER/CPlayer.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;
class CSceneAnime;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CCharaSelect : public CPhase
{
public:
	// �R���X�g���N�^
	CCharaSelect(void);

	// �f�X�g���N�^
	~CCharaSelect(void);

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
	static CCharaSelect* Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);

private:
	// �{�^��
	enum ButtonNumber{
		// ���̔ԍ��̎��̓{�^������Ȃ�
		BUTTON_NONE = -1,
		// �L������I�����Ă���{�^��
		BUTTON_CHARA,
		// ���������̃{�^��
		BUTTON_READY,
		// �R���t�B�O�̃{�^��
		BUTTON_CONFIG,
	};

	// BG�̏������@���S�����������Ă�
	void InitializeBG();
	// BG�̍X�V �A�j���[�V����������
	void UpdateBG();
	// �Q��
	void Join(int playerNum);
	// �s�Q��
	void UnJoin(int playerNum);
	// PUSH�̓_�ōX�V
	void UpdatePushAnimation();
	// READY�̓_�ōX�V
	void UpdateReadyAnimation();
	// �����̃A�j���[�V�����X�V
	void UpdateRimoAnimation();
	// ���͂ŎQ�킷��X�V
	void UpdateInputJoin();
	// �J�[�\���̍X�V�@�A�j���[�V��������͂ňړ��Ȃ�
	void UpdateCursol();
	// ���͂Ńe�N�X�`���A�j���[�V����
	void UpdateInput();
	// ����2D�Ƀe�N�X�`���̃��[�v���Z�b�g����֐�
	void SetRimoTextureIndex(int index, int min, int max){
		m_nRimoTextureMin[index] = min;
		m_nRimoTextureMax[index] = max;
	}
	// ������pos�Ɠ������Ă���{�^���ԍ���Ԃ� index�̓v���C���ԍ�
	ButtonNumber CollisionButton(int playerNum, D3DXVECTOR3 vCursolPos);
	// �����������������Ƃ�
	void Ready(int playerNum);
	// ������������Ȃ�
	void UnReady(int playerNum);

	// �w�i
	CScene2D* m_pBG;
	// �w�i�̈ړ���
	D3DXVECTOR3 m_vBgVelo;
	// �w�i���ړ����Ă������(0:�� 1:�� 2:�E 3:��)
	short m_nBgDirection;
	// �L�����N�^�[�I�����ĕ\������
	CScene2D* m_pLogo;
	// �v���C���w�i
	CScene2D* m_pPlayerBG[PLAYER_MAX];

	// PUSH�����̔w�i
	CScene2D *m_pPush2DBG[PLAYER_MAX];
	// PUSH����
	CScene2D *m_pPush2D[PLAYER_MAX];
	// PUSH������
	bool m_bPush[PLAYER_MAX];
	// PUSH�̓_�ŃA�j���[�V���p�J�E���g
	int m_nPushFlashCount[PLAYER_MAX];
	// PUSH�̓_�ŃA�j���[�V���p�t���O
	bool m_bPushFlash[PLAYER_MAX];

	// �����Q�c
	CSceneAnime *m_pRimo[PLAYER_MAX];
	// �����Q�c�̃e�N�X�`���ԍ�
	int m_nRimoTextureIndex[PLAYER_MAX];
	// �����Q�c�̃e�N�X�`���ύX���邽�߂̃J�E���g
	int m_nRimoTextureCount[PLAYER_MAX];
	// �����Q�c�̃e�N�X�`�����[�v�C���f�b�N�X�̔ԍ�
	int m_nRimoTextureMax[PLAYER_MAX];
	int m_nRimoTextureMin[PLAYER_MAX];

	// �Q��2D
	CScene2D *m_pJoin2D[PLAYER_MAX];

	// Start�{�^��
	CScene2D *m_pStart2D;

	// �L�[�R���t�B�O�{�^��
	CScene2D *m_pConfig;

	// �J�[�\���ʒu
	int m_nCursol[PLAYER_MAX];
	// �J�[�\��2D
	CScene2D *m_pCursol2D[PLAYER_MAX];
	// �J�[�\���̊p�x
	float m_fCursolRad[PLAYER_MAX];
	// �J�[�\���A�j���[�V�����̃t���O
	bool m_bCursolFlg[PLAYER_MAX];

	// ���������t���O
	bool m_bReady[PLAYER_MAX];
	// ��������2D
	CScene2D *m_pReady2D[PLAYER_MAX];
	// ���������̓_�ŃA�j���[�V���p�J�E���g
	int m_nReadyFlashCount[PLAYER_MAX];
	// ���������̓_�ŃA�j���[�V���p�t���O
	bool m_bReadyFlash[PLAYER_MAX];

	// �����̕ϐg���
	PLAYER_MODE m_PlayerMode[PLAYER_MAX];

	// �S��������������J�ڂ܂ł̃J�E���g
	int m_nNextPhaseCount;
	// �S��������������J�ڂ��邩�̂P�񂾂��̃t���O
	bool m_bNextPhaseOnece;
	// �Q������v���C���̐l��
	short m_nPlayerManualNum;
	// �������������l��
	short m_nPlayerReadyNum;

	// �O�̃V�[���ŎQ�����Ă����l��
	short m_nPlayerJoinedNum;

};

#endif
//----EOF----