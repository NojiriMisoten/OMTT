//=============================================================================
//
// CNowLoading�N���X [CNowLoading.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _C_NOWLOADING_H_
#define _C_NOWLOADING_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../MAIN/main.h"

//*****************************************************************************
// �N���X�錾
//*****************************************************************************
class CNowLoading
{
public:
	// �R���X�g���N�^
	CNowLoading(void);

	// �f�X�g���N�^
	~CNowLoading(void);

	//======================================================
	// ������
	// ����: �f�o�C�X
	//======================================================
	void Init(LPDIRECT3DDEVICE9* pDevice);

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �`��
	void Draw(void);

private:
	static const int		MAX_CHAR_NUM = 10;						// �ő啶����

	LPDIRECT3DDEVICE9		*m_pD3DDevice;							// �f�o�C�X�I�u�W�F�N�g(�`��ɕK�v)
	D3DXVECTOR3				m_vCharPos[MAX_CHAR_NUM];				// �����̍��W
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVtxBuff[MAX_CHAR_NUM];			// ���_���W�����i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	LPDIRECT3DTEXTURE9		m_pD3DTexBuff;							// �e�N�X�`�������i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	float					m_fMoveCounter[MAX_CHAR_NUM];			// �ړ��A�j���[�V�����Ǘ�
};

#endif
//----EOF----