//=============================================================================
//
// CScene2D�N���X [CScene2D.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CSCENE2D_H_
#define _CSCENE2D_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "CScene.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CScene2D : public CScene
{
public:
	//=======================================================================
	// �R���X�g���N�^
	// ����:�f�o�C�X�A�v���C�I���e�B�A�I�u�W�F�^�C�v
	//=======================================================================
	CScene2D(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objType = OBJTYPE_2D);

	// �f�X�g���N�^
	~CScene2D();

	//=======================================================================
	// �������֐�
	// ����:���W�A���A�����A�e�N�X�`���̎�ށA�i�ȉ��K�v�Ȃ��j�i���������A�c�������j
	// �߂�l:�쐬�����A�h���X
	//=======================================================================
	HRESULT Init(D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType, int wblock = 1, int hblock = 1);

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �`��
	void DrawUI(void);

	// �|�W�V�����A�N�Z�T
	D3DXVECTOR3& GetPos(void){ return m_Pos;};
	void		SetPos(D3DXVECTOR3& pos);
	void		SetPos(float x,float y, float z){ m_Pos.x = x; m_Pos.y = y; m_Pos.z = z;};

	// ���b�g�A�N�Z�T
	D3DXVECTOR3& GetRot(void){return m_Rot;};
	void		SetRot(D3DXVECTOR3& rot);
	void		SetRot(float x,float y,float z){ m_Rot.x = x; m_Rot.y = y; m_Rot.z = z;};

	// �T�C�Y�A�N�Z�T
	float GetWidth(void){return m_fWidth;};
	void SetWidth(float width){m_fWidth = width;};
	float GetHeight(void){return m_fHeight;};
	void SetHeight(float height){m_fHeight = height;};
	
	//=======================================================================
	// �T�C�Y�Əꏊ�ύX�֐�
	// ����:���W�A���A����
	//=======================================================================
	void SetVertexPolygon(D3DXVECTOR3 pos, float width, float height);

	//=======================================================================
	// �F�ύX�֐�
	// ����:�F
	//=======================================================================
	void SetColorPolygon(D3DXCOLOR color);

	//=======================================================================
	// UV�l�ύX�֐�
	// ����:UV�l�\���̃|�C���^
	//=======================================================================
	void SetUV(UV_INDEX *pUVIndex);

	//=======================================================================
	// �����`��t���O�Z�b�g�֐�
	// ����:�^�U�l
	//=======================================================================
	void SetDrawFlag(bool flag){m_AutoDraw = flag;};

	//=======================================================================
	// �e�N�X�`���[�ύX�֐�
	// ����:�e�N�X�`���[�̎��
	//=======================================================================
	void ChangeTexture(TEXTURE_TYPE texType){m_pD3DTexBuff = CTexture::GetTexture(texType);};

	//=======================================================================
	// �N���G�C�g�֐�
	// ����:�f�o�C�X�A���W�A���A�����A�e�N�X�`���̎��
	// �߂�l:�쐬�����A�h���X
	//=======================================================================
	static CScene2D* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType);

private:
	float		m_fLength;				// �|���S���̑Ίp���̒���
	float		m_fAngle;				// �|���S���̑Ίp���̊p�x
	float		m_fWidth;				// ��
	float		m_fHeight;				// ����
	bool		m_AutoDraw;				// �����ŕ`�悷�邩

protected:
	// ���_���Z�b�g�֐�
	void SetVertexPolygon(void);

	LPDIRECT3DDEVICE9		*m_pD3DDevice;			// �f�o�C�X�I�u�W�F�N�g(�`��ɕK�v)
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVtxBuff;			// ���_���W�����i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	LPDIRECT3DTEXTURE9		m_pD3DTexBuff;			// �e�N�X�`�������i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
};
#endif
//----EOF----