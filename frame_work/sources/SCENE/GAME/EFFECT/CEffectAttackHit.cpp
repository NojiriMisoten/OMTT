//=============================================================================
//
// CEffect�N���X [CEffect.cpp]
// Author : �˖{�@�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CEffectAttackHit.h"
#include "../../CSCENE/CSceneAnime.h"

// �f�o�b�O���Ă݂悤
#include "../../../DEBUG_PROC/CDebugProc.h"

//*****************************************************************************
// �萔
//*****************************************************************************
static const int ATTACK_HIT_COUNT_MAX = 180;
static const float ATTACK_HIT_WIDTH = 100;
static const float ATTACK_HIT_HEIGHT = 100;
static const TEXTURE_TYPE ATTACK_HIT_TEXTURE = TEXTURE_HIT_SMALL;
static const int ATTACK_HIT_TEXTURE_X = 10;
static const int ATTACK_HIT_TEXTURE_Y = 1;
static const int ATTACK_HIT_TEXTURE_LOOP = 10;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CEffectAttackHit::CEffectAttackHit(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CEffectAttackHit ::~CEffectAttackHit(void)
{

}

//*****************************************************************************
// ������
//*****************************************************************************
void CEffectAttackHit::Init(D3DXVECTOR3 pos)
{
	m_nCount = 0;
	m_nCountMax = ATTACK_HIT_COUNT_MAX;
	m_vPos = pos;
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CEffectAttackHit::Uninit(void)
{
	CEffectBase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CEffectAttackHit::Update(void)
{
	if (m_nCount == 0){
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos,
			ATTACK_HIT_WIDTH, ATTACK_HIT_HEIGHT,
			ATTACK_HIT_TEXTURE, ATTACK_HIT_TEXTURE_X, ATTACK_HIT_TEXTURE_Y,
			ATTACK_HIT_TEXTURE_LOOP);
	}

	// ���E�̍X�V
	CEffectBase::Update();
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CEffectAttackHit* CEffectAttackHit::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos)
{
	// �쐬
	CEffectAttackHit* p = new CEffectAttackHit(pDevice);

	p->Init(pos);

	return p;
}

//----EOF-------