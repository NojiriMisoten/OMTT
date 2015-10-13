//=============================================================================
//
// CEffect�N���X [CEffect.cpp]
// Author : �˖{�@�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CEffectTrapFoundation.h"
#include "../../CSCENE/CSceneAnime.h"
#include "../../../MANAGER/CManager.h"

// �f�o�b�O���Ă݂悤
#include "../../../DEBUG_PROC/CDebugProc.h"

//*****************************************************************************
// �萔
//*****************************************************************************
static const int TRAP_FOUNDATION_COUNT_MAX = 300;
static const float TRAP_FOUNDATION_WIDTH = 100;
static const float TRAP_FOUNDATION_HEIGHT = 100;
static const TEXTURE_TYPE TRAP_FOUNDATION_TEXTURE = TEXTURE_TRAP;
static const int TRAP_FOUNDATION_TEXTURE_X = 2;
static const int TRAP_FOUNDATION_TEXTURE_Y = 2;
static const int TRAP_FOUNDATION_TEXTURE_LOOP = 300;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CEffectTrapFoundation::CEffectTrapFoundation(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CEffectTrapFoundation ::~CEffectTrapFoundation(void)
{

}

//*****************************************************************************
// ������
//*****************************************************************************
void CEffectTrapFoundation::Init(D3DXVECTOR3 pos, short playerNum)
{
	m_nCount = 0;
	m_nCountMax = TRAP_FOUNDATION_COUNT_MAX;
	m_vPos = pos;

	m_pAnim = CSceneAnime::Create(
		m_pD3DDevice,
		m_vPos,
		TRAP_FOUNDATION_WIDTH, TRAP_FOUNDATION_HEIGHT,
		TRAP_FOUNDATION_TEXTURE, TRAP_FOUNDATION_TEXTURE_X, TRAP_FOUNDATION_TEXTURE_Y,
		TRAP_FOUNDATION_TEXTURE_LOOP);

	// �v���C���[�J���[�ɕύX
	m_pAnim->SetColorPolygon(PLAYER_COLOR[playerNum]);
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CEffectTrapFoundation::Uninit(void)
{
	CEffectBase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CEffectTrapFoundation::Update(void)
{
	// ���E�̍X�V
	CEffectBase::Update();
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CEffectTrapFoundation* CEffectTrapFoundation::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, short playerNum)
{
	// �쐬
	CEffectTrapFoundation* p = new CEffectTrapFoundation(pDevice);

	p->Init(pos,playerNum);

	return p;
}

//----EOF-------