//=============================================================================
//
// CSceneMeshField�N���X [CSceneMeshField.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CSCENEMESHFIELD_H_
#define _CSCENEMESHFIELD_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../MAIN/main.h"
#include "CScene.h"

class CManager;
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CSceneMeshField : CScene
{
public:
	//======================================================
	// �R���X�g���N�^
	// ����: �I�u�W�F�^�C�v
	//======================================================
	CSceneMeshField(LPDIRECT3DDEVICE9 *pDevice);

	// �f�X�g���N�^
	~CSceneMeshField();
	
	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �`��
	void DrawNormalRender(void);
	void DrawToonObjectDepthRender(void);
	void DrawNormalVecRender(void);
	
	// �|�W�V�����A�N�Z�T
	D3DXVECTOR3& GetPos(void){ return m_Pos;};
	void		SetPos(D3DXVECTOR3& pos){ m_Pos = pos;};
	void		SetPos(float x,float y, float z){ m_Pos.x = x; m_Pos.y = y; m_Pos.z = z;};
	
	// ���b�g�A�N�Z�T
	D3DXVECTOR3& GetRot(void){return m_Rot;};
	void		SetRot(D3DXVECTOR3& rot){m_Rot = rot;};
	void		SetRot(float x,float y,float z){ m_Rot.x = x; m_Rot.y = y; m_Rot.z = z;};
	
	//=================================================
	// �����擾
	// ����: �m�肽���ꏊ�A(out)�@��
	// �߂�l: ����
	//=================================================
	float GetHeight(const D3DXVECTOR3& pos, D3DXVECTOR3* pNormal);

	//=================================================
	// �T�C�Y�Q�b�g
	// ����: (out)�w�T�C�Y�A(out)�y�T�C�Y
	//=================================================
	void GetSize(float* pOutXSize, float* pOutZSize){*pOutXSize = m_fWidth; *pOutZSize = m_fHeight;};

	//=================================================
	// �p�̍��W�Q�b�g
	// ����: �p�̔ԍ�
	// �߂�l: �p�̍��W
	//=================================================
	D3DXVECTOR3& GetCornerPos(int idx){ return m_vCornerPos[idx];};

	//=================================================
	// �N���G�C�g�֐�
	// ����: �f�o�C�X�A�t�B�[���h�t�@�C���p�X
	// �߂�l: �������̃|�C���^
	//=================================================
	static CSceneMeshField* Create(LPDIRECT3DDEVICE9 *pDevice, char* fieldFileName, CManager* pManager);

private:
	//======================================================
	// ������
	// ����: ���W�A���A�����A�e�N�X�`���[�t�@�C���l�[���A(���������A�c������)
	// �߂�l: ����
	//======================================================
	void Init(D3DXVECTOR3& pos, float width, float height, char* texFileName, CManager* pManager, int wblock = 1, int hblock = 1, bool useMap = true);

	// ���_�Z�b�g
	void SetVertexPolygon(void);

	//=================================================
	// �F�ύX�֐�
	// ����: �F
	//=================================================
	void SetColorPolygon(D3DXCOLOR& color);

	//=================================================
	// �|���S���̍����擾
	// ����: ���_���W0�A���_���W1�A���_���W2�A���ׂ������W�A�iout�j�@���|�C���^
	// �߂�l: ����
	//=================================================
	float GetHeightPolygon(const D3DXVECTOR3& p0, const D3DXVECTOR3& p1, const D3DXVECTOR3& p2, const D3DXVECTOR3& pos, D3DXVECTOR3* pNormal);

	LPDIRECT3DVERTEXDECLARATION9 m_pDecl;
	LPDIRECT3DDEVICE9		*m_pD3DDevice;						// �f�o�C�X�I�u�W�F�N�g(�`��ɕK�v)
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVtxBuff;						// ���_���W�����i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	LPDIRECT3DINDEXBUFFER9	m_pD3DIndexBuff;					// �C���f�b�N�X�����i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	LPDIRECT3DTEXTURE9		m_pD3DTexBuff;						// �e�N�X�`�������i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	D3DXVECTOR3				m_vCornerPos[4];					// �p�̍��W
	D3DXVECTOR3*			m_vFaceNormalBuf;					// �ʖ@���o�b�t�@
	D3DXVECTOR3*			m_vVtxNormalBuf;					// ���_�@���o�b�t�@
	float					m_fAngle;							// �|���S���̑Ίp���̊p�x
	float					m_fWidth;							// ��
	float					m_fHeight;							// ����
	float					m_fGridWidth;						// �O���b�h�̕�
	float					m_fGridHeight;						// �O���b�h�̍���
	int						m_nNumPolygon;						// �|���S����
	int						m_nNumVertexIndex;					// �C���f�b�N�X��
	int						m_nNumVtxNum;						// ���_��
	int						m_nNumWidthBlock;					// ���̕�����
	int						m_nNumHeightBlock;					// �c�̕�����
	int						m_nID;								// ID
	CManager				*m_pManager;
	static D3DXMATRIX		m_mtxWorld;							// ���[���h�}�g���b�N�X
};
#endif
//----EOF----