//=============================================================================
//
// CDirectRope�N���X [CDirectRope.cpp]
// Author : �r���@���
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CDirectRope.h"
#include "../../MANAGER/CManager.h"
#include "../../CAMERA/CameraManager.h"
#include "../../EFECT/CEffect.h"
#include "../../EFECT/CEffectManager.h"
#include "../../SCENE/GAME/PLAYER/CPlayerManager.h"
#include "../CDirectorManager.h"

const D3DXVECTOR3 ROPE_EFFECT_AURA_OFFSET = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
const D3DXVECTOR3 ROPE_EFFECT_AURA_SCALE = D3DXVECTOR3( 10.0f, 10.0f, 10.0f );

const D3DXVECTOR3 ROPE_EFFECT_HIT_OFFSET = D3DXVECTOR3( 40.0f, 60.0f, 0.0f );
const D3DXVECTOR3 ROPE_EFFECT_HIT_SCALE = D3DXVECTOR3( 3.0f, 3.0f, 3.0f );

const int ROPE_DAMAGE = 10;

//=================================================
// �R���X�g���N�^
//=================================================
CDirectRope::CDirectRope(CManager *pManager) : CDirect( pManager )
{
	
}

//=================================================
// �f�X�g���N�^
//=================================================
CDirectRope::~CDirectRope( void )
{

}

//=================================================
// ������
//=================================================
void CDirectRope::Init( PLAYER_ID playerID )
{
	m_FrameCount = 0;		// �Œ�
	m_TotalFrame = 250;		// �Z���Ƃɕ�

	m_pPlayerManager = m_pManager->GetPlayerManager();
	m_pCameraManager = m_pManager->GetCameraManager();

	CDirect::SetPlayerID( playerID );
}

//=================================================
// �I��
//=================================================
void CDirectRope::Uninit( void )
{

}

//=================================================
// �X�V
//=================================================
void CDirectRope::Update( void )
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
		m_pPlayerManager->SetAnimType( m_Player, CPlayer::PLAYER_ROPE );
		CEffect::Create( 60, EFFECT_AURA_START, false, pos[m_Player] + TranslateCoord( m_Player, ROPE_EFFECT_AURA_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)ROPE_EFFECT_AURA_SCALE );
		break;
	
	case 50:
		m_pPlayerManager->TakeDamage( m_Enemy, ROPE_DAMAGE );
		m_pCameraManager->CameraMoveToCoord(
			pos[m_Enemy] + TranslateCoord( m_Player, D3DXVECTOR3( 10.0f, 200.0f, 1.0f ) ),
			pos[m_Enemy] + TranslateCoord( m_Player, D3DXVECTOR3( 20.0f, 100.0f, 1.0f ) ),
			pos[m_Enemy] + TranslateCoord( m_Player, D3DXVECTOR3( 10.0f, 20.0f, 0.0f ) ),
			pos[m_Enemy] + TranslateCoord( m_Player, D3DXVECTOR3( 20.0f, 20.0f, 0.0f ) ),
			120 );
		break;

	case 200:
		m_pPlayerManager->SetAnimType( m_Enemy, CPlayer::PLAYER_WAIT );
		break;
	}
	/* �����܂Ō� */



	CDirect::CheckEnd();
}

//----EOF----