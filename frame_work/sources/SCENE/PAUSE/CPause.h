//=============================================================================
//
// CPause�N���X [CPause.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CPAUSE_H_
#define _CPAUSE_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../MAIN/main.h"
#include "../CSCENE/CScene2D.h"
#include "../UI/CCursol.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CCursol;

//*****************************************************************************
// �}�N��
//*****************************************************************************
static const D3DXVECTOR3	DEFFAULT_PAUSE_POS((float)SCREEN_WIDTH/2, (float)SCREEN_HEIGHT/2, 0.f);	// �f�t�H���g�̃|�[�Y�̏������W
static const float			DEFFAULT_PAUSE_WIDTH = (float)SCREEN_WIDTH;								// �f�t�H���g�̃|�[�Y�̕�
static const float			DEFFAULT_PAUSE_HEIGHT = (float)SCREEN_HEIGHT;							// �f�t�H���g�̃|�[�Y�̍���

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CPause : public CScene2D
{
public:
	// �|�[�Y���j���[�̑I�𒆂̎��
	typedef enum
	{
		PAUSE_RETRY,
		PAUSE_TITLE,
		PAUSE_RETURN,
		PAUSE_MAX
	}PAUSE_MENU;

	//======================================================
	// �R���X�g���N�^
	// ����: �f�o�C�X�A�v���C�I���e�B
	//======================================================
	CPause(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_PAUSE);

	// �f�X�g���N�^
	~CPause();

	//======================================================
	// ������
	// ����: �f�o�C�X�A���W�A��]�A���A�e�N�X�`���^�C�v�A�i���������A�c�������j
	// �߂�l: ����
	//======================================================
	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock = 1, int hblock = 1);

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �`��
	void Draw(void);

	//======================================================
	// �^�C�g���ɖ߂�t���O�Q�b�^�[
	// �߂�l: �^�U
	//======================================================
	bool GetReturnTitleFlag(void){return m_bReturnTititle;};

	//======================================================
	// ���g���C���邩�t���O�Q�b�^�[
	// �߂�l: �^�U
	//======================================================
	static bool GetRetryFlag(void){return m_bRetry;};

	//======================================================
	// �|�[�Y�����t���O�Q�b�^�[
	// �߂�l: �^�U
	//======================================================
	static bool GetPauseFlag(void){return m_bPause;};

	//======================================================
	// �J�[�\���`��t���O�Z�b�^�[
	// ����: �^�U
	//======================================================
	void SetCursolDrawFlag(bool flag){m_pCursol->SetDrawFlag(flag);};

	//======================================================
	// �|�[�Y�t���O�Z�b�^�[
	// ����: �^�U
	//======================================================
	static void SetPauseFlag(bool flag){m_bPause = flag;};

	//======================================================
	// �I�𒆃|�[�Y���j���[�Z�b�^�[
	// ����: �|�[�Y���j���[�̎��
	//======================================================
	void SetChoiceMenu(PAUSE_MENU type){m_nChoiceMenu = type;};

	//======================================================
	// ���g���C�t���O�Z�b�^�[
	// ����: �^�U
	//======================================================
	static void SetRetryFlag(bool flag){m_bRetry = flag;};

	// �|�[�Y�t���O���]
	static void ReverceFlag(void){m_bPause = !m_bPause;};

private:
	static bool	m_bPause;			// �|�[�Y�t���O
	static bool	m_bRetry;			// ���g���C�t���O
	CCursol*	m_pCursol;			// �J�[�\��
	int			m_nChoiceMenu;		// �I�𒆂̃��j���[
	bool		m_bReturnTititle;	// �^�C�g���ɖ߂邩�̃t���O
};
#endif
//----EOF----