//=============================================================================
//
// CThreadSpecialTrap�N���X [CThreadSpecialTrap.cpp]
// Author : �˖{�@�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CThreadSpecialTrap.h"
#include "../EFFECT/CEffectManager.h"
#include "../../../MANAGER/CManager.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
// ����
static const short THREAD_TRAP_END_TIME = 300;

// �����蔻��̏I��鎞��
static const short THREAD_TRAP_HIT_END_TIME = 300;
// �����蔻 �蕝,����
static const float THREAD_TRAP_HIT_WIDTH = 150;
static const float THREAD_TRAP_HIT_HEIGHT = 150;


//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CThreadSpecialTrap::CThreadSpecialTrap(LPDIRECT3DDEVICE9 *pDevice, int priority, OBJTYPE type) : CThreadBase(pDevice, priority, type)
{
	// �ϐ�������
	m_ThreadType = THREAD_TYPE_TRAP;

	// ���̎��̌ŗL�X�e�[�^�X������
	m_fWidth = THREAD_TRAP_HIT_WIDTH;
	m_fHeight = THREAD_TRAP_HIT_HEIGHT;
	m_vRot = D3DXVECTOR3(0, 0, 0);
	m_nEndTime = THREAD_TRAP_END_TIME;
	m_nHitStartTime = THREAD_TRAP_HIT_START_TIME;
	m_nHitEndTime = THREAD_TRAP_HIT_END_TIME;

	//// �������
	//m_pBulletAnime = CSceneAnime::Create(
	//	pDevice,
	//	m_vPos, THREAD_TRAP_HIT_WIDTH, THREAD_TRAP_HIT_HEIGHT,
	//	TEXTURE_THREAD, 1, 1, THREAD_TRAP_END_TIME);
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CThreadSpecialTrap ::~CThreadSpecialTrap(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CThreadSpecialTrap::Init()
{
	CThreadBase::Init(m_vPos, THREAD_TRAP_HIT_WIDTH, THREAD_TRAP_HIT_HEIGHT, TEXTURE_THREAD);

	//// ���̈ʒu���Z�b�g
	//m_pBulletAnime->SetPos(m_vPos);

	//// ���̐F��ς���
	//if (m_nPlayerNum < 0 || m_nPlayerNum > 3){
	//	return E_FAIL;
	//}
	//m_pBulletAnime->SetColorPolygon(PLAYER_COLOR[m_nPlayerNum]);

	return S_OK;  
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CThreadSpecialTrap::Uninit(void)
{
	CThreadBase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CThreadSpecialTrap::Update(void)
{
	CThreadBase::Update();

	// �J�E���g�ŏ���
	if (m_nCount == THREAD_TRAP_END_TIME){
		Release();
	}
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CThreadSpecialTrap* CThreadSpecialTrap::Create(
	LPDIRECT3DDEVICE9 *pDevice,
	short nPlayerNum, D3DXVECTOR3 pos)
{
	// �쐬
	CThreadSpecialTrap* p = new CThreadSpecialTrap(pDevice);

	p->m_nPlayerNum = nPlayerNum;
	p->m_vPos = pos;

	// ������
	p->Init();

	CEffectManager::CreateEffectTrap(pos, nPlayerNum);

	return p;
}

//*****************************************************************************
// �`��
//*****************************************************************************
void CThreadSpecialTrap::Draw(void)
{
	// Base�̕\����؂���m_pBulletAnime�̕\�������ɂ���
//	CThreadBase::Draw();
}

//*****************************************************************************
// �v���C���ɓ����������ɌĂ΂��֐�
//*****************************************************************************
void CThreadSpecialTrap::HitPlayer(CPlayer* pPlayer)
{
	pPlayer->AddMp(-4.f);
	pPlayer->AddHp(-1.f);
	// �ݑ�
	pPlayer->SetSlowSpeed(true);
}

//----EOF-------