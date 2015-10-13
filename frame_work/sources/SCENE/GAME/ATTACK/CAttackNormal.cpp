//=============================================================================
//
// CAttacjManager�N���X [CAttacManager.cpp]
// Author : �˖{�@�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CAttackNormal.h"
#include "../EFFECT/CEffectManager.h"
#include "../PLAYER/CPlayer.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
// �����蔻��̎n�܂鎞��
static const short ATTACK_NORMAL_HIT_START_TIME = 5;
// �����蔻��̏I��鎞��
static const short ATTACK_NORMAL_HIT_END_TIME = 30;

// �����蔻 �蕝,����
static const float ATTACK_NORMAL_HIT_WIDTH = 50;
static const float ATTACK_NORMAL_HIT_HEIGHT = 50;

// �v���C���ƍU���G�t�F�N�g�̋���
static const float ATTACK_NORMAL_RANGE = 50;

static const float ATTACK_DAMAGE = -200.f;
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CAttackNormal::CAttackNormal(LPDIRECT3DDEVICE9 *pDevice) : CAttackBase(pDevice)
{
	// �ϐ�������
	m_AttackType = ATTACK_TYPE_NORMAL;

	// ���̍U���̌ŗL�X�e�[�^�X������
	m_fWidth = ATTACK_NORMAL_HIT_WIDTH;
	m_fHeight = ATTACK_NORMAL_HIT_HEIGHT;
	m_vRot = D3DXVECTOR3(0, 0, 0);
	m_nEndTime = ATTACK_NORMAL_END_TIME;
	m_nHitStartTime = ATTACK_NORMAL_HIT_START_TIME;
	m_nHitEndTime = ATTACK_NORMAL_HIT_END_TIME;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CAttackNormal ::~CAttackNormal(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CAttackNormal::Init()
{
	CAttackBase::Init();
	CManager::PlaySoundA(SOUND_LABEL_SE_NORMAL_ATTACK);
	return S_OK;
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CAttackNormal::Uninit(void)
{
	CAttackBase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CAttackNormal::Update(void)
{
	CAttackBase::Update();
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CAttackNormal* CAttackNormal::Create(
	LPDIRECT3DDEVICE9 *pDevice,
	short nPlayerNum,
	D3DXVECTOR3 pos,
	D3DXVECTOR3 velocity)
{
	// �쐬
	CAttackNormal* p = new CAttackNormal(pDevice);

	p->m_nPlayerNum = nPlayerNum;
	p->m_vPos = pos + (velocity * ATTACK_NORMAL_RANGE);

	// ������
	p->Init();

	// �G�t�F�N�g����
	CEffectManager::CreateEffect(p->m_vPos, EFFECT_NORMAL_ATTACK_CAP, velocity);

	return p;
}

//*****************************************************************************
//	�U�����q�b�g�����ۂɌĂяo�����֐�
//	 �����@�@�q�b�g�����v���C���[�̃v���C���[�ԍ�
//*****************************************************************************
void CAttackNormal::HitPlayer(CPlayer* pPlayer)
{
	pPlayer->AddHp(ATTACK_DAMAGE);
	CAttackBase::HitPlayer(pPlayer);
}

//----EOF-------