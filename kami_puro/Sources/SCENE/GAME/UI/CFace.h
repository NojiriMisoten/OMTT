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
		// �ӂ��ȕ\��
		EXPRESSION_NORAML,
		// �_���ȕ\��
		EXPRESSION_BAD,
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

	void SetExpressionLeft();

private:

	struct FaceBace{
		// ���W
		D3DXVECTOR2 m_Pos;
		// ���2D
		CScene2D *m_pFace2D;
		// ��̔w�i��2D
		CScene2D *m_pBack2D;
		// �\��
		Expression m_Expression;
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

	// �\��
	Expression m_ExpressionLeft;
	Expression m_ExpressionRight;

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