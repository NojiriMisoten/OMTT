//=============================================================================
//
// CThreadManager�N���X [CAttacManager.cpp]
// Author : �˖{�@�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CThreadSpecialAttack.h"
#include "../PLAYER/CPlayer.h"
#include "../EFFECT/CEffectManager.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
// �����蔻��̎n�܂鎞��
static const short THREAD_ATTACK_HIT_START_TIME = 60;
// �����蔻��̏I��鎞��
static const short THREAD_ATTACK_HIT_END_TIME = 100;

// �����蔻 �蕝,����
static const float THREAD_ATTACK_HIT_WIDTH = 320.f;
static const float THREAD_ATTACK_HIT_HEIGHT = 180.f;

// �v���C���ƍU���G�t�F�N�g�̋���
static const short THREAD_ATTACK_RANGE = 50;

static const float ATTACK_DAMAGE = -600.f;

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CThreadSpecialAttack::CThreadSpecialAttack(LPDIRECT3DDEVICE9 *pDevice, int priority, OBJTYPE type) : CAttackBase(pDevice)
{
	// �ϐ�������
	m_AttackType = ATTACK_TYPE_ATTACK_THREAD;

	// ���̎��̌ŗL�X�e�[�^�X������
	m_fWidth = 0;
	m_fHeight = 0;
	m_vRot = D3DXVECTOR3(0, 0, 0);
	m_nEndTime = THREAD_ATTACK_END_TIME;
	m_nHitStartTime = THREAD_ATTACK_HIT_START_TIME;
	m_nHitEndTime = THREAD_ATTACK_HIT_END_TIME;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CThreadSpecialAttack ::~CThreadSpecialAttack(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CThreadSpecialAttack::Init(D3DXVECTOR3 pos, short playerNumber , D3DXVECTOR3 velocity)
{
	CAttackBase::Init();

	m_nPlayerNum = playerNumber;
	m_vPos = pos + (velocity * THREAD_ATTACK_RANGE);
	m_vVelocity = velocity;

	for (int i = 0; i < 4; i++)
	{
		if (m_vVelocity.y == -1)
		{
			m_vRot.z = D3DX_PI*0.5f;
			SetRot(m_vRot);
		}
		if (m_vVelocity.y == 1)
		{
			m_vRot.z = D3DX_PI*1.5f;
			SetRot(m_vRot);
		}
		if (m_vVelocity.x == 1)
		{
			m_vRot.z = 0.0f;
			SetRot(m_vRot);
		}
		if (m_vVelocity.x == -1)
		{
			m_vRot.z = D3DX_PI;
			SetRot(m_vRot);
		}
	}
	m_bHitFlg = false;

	// �G�t�F�N�g����
	CEffectManager::CreateEffect(m_vPos, EFFECT_SPECIAL_THREAD_ATTACK, velocity);
	return S_OK;
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CThreadSpecialAttack::Uninit(void)
{
	CAttackBase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CThreadSpecialAttack::Update(void)
{
	CAttackBase::Update();
	if (m_bHitFlg)
	{
		m_fWidth = THREAD_ATTACK_HIT_WIDTH;
		m_fHeight = THREAD_ATTACK_HIT_HEIGHT;

		m_vPos.x += m_vVelocity.x * 5.0f + m_vVelocity.x * 100.0f;
		m_vPos.y += m_vVelocity.y * 5.0f + m_vVelocity.y * 100.0f;
		m_vVelocity.x = 0;
		m_vVelocity.y = 0;
	}
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CThreadSpecialAttack* CThreadSpecialAttack::Create(LPDIRECT3DDEVICE9 *pDevice, short nPlayerNum, D3DXVECTOR3 pos, D3DXVECTOR3 velocity)
{
	// �쐬
	CThreadSpecialAttack* p = new CThreadSpecialAttack(pDevice);

	// ������
	p->Init(pos, nPlayerNum, velocity);

	return p;
}

//*****************************************************************************
//	�`��
//*****************************************************************************
void CThreadSpecialAttack::Draw(void)
{
	CAttackBase::Draw();
}

//*****************************************************************************
//
//*****************************************************************************
void CThreadSpecialAttack::HitPlayer(CPlayer* pPlayer)
{
	CAttackBase::HitPlayer(pPlayer);
	pPlayer->AddHp(ATTACK_DAMAGE);

	CSceneAnime::Create(
		m_pD3DDevice,
		pPlayer->GetPos(), 100, 100,
		TEXTURE_FIRE_1, 10, 1, 40);
}

//----EOF-------