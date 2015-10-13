//=============================================================================
//
//	�����G�t�F�N�g
//
//	Author : �����@�Ȉ�
//
//=============================================================================
#include "CEffectCaughtTrap.h"

#include "../../CSCENE/CSceneAnime.h"

// �f�o�b�O���Ă݂悤
#ifdef _DEBUG
#include "../../../DEBUG_PROC/CDebugProc.h"
#endif

//*****************************************************************************
// �萔
//*****************************************************************************
static const int TRAP_CAUGHT_COUNT_MAX = 300;
static const float TRAP_CAUGHT_WIDTH = 50;
static const float TRAP_CAUGHT_HEIGHT = 50;
static const TEXTURE_TYPE TRAP_CAUGHT_TEXTURE = TEXTURE_SLOW;
static const int TRAP_CAUGHT_TEXTURE_X = 10;
static const int TRAP_CAUGHT_TEXTURE_Y = 1;
static const int TRAP_CAUGHT_TEXTURE_LOOP = 100;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CEffectCaughtTrap::CEffectCaughtTrap(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CEffectCaughtTrap ::~CEffectCaughtTrap(void)
{

}

//*****************************************************************************
// ������
//*****************************************************************************
void CEffectCaughtTrap::Init(D3DXVECTOR3 pos)
{
	m_nCount = 0;
	m_nCountMax = TRAP_CAUGHT_COUNT_MAX;
	m_vPos = pos;
	m_vPos.y = pos.y-30.0f;
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CEffectCaughtTrap::Uninit(void)
{
	CEffectBase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CEffectCaughtTrap::Update(void)
{
	if (m_nCount %100 == 0){
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos,
			TRAP_CAUGHT_WIDTH, TRAP_CAUGHT_HEIGHT,
			TRAP_CAUGHT_TEXTURE, TRAP_CAUGHT_TEXTURE_X, TRAP_CAUGHT_TEXTURE_Y,
			TRAP_CAUGHT_TEXTURE_LOOP);
	}

	// ���E�̍X�V
	CEffectBase::Update();

#ifdef _DEBUG
	CDebugProc::Print("�����G�t�F�N�g�Ȃ�\n");
#endif
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CEffectCaughtTrap* CEffectCaughtTrap::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos)
{
	// �쐬
	CEffectCaughtTrap* p = new CEffectCaughtTrap(pDevice);

	p->Init(pos);

	return p;
}

//EOF