//=============================================================================
//
// CScene3D�N���X [CScene3D.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CSCENE3D_H_
#define _CSCENE3D_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "CScene.h"

class CManager;
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CScene3D : public CScene
{
public:
	//======================================================
	// �R���X�g���N�^
	// ����: �f�o�C�X�A�v���C�I���e�B�A�I�u�W�F�^�C�v
	//======================================================
	CScene3D(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE m_objType = OBJTYPE_3D);

	// �f�X�g���N�^
	~CScene3D();

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �`��
	void DrawNormalRender(void);
	
	// �|�W�V�����A�N�Z�T
	D3DXVECTOR3& GetPos(void){ return m_Pos;};
	void SetPos(D3DXVECTOR3& pos){ m_Pos = pos;};
	void SetPos(float x,float y, float z){ m_Pos.x = x; m_Pos.y = y; m_Pos.z = z;};
	
	// ���b�g�A�N�Z�T
	D3DXVECTOR3& GetRot(void){return m_Rot;};
	void SetRot(D3DXVECTOR3& rot){m_Rot = rot;};
	void SetRot(float x,float y,float z){ m_Rot.x = x; m_Rot.y = y; m_Rot.z = z;};
	
	//=================================================
	// �����擾
	// ����: �m�肽���ꏊ�A(out)�@��
	// �߂�l: ����
	//=================================================
	float GetHeight(const D3DXVECTOR3& pos, D3DXVECTOR3* pNormal);

	//=================================================
	// �N���G�C�g�֐�
	// ����: �f�o�C�X�A���W�A���A�����A�e�N�X�`���^�C�v
	// �߂�l: �������̃A�h���X
	//=================================================
	static CScene3D* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, CManager* pManager);

private:
	//======================================================
	// ������
	// ����: ���W�A���A�����A�e�N�X�`���[�^�C�v�A(���������A�c������)
	// �߂�l: ����
	//======================================================
	void Init(D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType, CManager* pManager, int wblock = 1, int hblock = 1);

	// ���_���f�֐�
	void SetVertexPolygon(void);

	//=================================================
	// �F�ύX�֐�
	// ����: �F
	//=================================================
	void SetColorPolygon(D3DXCOLOR color);
	
	LPDIRECT3DDEVICE9		*m_pD3DDevice;			// �f�o�C�X�I�u�W�F�N�g(�`��ɕK�v)
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVtxBuff;			// ���_���W�����i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	LPDIRECT3DTEXTURE9		m_pD3DTexBuff;			// �e�N�X�`�������i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	D3DXVECTOR3				m_vScl;					// �n�ʂ̑傫���i�X�P�[���j
	static D3DXMATRIX		m_mtxWorld;				// ���[���h�}�g���b�N�X

protected:
	LPDIRECT3DVERTEXDECLARATION9 m_pDecl;
	float					m_fWidth;				// �|���S���̑Ίp���̒���
	float					m_fHeight;				// �|���S���̑Ίp���̊p�x
	CManager				*m_pManager;
};
#endif
//----EOF----