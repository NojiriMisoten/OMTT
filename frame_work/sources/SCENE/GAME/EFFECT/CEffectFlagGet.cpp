//=============================================================================
//
// CEffect�N���X [CEffect.cpp]
// Author : �˖{�@�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CEffectFlagGet.h"
#include "../../CSCENE/CSceneAnime.h"

// �f�o�b�O���Ă݂悤
#include "../../../DEBUG_PROC/CDebugProc.h"

//*****************************************************************************
// �萔
//*****************************************************************************
static const int FLAG_GET_COUNT_MAX = 180;
static const float FLAG_GET_WIDTH = 100;
static const float FLAG_GET_HEIGHT = 100;
static const TEXTURE_TYPE FLAG_GET_TEXTURE = TEXTURE_HIT_SMALL;
static const int FLAG_GET_TEXTURE_X = 10;
static const int FLAG_GET_TEXTURE_Y = 1;
static const int FLAG_GET_TEXTURE_LOOP = 10;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CEffectFlagGet::CEffectFlagGet(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CEffectFlagGet ::~CEffectFlagGet(void)
{

}

//*****************************************************************************
// ������
//*****************************************************************************
void CEffectFlagGet::Init(D3DXVECTOR3 pos)
{
	m_nCount = 0;
	m_nCountMax = FLAG_GET_COUNT_MAX;
	m_vPos = pos;
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CEffectFlagGet::Uninit(void)
{
	CEffectBase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CEffectFlagGet::Update(void)
{
	if (m_nCount == 0){
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos,
			FLAG_GET_WIDTH, FLAG_GET_HEIGHT,
			FLAG_GET_TEXTURE, FLAG_GET_TEXTURE_X, FLAG_GET_TEXTURE_Y,
			FLAG_GET_TEXTURE_LOOP);
	}

	// ���E�̍X�V
	CEffectBase::Update();
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CEffectFlagGet* CEffectFlagGet::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos)
{
	// �쐬
	CEffectFlagGet* p = new CEffectFlagGet(pDevice);

	p->Init(pos);

	return p;
}

//----EOF-------