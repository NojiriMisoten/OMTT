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
	TEXTURE_PLAYER,
	TEXTURE_HP_GAGE_G,			// �̗̓Q�[�W�݂ǂ�
	TEXTURE_HP_GAGE_R,			// �̗̓Q�[�W���������̐�
	TEXTURE_HP_GAGE_FRAME,		// �̗̓Q�[�W�̘g
	TEXTURE_CROWD_GAGE_FRAME,	// �ϋq�Q�[�W�̘g
	TEXTURE_CROWD_GAGE_HUMAN,	// �ϋq�Q�[�W��̐l��
	TEXTURE_CROWD_SPARK,		// �ϋq�Q�[�W�����̃o�`�o�`
	TEXTURE_BLUE,				// �����O�e�N�X�`��
	TEXTURE_RED,				// �����O�e�N�X�`��
	TEXTURE_METALICGRAY,		// �����O�e�N�X�`��
	TEXTURE_WINDOWS,			// �����O�e�N�X�`��

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