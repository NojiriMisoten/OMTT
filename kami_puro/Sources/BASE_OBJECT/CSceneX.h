//=============================================================================
//
// CSceneX�N���X [CSceneX.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CSCENEX_H_
#define _CSCENEX_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../MAIN/main.h"
#include "CScene.h"
#include "../MODEL/CModel.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CSceneX : public CScene
{
public:
	//======================================================
	// �R���X�g���N�^
	// ����: �f�o�C�X�A�I�u�W�F�^�C�v
	//======================================================
	CSceneX(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE m_objType = OBJTYPE_X);

	// �f�X�g���N�^
	~CSceneX();

	//======================================================
	// ������
	// ����: ���W�A���f���̃t�@�C���p�X
	// �߂�l: ����
	//======================================================
	void Init(D3DXVECTOR3& pos, char* modelFileName);

	//======================================================
	// ������
	// ����: ���W�A���f���^�C�v
	// �߂�l: ����
	//======================================================
	void Init(D3DXVECTOR3& pos, MODEL_TYPE type);
	
	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �`��
	void Draw(void);

	// �|�W�V�����A�N�Z�T
	D3DXVECTOR3& GetPos(void){ return m_Pos;};
	D3DXVECTOR3* GetPosAdr(void){ return &m_Pos;};
	void		SetPos(D3DXVECTOR3& pos){ m_Pos = pos;};
	void		SetPos(float x,float y, float z){ m_Pos.x = x; m_Pos.y = y; m_Pos.z = z;};

	// ���b�g�A�N�Z�T
	D3DXVECTOR3& GetRot(void){return m_Rot;};
	void		SetRot(D3DXVECTOR3& rot){m_Rot = rot;};
	void		SetRot(float x,float y,float z){ m_Rot.x = x; m_Rot.y = y; m_Rot.z = z;};
	
	// �X�P�[���A�N�Z�T
	D3DXVECTOR3& GetScl(void){return m_vScl;};
	void		SetScl(D3DXVECTOR3& scl){m_vScl = scl;};
	void		SetScl(float x,float y,float z){ m_vScl.x = x; m_vScl.y = y; m_vScl.z = z;};
	
	//=================================================
	// ���a�擾
	// �߂�l: ���a
	//=================================================
	float GetRadius(void){return m_fRadius;};

	//=================================================
	// �����擾
	// �߂�l: ����
	//=================================================
	float GetHeight(void){return m_fHeight;};

	//=================================================
	// �A���t�@�l�Z�b�g
	// ����: �A���t�@�l
	//=================================================
	void SetAlpha(float alpha);

	//=================================================
	// �X�y�L�����[�Z�b�g
	// ����: �X�y�L�����[�p���[
	//=================================================
	void SetSpecularPow(float pow);

	//=================================================
	// ���b�V�����擾
	// �߂�l: ���b�V���|�C���^
	//=================================================
	LPD3DXMESH* GetMesh(void){return &m_ModelInfo->pD3DXMeshModel;};

	//=================================================
	// �C���f�b�N�X�o�b�t�@�[�擾
	// �߂�l: �C���f�b�N�X�o�b�t�@�[
	//=================================================
	WORD* GetIndexBuffer(void){return m_ModelInfo->pIndex;};
	
	//=================================================
	// �o�[�e�b�N�X�o�b�t�@�[�擾
	// �߂�l: �o�[�e�b�N�X�o�b�t�@�[
	//=================================================
	VERTEX* GetVertexBuffer(void){return m_ModelInfo->pVertex;};

	// �G�b�W�e�N�X�`���쐬
	void CreateEdgeTexture(void);

	// �V���h�E�e�N�X�`���쐬
	void CreateShadowTexture(void);

	//=================================================
	// �N���G�C�g�֐�
	// ����: �f�o�C�X�A���W�A���f���t�@�C���p�X
	// �߂�l: �������̃A�h���X
	//=================================================
	static CSceneX* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, char* modelFileName);

	//=================================================
	// �N���G�C�g�֐�
	// ����: �f�o�C�X�A���W�A���f���^�C�v
	// �߂�l: �������̃A�h���X
	//=================================================
	static CSceneX* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, MODEL_TYPE type);

protected:
	float				m_fRadius;								// ���a
	float				m_fWidth;								// ���i�����蔻��p�j
	float				m_fDepth;								// ���s�i�����蔻��p�j
	float				m_fHeight;								// �����i�����蔻��p�j
	LPDIRECT3DDEVICE9	*m_pD3DDevice;							// �f�o�C�X�I�u�W�F�N�g(�`��ɕK�v)
	X_MODEL				*m_ModelInfo;							// ���f���̏��
	D3DXVECTOR3			m_vScl;									// �傫���i�X�P�[���j
	char				m_cModelFileName[MAX_LENGTH_FILE_PATH];	// ���f���̃t�@�C���l�[��
	static D3DXMATRIX	m_mtxWorld;								// ���[���h�}�g���b�N�X
};
#endif
//----EOF----