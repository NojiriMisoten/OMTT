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
#include "CCutIn.h"
#include "COverLay.h"
#include "CRopeTimer.h"
#include "CCountTime.h"
//*****************************************************************************
// �O���錾
//*****************************************************************************
class CStaminaBar;
class CCountTime;
class CCrowdBar;
class CHpBar;
class CManager;
class CGame;
class CCommandChartManager;
class CCutIn;
class CBattleFade;
class COverLay;
class CForcusLine;

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

	// ������
	void Init(CGame *pGame);

	// �Q�[���J�n�̃A�j���[�V����������֐�
	// �I������J�E���g������
	void StartAnimation(int interval);

	// �J�b�g�C���J�n
	void StartCutIn(int ID, CutInType type) { m_pCutIn->Start(ID, type); };

	// �쐬
	//static CUiManager *Create(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager, CGame *pGame);

	CCommandChartManager *GetCommandChartManager(void) { return m_pCommandChartManager; };

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

	void StartRopeTimer( int openFrameCount, int timerFrameCount ) { m_pRopeTimer->Start( openFrameCount, timerFrameCount ); };

	// �o�g���t�F�[�h�X�^�[�g
	void StartBattleFade(void);

	// �I�[�o�[���C�X�^�[�g
	void StartOverLay(COverLay::Data *data);

	// �W�������o��
	// ��1�����F�\������t���[����
	// ��2�����Ftrue�Ȃ�h��ȃe�N�X�`���ɂȂ� false�Ȃ甒��
	void StartForcusLine(int timerFrameCount, bool isFinish);

	void SetImvisible(void);
	void SetVisible(void);

	// ���̃A�j���\�V���������������
	void ChainAnimeStart(void) { m_pTimer->ChainAnimeStart(); };
	void ChainAnimeStop(void) { m_pTimer->ChainAnimeStop(); };

private:
	// ��������R�₷�Ǘ��̍X�V
	void UpdateFireOldMan();

	// ��������
	CCountTime *m_pTimer;
	// �ϋq�Q�[�W
	CCrowdBar *m_pCrowdBar;
	// HP�o�|
	CHpBar *m_pHpBar;
	// �X�^�~�i�o�[ �������ĂȂ�
	CStaminaBar *m_pStaminaBarL;
	CStaminaBar *m_pStaminaBarR;
	// �J�b�g�C��
	CCutIn *m_pCutIn;
	// �o�g���t�F�[�h
	CBattleFade *m_pBattleFade;
	// READY�Ƃ��\������悤
	COverLay *m_pOverLay;
	// ���[�v�^�C�}�[
	CRopeTimer *m_pRopeTimer;
	// �W����
	CForcusLine *m_pForcusLine;

	// �f�o�C�X�̕ۑ�
	LPDIRECT3DDEVICE9 *m_pDevice;
	// �}�l�[�W���[
	CManager *m_pManager;
	CGame	*m_pGame;
	CCommandChartManager* m_pCommandChartManager;
};

#endif
//----EOF----