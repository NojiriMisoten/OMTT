//=============================================================================
//
// CUiManager�N���X [CUiManager.h]
// Author : �˖{�r�F
//
// �Q�[���V�[�����ł�UI���܂Ƃ߂��Ǘ��֐�
//
//
//=============================================================================
#ifndef _CUIMANAGER_H_
#define _CUIMANAGER_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../RENDERER/CRenderer.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CStaminaBar;
class CCountTime;
class CCrowdBar;
class CHpBar;
class CFace;
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CUiManager
{
public:

	CUiManager(LPDIRECT3DDEVICE9 *pDevice);
	~CUiManager(void);

	void Update();
	void Uninit();

	// �Q�[���J�n�̃A�j���[�V����������֐�
	void StartAnimation();

	// �쐬
	static CUiManager *Create(LPDIRECT3DDEVICE9 *pDevice);

private:
	// ������
	void Init();

	// ��������
	CCountTime *m_pTimer;
	// �X�^�~�i�o�[
	CStaminaBar *m_pStaminaBarL;
	CStaminaBar *m_pStaminaBarR;
	// �ϋq�Q�[�W
	CCrowdBar *m_pCrowdBar;
	// HP�o�|
	CHpBar *m_pHpBarL;
	CHpBar *m_pHpBarR;
	// �������̊�
	CFace *m_pFace;
	// �f�o�C�X�̕ۑ�
	LPDIRECT3DDEVICE9 *m_pDevice;

	bool m_isAnimation;
	int m_AnimationCount;
};

#endif
//----EOF----