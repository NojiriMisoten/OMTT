//=============================================================================
//
//	�����G�t�F�N�g
//
//	Author : �����@�Ȉ�
//
//=============================================================================
#include "CEffectCreateRoad.h"

#include "../../CSCENE/CSceneAnime.h"

// �f�o�b�O���Ă݂悤
#ifdef _DEBUG
#include "../../../DEBUG_PROC/CDebugProc.h"
#endif

//*****************************************************************************
// �萔
//*****************************************************************************
static const int CREATE_ROAD_COUNT_MAX = 1;
static const float CREATE_ROAD_WIDTH = 100;
static const float CREATE_ROAD_HEIGHT = 100;
static const TEXTURE_TYPE CREATE_ROAD_TEXTURE = TEXTURE_TREASURE_KIRAKIRA;
static const int CREATE_ROAD_TEXTURE_X = 8;
static const int CREATE_ROAD_TEXTURE_Y = 1;
static const int CREATE_ROAD_TEXTURE_LOOP = 100;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CEffectCreateRoad::CEffectCreateRoad(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CEffectCreateRoad ::~CEffectCreateRoad(void)
{

}

//*****************************************************************************
// ������
//*****************************************************************************
void CEffectCreateRoad::Init(D3DXVECTOR3 pos)
{
	m_nCount = 0;
	m_nCountMax = CREATE_ROAD_COUNT_MAX;
	m_vPos = pos;
	m_vPos.y = pos.y - 30.0f;
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CEffectCreateRoad::Uninit(void)
{
	CEffectBase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CEffectCreateRoad::Update(void)
{
	if (m_nCount == 0){
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos,
			CREATE_ROAD_WIDTH, CREATE_ROAD_HEIGHT,
			CREATE_ROAD_TEXTURE, CREATE_ROAD_TEXTURE_X, CREATE_ROAD_TEXTURE_Y,
			CREATE_ROAD_TEXTURE_LOOP);
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
CEffectCreateRoad* CEffectCreateRoad::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos)
{
	// �쐬
	CEffectCreateRoad* p = new CEffectCreateRoad(pDevice);

	p->Init(pos);

	return p;
}

//EOF