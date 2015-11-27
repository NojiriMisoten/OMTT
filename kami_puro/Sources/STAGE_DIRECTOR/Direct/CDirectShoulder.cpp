//=============================================================================
//
// CDirectShoulder�N���X [CDirectShoulder.cpp]
// Author : �r���@���
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CDirectShoulder.h"
#include "../../MANAGER/CManager.h"
#include "../../CAMERA/CameraManager.h"
#include "../../EFECT/CEffect.h"
#include "../../EFECT/CEffectManager.h"
#include "../../SCENE/GAME/PLAYER/CPlayerManager.h"
#include "../CDirectorManager.h"

const D3DXVECTOR3 SHOULDER_EFFECT_AURA_OFFSET = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
const D3DXVECTOR3 SHOULDER_EFFECT_AURA_SCALE = D3DXVECTOR3( 10.0f, 10.0f, 10.0f );

const D3DXVECTOR3 SHOULDER_EFFECT_HIT_OFFSET = D3DXVECTOR3( 40.0f, 60.0f, 0.0f );
const D3DXVECTOR3 SHOULDER_EFFECT_HIT_SCALE = D3DXVECTOR3( 3.0f, 3.0f, 3.0f );

const int SHOULDER_DAMAGE = 20;

//=================================================
// �R���X�g���N�^
//=================================================
CDirectShoulder::CDirectShoulder(CManager *pManager) : CDirect( pManager )
{
	
}

//=================================================
// �f�X�g���N�^
//=================================================
CDirectShoulder::~CDirectShoulder( void )
{

}

//=================================================
// ������
//=================================================
void CDirectShoulder::Init( PLAYER_ID playerID )
{
	m_FrameCount = 0;		// �Œ�
	m_TotalFrame = 120;		// �Z���Ƃɕ�

	m_pPlayerManager = m_pManager->GetPlayerManager();
	m_pCameraManager = m_pManager->GetCameraManager();

	CDirect::SetPlayerID( playerID );
}

//=================================================
// �I��
//=================================================
void CDirectShoulder::Uninit( void )
{

}

//=================================================
// �X�V
//=================================================
void CDirectShoulder::Update( void )
{
	D3DXVECTOR3 pos[2] = {
		m_pPlayerManager->GetPlayerPos( PLAYER_1 ),
		m_pPlayerManager->GetPlayerPos( PLAYER_2 )
	};

	D3DXVECTOR3 rot[2] = {
		m_pPlayerManager->GetPlayerRot( PLAYER_1 ),
		m_pPlayerManager->GetPlayerRot( PLAYER_2 )
	};

	/* ��������� */
	switch( m_FrameCount )
	{
		// �t���[���ʂ̏���
	case 0:
		m_pPlayerManager->SetAnimType( m_Player, CPlayer::PLAYER_FLYING_SHOULDER_ATTACK_LEFT );
		CEffect::Create( 60, EFFECT_AURA_START, false, pos[m_Player] + TranslateCoord( m_Player, SHOULDER_EFFECT_AURA_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)SHOULDER_EFFECT_AURA_SCALE );
		m_pCameraManager->CameraMoveToCoord(
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 25.0f, 150.0f, -100.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 25.0f, 300.0f, -200.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 25.0f, 70.0f, 0.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 100.0f, 0.0f, 0.0f ) ),
			100 );
		break;
	
	case 40:
		m_pPlayerManager->SetAnimType( m_Enemy, CPlayer::PLAYER_FLYING_SHOULDER_ATTACK_DAMAGE_LEFT );
		m_pCameraManager->StartCameraShake( VECTOR3_ZERO, 10.0f, 20, 0 );
		m_pPlayerManager->TakeDamage( m_Enemy, SHOULDER_DAMAGE );
		CEffect::Create( 30, EFFECT_DAGEKI_KYO, false, pos[m_Player] + TranslateCoord( m_Player, SHOULDER_EFFECT_HIT_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)SHOULDER_EFFECT_HIT_SCALE );
		break;

	case 70:
		m_pPlayerManager->SetAnimSpd( m_Player, 0.0f );
		m_pPlayerManager->SetAnimSpd( m_Enemy, 0.0f );
		break;
	}
	/* �����܂Ō� */



	CDirect::CheckEnd();
}

//----EOF----