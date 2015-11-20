//=============================================================================
//
// CRope�N���X [CRope.h]
// Author : �˖{�r�F
//
//=============================================================================
#ifndef _CROPE_H_
#define _CROPE_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../../BASE_OBJECT/CScene.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CManager;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CRope : public CScene
{
public:
	//======================================================
	// �R���X�g���N�^
	// ����: �f�o�C�X�A�v���C�I���e�B�A�I�u�W�F�^�C�v
	//======================================================
	CRope(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE m_objType = OBJTYPE_3D);

	// �f�X�g���N�^
	~CRope();

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �`��
	void DrawNormalRender(void);

	// �|�W�V�����A�N�Z�T
	D3DXVECTOR3& GetPos(void){ return m_Pos; };
	void SetPos(D3DXVECTOR3& pos){ m_Pos = pos; };
	void SetPos(float x, float y, float z){ m_Pos.x = x; m_Pos.y = y; m_Pos.z = z; };

	// ���b�g�A�N�Z�T
	D3DXVECTOR3& GetRot(void){ return m_Rot; };
	void SetRot(D3DXVECTOR3& rot){ m_Rot = rot; };
	void SetRot(float x, float y, float z){ m_Rot.x = x; m_Rot.y = y; m_Rot.z = z; };

	//=================================================
	// �����擾
	// ����: �m�肽���ꏊ�A(out)�@��
	// �߂�l: ����
	//=================================================
	float GetHeight(const D3DXVECTOR3& pos, D3DXVECTOR3* pNormal);

	//=================================================
	// �N���G�C�g�֐�
	// ����: �f�o�C�X�A���W�A���A�����A�u���b�N���A�e�N�X�`���^�C�v
	// �߂�l: �������̃A�h���X
	//=================================================
	static CRope* Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos, float width, float height,
		int wBlock, int hBlock,
		TEXTURE_TYPE texType,
		CManager* pManager);

	// �S���A�j���\�V�����X�^�[�g
	// ��������́A���̎�𗣂��܂ł̃t���[���J�E���g
	void Pull(float pullPower, int pullInterval);

private:
	//======================================================
	// ������
	// ����: ���W�A���A�����A�e�N�X�`���[�^�C�v�A(���������A�c������)
	// �߂�l: ����
	//======================================================
	void Init(D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType, CManager* pManager, int wblock = 1, int hblock = 1);

	// �ۑ����Ƃ������|�C���^
	LPDIRECT3DDEVICE9		*m_pD3DDevice;			// �f�o�C�X�I�u�W�F�N�g(�`��ɕK�v)
	CManager				*m_pManager;

	// ���b�V�����o�����߂̕ϐ�
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVtxBuff;			// ���_���W�����i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	LPDIRECT3DTEXTURE9		m_pD3DTexBuff;			// �e�N�X�`�������i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	D3DXVECTOR3				m_vScl;					// �n�ʂ̑傫���i�X�P�[���j
	D3DXMATRIX				m_mtxWorld;				// ���[���h�}�g���b�N�X
	int						m_nNumVtxNum;						// ���_��
	int						m_nNumPolygon;						// �|���S����
	LPDIRECT3DINDEXBUFFER9	m_pD3DIndexBuff;		// �C���f�b�N�X�����i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	D3DXVECTOR3				m_vCornerPos[4];		// �p�̍��W
	D3DXVECTOR3*			m_vFaceNormalBuf;		// �ʖ@���o�b�t�@
	D3DXVECTOR3*			m_vVtxNormalBuf;		// ���_�@���o�b�t�@
	float					m_fWidth;				// �|���S���̑Ίp���̒���
	float					m_fHeight;				// �|���S���̑Ίp���̊p�x
	float					m_fAngle;				// �|���S���̑Ίp���̊p�x
	float					m_fGridWidth;			// �O���b�h�̕�
	float					m_fGridHeight;			// �O���b�h�̍���
	int						m_nNumVertexIndex;		// �C���f�b�N�X��
	int						m_nNumWidthBlock;		// ���̕�����
	int						m_nNumHeightBlock;		// �c�̕�����
	LPDIRECT3DVERTEXDECLARATION9 m_pDecl;

	// �S���̓��������邽�߂̕ϐ�
	float m_PosMax;		// ���͈͂̍��W�ő�
	float m_PosMin;		// ���͈͂̍��W�ŏ�
	float m_PullPower;	// ���W�𓮂���������
	int m_PullInterval;	// �����t���[���J�E���g
	int m_PullCount;	// ������J�E���g����
};

#endif
//----EOF----