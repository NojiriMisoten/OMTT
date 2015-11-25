//=============================================================================
//
// CDirectSlap�N���X [CDirectSlap.cpp]
// Author : �r���@���
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CDirectSlap.h"
#include "../../MANAGER/CManager.h"
#include "../../CAMERA/CameraManager.h"
#include "../../EFECT/CEffect.h"
#include "../../EFECT/CEffectManager.h"
#include "../../SCENE/GAME/PLAYER/CPlayerManager.h"
#include "../CDirectorManager.h"

const D3DXVECTOR3 SLAP_HIT_OFFSET[2] = {
	D3DXVECTOR3( 40.0f, 60.0f, 0.0f ),
	D3DXVECTOR3( -40.0f, 60.0f, 0.0f )
};

//=================================================
// �R���X�g���N�^
//=================================================
CDirectSlap::CDirectSlap(CManager *pManager) : CDirect( pManager )
{
	
}

//=================================================
// �f�X�g���N�^
//=================================================
CDirectSlap::~CDirectSlap( void )
{

}

//=================================================
// ������
//=================================================
void CDirectSlap::Init( PLAYER_ID playerID )
{
	m_FrameCount = 0;		// �Œ�
	m_TotalFrame = 130;		// �Z���Ƃɕ�

	CDirect::SetPlayerID( playerID );
}

//=================================================
// �I��
//=================================================
void CDirectSlap::Uninit( void )
{

}

//=================================================
// �X�V
//=================================================
void CDirectSlap::Update( void )
{
	D3DXVECTOR3 pos[2] = {
		m_pManager->GetPlayerManager()->GetPlayerPos( m_Player ),
		m_pManager->GetPlayerManager()->GetPlayerPos( m_Enemy )
	};

	D3DXVECTOR3 rot[2] = {
		m_pManager->GetPlayerManager()->GetPlayerRot( m_Player ),
		m_pManager->GetPlayerManager()->GetPlayerRot( m_Enemy )
	};

	/* ��������� */
	switch( m_FrameCount )
	{
		// �t���[���ʂ̏���
	case 0:
		m_pManager->GetPlayerManager()->SetPos( PLAYER_2, pos[PLAYER_1] + D3DXVECTOR3( 45.0f, 0.0f, 0.0f ) );
		m_pManager->GetPlayerManager()->SetAnimType( m_Player, CPlayer::PLAYER_SLAPPING_RIGHT );
		CEffect::Create( 60, EFFECT_AURA_START, false, pos[m_Player] + D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), VECTOR3_ZERO, D3DXVECTOR3( 10.0f, 10.0f, 10.0f ) );
		m_pManager->GetPlayerManager()->SetAnimSpd( m_Player, DEFFAULT_ANIM_SPD * 0.7f );
		m_pManager->GetPlayerManager()->SetAnimSpd( m_Enemy, DEFFAULT_ANIM_SPD * 0.7f );

		m_pManager->GetCameraManager()->CameraMoveToCoord(
			pos[PLAYER_1] + D3DXVECTOR3( -30.0f, 100.0f, -50.0f ),
			pos[PLAYER_1] + D3DXVECTOR3( 30.0f, 100.0f, -50.0f),
			pos[PLAYER_1] + D3DXVECTOR3( 25.0f, 70.0f, 0.0f ),
			pos[PLAYER_1] + D3DXVECTOR3( 25.0f, 70.0f, 0.0f ),
			80 );
		break;

	case 20:
		CEffect::Create( 30, EFFECT_DAGEKI_TYU, false, pos[m_Player] + SLAP_HIT_OFFSET[m_Player], VECTOR3_ZERO, D3DXVECTOR3( 3.0f, 3.0f, 3.0f ) );
		m_pManager->GetPlayerManager()->TakeDamage( m_Enemy, 1 );
		m_pManager->GetPlayerManager()->SetAnimSpd( m_Player, DEFFAULT_ANIM_SPD * 1.0f );
		m_pManager->GetPlayerManager()->SetAnimSpd( m_Enemy, DEFFAULT_ANIM_SPD * 1.0f );
		break;

	case 70:
		m_pManager->GetCameraManager()->StartCameraShake( VECTOR3_ZERO, 10.0f, 20, 0 );
		m_pManager->GetPlayerManager()->SetAnimType( m_Enemy, CPlayer::PLAYER_SLAPPING_DAMAGE_RIGHT );
		m_pManager->GetPlayerManager()->TakeDamage( m_Enemy, 9 );
		CEffect::Create( 30, EFFECT_DAGEKI_KYO, false, pos[m_Player] + SLAP_HIT_OFFSET[m_Player], VECTOR3_ZERO, D3DXVECTOR3( 3.0f, 3.0f, 3.0f ) );
		break;
	}
	/* �����܂Ō� */



	CDirect::CheckEnd();
}

//----EOF----