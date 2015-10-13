//=============================================================================
//
// CEffect�N���X [CEffect.cpp]
// Author : �˖{�@�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CEffectMetamorphose.h"
#include "../../CSCENE/CSceneAnime.h"

// �f�o�b�O���Ă݂悤
#include "../../../DEBUG_PROC/CDebugProc.h"

//*****************************************************************************
// �萔
//*****************************************************************************
static const int METAMORPHOSE_COUNT_MAX = 10;
static const float METAMORPHOSE_WIDTH = 100;
static const float METAMORPHOSE_HEIGHT = 100;
//static const TEXTURE_TYPE METAMORPHOSE_TEXTURE = TEXTURE_HIT_SMALL;
static const int METAMORPHOSE_TEXTURE_X = 5;
static const int METAMORPHOSE_TEXTURE_Y = 2;
static const int METAMORPHOSE_TEXTURE_LOOP = 10;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CEffectMetamorphose::CEffectMetamorphose(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CEffectMetamorphose ::~CEffectMetamorphose(void)
{

}

//*****************************************************************************
// ������
//*****************************************************************************
void CEffectMetamorphose::Init(D3DXVECTOR3 pos, short playerNum)
{
	m_nCount = 0;
	m_nCountMax = METAMORPHOSE_COUNT_MAX;
	m_vPos = pos;

	switch (playerNum)
	{
	case 0:
		m_Texture = TEXTURE_PLAYER_ETAMORPHOSE_YELLOW;
		break;
	case 1:
		m_Texture = TEXTURE_PLAYER_ETAMORPHOSE_RED;
		break;
	case 2:
		m_Texture = TEXTURE_PLAYER_ETAMORPHOSE_BLUE;
		break;
	case 3:
		m_Texture = TEXTURE_PLAYER_ETAMORPHOSE_GREEN;
		break;
	default:
		break;
	}
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CEffectMetamorphose::Uninit(void)
{
	CEffectBase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CEffectMetamorphose::Update(void)
{
	if (m_nCount == 0){
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos,
			METAMORPHOSE_WIDTH, METAMORPHOSE_HEIGHT,
			m_Texture, METAMORPHOSE_TEXTURE_X, METAMORPHOSE_TEXTURE_Y,
			METAMORPHOSE_TEXTURE_LOOP);
	}

	// ���E�̍X�V
	CEffectBase::Update();
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CEffectMetamorphose* CEffectMetamorphose::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos , short playerNum)
{
	// �쐬
	CEffectMetamorphose* p = new CEffectMetamorphose(pDevice);

	p->Init(pos, playerNum);

	return p;
}

//----EOF-------