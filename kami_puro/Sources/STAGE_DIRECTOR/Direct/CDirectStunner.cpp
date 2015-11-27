//=============================================================================
//
// CDirectStunner�N���X [CDirectStunner.cpp]
// Author : �r���@���
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CDirectStunner.h"
#include "../../MANAGER/CManager.h"
#include "../../CAMERA/CameraManager.h"
#include "../../EFECT/CEffect.h"
#include "../../EFECT/CEffectManager.h"
#include "../../SCENE/GAME/PLAYER/CPlayerManager.h"
#include "../CDirectorManager.h"

const D3DXVECTOR3 STUNNER_POS_OFFSET = D3DXVECTOR3( 35.0f, 0.0f, 0.0f );
const D3DXVECTOR3 STUNNER_ROT_OFFSET = D3DXVECTOR3( 0.0f, D3DXToRadian( 180.0f ), 0.0f );

const D3DXVECTOR3 STUNNER_EFFECT_AURA_OFFSET = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
const D3DXVECTOR3 STUNNER_EFFECT_AURA_SCALE = D3DXVECTOR3( 10.0f, 10.0f, 10.0f );

const D3DXVECTOR3 STUNNER_EFFECT_HIT_OFFSET = D3DXVECTOR3( 40.0f, 60.0f, 0.0f );
const D3DXVECTOR3 STUNNER_EFFECT_HIT_SCALE = D3DXVECTOR3( 3.0f, 3.0f, 3.0f );

const int STUNNER_DAMAGE1 = 1;
const int STUNNER_DAMAGE2 = 19;

//=================================================
// �R���X�g���N�^
//=================================================
CDirectStunner::CDirectStunner(CManager *pManager) : CDirect( pManager )
{
	
}

//=================================================
// �f�X�g���N�^
//=================================================
CDirectStunner::~CDirectStunner( void )
{

}

//=================================================
// ������
//=================================================
void CDirectStunner::Init( PLAYER_ID playerID )
{
	m_FrameCount = 0;		// �Œ�
	m_TotalFrame = 130;		// �Z���Ƃɕ�

	m_pPlayerManager = m_pManager->GetPlayerManager();
	m_pCameraManager = m_pManager->GetCameraManager();

	CDirect::SetPlayerID( playerID );
}

//=================================================
// �I��
//=================================================
void CDirectStunner::Uninit( void )
{

}

//=================================================
// �X�V
//=================================================
void CDirectStunner::Update( void )
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
		m_pPlayerManager->SetPos( m_Player, pos[m_Enemy] + TranslateCoord( m_Player, STUNNER_POS_OFFSET ) );
		m_pPlayerManager->SetRot( m_Enemy, rot[m_Enemy] + TranslateCoord( m_Player, STUNNER_ROT_OFFSET ) );
		m_pPlayerManager->SetAnimType( m_Player, CPlayer::PLAYER_STUNNER_RIGHT );
		m_pPlayerManager->SetAnimType( m_Enemy, CPlayer::PLAYER_STUNNER_DAMAGE_RIGHT );
		m_pPlayerManager->SetAnimSpd( m_Player, DEFFAULT_ANIM_SPD * 0.7f );
		m_pPlayerManager->SetAnimSpd( m_Enemy, DEFFAULT_ANIM_SPD * 0.7f );
		CEffect::Create( 60, EFFECT_AURA_START, false, pos[m_Player] + TranslateCoord( m_Player, STUNNER_EFFECT_AURA_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)STUNNER_EFFECT_AURA_SCALE );
		m_pPlayerManager->TakeDamage( m_Enemy, STUNNER_DAMAGE1 );
		m_pCameraManager->CameraMoveToCoord(
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 200.0f, 100.0f, -100.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 200.0f, 100.0f, +100.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 70.0f, 0.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 40.0f, 0.0f, 0.0f ) ),
			100 );
		break;

	case 70:
		m_pPlayerManager->SetAnimSpd( m_Player, DEFFAULT_ANIM_SPD * 1.0f );
		m_pPlayerManager->SetAnimSpd( m_Enemy, DEFFAULT_ANIM_SPD * 1.0f );
		break;
	
	case 75:
		m_pCameraManager->StartCameraShake( VECTOR3_ZERO, 10.0f, 20, 0 );
		m_pPlayerManager->TakeDamage( m_Enemy, STUNNER_DAMAGE2 );
		CEffect::Create( 30, EFFECT_DAGEKI_KYO, false, pos[m_Player] + TranslateCoord( m_Player, STUNNER_EFFECT_HIT_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)STUNNER_EFFECT_HIT_SCALE );
		break;
	}
	/* �����܂Ō� */



	CDirect::CheckEnd();
}

//----EOF----