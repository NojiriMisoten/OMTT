//=============================================================================
//
// CSceneNumber�N���X [CSceneNumber.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CSCENENUMBER_H_
#define _CSCENENUMBER_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../MAIN/main.h"
#include "../../TEXTURE/CTexture.h"
#include "CScene.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CSceneNumber : public CScene
{
public:
	//======================================================
	// �R���X�g���N�^
	// ����: �f�o�C�X
	//======================================================
	CSceneNumber(LPDIRECT3DDEVICE9 *pDevice, int nPriority = OBJTYPE_SCORE);

	// �f�X�g���N�^
	~CSceneNumber();

	//======================================================
	// ������
	// ����: ���W�A���A�����A�����A�e�N�X�`���[�^�C�v
	//======================================================
	void Init(D3DXVECTOR3 pos, float width, float height, int number, TEXTURE_TYPE texType);

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �`��
	void Draw(void);

	//======================================================
	// �����ύX�֐�
	// ����: ����
	//======================================================
	void SetNumber(int number);

	//======================================================
	// �`�悷�邩
	// ����: ���邩�ǂ���
	//======================================================
	void SetDrawFlag(bool flag){m_bDraw = flag;};

	// �|�W�V�����A�N�Z�T
	D3DXVECTOR3 GetPos(void){ return m_vPos;};
	void		SetPos(D3DXVECTOR3 pos);
	void		SetPos(float x,float y, float z){ m_vPos.x = x; m_vPos.y = y; m_vPos.z = z;};

	// ���b�g�A�N�Z�T
	D3DXVECTOR3 GetRot(void){return m_vRot;};
	void		SetRot(D3DXVECTOR3 rot){m_vRot = rot;};
	void		SetRot(float x,float y,float z){ m_vRot.x = x; m_vRot.y = y; m_vRot.z = z;};

	//=================================================
	// �N���G�C�g�֐�
	// ����: �f�o�C�X�A���W�A���A�����A�����A�e�N�X�`���^�C�v
	// �߂�l: �������̃A�h���X
	//=================================================
	static CSceneNumber* CSceneNumber::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, int number, TEXTURE_TYPE texType);
private:
	// �g���ĂȂ�������
	HRESULT		Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock = 1, int hblock = 1){return E_FAIL;};
	
	LPDIRECT3DDEVICE9		*m_pD3DDevice;			// �f�o�C�X�I�u�W�F�N�g(�`��ɕK�v)
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVtxBuff;			// ���_���W�����i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	LPDIRECT3DTEXTURE9		m_pD3DTexBuff;			// �e�N�X�`�������i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	D3DXVECTOR3				m_vPos;					// �|���S���̒����̈ʒu
	D3DXVECTOR3				m_vRot;					// �|���S���̉�]
	float					m_fWidth;				// ��
	float					m_fHeight;				// ����
	bool					m_bDraw;				// �`�悷�邩
};

#endif
//----EOF----