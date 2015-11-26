//=============================================================================
//
// CDirectDropkick�N���X [CDirectDropkick.cpp]
// Author : �r���@���
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CDirectDropkick.h"
#include "../../MANAGER/CManager.h"
#include "../../CAMERA/CameraManager.h"
#include "../../EFECT/CEffect.h"
#include "../../EFECT/CEffectManager.h"
#include "../../SCENE/GAME/PLAYER/CPlayerManager.h"
#include "../CDirectorManager.h"

const D3DXVECTOR3 DROPKICK_EFFECT_AURA_OFFSET = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
const D3DXVECTOR3 DROPKICK_EFFECT_AURA_SCALE = D3DXVECTOR3( 10.0f, 10.0f, 10.0f );

const D3DXVECTOR3 DROPKICK_EFFECT_HIT_OFFSET = D3DXVECTOR3( 40.0f, 60.0f, 0.0f );
const D3DXVECTOR3 DROPKICK_EFFECT_HIT_SCALE = D3DXVECTOR3( 3.0f, 3.0f, 3.0f );

const int DROPKICK_DAMAGE = 20;

//=================================================
// �R���X�g���N�^
//=================================================
CDirectDropkick::CDirectDropkick(CManager *pManager) : CDirect( pManager )
{
	
}

//=================================================
// �f�X�g���N�^
//=================================================
CDirectDropkick::~CDirectDropkick( void )
{

}

//=================================================
// ������
//=================================================
void CDirectDropkick::Init( PLAYER_ID playerID )
{
	m_FrameCount = 0;		// �Œ�
	m_TotalFrame = 200;		// �Z���Ƃɕ�

	CDirect::SetPlayerID( playerID );
}

//=================================================
// �I��
//=================================================
void CDirectDropkick::Uninit( void )
{

}

//=================================================
// �X�V
//=================================================
void CDirectDropkick::Update( void )
{
	D3DXVECTOR3 pos[2] = {
		m_pManager->GetPlayerManager()->GetPlayerPos( PLAYER_1 ),
		m_pManager->GetPlayerManager()->GetPlayerPos( PLAYER_2 )
	};

	D3DXVECTOR3 rot[2] = {
		m_pManager->GetPlayerManager()->GetPlayerRot( PLAYER_1 ),
		m_pManager->GetPlayerManager()->GetPlayerRot( PLAYER_2 )
	};

	/* ��������� */
	switch( m_FrameCount )
	{
		// �t���[���ʂ̏���
	case 0:
		m_pManager->GetPlayerManager()->SetAnimType( m_Player, CPlayer::PLAYER_DROP_KICK_LEFT );
		CEffect::Create( 60, EFFECT_AURA_START, false, pos[m_Player] + TranslateCoord( m_Player, DROPKICK_EFFECT_AURA_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)DROPKICK_EFFECT_AURA_SCALE );
		m_pManager->GetPlayerManager()->SetAnimSpd( m_Player, DEFFAULT_ANIM_SPD * 0.5f );
		m_pManager->GetCameraManager()->CameraMoveToCoord(
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 100.0f, -100.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 40.0f, 200.0f, -140.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 70.0f, 0.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 40.0f, 0.0f, 0.0f ) ),
			80 );
		break;

	case 75:
		m_pManager->GetPlayerManager()->SetAnimType( m_Enemy, CPlayer::PLAYER_DROP_KICK_DAMAGE_LEFT );
		m_pEffect = CEffect::Create( 30, EFFECT_DAGEKI_KYO, false, pos[m_Player] + TranslateCoord( m_Player, DROPKICK_EFFECT_HIT_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)DROPKICK_EFFECT_HIT_SCALE );
		break;

	case 80:
		m_pEffect->SetPlaySpeed( 0.0f );
		m_pManager->GetPlayerManager()->SetAnimSpd( m_Player, DEFFAULT_ANIM_SPD * 0.0f );
		m_pManager->GetPlayerManager()->SetAnimSpd( m_Enemy, DEFFAULT_ANIM_SPD * 0.0f );
		m_pManager->GetCameraManager()->CameraMoveToCoord(
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 40.0f, 200.0f, -140.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 200.0f, 200.0f, -140.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 40.0f, 0.0f, 0.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 40.0f, 0.0f, 0.0f ) ),
			120 );
		break;

	case 140:
		m_pEffect->SetPlaySpeed( 1.0f );
		m_pEffect = NULL;
		m_pManager->GetPlayerManager()->SetAnimSpd( m_Player, DEFFAULT_ANIM_SPD * 1.0f );
		m_pManager->GetPlayerManager()->SetAnimSpd( m_Enemy, DEFFAULT_ANIM_SPD * 1.0f );
		m_pManager->GetCameraManager()->StartCameraShake( VECTOR3_ZERO, 10.0f, 20, 0 );
		m_pManager->GetPlayerManager()->TakeDamage( m_Enemy, DROPKICK_DAMAGE );
		break;

	case 180:
		m_pManager->GetPlayerManager()->SetAnimSpd( m_Player, DEFFAULT_ANIM_SPD * 0.0f );
		m_pManager->GetPlayerManager()->SetAnimSpd( m_Enemy, DEFFAULT_ANIM_SPD * 0.0f );
		break;
	}
	/* �����܂Ō� */

	CDirect::CheckEnd();
}

//----EOF----