//=============================================================================
//
// CTitle�N���X [CTitle.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CTITLE_H_
#define _CTITLE_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../MAIN/main.h"
#include "../../PHASE/CPhase.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;
class CTextureAnimator;
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTitle : public CPhase
{
public:
	// �R���X�g���N�^
	CTitle(void);

	// �f�X�g���N�^
	~CTitle(void);

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
	static CTitle* Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);

private:

	//�I�u�W�F�N�g�̍X�V����
	void ObjectUpdate();
	//�I�u�W�F�N�g��������
	void ObjectSet(LPDIRECT3DDEVICE9* pDevice);
	//�I�u�W�F�N�g�̈ړ�����
	void ObjectMove();
	//�I�u�W�F�N�g��UV�X�V����
	void ObjectAnimation();
	//�I�u�W�F�N�g�̐F�̍X�V����
	void ObjectColorSet();

	//�^�C�g����ʂŎg�p�����p�̃|���S���p�����[�^�[
	typedef struct
	{
		CScene2D*	Scene2D;
		D3DXVECTOR3 V3Offset;
		UV_INDEX	UV;
		D3DXCOLOR	Color;
		float		fRotation;
	}POLYGON_PARAMETER;

	POLYGON_PARAMETER	m_BG;
	POLYGON_PARAMETER	m_ScrollBG;
	POLYGON_PARAMETER	m_Fire;
	POLYGON_PARAMETER	m_Logo;
	POLYGON_PARAMETER	m_AnyCongaLogo;

	CTextureAnimator *m_FireAnimatior;

	bool isObject;
};

#endif
//----EOF----