//=============================================================================
//
// CDirectChop�N���X [CDirectChop.cpp]
// Author : �r���@���
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CDirectChop.h"
#include "../../MANAGER/CManager.h"
#include "../../CAMERA/CameraManager.h"
#include "../../EFECT/CEffect.h"
#include "../../EFECT/CEffectManager.h"
#include "../../SCENE/GAME/PLAYER/CPlayerManager.h"
#include "../CDirectorManager.h"

const D3DXVECTOR3 CHOP_HIT_OFFSET[2] = {
	D3DXVECTOR3( 40.0f, 60.0f, 0.0f ),
	D3DXVECTOR3( -40.0f, 60.0f, 0.0f )
};

//=================================================
// �R���X�g���N�^
//=================================================
CDirectChop::CDirectChop(CManager *pManager) : CDirect( pManager )
{
	
}

//=================================================
// �f�X�g���N�^
//=================================================
CDirectChop::~CDirectChop( void )
{

}

//=================================================
// ������
//=================================================
void CDirectChop::Init( PLAYER_ID playerID )
{
	m_FrameCount = 0;		// �Œ�
	m_TotalFrame = 90;		// �Z���Ƃɕ�

	CDirect::SetPlayerID( playerID );
}

//=================================================
// �I��
//=================================================
void CDirectChop::Uninit( void )
{

}

//=================================================
// �X�V
//=================================================
void CDirectChop::Update( void )
{
	D3DXVECTOR3 playerPos = m_pManager->GetPlayerManager()->GetPlayerPos( m_Player );
	D3DXVECTOR3 enemyPos = m_pManager->GetPlayerManager()->GetPlayerPos( m_Enemy );



	/* ��������� */
	switch( m_FrameCount )
	{
		// �t���[���ʂ̏���
	case 0:
		m_pManager->GetPlayerManager()->SetAnimType( m_Player, CPlayer::PLAYER_CHOP_LEFT );
		CEffect::Create( 60, EFFECT_AURA_START, false, playerPos + D3DXVECTOR3( 0.0f, 0.0f, 0.0f), VECTOR3_ZERO, D3DXVECTOR3( 10.0f, 10.0f, 10.0f ) );
		break;
	
	case 50:
		m_pManager->GetCameraManager()->StartCameraShake( VECTOR3_ZERO, 3.0f, 10, 0 );
		m_pManager->GetPlayerManager()->SetAnimType( m_Enemy, CPlayer::PLAYER_DAMAGE_SMALL );
		m_pManager->GetPlayerManager()->TakeDamage( m_Enemy, 5 );
		CEffect::Create( 30, EFFECT_DAGEKI_KYO, false, playerPos + CHOP_HIT_OFFSET[m_Player], VECTOR3_ZERO, D3DXVECTOR3( 3.0f, 3.0f, 3.0f ) );
		break;

	case 80:
		m_pManager->GetPlayerManager()->SetAnimType( m_Enemy, CPlayer::PLAYER_WAIT );
		break;
	}
	/* �����܂Ō� */



	CDirect::CheckEnd();
}

//----EOF----