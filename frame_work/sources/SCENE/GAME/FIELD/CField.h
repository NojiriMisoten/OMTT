//=============================================================================
//
// CField�N���X [CField.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CFIELD_H_
#define _CFIELD_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../CSCENE/CScene2D.h"
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CField : public CScene2D
{
public:
	//=======================================================================
	// �R���X�g���N�^
	// ����:�f�o�C�X�A�v���C�I���e�B�A�I�u�W�F�^�C�v
	//=======================================================================
	CField(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_FIELD, OBJTYPE m_objType = OBJTYPE_FIELD);

	// �f�X�g���N�^
	~CField();

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �`��
	void Draw(void);

	// �f�X
	void Death(void){ Release(); };

	// �|�W�V�����A�N�Z�T
	D3DXVECTOR3 GetPos(void){ return m_vPos; };
	void		SetPos(D3DXVECTOR3 pos){ m_vPos = pos; };
	void		SetPos(float x, float y, float z){ m_vPos.x = x; m_vPos.y = y; m_vPos.z = z; };

	// ���b�g�A�N�Z�T
	D3DXVECTOR3 GetRot(void){ return m_vRot; };
	void		SetRot(D3DXVECTOR3 rot){ m_vRot = rot; };
	void		SetRot(float x, float y, float z){ m_vRot.x = x; m_vRot.y = y; m_vRot.z = z; };

	// �T�C�Y�A�N�Z�T
	float GetWidth(void){ return m_fWidth; };
	void SetWidth(float width){ m_fWidth = width; };
	float GetHeight(void){ return m_fHeight; };
	void SetHeight(float height){ m_fHeight = height; };

	// ����Ă�
	void Ride(bool ride);

	//=======================================================================
	// �N���G�C�g�֐�
	// ����:�f�o�C�X�A���W�A���A�����A�e�N�X�`���̎��
	// �߂�l:�쐬�����A�h���X
	//=======================================================================
	static CField* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType);

private:
	//=======================================================================
	// �������֐�
	// ����:���W�A���A�����A�e�N�X�`���̎�ށA�i�ȉ��K�v�Ȃ��j�i���������A�c�������j
	// �߂�l:�쐬�����A�h���X
	//=======================================================================
	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock = 1, int hblock = 1);

	// �����蔻��X�e�[�^�X�Z�b�g�֐�
	void SetJudgeParam(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType);

	D3DXVECTOR3 m_vDefaultPos;
	D3DXVECTOR3 m_vDestPos;
protected:

};
#endif
//----EOF----