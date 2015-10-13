//=============================================================================
//
//	�����G�t�F�N�g
//
//	Author : �����@�Ȉ�
//
//=============================================================================
#include "CEffectDead.h"

#include "../../CSCENE/CSceneAnime.h"

// �f�o�b�O���Ă݂悤
#ifdef _DEBUG
#include "../../../DEBUG_PROC/CDebugProc.h"
#endif

//*****************************************************************************
// �萔
//*****************************************************************************
static const int PLAYER_DEAD_COUNT_MAX = 1;
static const float PLAYER_DEAD_WIDTH = 100;
static const float PLAYER_DEAD_HEIGHT = 100;
static const TEXTURE_TYPE PLAYER_DEAD_TEXTURE = TEXTURE_TREASURE_KIRAKIRA;
static const int PLAYER_DEAD_TEXTURE_X = 8;
static const int PLAYER_DEAD_TEXTURE_Y = 1;
static const int PLAYER_DEAD_TEXTURE_LOOP = 100;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CEffectDead::CEffectDead(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CEffectDead ::~CEffectDead(void)
{

}

//*****************************************************************************
// ������
//*****************************************************************************
void CEffectDead::Init(D3DXVECTOR3 pos)
{
	m_nCount = 0;
	m_nCountMax = PLAYER_DEAD_COUNT_MAX;
	m_vPos = pos;
	m_vPos.y = pos.y - 30.0f;
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CEffectDead::Uninit(void)
{
	CEffectBase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CEffectDead::Update(void)
{
	if (m_nCount == 0){
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos,
			PLAYER_DEAD_WIDTH, PLAYER_DEAD_HEIGHT,
			PLAYER_DEAD_TEXTURE, PLAYER_DEAD_TEXTURE_X, PLAYER_DEAD_TEXTURE_Y,
			PLAYER_DEAD_TEXTURE_LOOP);
	}

	// ���E�̍X�V
	CEffectBase::Update();

#ifdef _DEBUG
	CDebugProc::Print("���쐬�G�t�F�N�g�Ȃ�\n");
#endif
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CEffectDead* CEffectDead::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos)
{
	// �쐬
	CEffectDead* p = new CEffectDead(pDevice);

	p->Init(pos);

	return p;
}

//EOF