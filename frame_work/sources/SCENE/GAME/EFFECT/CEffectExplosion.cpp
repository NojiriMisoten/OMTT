//=============================================================================
//
//	�����G�t�F�N�g
//
//	Author : �����@�Ȉ�
//
//=============================================================================
#include "CEffectExplosion.h"

#include "../../CSCENE/CSceneAnime.h"

// �f�o�b�O���Ă݂悤
#ifdef _DEBUG
#include "../../../DEBUG_PROC/CDebugProc.h"
#endif

//*****************************************************************************
// �萔
//*****************************************************************************
static const int EXPLOSION_COUNT_MAX = 75;
static const float EXPLOSION_WIDTH = 200;
static const float EXPLOSION_HEIGHT = 200;
static const TEXTURE_TYPE EXPLOSION_TEXTURE = TEXTURE_FIRE_0;
static const int EXPLOSION_TEXTURE_X = 10;
static const int EXPLOSION_TEXTURE_Y = 1;
static const int EXPLOSION_TEXTURE_LOOP = 100;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CEffectExplosion::CEffectExplosion(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CEffectExplosion ::~CEffectExplosion(void)
{

}

//*****************************************************************************
// ������
//*****************************************************************************
void CEffectExplosion::Init(D3DXVECTOR3 pos)
{
	m_nCount = 0;
	m_nCountMax = EXPLOSION_COUNT_MAX;
	m_vPos = pos;
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CEffectExplosion::Uninit(void)
{
	CEffectBase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CEffectExplosion::Update(void)
{
	if (m_nCount == 0){
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos,
			EXPLOSION_WIDTH, EXPLOSION_HEIGHT,
			EXPLOSION_TEXTURE, EXPLOSION_TEXTURE_X, EXPLOSION_TEXTURE_Y,
			EXPLOSION_TEXTURE_LOOP);
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
CEffectExplosion* CEffectExplosion::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos)
{
	// �쐬
	CEffectExplosion* p = new CEffectExplosion(pDevice);

	p->Init(pos);

	return p;
}

//EOF