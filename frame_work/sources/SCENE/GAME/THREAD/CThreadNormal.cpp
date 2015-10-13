//=============================================================================
//
// CThreadNormal�N���X [CThreadNormal.cpp]
// Author : �˖{�@�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CThreadNormal.h"
#include "../EFFECT/CEffectManager.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
// �����蔻��̎n�܂鎞��
static const short THREAD_NORMAL_HIT_START_TIME = 1;
// �����蔻��̏I��鎞��
static const short THREAD_NORMAL_HIT_END_TIME = 30;
// �����蔻 �蕝,����
static const float THREAD_NORMAL_HIT_WIDTH = 30;
static const float THREAD_NORMAL_HIT_HEIGHT = 30;

// �e�̈ړ���
static const float THREAD_NORMAL_VEL = 10.f;


//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CThreadNormal::CThreadNormal(LPDIRECT3DDEVICE9 *pDevice, int priority, OBJTYPE type) : CThreadBase(pDevice, priority, type)
{
	// �ϐ�������
	m_ThreadType = THREAD_TYPE_NORMAL;

	// ���̎��̌ŗL�X�e�[�^�X������
	m_fWidth = THREAD_NORMAL_HIT_WIDTH;
	m_fHeight = THREAD_NORMAL_HIT_HEIGHT;
	m_vRot = D3DXVECTOR3(0, 0, 0);
	m_nEndTime = THREAD_NORMAL_END_TIME;
	m_nHitStartTime = THREAD_NORMAL_HIT_START_TIME;
	m_nHitEndTime = THREAD_NORMAL_HIT_END_TIME;

	// �e�����
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CThreadNormal ::~CThreadNormal(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CThreadNormal::Init()
{
	return S_OK;
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CThreadNormal::Uninit(void)
{
	CThreadBase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CThreadNormal::Update(void)
{

	// �e�̈ړ�
	m_vPos += m_vPlayerDirection * THREAD_NORMAL_VEL;

	// �J�E���g�ŏ���
	if (m_nCount == THREAD_NORMAL_END_TIME){
		Release();
	}
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CThreadNormal* CThreadNormal::Create(
	LPDIRECT3DDEVICE9 *pDevice,
	short nPlayerNum, D3DXVECTOR3 pos, D3DXVECTOR3 direction)
{
	// �쐬
	CThreadNormal* p = new CThreadNormal(pDevice);

	p->m_nPlayerNum = nPlayerNum;
	p->m_vPos = pos;
	p->m_vPlayerDirection = direction;

	// ������
	p->Init();

	// �G�t�F�N�g����
	CEffectManager::CreateEffect(pos, EFFECT_NORMAL_ATTACK_THREAD, direction);

	return p;
}

//*****************************************************************************
// �`��
//*****************************************************************************
void CThreadNormal::Draw(void)
{
}

//*****************************************************************************
// �v���C���ɓ����������ɌĂ΂��֐�
//*****************************************************************************
void CThreadNormal::HitPlayer(CPlayer* pPlayer)
{
	// �ݑ��ɂ��ă_���[�W�͖���
	pPlayer->SetSlowSpeed(true);

	CManager::PlaySoundA(SOUND_LABEL_SE_SPEED_DOWN);
}

//----EOF-------