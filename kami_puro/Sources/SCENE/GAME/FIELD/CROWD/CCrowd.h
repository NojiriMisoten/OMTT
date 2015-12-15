//=============================================================================
//
// CCrowd�N���X [CCrowd.h]
// Author : �˖{�r�F
//
//=============================================================================
#ifndef _CCROWD_H_
#define _CCROWD_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../../BASE_OBJECT/CScene3D.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CManager;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CCrowd : public CScene3D
{
public:
	//======================================================
	// �R���X�g���N�^
	// ����: �f�o�C�X�A�v���C�I���e�B�A�I�u�W�F�^�C�v
	//======================================================
	CCrowd(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE m_objType = OBJTYPE_3D);

	// �f�X�g���N�^
	~CCrowd();

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �`��
	void DrawNormalRender(void);

	//=================================================
	// �N���G�C�g�֐�
	// ����: �f�o�C�X�A���W�A���A�����A�u���b�N���A�e�N�X�`���^�C�v
	// �߂�l: �������̃A�h���X
	//=================================================
	static CCrowd* Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos, float width, float height,
		TEXTURE_TYPE texType,
		CManager* pManager);

	void SetTextureOffsetV(float v)
	{
		m_TextureOffsetV = v;
	}
private:
	//======================================================
	// ������
	// ����: ���W�A���A�����A�e�N�X�`���[�^�C�v�A(���������A�c������)
	// �߂�l: ����
	//======================================================
	void Init(D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType, CManager* pManager);

	// �ۑ����Ƃ������|�C���^
	LPDIRECT3DDEVICE9		*m_pD3DDevice;			// �f�o�C�X�I�u�W�F�N�g(�`��ɕK�v)
	CManager				*m_pManager;

	// �e�N�X�`���̃I�t�Z�b�g�l�iUV��V�����j
	float m_TextureOffsetV;

};

#endif
//----EOF----