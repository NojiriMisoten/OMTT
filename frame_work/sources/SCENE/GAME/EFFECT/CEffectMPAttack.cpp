//=============================================================================
//
//	�����G�t�F�N�g
//
//	Author : �����@�Ȉ�
//
//=============================================================================
#include "CEffectMPAttack.h"

#include "../../CSCENE/CSceneAnime.h"

// �f�o�b�O���Ă݂悤
#ifdef _DEBUG
#include "../../../DEBUG_PROC/CDebugProc.h"
#endif

//*****************************************************************************
// �萔
//*****************************************************************************
static const int MP_ATTACK_COUNT_MAX = 29;
static const float MP_ATTACK_WIDTH = 30;
static const float MP_ATTACK_HEIGHT = 30;
static const TEXTURE_TYPE MP_ATTACK_TEXTURE = TEXTURE_THREAD_POISON;
static const int MP_ATTACK_TEXTURE_X = 2;
static const int MP_ATTACK_TEXTURE_Y = 4;
static const int MP_ATTACK_END_TIME = 30;
// ���x
static const float ATTACK_JAMMER_SPD = 10;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CEffectMPAttack::CEffectMPAttack(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
	m_pAnim = NULL;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CEffectMPAttack ::~CEffectMPAttack(void)
{

}

//*****************************************************************************
// ������
//*****************************************************************************
void CEffectMPAttack::Init(D3DXVECTOR3 pos, D3DXVECTOR3 velocity)
{
	m_nCount = 0;
	m_nCountMax = MP_ATTACK_COUNT_MAX;
	m_vPos = pos;
	m_vPos.y = pos.y - 30.0f;
	m_vVelocity = velocity;
	m_vRot.z = 0.0f;

	m_pAnim = CSceneAnime::Create(
		m_pD3DDevice,
		m_vPos,
		MP_ATTACK_WIDTH, MP_ATTACK_HEIGHT,
		MP_ATTACK_TEXTURE, MP_ATTACK_TEXTURE_X, MP_ATTACK_TEXTURE_Y,
		MP_ATTACK_END_TIME);

	if (m_vVelocity.y == 1)
	{
		m_vRot.z = D3DX_PI*0.5f;
		m_pAnim->SetRot(m_vRot);
	}
	if (m_vVelocity.y == -1)
	{
		m_vRot.z = D3DX_PI*1.5f;
		m_pAnim->SetRot(m_vRot);
	}
	if (m_vVelocity.x == -1)
	{
		m_vRot.z = 0.0f;
	}
	if (m_vVelocity.x == 1)
	{
		m_vRot.z = D3DX_PI;
		m_pAnim->SetRot(m_vRot);
	}
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CEffectMPAttack::Uninit(void)
{
	CEffectBase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CEffectMPAttack::Update(void)
{
	m_pAnim->SetPos(m_vPos);

	// ���E�̍X�V
	CEffectBase::Update();

	m_vPos += m_vVelocity * ATTACK_JAMMER_SPD;
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CEffectMPAttack* CEffectMPAttack::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, D3DXVECTOR3 velocity)
{
	// �쐬
	CEffectMPAttack* p = new CEffectMPAttack(pDevice);

	p->Init(pos, velocity);

	return p;
}

//EOF