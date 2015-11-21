//=============================================================================
//
// CDirectLariat�N���X [CDirectLariat.cpp]
// Author : �r���@���
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CDirectLariat.h"
#include "../../MANAGER/CManager.h"
#include "../../CAMERA/CameraManager.h"
#include "../../EFECT/CEffect.h"
#include "../../EFECT/CEffectManager.h"
#include "../../SCENE/GAME/PLAYER/CPlayerManager.h"
#include "../CDirectorManager.h"

const D3DXVECTOR3 LARIAT_HIT_OFFSET[2] = {
	D3DXVECTOR3( 40.0f, 60.0f, 0.0f ),
	D3DXVECTOR3( -40.0f, 60.0f, 0.0f )
};

//=================================================
// �R���X�g���N�^
//=================================================
CDirectLariat::CDirectLariat(CManager *pManager) : CDirect( pManager )
{
	
}

//=================================================
// �f�X�g���N�^
//=================================================
CDirectLariat::~CDirectLariat( void )
{

}

//=================================================
// ������
//=================================================
void CDirectLariat::Init( PLAYER_ID playerID )
{
	m_FrameCount = 0;		// �Œ�
	m_TotalFrame = 100;		// �Z���Ƃɕ�

	CDirect::SetPlayerID( playerID );
}

//=================================================
// �I��
//=================================================
void CDirectLariat::Uninit( void )
{

}

//=================================================
// �X�V
//=================================================
void CDirectLariat::Update( void )
{
	D3DXVECTOR3 playerPos = m_pManager->GetPlayerManager()->GetPlayerPos( m_Player );
	D3DXVECTOR3 enemyPos = m_pManager->GetPlayerManager()->GetPlayerPos( m_Enemy );



	/* ��������� */
	switch( m_FrameCount )
	{
		// �t���[���ʂ̏���
	case 0:
		m_pManager->GetPlayerManager()->SetAnimType( m_Player, CPlayer::PLAYER_LARIAT_LEFT );
		CEffect::Create( 60, EFFECT_AURA_START, false, playerPos + D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), VECTOR3_ZERO, D3DXVECTOR3( 10.0f, 10.0f, 10.0f ) );
		break;
	
	case 20:
		m_pManager->GetCameraManager()->CameraMoveToCoord(
			D3DXVECTOR3( 200.0f, 90.0f, -20.0f ),
			D3DXVECTOR3( 200.0f, 90.0f, 20.0f ),
			D3DXVECTOR3( 0.0f, 70.0f, 0.0f ),
			D3DXVECTOR3( 0.0f, 70.0f, 0.0f ),
			80 );
		break;

	case 40:
		m_pManager->GetCameraManager()->StartCameraShake( VECTOR3_ZERO, 10.0f, 20, 0 );
		m_pManager->GetPlayerManager()->SetAnimType( m_Enemy, CPlayer::PLAYER_DAMAGE_SMALL );
		m_pManager->GetPlayerManager()->TakeDamage( m_Enemy, 20 );
		CEffect::Create( 30, EFFECT_DAGEKI_KYO, false, playerPos + LARIAT_HIT_OFFSET[m_Player], VECTOR3_ZERO, D3DXVECTOR3( 3.0f, 3.0f, 3.0f ) );
		break;
	}
	/* �����܂Ō� */



	CDirect::CheckEnd();
}

//----EOF----