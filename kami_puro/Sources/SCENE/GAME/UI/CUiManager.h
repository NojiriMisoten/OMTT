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
class CManager;
class CGame;
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CUiManager
{
public:

	CUiManager(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager);
	~CUiManager(void);

	void Update();
	void Uninit();

	// �Q�[���J�n�̃A�j���[�V����������֐�
	// �I������J�E���g������
	void StartAnimation(int interval);

	// �쐬
	static CUiManager *Create(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager, CGame *pGame);

	// �������̊�
	CFace *GetFace()
	{
		return m_pFace;
	}
	// HP�o�[
	CHpBar *GetHpBar()
	{
		return m_pHpBar;
	}
	// �ϋq�o�[
	CCrowdBar *GetCrowdBar()
	{
		return m_pCrowdBar;
	}

	CGame* GetGame(void)
	{
		return m_pGame;
	}

private:
	// ������
	void Init(CGame *pGame);

	// ��������
	CCountTime *m_pTimer;
	// �ϋq�Q�[�W
	CCrowdBar *m_pCrowdBar;
	// HP�o�|
	CHpBar *m_pHpBar;
	// �������̊�
	CFace *m_pFace;
	// �X�^�~�i�o�[ �������ĂȂ�
	CStaminaBar *m_pStaminaBarL;
	CStaminaBar *m_pStaminaBarR;
	// �f�o�C�X�̕ۑ�
	LPDIRECT3DDEVICE9 *m_pDevice;
	// �}�l�[�W���[
	CManager *m_pManager;
	CGame	*m_pGame;
};

#endif
//----EOF----