//=============================================================================
//
// CSceneBillBoard�N���X [CSceneBillBoard.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CSCENE_BILLBOARD_H_
#define _CSCENE_BILLBOARD_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "CScene.h"

//=============================================================================
// �O���錾
//=============================================================================
class CCameraManager;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CSceneBillBoard : public CScene
{
public:
	//======================================================
	// �R���X�g���N�^
	// ����: �f�o�C�X�A�v���C�I���e�B
	//======================================================
	CSceneBillBoard(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objType);

	// �f�X�g���N�^
	~CSceneBillBoard();

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �`��
	void Draw(void);

	// �|�W�V�����A�N�Z�T
	D3DXVECTOR3& GetPos(void){ return m_Pos;};
	void		SetPos(D3DXVECTOR3& pos){ m_Pos = pos;};
	void		SetPos(float x,float y, float z){ m_Pos.x = x; m_Pos.y = y; m_Pos.z = z;};

	// ���b�g�A�N�Z�T
	D3DXVECTOR3& GetRot(void){return m_Rot;};
	void		SetRot(D3DXVECTOR3& rot){m_Rot = rot;};
	void		SetRot(float x,float y,float z){ m_Rot.x = x; m_Rot.y = y; m_Rot.z = z;};

	// UV�l�ύX
	void SetTexcordPolygon(D3DXVECTOR2& leftTop, D3DXVECTOR2& rightTop, D3DXVECTOR2& leftBottom, D3DXVECTOR2& rightBottom);
	void SetUV(float left, float right, float top, float bottom);

	//==================================================
	// �N���G�C�g�֐�
	// ����: �f�o�C�X�A���W�A���A�����A�e�N�X�`���[�^�C�v�A�J�����}�l�[�W���[�|�C���^�[
	// �߂�l: �������̃|�C���^
	//==================================================
	static CSceneBillBoard* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType, CCameraManager* pCameraManager, OBJTYPE objType = OBJTYPE_3D);

	// �F�ݒ�֐�
	void SetColorPolygon(D3DXCOLOR& color);

protected:
	//======================================================
	// ������
	// ����: ���W�A���A�����A�e�N�X�`���̎�ށA�J�����}�l�[�W���[�|�C���^�[
	// �߂�l: ����
	//======================================================
	void Init(D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType, CCameraManager* pCameraManager);

	// ���_���f�֐�
	void SetVertexPolygon(void);

	LPDIRECT3DVERTEXDECLARATION9 m_pDecl;
	LPDIRECT3DDEVICE9		*m_pD3DDevice;			// �f�o�C�X�I�u�W�F�N�g(�`��ɕK�v)
	D3DXMATRIX		m_mtxWorld;				// ���[���h�}�g���b�N�X
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVtxBuff;			// ���_���W�����i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	LPDIRECT3DTEXTURE9		m_pD3DTexBuff;			// �e�N�X�`�������i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	D3DXVECTOR3				m_Scl;					// �n�ʂ̑傫���i�X�P�[���j
	float					m_fLength;				// �|���S���̑Ίp���̒���
	float					m_fAngle;				// �|���S���̑Ίp���̊p�x
	CCameraManager			*m_pCameraManager;		// �J�����}�l�[�W���[

};
#endif
//----EOF----