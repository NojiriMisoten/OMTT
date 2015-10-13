//=============================================================================
//
// CSceneCircle�N���X [CSceneCircle.h]
// Author : �˖{�r�F
//
//=============================================================================
#ifndef _CSCENECIRCLE_H_
#define _CSCENECIRCLE_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../MAIN/main.h"
#include "CScene2D.h"

//=============================================================================
// �O���錾
//=============================================================================
class CNumber;

//*****************************************************************************
// �}�N��
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CSceneCircle : public CScene2D
{
public:
	//======================================================
	// �R���X�g���N�^
	// ����: �f�o�C�X
	//======================================================
	CSceneCircle(LPDIRECT3DDEVICE9 *pDevice);

	// �f�X�g���N�^
	~CSceneCircle();

	//======================================================
	// ������
	// ����: ���S���W�A���a�A�e�N�X�`���^�C�v
	//======================================================
	HRESULT Init(D3DXVECTOR3 pos, float radius, D3DXCOLOR color);
	
	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �`��
	void Draw(void);
	
	//======================================================
	// �N���G�C�g�֐�
	// ����: �f�o�C�X�@���W�@���a�@�F�@�ő�l
	// �߂�l: �������̃A�h���X
	//======================================================
	static CSceneCircle* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float radius, D3DXCOLOR color, float fMaxValue);

	// float�^�̐��l������ƍő�l�ɉ����ĉ~�̗ʂ�ύX����
	void ChangeSize(float fValue);

	// �`��t���O
	void SetDrawFlag(bool flg){
		m_bDraw = flg;
	}

private:
	// ���_�o�b�t�@�̊m��
	HRESULT CreateVertexBuff();
	// ���_���̏�����
	void SetVertexBuff();
	// ���݂̊p�x�Œ��_�o�b�t�@�̒l���X�V�i�X�V�ŌĂԁj
	void SetVertexBuffVtx();
	// �ő�l�ɑ΂��鐔�l�̊������ő�l����v�Z
	void CalcRatio(float max);

	// �f�o�C�X
	LPDIRECT3DDEVICE9 *m_pD3DDevice;
	// ���a
	float m_fRadius;
	// �~�̐F
	D3DXCOLOR m_Color;
	// ���݂̊p�x
	float m_fRad;
	// �ő�l�ɑ΂��鐔�l�̊���(ChangeSize��fValue(1)������̊p�x)
	float m_fValueRaio;
	// �`��t���O(CScene2D�̕`����g��Ȃ��̂Ŏ��O)
	bool m_bDraw;
};
#endif
//----EOF----