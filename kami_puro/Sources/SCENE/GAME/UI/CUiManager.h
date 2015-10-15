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
	CCountTime *m_Timer;
	// �X�^�~�i�o�[
	CStaminaBar *m_StaminaBarL;
	CStaminaBar *m_StaminaBarR;
	// �ϋq�Q�[�W
	CCrowdBar *m_CrowdBar;
	// HP�o�|
	CHpBar *m_HpBarL;
	CHpBar *m_HpBarR;
	// �������̊�
	CFace *m_Face;
	// �f�o�C�X�̕ۑ�
	LPDIRECT3DDEVICE9 *m_pDevice;

	bool m_isAnimation;
	int m_AnimationCount;
};

#endif
//----EOF----