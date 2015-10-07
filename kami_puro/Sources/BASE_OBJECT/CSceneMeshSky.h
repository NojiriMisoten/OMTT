//=============================================================================
//
// CSceneMeshSky�N���X [CSceneMeshSky.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CSCENEMESHSKY_H_
#define _CSCENEMESHSKY_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../MAIN/main.h"
#include "CScene.h"

//=============================================================================
// �}�N��
//=============================================================================
static const D3DXVECTOR3	DEFAULT_SKY_POS(0.f, -50.f, 0.f);	// �f�t�H���g�̍��W
static const float			DEFAULT_SKY_RADIUS = 400.f;	// �f�t�H���g�̔��a
static const float			DEFAULT_SKY_HEIGHT = 400.f;	// �f�t�H���g�̍���
static const int			DEFAULT_SKY_SEPLATE_NUM = 20;	// �f�t�H���g�̕�����

class CManager;
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CSceneMeshSky : public CScene
{
public:
	//======================================================
	// �R���X�g���N�^
	// ����: �f�o�C�X�A�v���C�I���e�B
	//======================================================
	CSceneMeshSky(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objtype = OBJTYPE_3D);

	// �f�X�g���N�^
	~CSceneMeshSky();

	//======================================================
	// ������
	// ����: ���W�A���A�����A�e�N�X�`���[�^�C�v�A���������A�c������
	// �߂�l: ����
	//======================================================
	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock = 1, int hblock = 1);

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �`��
	void DrawNormalRender(void);
	void DrawBaseHighLuminanceRender(void);

	// �|�W�V�����A�N�Z�T
	D3DXVECTOR3& GetPos(void){ return m_Pos; };
	void		SetPos(D3DXVECTOR3& pos){ m_Pos = pos; };
	void		SetPos(float x, float y, float z){ m_Pos.x = x; m_Pos.y = y; m_Pos.z = z; };

	// ���b�g�A�N�Z�T
	D3DXVECTOR3& GetRot(void){ return m_Rot; };
	void		SetRot(D3DXVECTOR3& rot){ m_Rot = rot; };
	void		SetRot(float x, float y, float z){ m_Rot.x = x; m_Rot.y = y; m_Rot.z = z; };

	//=================================================
	// �N���G�C�g�֐�
	// ����: �f�o�C�X�A���W�A���a�A���A�����A���������A�c�������A�e�N�X�`���^�C�v�A�p�x
	// �߂�l: �������̃A�h���X
	//=================================================
	static CSceneMeshSky* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float r, float height, int wblock, int hblock, TEXTURE_TYPE texType, CManager* pManager, D3DXVECTOR3 rot = D3DXVECTOR3(0, 0, 0));

private:
	// ���_���f�֐�
	void SetVertexPolygon(void);

	//=================================================
	// �F�ύX�֐�
	// ����: �F
	//=================================================
	void SetColorPolygon(D3DXCOLOR color);

	LPDIRECT3DVERTEXDECLARATION9 m_pDecl;
	LPDIRECT3DDEVICE9		*m_pD3DDevice;			// �f�o�C�X�I�u�W�F�N�g(�`��ɕK�v)
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVtxBuff;			// ���_���W�����i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	LPDIRECT3DINDEXBUFFER9	m_pD3DIndexBuff;		// �C���f�b�N�X�����i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	LPDIRECT3DTEXTURE9		m_pD3DTexBuff;			// �e�N�X�`�������i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	D3DXVECTOR3				m_vPos;					// �|���S���̒����̈ʒu
	D3DXVECTOR3				m_vRot;					// �|���S���̉�]
	D3DXVECTOR3				m_vScl;					// �傫���i�X�P�[���j
	float					m_fAngle;				// �|���S���̑Ίp���̊p�x
	float					m_fRadius;				// ���a
	float					m_fHeight;				// ����
	int						m_nNumPolygon;			// �|���S����
	int						m_nNumVertexIndex;		// �C���f�b�N�X��
	int						m_nNumVtxNum;			// ���_��
	int						m_nNumWidthBlock;		// ���̕�����
	int						m_nNumHeightBlock;		// �c�̕�����
	CManager				*m_pManager;
	D3DXMATRIX				m_mtxWorld;				// ���[���h�}�g���b�N�X
};
#endif
//----EOF----