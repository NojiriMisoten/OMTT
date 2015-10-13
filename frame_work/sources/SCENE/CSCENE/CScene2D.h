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
#include "../../MAIN/main.h"
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
	CScene2D(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_0, OBJTYPE m_objType = OBJTYPE_2D);

	// �f�X�g���N�^
	~CScene2D();

	//=======================================================================
	// �������֐�
	// ����:���W�A���A�����A�e�N�X�`���̎�ށA�i�ȉ��K�v�Ȃ��j�i���������A�c�������j
	// �߂�l:�쐬�����A�h���X
	//=======================================================================
	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock = 1, int hblock = 1);

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �`��
	void Draw(void);

	// �|�W�V�����A�N�Z�T
	D3DXVECTOR3 GetPos(void){ return m_vPos; };
	void		SetPos(D3DXVECTOR3 pos);
	void		SetPos(float x, float y, float z){ m_vPos.x = x; m_vPos.y = y; m_vPos.z = z; };
	void		AddPos(D3DXVECTOR3 pos){ m_vPos += pos; };

	// ���b�g�A�N�Z�T
	D3DXVECTOR3 GetRot(void){ return m_vRot; };
	void		SetRot(D3DXVECTOR3 rot);
	void		SetRot(float x, float y, float z){ m_vRot.x = x; m_vRot.y = y; m_vRot.z = z; };

	// �T�C�Y�A�N�Z�T
	float GetWidth(void){ return m_fWidth; };
	void SetWidth(float width){ m_fWidth = width; };
	float GetHeight(void){ return m_fHeight; };
	void SetHeight(float height){ m_fHeight = height; };

	//=======================================================================
	// �T�C�Y�Əꏊ�ύX�֐�
	// ����:���W�A���A����
	//=======================================================================
	void SetVertexPolygon(D3DXVECTOR3 pos, float width, float height);

	//=======================================================================
	// ���ύX�֐�_������_�i�E���L�т�j
	// ����:����������
	//=======================================================================
	void AddWidth_BaseLeft(float addWidth);

	//=======================================================================
	// ���ύX�֐�_�E����_�i�����L�т�j
	// ����:����������
	//=======================================================================
	void AddWidth_BaseRight(float addWidth);

	//=======================================================================
	// ���ύX�֐�_������_�i�㑤�L�т�j
	// ����:������������
	//=======================================================================
	void AddHeight_BaseBottom(float addHeight);

	//=======================================================================
	// ���ύX�֐�_�㑤��_�i�����L�т�j
	// ����:������������
	//=======================================================================
	void AddHeight_BaseTop(float addHeight);

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
	// UV�l90�x��]�֐�
	//=======================================================================
	void Rot90_UV(void);

	//=======================================================================
	// �����`��t���O�Z�b�g�֐�
	// ����:�^�U�l
	//=======================================================================
	void SetDrawFlag(bool flag){ m_AutoDraw = flag; };

	//=======================================================================
	// �e�N�X�`���[�ύX�֐�
	// ����:�e�N�X�`���[�̎��
	//=======================================================================
	void ChangeTexture(TEXTURE_TYPE texType){ m_pD3DTexBuff = CTexture::GetTexture(texType); };

	//=======================================================================
	// �N���G�C�g�֐�
	// ����:�f�o�C�X�A���W�A���A�����A�e�N�X�`���̎��
	// �߂�l:�쐬�����A�h���X
	//=======================================================================
	static CScene2D* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int nPriority = TYPE_PRIORITY_0);

	//=======================================================================
	// ���_���W�擾�֐�
	// ����:���_�ԍ�
	// �߂�l:���W
	//=======================================================================
	D3DXVECTOR2* GetVertexPos(int idx){ return &m_vtxPos[idx]; };

	//=======================================================================
	// �����蔻��p���_���W�擾�֐�
	// ����:���_�ԍ�
	// �߂�l:���W
	//=======================================================================
	D3DXVECTOR2* GetVertexJudgePos(int idx){ return &m_vtxJudgePos[idx]; };

	D3DXVECTOR3 GetJudgePos(void){ return m_vJudgePos; };
	float GetJudgeWidth(void){ return m_fJudgeWidth; };
	float GetJudgeHeight(void){ return m_fJudgeHeight; };

private:
	static const int VERTEX_NUM = 4;	// ���_�̐�
	bool		m_AutoDraw;				// �����ŕ`�悷�邩
	D3DXVECTOR2	m_vtxPos[VERTEX_NUM];	// ���_�̍��W
	D3DXVECTOR2	m_vtxJudgePos[VERTEX_NUM];	// ���_�̍��W

protected:
	// ���_���Z�b�g���[���h�ϊ��֐�
	void SetVertexPolygon(void);

	LPDIRECT3DDEVICE9		*m_pD3DDevice;			// �f�o�C�X�I�u�W�F�N�g(�`��ɕK�v)
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVtxBuff;			// ���_���W�����i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	LPDIRECT3DTEXTURE9		m_pD3DTexBuff;			// �e�N�X�`�������i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	D3DXVECTOR3	m_vPos;								// �|���S���̒����̈ʒu
	D3DXVECTOR3	m_vRot;								// �|���S���̉�]
	float		m_fWidth;							// ��
	float		m_fHeight;							// ����
	// �����蔻��p�̃p�����[�^�[
	D3DXVECTOR3 m_vJudgePos;
	float m_fJudgeWidth;
	float m_fJudgeHeight;
};
#endif
//----EOF----