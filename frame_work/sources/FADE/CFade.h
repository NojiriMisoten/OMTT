//=============================================================================
//
// CFade�N���X [CFade.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CFADE_H_
#define _CFADE_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../MAIN/main.h"
#include "../SCENE/CSCENE/CScene2D.h"

//*****************************************************************************
// �񋓑�
//*****************************************************************************
// �t�F�[�h�̏��
typedef enum
{
	MODE_FADE_NULL,		// �����Ȃ�
	MODE_FADE_IN,		// �t�F�[�h�C�����
	MODE_FADE_IN_END,	// �t�F�[�h�C���I��
	MODE_FADE_OUT,		// �t�F�[�h�A�E�g���
	MODE_FADE_OUT_END,	// �t�F�[�h�A�E�g�I��
	MODE_FADE_INFINITY,	// �����t�F�[�h
	MODE_FADE_MAX
}FADE_MODE;

//*****************************************************************************
// �}�N��
//*****************************************************************************
static const D3DXVECTOR3	DEFFAULT_FADE_POS(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0);	// �f�t�H���g�̃t�F�[�h�̏������W
static const D3DXCOLOR		DEFFAULT_FADE_IN_COLOR(1.f, 1.f, 1.f, 1.f);							// �f�t�H���g�̃t�F�[�h�C���̐F
static const D3DXCOLOR		DEFFAULT_FADE_OUT_COLOR(1.f, 1.f, 1.f, 0.f);						// �f�t�H���g�̃t�F�[�h�A�E�g�̐F
static const float			DEFFAULT_FADE_WIDTH = 2000.f;										// �f�t�H���g�̃t�F�[�h�̕�
static const float			DEFFAULT_FADE_HEIGHT = 2000.f;										// �f�t�H���g�̃t�F�[�h�̍���
static const int			DEFFAULT_FADE_TIME = 60;											// �f�t�H���g�̃t�F�[�h����

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CFade : public CScene2D
{
public:
	//=================================================================
	// �R���X�g���N�^
	// ����: �f�o�C�X�A(�v���C�I���e�B)
	// �߂�l: ����
	//=================================================================
	CFade(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_FADE);

	// �f�X�g���N�^
	~CFade();

	//=================================================================
	// �N���G�C�g�֐�
	// ����: ���W�A���A�����A�e�N�X�`���[�̎�ށA(���������A�c������)
	// �߂�l: ����
	//=================================================================
	HRESULT		Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock = 1, int hblock = 1);

	// �I��
	void		Uninit(void);

	// �X�V
	void		Update(void);

	// �`��
	void		Draw(void);

	//=================================================================
	// �t�F�[�h�X�^�[�g�֐�
	// ����:�������t�F�[�h�̎�ށA�t�F�[�h�̐F�A�t�F�[�h�̎���
	//=================================================================
	void Start(FADE_MODE mode, D3DXCOLOR color, int endToTime);

	//=================================================================
	// ��Ԃ̎擾
	// �߂�l:���݂̃t�F�[�h���[�h
	//=================================================================
	static FADE_MODE GetMode(void){return m_mode;};

private:
	static const int	POLYGON_NUM = 2;// �|���S���̐�
	
	static FADE_MODE	m_mode;			// ���
	D3DXCOLOR			m_color;		// �F
	int					m_nEndTime;		// �I������
	bool				m_bFade;		// �t�F�[�h�t���O
	float				m_fMovAlpha;	// �����x(���x)
};
#endif
//----EOF----