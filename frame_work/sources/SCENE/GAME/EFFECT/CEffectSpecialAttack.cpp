//=============================================================================
//
//	�U��������Ԃ̍U���G�t�F�N�g
//
//	Author : �����@�Ȉ�
//
//=============================================================================
#include "CEffectSpecialAttack.h"

#include "../../CSCENE/CSceneAnime.h"

// �f�o�b�O���Ă݂悤
#ifdef _DEBUG
#include "../../../DEBUG_PROC/CDebugProc.h"
#endif

//*****************************************************************************
// �萔
//*****************************************************************************
static const int ATTACK_ATTACK_COUNT_MAX = 30;
static const float ATTACK_ATTACK_WIDTH = 100;
static const float ATTACK_ATTACK_HEIGHT = 100;
static const TEXTURE_TYPE ATTACK_ATTACK_TEXTURE = TEXTURE_FIRE_1;
static const int ATTACK_ATTACK_TEXTURE_X = 10;
static const int ATTACK_ATTACK_TEXTURE_Y = 1;
static const int ATTACK_ATTACK_TEXTURE_LOOP = 10;
static const float ATTACK_ATTACK_RADIUS = 75;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CEffectSpecialAttack::CEffectSpecialAttack(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CEffectSpecialAttack ::~CEffectSpecialAttack(void)
{

}

//*****************************************************************************
// ������
//*****************************************************************************
void CEffectSpecialAttack::Init(D3DXVECTOR3 pos, D3DXVECTOR3 velocity)
{
	m_nCount = 0;
	m_nCountMax = ATTACK_ATTACK_COUNT_MAX;
	m_vPos = pos;
	m_vRot.z = 0.0f;
	m_vVelocity = velocity;
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CEffectSpecialAttack::Uninit(void)
{
	CEffectBase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CEffectSpecialAttack::Update(void)
{
	if (m_nCount % 3 == 0){
		D3DXVECTOR3 pos = m_vPos;
		pos.x += ATTACK_ATTACK_WIDTH * 0.5f * cosf(m_vRot.z * D3DX_PI);
		pos.y -= ATTACK_ATTACK_WIDTH * 0.5f * sinf(m_vRot.z * D3DX_PI);
		CSceneAnime::Create(
			m_pD3DDevice,
			pos,
			ATTACK_ATTACK_WIDTH, ATTACK_ATTACK_HEIGHT,
			ATTACK_ATTACK_TEXTURE, ATTACK_ATTACK_TEXTURE_X, ATTACK_ATTACK_TEXTURE_Y,
			ATTACK_ATTACK_TEXTURE_LOOP);
	}

	m_vRot.z += 0.2831853f;

	// ���E�̍X�V
	CEffectBase::Update();
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CEffectSpecialAttack* CEffectSpecialAttack::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, D3DXVECTOR3 velocity)
{
	// �쐬
	CEffectSpecialAttack* p = new CEffectSpecialAttack(pDevice);

	p->Init(pos, velocity);

	return p;
}

//EOF