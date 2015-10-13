//=============================================================================
//
// CBackGroundManager�N���X [CBackGroundManager.h]
// Author : �˖{�@�r�F
//
// �Q�[���J�n�̃J�E���g�_�E�����s��
//
//=============================================================================
#ifndef _CBACKGROUND_H_
#define _CBACKGROUND_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../RENDERER/CRenderer.h"
#include "../TEXTURE/CTexture.h"

//=============================================================================
// �萔
//=============================================================================
// �w�i�̃��C���[���i�X�v���C�g�̐��j
static const short BACKGROUND_MAX = 3;

enum BG_DIRECTION{
	BG_DIRECTION_NONE = 0,
	BG_DIRECTION_LEFT,
	BG_DIRECTION_RIGHT,
};

//=============================================================================
// �O���錾
//=============================================================================
class CScene2D;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CBackGroundManager
{
	// ���J�����o
public:
	CBackGroundManager(LPDIRECT3DDEVICE9 *pDevice);
	~CBackGroundManager(void);

	void Init(void);
	void Uninit(void);
	void Update(void);

	// �w�i�����֐�3���܂Ő����\�ōő���𒴂�����false
	bool CreateBG(TEXTURE_TYPE texture);
	// �w�i�����֐�UV�A�j���[�V�����p ���Ɉړ�����ʂ�����
	bool CreateBG(TEXTURE_TYPE texture, float velocityX);

	// ����J�����o
private:
	LPDIRECT3DDEVICE9 *m_pDevice;

	CScene2D *m_pSprite[BACKGROUND_MAX];

	D3DXVECTOR3 m_vPosition[BACKGROUND_MAX];
	float m_fVelocityX[BACKGROUND_MAX];
	bool m_bAnimation[BACKGROUND_MAX];
	BG_DIRECTION m_bDirection[BACKGROUND_MAX];

	// ���݂̖���
	short m_nBackGroundNum;


};

#endif
//----EOF----