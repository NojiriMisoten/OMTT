//=============================================================================
//
// CEffect�N���X [CEffect.cpp]
// Author : �˖{�@�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CEffectFlagHold.h"
#include "../../CSCENE/CSceneAnime.h"

// �f�o�b�O���Ă݂悤
#include "../../../DEBUG_PROC/CDebugProc.h"

//*****************************************************************************
// �萔
//*****************************************************************************
static const int FLAG_HOLD_COUNT_MAX = 180;
static const float FLAG_HOLD_WIDTH = 100;
static const float FLAG_HOLD_HEIGHT = 100;
static const TEXTURE_TYPE FLAG_HOLD_TEXTURE = TEXTURE_HIT_SMALL;
static const int FLAG_HOLD_TEXTURE_X = 10;
static const int FLAG_HOLD_TEXTURE_Y = 1;
static const int FLAG_HOLD_TEXTURE_LOOP = 10;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CEffectFlagHold::CEffectFlagHold(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CEffectFlagHold ::~CEffectFlagHold(void)
{

}

//*****************************************************************************
// ������
//*****************************************************************************
void CEffectFlagHold::Init(D3DXVECTOR3 pos)
{
	m_nCount = 0;
	m_nCountMax = FLAG_HOLD_COUNT_MAX;
	m_vPos = pos;
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CEffectFlagHold::Uninit(void)
{
	CEffectBase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CEffectFlagHold::Update(void)
{
	if (m_nCount == 0){
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos,
			FLAG_HOLD_WIDTH, FLAG_HOLD_HEIGHT,
			FLAG_HOLD_TEXTURE, FLAG_HOLD_TEXTURE_X, FLAG_HOLD_TEXTURE_Y,
			FLAG_HOLD_TEXTURE_LOOP);
	}

	// ���E�̍X�V
	CEffectBase::Update();
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CEffectFlagHold* CEffectFlagHold::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos)
{
	// �쐬
	CEffectFlagHold* p = new CEffectFlagHold(pDevice);

	p->Init(pos);

	return p;
}

//----EOF-------