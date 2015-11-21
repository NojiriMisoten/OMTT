//=============================================================================
//
// CFace�N���X [CFace.h]
// Author : �˖{�r�F
//
//=============================================================================
#ifndef _CFACE_H_
#define _CFACE_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../RENDERER/CRenderer.h"
#include "../../../BASE_OBJECT/CScene2D.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************


//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CFace
{
public:

	// �\��
	enum Expression{
		// �ǂ��\��
		EXPRESSION_GOOD,
		// �_���ȕ\��
		EXPRESSION_BAD,
		// �ӂ��ȕ\��
		EXPRESSION_NORAML,
		EXPRESSION_MAX
	};

	CFace(LPDIRECT3DDEVICE9 *pDevice);
	~CFace(void);

	void Update();
	void Uninit();
	void Draw();

	// �쐬
	static CFace *Create(
		D3DXVECTOR2 &posLeft,
		D3DXVECTOR2 &posRight,
		D3DXVECTOR2 &size,
		LPDIRECT3DDEVICE9 *pDevice);

	// �J�n�A�j���[�V����������@������
	// �I������܂ł̃J�E���g(���t���[���A�j���[�V�������邩)
	void StartAnimation(int endCount);

private:

	struct FaceBace{
		// �������̃e�N�X�`���̈�R�}�̃T�C�Y
		static const float JIJII_TEX_U;
		static const float JIJII_TEX_V;

		// ���W
		D3DXVECTOR2 m_Pos;
		// ���2D
		CScene2D *m_pFace2D;
		// ��̔w�i��2D
		CScene2D *m_pBack2D;
		// �\��
		Expression m_Expression;
		// �e�N�X�`�����W
		UV_INDEX m_UV;

		// ���݂̎����̕\����e�N�X�`���ɃZ�b�g����
		void SetUV(){
			m_UV.left = JIJII_TEX_U * m_Expression;
			m_UV.right = JIJII_TEX_U * (m_Expression + 1);
			m_pFace2D->SetUV(m_UV.left, m_UV.right);
		}
	};

	// ������
	void Init(
		D3DXVECTOR2 &posLeft,
		D3DXVECTOR2 &posRight,
		D3DXVECTOR2 &size);
	// �J�n�A�j���[�V����������X�V
	void UpdateAnime();

	FaceBace m_FaceLeft;
	FaceBace m_FaceRight;

	// �J�n�A�j�������邽�߂̃J�E���g
	int m_AnimeCount;
	// �J�n�A�j���̏I���@�t���[����
	int m_AnimeCountMax;
	// �J�n�A�j�����Ă���t���O
	bool m_isAnime;
	// �J�n�A�j��1�t���[���ŕύX����A���t�@�l
	float m_AnimeOneFrameAlpha;
	D3DXCOLOR m_Anime2DColor;
	// �f�o�C�X
	LPDIRECT3DDEVICE9 *m_pD3DDevice;
};

#endif
//----EOF----