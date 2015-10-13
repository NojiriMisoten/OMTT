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
	TEXTURE_NULL = 0,			// �Ȃ�
	TEXTURE_BG_0,
	TEXTURE_BG_1,
	TEXTURE_FIRE_0,
	TEXTURE_FIRE_1,
	TEXTURE_THREAD,
	TEXTURE_PLAYER_YELLOW,
	TEXTURE_PLAYER_RED,
	TEXTURE_PLAYER_BLUE,
	TEXTURE_PLAYER_GREEN,
	TEXTURE_NUMBER_BLACK,
	TEXTURE_NUMBER_WHITE,
	TEXTURE_NUMBER_GREEN,
	TEXTURE_TREASURE,
	TEXTURE_TREASURE_ICON,
	TEXTURE_BG_TITLE,
	TEXTURE_BG_TUTORIAL,
	TEXTURE_BG_RESULT,
	TEXTURE_BG_CHARA_SELECT,
	TEXTURE_BG_STAGE_SELECT,
	TEXTURE_STAGE_1,
	TEXTURE_STAGE_2,
	TEXTURE_STAGE_3,
	TEXTURE_STAGE_4,
	TEXTURE_STAGE_5,
	TEXTURE_STAGE_6,
	TEXTURE_STAGE_7,
	TEXTURE_STAGE_8,
	TEXTURE_WIN_LOGO,
	TEXTURE_DRAW_LOGO,
	TEXTURE_FIELD_01,	// ����
	TEXTURE_FIELD_02,	// �󂨂��Ă���
	TEXTURE_FIELD_03,	// �X�^�[�g���S�[��
	TEXTURE_SPAWN_YELLOW,
	TEXTURE_SPAWN_RED,
	TEXTURE_SPAWN_BLUE,
	TEXTURE_SPAWN_GREEN,
	TEXTURE_ATTACK_THUNDERBOLT,	// �d��
	TEXTURE_HIT_SMALL,
	TEXTURE_HIT_BIG,
	TEXTURE_ATTACK_AROUND,
	TEXTURE_ATTACK_TACKLE,
	TEXTURE_SLOW,
	TEXTURE_PLAYER_ETAMORPHOSE_YELLOW,
	TEXTURE_PLAYER_ETAMORPHOSE_RED,
	TEXTURE_PLAYER_ETAMORPHOSE_BLUE,
	TEXTURE_PLAYER_ETAMORPHOSE_GREEN,
	TEXTURE_TREASURE_KIRAKIRA,
	TEXTURE_TREASURE_LIGHT,
	TEXTURE_TITLE_LOGO,
	TEXTURE_TITLE_PUSHSTART,
	TEXTURE_TITLE_MENU,
	TEXTURE_TITLE_CURSOL,
	TEXTURE_OPTION_MENU,			// �I�v�V�������j���[
	TEXTURE_OPTION_BG,				// �I�v�V����BG
	TEXTURE_CURSOL,					// �J�[�\��
	TEXTURE_RETURN,					// �߂�
	TEXTURE_PLAYER_NUM,				// �v���C���[�ԍ�
	TEXTURE_RESULT_CIRCLE_LIGHT,	// RESULT�̂��邭�郉�C�g
	TEXTURE_RESULT_LIGHT,			// RESULT�̏��҂ɂ��Ă郉�C�g
	TEXTURE_RESULT_PUSH,			// RESULT_PUSH
	TEXTURE_RESULT_MENU_BACK_0,		// RESULT_MENU �w�i
	TEXTURE_RESULT_MENU_BACK_1,		// RESULT_MENU �w�i
	TEXTURE_RESULT_MENU_BACK_2,		// RESULT_MENU �w�i
	TEXTURE_RESULT_MENU_MOJI_0,		// RESULT_MENU ������x
	TEXTURE_RESULT_MENU_MOJI_1,		// RESULT_MENU �L�����I����
	TEXTURE_RESULT_MENU_MOJI_2,		// RESULT_MENU �X�e�[�W�I����
	TEXTURE_CHARA_SELECT_PLAYER_BG,	// �L�����N�^�I���̃v���C���w�i
	TEXTURE_CHARA_SELECT_JOIN,		// �L�����N�^�I���̎Q��2D
	TEXTURE_CHARA_SELECT_START,		// �L�����N�^�I���̃X�^�[�g�{�^��
	TEXTURE_CHARA_SELECT_CONFIG,	// �L�����N�^�I���̃R���t�B�O�{�^��
	TEXTURE_CHARA_SELECT_CURSOL,	// �L�����N�^�I���̃J�[�\��
	TEXTURE_CHARA_SELECT_READY,		// �L�����N�^�I���̏�������
	TEXTURE_CHARA_SELECT_LOGO,		// �L�����N�^�I���̃��S
	TEXTURE_STAGE_SELECT_LOGO,		// �X�e�[�W�I���̃��S
	TEXTURE_RESULT_LOGO,			// ���U���g�̃��S
	TEXTURE_TUTORIAL_0,				// �`���[�g���A���̊G
	TEXTURE_TUTORIAL_1,				// �`���[�g���A���̊G
	TEXTURE_TUTORIAL_2,				// �`���[�g���A���̊G
	TEXTURE_TUTORIAL_3,				// �`���[�g���A���̊G
	TEXTURE_TUTORIAL_ARROW,			// �`���[�g���A���̖��
	TEXTURE_TUTORIAL_PAGE_1_4,		// �`���[�g���A���̃y�[�W�e�N�X�`��
	TEXTURE_TUTORIAL_PAGE_2_4,		// �`���[�g���A���̃y�[�W�e�N�X�`��
	TEXTURE_TUTORIAL_PAGE_3_4,		// �`���[�g���A���̃y�[�W�e�N�X�`��
	TEXTURE_TUTORIAL_PAGE_4_4,		// �`���[�g���A���̃y�[�W�e�N�X�`��
	TEXTURE_GAME_START,				// �Q�[���X�^�[�g�̃J�E���g�e�N�X�`���i4,1�j
	TEXTURE_THREAD_POISON,			// �W�Q�`�Ԃ̍U��(�΂̎�)
	TEXTURE_THREAD_DROP,			// �ʏ�`�Ԃ̎��U��
	TEXTURE_TRAP,					// 㩂̃e�N�X�`��
	TEXTURE_MAX				// �e�N�X�`���^�C�v����
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
	static void Uninit(void);

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

	//============================================================
	// �e�N�X�`���^�C�v�Q�b�^�[
	// ����:�e�N�X�`���̃t�@�C���p�X
	// �߂�l:�e�N�X�`���^�C�v
	//============================================================
	static TEXTURE_TYPE GetTextureTye(char* texFilePath);

private:
	//============================================================
	// ������
	// ����:�f�o�C�X
	// �߂�l:����
	//============================================================
	static	HRESULT Init(LPDIRECT3DDEVICE9 *pDevice);

	static	LPDIRECT3DTEXTURE9 m_pD3DTex[TEXTURE_MAX];				// �e�N�X�`���|�C���^
	static	bool m_bDoneCreateFlag;									// ���łɍ�������̃t���O
};

#endif
//----EOF----