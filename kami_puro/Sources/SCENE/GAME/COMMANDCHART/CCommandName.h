//=============================================================================
//
//	CCommandName�w�b�_�[[CComandName.h]
//
//	Author : �����@�Ȉ�
//
//=============================================================================
#ifndef __CCOMMANDNAME_H__
#define __CCOMMANDNAME_H__

//-----------------------------------------------------------------------------
//	�C���N���[�h
//-----------------------------------------------------------------------------
#include "../../../BASE_OBJECT/CScene2D.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
static const float COMMAND_NAME_POLYGON_WIDTH = 100.0f * (SCREEN_HEIGHT * 0.5f / 150.f);			// �R�}���h�̃|���S���̉���
static const float COMMAND_NAME_POLYGON_HEIGHT = 20.0f * (SCREEN_HEIGHT * 0.5f / 150.f);			// �R�}���h�̃|���S���̍���

//-----------------------------------------------------------------------------
// �N���X��`
//-----------------------------------------------------------------------------
class CCommandName : public CScene2D
{
public:
	// �R���X�g���N�^
	CCommandName(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objType = OBJTYPE_2D);

	// �f�X�g���N�^
	~CCommandName();

	// ��������
	// �����F�f�o�C�X�A���W�A�e�N�X�`��
	static CCommandName* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, TEXTURE_TYPE Texture);

	// ������
	// �����F�{�^���̃^�C�v�A���W
	void Init(D3DXVECTOR3 pos, TEXTURE_TYPE Texture);

	// �X�V
	void Update(void);

	// �I��
	void Uninit(void);

	// �`��
	void Draw(void);

	// �ړ������邽�߂̖ڕW���W�̐ݒ�
	// �����F�ڕW�̍��W
	void SetDestPos(D3DXVECTOR3 DestPos){ m_DestPos = DestPos; m_isMoveX = true; }

	// �ړ��p�֐�
	void Move(void);

private:
	// �F
	D3DXCOLOR m_Color;
	// �ŏI�I�ȕ\�����W
	D3DXVECTOR3 m_DestPos;
	// x�����̈ړ����s�����ǂ����̃t���O
	bool m_isMoveX;
};

#endif

// EOF