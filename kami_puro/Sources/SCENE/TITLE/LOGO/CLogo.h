//=============================================================================
//
// CLogo�N���X [CLogo.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CLOGO_H_
#define _CLOGO_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../../BASE_OBJECT/CScene2D.h"
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CLogo : public CScene2D
{
public:
	//=======================================================================
	// �R���X�g���N�^
	// ����:�f�o�C�X�A�v���C�I���e�B�A�I�u�W�F�^�C�v
	//=======================================================================
	CLogo(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objType = OBJTYPE_2D);

	// �f�X�g���N�^
	~CLogo();

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
	void DrawUIRender(void);

	//=======================================================================
	// �N���G�C�g�֐�
	// ����:�f�o�C�X�A���W�A���A�����A�e�N�X�`���̎��
	// �߂�l:�쐬�����A�h���X
	//=======================================================================
	static CLogo* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType);

	void StartFadeIN(int maxFadeInFrame);
	void StartFadeOUT(int maxFadeInFrame);
	void SetGrayScalColor(D3DXVECTOR3 color) { m_GrayScal = color; };
private:
	void UpdateFade(void);
	
	float	m_Alpha;
	bool	m_isFade;
	float	m_FadeTime;
	float	m_FadeSpd;
	D3DXVECTOR3 m_GrayScal;
	bool	m_isFadeIN;
};
#endif
//----EOF----