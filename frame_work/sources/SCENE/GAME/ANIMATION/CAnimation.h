//=============================================================================
//
// �A�j���[�V�����N���X [CAnimation.h]
// Author : �˖{�@�r�F
//
// �����̃J�E���g�_�E���Ɏg���Ă�B
//
//=============================================================================
#ifndef _CANIMATION_H_
#define _CANIMATION_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../../MAIN/main.h"
#include "../../CSCENE/CScene.h"
#include "../../CSCENE/CScene2D.h"

//*****************************************************************************
// �萔
//*****************************************************************************
// �d�͉����x
static const float GRAVITY_SCALE = 2.0f;
// ���ɓ����������̔��˗�
static const float REFLECT_SCALE = 0.34f;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CAnimation : public CScene2D
{
public:
	//======================================================
	// �R���X�g���N�^
	// ����: �f�o�C�X�A�v���C�I���e�B
	//======================================================
	CAnimation(LPDIRECT3DDEVICE9 *pDevice, int nPriority, OBJTYPE m_objType = OBJTYPE_2D);

	// �f�X�g���N�^
	~CAnimation();

	//======================================================
	// ������
	// ����: ���W�A���A�����A�e�N�X�`���[�^�C�v�A�e�N�X�`�����������A�e�N�X�`���c������
	// �߂�l: ����
	//======================================================
	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int sepalateX,int sepalateY);
	
	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �`��
	void Draw(void);
	
	//======================================================
	// �A�j���[�V�����ԍ��ݒ�
	// ����: �A�j���[�V�����ԍ�,���]���邩
	//======================================================
	void SetIndex(int nIdx);
	void SetIndex(int nIdx, bool reverse);

	//======================================================
	// �`�悷�邩
	// ����: �t���O
	//======================================================
	void SetDrawFlag(bool flag){m_bDraw = flag;};

	// ������Z�b�g
	void SetFall(D3DXVECTOR3 pos, float fFloorHeight);
	// ���Ȃ��Z�b�g
	void SetNonFall(D3DXVECTOR3 pos);
	// �t�F�[�h�A�E�g�Z�b�g
	void SetFadeOut(float alpha, float fFadeSpeed);
	// �t�F�[�h�A�E�g���Ȃ��Z�b�g
	void SetNonFadeOut();
	// �t�F�[�h�C���Z�b�g
	void SetFadeIn(float alpha, float fFadeSpeed);
	// �t�F�[�h�C�����Ȃ��Z�b�g
	void SetNonFadeIn();


	//======================================================
	// �N���G�C�g�֐�
	// ����: �f�o�C�X�A���W�A���A�����A�e�N�X�`���[�^�C�v�A�e�N�X�`�����������A�e�N�X�`���c������
	// �߂�l: �������̃A�h���X
	//======================================================
	static CAnimation* Create(LPDIRECT3DDEVICE9 *pDevice, int nPriority, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int sepalateX, int sepalateY);

protected:
	// ���[�v�Ȃ��X�V
	void Update_Normal(void);

	// ���[�v����X�V
	void Update_Loop(void);
	
	LPDIRECT3DDEVICE9	*m_pD3DDevice;			// �f�o�C�X�I�u�W�F�N�g(�`��ɕK�v)
	float				m_fLeft;				// UV�l(��)
	float				m_fRight;				// UV�l(�E)
	float				m_fTop;					// UV�l(��)
	float				m_fBottom;				// UV�l(��)
	int					m_nWidth;				// ����
	int					m_nHeight;				// �c��
	int					m_nSepalateNumX;		// �摜������(��)�E�E�Eex:���ɂT����ł�Ȃ�T
	int					m_nSepalateNumY;		// �摜������(�c)
	int					m_nIdx;					// �����ԍ�
	int					m_nMaxIdx;				// idx�̍ő�l
	bool				m_bDraw;				// �`�悷�邩

	// ������A�j���\�V�����@�f�t�H���gOFF
	bool				m_bFall;				// ������t���O
	float				m_fGravity;				// �d��
	float				m_fFloor;				// �����鎞�̏��̈ʒu
	// �t�F�[�h�A�j���\�V�����@�f�t�H���gOFF
	bool				m_bFadeOut;				// �t�F�[�h�A�E�g����t���O
	bool				m_bFadeIn;				// �t�F�[�h�C������t���O
	float				m_fAlpha;				// 2D�̃A���t�@�l
	float				m_fFadeSpeed;			// �t�F�[�h����X�s�[�h
};
#endif
//----EOF----