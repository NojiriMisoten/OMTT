//=============================================================================
//
// CAttacSpecialAttack�N���X [CAttacSpecialAttack.cpp]
// Author : �����@�Ȉ�
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CAttackSpecialAttack.h"
#include "../EFFECT/CEffectManager.h"
#include "../PLAYER/CPlayer.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
// �����蔻��̎n�܂鎞��
static const short ATTACK_ATTACK_HIT_START_TIME = 10;
// �����蔻��̏I��鎞��
static const short ATTACK_ATTACK_HIT_END_TIME = 30;

// �����蔻 �蕝,����
static const float ATTACK_ATTACK_HIT_WIDTH = 100;
static const float ATTACK_ATTACK_HIT_HEIGHT = 100;

// �v���C���ƍU���G�t�F�N�g�̋���
static const float ATTACK_ATTACK_RANGE = 50;

static const float ATTACK_DAMAGE = -200;

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CAttackSpecialAttack::CAttackSpecialAttack(LPDIRECT3DDEVICE9 *pDevice) : CAttackBase(pDevice)
{
	// �ϐ�������
	m_AttackType = ATTACK_TYPE_NORMAL;

	// ���̍U���̌ŗL�X�e�[�^�X������
	m_fWidth = ATTACK_ATTACK_HIT_WIDTH;
	m_fHeight = ATTACK_ATTACK_HIT_HEIGHT;
	m_vRot = D3DXVECTOR3(0, 0, 0);
	m_nEndTime = ATTACK_ATTACK_END_TIME;
	m_nHitStartTime = ATTACK_ATTACK_HIT_START_TIME;
	m_nHitEndTime = ATTACK_ATTACK_HIT_END_TIME;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CAttackSpecialAttack ::~CAttackSpecialAttack(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CAttackSpecialAttack::Init()
{
	CAttackBase::Init();
	CManager::PlaySoundA(SOUND_LABEL_SE_AROUND_ATTACK);
	return S_OK;
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CAttackSpecialAttack::Uninit(void)
{
	CAttackBase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CAttackSpecialAttack::Update(void)
{
	CAttackBase::Update();
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CAttackSpecialAttack* CAttackSpecialAttack::Create(
	LPDIRECT3DDEVICE9 *pDevice,
	short nPlayerNum,
	D3DXVECTOR3 pos,
	D3DXVECTOR3 velocity)
{
	// �쐬
	CAttackSpecialAttack* p = new CAttackSpecialAttack(pDevice);

	p->m_nPlayerNum = nPlayerNum;
	p->m_vPos = pos;// +(velocity * ATTACK_ATTACK_RANGE);
	//p->m_vPos.y = pos.y + ATTACK_ATTACK_RANGE;
	//p->m_vPos.x = pos.x + 25.0f;

	// ������
	p->Init();

	// �U���G�t�F�N�g����
	CEffectManager::CreateEffect(p->m_vPos, EFFECT_SPECIAL_ATTACK_ATTACK, velocity);

	return p;
}

//=============================================================================
//	�v���C���[�ɓ����������̏���
//=============================================================================
void CAttackSpecialAttack::HitPlayer(CPlayer* pPlayer)
{
	pPlayer->AddHp(ATTACK_DAMAGE);
	CAttackBase::HitPlayer(pPlayer);
}

//----EOF-------