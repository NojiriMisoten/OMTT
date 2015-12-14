//=============================================================================
//
// �e�N�X�`���[���� [CTexture.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CTEXTURE_H_
#define _CTEXTURE_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../MAIN/main.h"

//*****************************************************************************
// �񋓑�
//*****************************************************************************
// �e�N�X�`���̎��
typedef enum
{
	TEXTURE_MONO = 0,				// �g�D�[���p
	TEXTURE_DEFAULT,			// ���f���̃f�t�H���g�e�N�X�`��
	TEXTURE_NUMBER,				// ����
	TEXTURE_TITLE,
	TEXTURE_RESULT,
	TEXTURE_HP_GAGE_G,			// �̗̓Q�[�W�݂ǂ�
	TEXTURE_HP_GAGE_R,			// �̗̓Q�[�W���������̐�
	TEXTURE_HP_GAGE_FRAME,		// �̗̓Q�[�W�̘g
	TEXTURE_HP_GAGE_FRAME_TOP,	// �̗̓Q�[�W�̘g �㔼��
	TEXTURE_CROWD_GAGE_FRAME,	// �ϋq�Q�[�W�̘g
	TEXTURE_CROWD_GAGE_HUMAN,	// �ϋq�Q�[�W��̐l��
	TEXTURE_CROWD_SPARK,		// �ϋq�Q�[�W�����̃o�`�o�`
	TEXTURE_JIJII,				// �Q�[����ʂ̂������̃e�N�X�`��
	TEXTURE_JIJII_BACK,			// �Q�[����ʂ̂������̔w�i
	TEXTURE_BLUE,				// �����O�e�N�X�`��
	TEXTURE_RED,				// �����O�e�N�X�`��
	TEXTURE_METALICGRAY,		// �����O�e�N�X�`��
	TEXTURE_WINDOWS,			// �����O�e�N�X�`��
	TEXTURE_BUTTON,				// �{�^���̃e�N�X�`��
	TEXTURE_ROPE,				// ���[�v�e�N�X�`��
	TEXTURE_UI_FADE_LINE,		// �t�F�[�h�̃��C��
	TEXTURE_UI_FADE_BALL_LARGE,	// �t�F�[�h�̃{�[��
	TEXTURE_UI_FADE_BALL_SMALL,	// �t�F�[�h�̃{�[��
	TEXTURE_UI_CUT_IN_0,		// �J�b�g�C��0
	TEXTURE_UI_CUT_IN_1,		// �J�b�g�C��1
	TEXTURE_UI_CUT_IN_BACK_1,	// �J�b�g�C���w�i
	TEXTURE_UI_CUT_IN_BACK_2,	// �J�b�g�C���w�i
	TEXTURE_SKILL_NAME_CHOP,	// �Z�� �`���b�v
	TEXTURE_SKILL_NAME_ELBOW,	// �Z�� �G���{�[
	TEXTURE_SKILL_NAME_LARIAT,	// �Z�� �����A�b�g
	TEXTURE_SKILL_NAME_SLAP,	// �Z�� �r���^
	TEXTURE_SKILL_NAME_BACKDROP,// �Z�� �o�b�N�h���b�v
	TEXTURE_SKILL_NAME_STUNNER,	// �Z�� �X�^�i�[
	TEXTURE_SKILL_NAME_ROLLING,	// �Z�� ���[�����O�G���{�[
	TEXTURE_SKILL_NAME_SHOULDER,// �Z�� �t���C���V�����_�[
	TEXTURE_SKILL_NAME_DROPKICK,// �Z�� �h���b�v�L�b�N
	TEXTURE_SKILL_NAME_SMALL_ATTACK,// �Z�� ���U��
	TEXTURE_SKILL_NAME_BIG_ATTACK,	// �Z�� ��U��
	TEXTURE_SKILL_NAME_THROW,		// �Z�� ����
	TEXTURE_SKILL_NAME_ROPE,	// �Z�� ���[�v
	TEXTURE_SKILL_NAME_FINISH,	// �Z�� FINISH
	TEXTURE_UI_TIME_BACK,		// �������Ԃ̔w�i
	TEXTURE_COMMAND_CHART_BACK,	// �R�}���h�`���[�g�̔w�i
	TEXTURE_COMMAND_BACK,		// �R�}���h�̈��̔w�i
	TEXTURE_BG_TITLE,			// �^�C�g����BG
	TEXTURE_LOGO_TITLE,			// �^�C�g�����S
	TEXTURE_LOGOANY_TITLE,		// �^�C�g���R���K�������ă��S
	TEXTURE_BG_SCROLL_TITLE,	// �^�C�g���̃X�N���[���pBG
	TEXTURE_FIRE_TITLE,			// �^�C�g����
	TEXTURE_CHAIN,				// ������
	TEXTURE_READY,				// READY
	TEXTURE_FIGHT,				// FIGHT
	TEXTURE_KO,					// K.O
	TEXTURE_HUMAN,				// �ϋq�̃e�N�X�`��
	TEXTURE_MAX					// �e�N�X�`���^�C�v����
}TEXTURE_TYPE;

//*****************************************************************************
// �N���X�錾
//*****************************************************************************
class CTexture
{
public:
	// �R���X�g���N�^
	CTexture(void){};

	// �f�X�g���N�^
	~CTexture(void){};

	// �I��
	static void Uninit( void );

	//============================================================
	// �N���G�C�g
	// ����:�f�o�C�X
	//============================================================
	static void CreateTexture(LPDIRECT3DDEVICE9 *pDevice);

	//============================================================
	// �e�N�X�`���Q�b�^�[
	// ����:�e�N�X�`���̎��
	// �߂�l:�e�N�X�`���̃|�C���^
	//============================================================
	static LPDIRECT3DTEXTURE9 GetTexture(const TEXTURE_TYPE type);

private:
	//============================================================
	// ������
	// ����:�f�o�C�X
	// �߂�l:����
	//============================================================
	static	HRESULT Init( LPDIRECT3DDEVICE9 *pDevice );

	static	LPDIRECT3DTEXTURE9 m_pD3DTex[TEXTURE_MAX];				// �e�N�X�`���|�C���^
	static	bool m_bDoneCreateFlag;									// ���łɍ�������̃t���O
};

#endif
//----EOF----