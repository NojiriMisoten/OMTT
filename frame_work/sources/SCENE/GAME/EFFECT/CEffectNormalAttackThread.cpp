//=============================================================================
//
//	�����G�t�F�N�g
//
//	Author : �����@�Ȉ�
//
//=============================================================================
#include "CEffectNormalAttackThread.h"

#include "../../CSCENE/CSceneAnime.h"

// �f�o�b�O���Ă݂悤
#ifdef _DEBUG
#include "../../../DEBUG_PROC/CDebugProc.h"
#endif

//*****************************************************************************
// �萔
//*****************************************************************************
static const int NORMAL_ATTACK_THREAD_COUNT_MAX = 99;
static const float NORMAL_ATTACK_THREAD_WIDTH = 30;
static const float NORMAL_ATTACK_THREAD_HEIGHT = 30;
static const TEXTURE_TYPE NORMAL_ATTACK_THREAD_TEXTURE = TEXTURE_THREAD_DROP;
static const int NORMAL_ATTACK_THREAD_TEXTURE_X = 2;
static const int NORMAL_ATTACK_THREAD_TEXTURE_Y = 5;
static const int NORMAL_ATTACK_THREAD_TEXTURE_LOOP = 100;

static const float NORMAL_ATTACK_THREAD_SPD = 10.0f;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CEffectNormalAttackThread::CEffectNormalAttackThread(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CEffectNormalAttackThread ::~CEffectNormalAttackThread(void)
{

}

//*****************************************************************************
// ������
//*****************************************************************************
void CEffectNormalAttackThread::Init(D3DXVECTOR3 pos, D3DXVECTOR3 velocity)
{
	m_nCount = 0;
	m_nCountMax = NORMAL_ATTACK_THREAD_COUNT_MAX;
	m_vPos = pos;
	m_vPos.y = pos.y - 30.0f;
	m_vVelocity = velocity;

	m_pAnim = CSceneAnime::Create(
		m_pD3DDevice,
		m_vPos,
		NORMAL_ATTACK_THREAD_WIDTH, NORMAL_ATTACK_THREAD_HEIGHT,
		NORMAL_ATTACK_THREAD_TEXTURE, NORMAL_ATTACK_THREAD_TEXTURE_X, NORMAL_ATTACK_THREAD_TEXTURE_Y,
		NORMAL_ATTACK_THREAD_TEXTURE_LOOP);

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
void CEffectNormalAttackThread::Uninit(void)
{
	CEffectBase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CEffectNormalAttackThread::Update(void)
{
	m_pAnim->SetPos(m_vPos);

	if (m_nCount > NORMAL_ATTACK_THREAD_COUNT_MAX / (NORMAL_ATTACK_THREAD_TEXTURE_X*NORMAL_ATTACK_THREAD_TEXTURE_Y) * 7)
	{
		m_pAnim->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	// ���E�̍X�V
	CEffectBase::Update();

	m_vPos += m_vVelocity * NORMAL_ATTACK_THREAD_SPD;
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CEffectNormalAttackThread* CEffectNormalAttackThread::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos , D3DXVECTOR3 velocity)
{
	// �쐬
	CEffectNormalAttackThread* p = new CEffectNormalAttackThread(pDevice);

	p->Init(pos , velocity);

	return p;
}

//EOF