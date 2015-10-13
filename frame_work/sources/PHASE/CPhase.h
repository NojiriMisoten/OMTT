//=============================================================================
//
// CPhase�N���X [CPhase.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CPHASE_H_
#define _CPHASE_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../MAIN/main.h"
#include "../FADE/CFade.h"
#include "../INPUT/CInputKeyboard.h"
#include "../INPUT/CInputGamePad.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CManager;

//*****************************************************************************
// �񋓑�
//*****************************************************************************
// �t�F�[�Y�̎��
typedef enum
{
	MODE_PHASE_NULL,
	MODE_PHASE_TITLE,
	MODE_PHASE_TUTORIAL,
	MODE_PHASE_CHARA_SELECT,
	MODE_PHASE_STAGE_SELECT,
	MODE_PHASE_GAME,
	MODE_PHASE_RESULT,
	MODE_PHASE_OPTION,
	MODE_PHASE_MAX
}MODE_PHASE;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CPhase
{
public:
	// �R���X�g���N�^
	CPhase(void);

	// �f�X�g���N�^
	~CPhase(void);

	// �I��
	virtual void Uninit(void);

	// �X�V
	virtual void Update(void);

	// �`��
	virtual void Draw(void){};

	//======================================================
	// �N���G�C�g
	// ����: �t�F�[�Y�̎�ށA�f�o�C�X�A�}�l�[�W���[�̃|�C���^
	// �߂�l: ������t�F�C�Y�̃|�C���^
	//======================================================
	CPhase* Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice, CManager* pManager);

private:

protected:
	//======================================================
	// ������
	// ����: �f�o�C�X�A�t�F�[�Y�̎��
	//======================================================
	virtual void Init(LPDIRECT3DDEVICE9* pD3DDevice, MODE_PHASE mode);

	CFade*					m_pFade;		// �t�F�[�h�|�C���^
	LPDIRECT3DDEVICE9*		m_pD3DDevice;	// �f�o�C�X
	static CManager*		m_pManager;		// �}�l�[�W���[
};
#endif
//----EOF----