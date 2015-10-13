//=============================================================================
//
// CTreasure�N���X [CTreasure.cpp]
// Author : �˖{�@�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CTreasure.h"
#include "../PLAYER/AI/mersenne_twister.h"
#include "../../../MANAGER/CManager.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
// �e�N�X�`���֘A
static const TEXTURE_TYPE TREASURE_TEXTURE = TEXTURE_TREASURE;
static const int TREASURE_TEXTURE_X = 13;
static const int TREASURE_TEXTURE_Y = 1;
// ����Q�c
static const float TREASURE_WIDTH = 70;
static const float TREASURE_HEIGHT = 70;
static const int TREASURE_ANIME_SPEED = 5;
// ����A�C�R���Q�c
static const float TREASURE_ICON_WIDTH = 40;
static const float TREASURE_ICON_HEIGHT = 40;
static const int TREASURE_ICON_ANIME_SPEED = 3;

// ����̏����ԍ��i�N�������Ă��Ȃ��Ƃ���-1�j
static const short OWNER_NONE = -1;

// ���炫��G
static const D3DXVECTOR3 TREASURE_KIRA_SIZE = D3DXVECTOR3(80, 80, 0);
static const int TREASURE_KIRA_TEX_X = 8;
static const int TREASURE_KIRA_TEX_Y = 1;
static const int TREASURE_KIRA_ANIME_SPEED = 2;
// �_�X�����G�t�F�N�g
static const D3DXVECTOR3 TREASURE_LIGHT_SIZE = D3DXVECTOR3(100, 100, 0);
static const int TREASURE_LIGHT_TEX_X = 10;
static const int TREASURE_LIGHT_TEX_Y = 1;
static const int TREASURE_LIGHT_ANIME_SPEED = 4;

// �󕨂��W�����v���鎞�ԁi�v���C�������Ƃ��Ă��玟�ɃL���b�`�ł���܂ł̎��ԁj
static const int TREASURE_JUMP_INTERVAL = 60;

// �󕨂𗎂Ƃ�����
static const float TREASURE_FALL_DIST = 10;

// �󕨂𗎂Ƃ��ʒu�̃e�[�u��
static const int TREASURE_FALL_TABLE_NUM = 8;
static const D3DXVECTOR3 TREASURE_FALL_VELOCITY[TREASURE_FALL_TABLE_NUM] = {
	D3DXVECTOR3(TREASURE_FALL_DIST * +0.0f, -8, 0),
	D3DXVECTOR3(TREASURE_FALL_DIST * +0.0f, -4, 0),
	D3DXVECTOR3(TREASURE_FALL_DIST * +1.0f, -6, 0),
	D3DXVECTOR3(TREASURE_FALL_DIST * -1.0f, -6, 0),
	D3DXVECTOR3(TREASURE_FALL_DIST * +0.5f, -7, 0),
	D3DXVECTOR3(TREASURE_FALL_DIST * +0.5f, -7, 0),
	D3DXVECTOR3(TREASURE_FALL_DIST * -0.5f, -5, 0),
	D3DXVECTOR3(TREASURE_FALL_DIST * -0.5f, -5, 0),
};

// �ړ��ʂ̌�����
static const float TREASURE_MOVE_RESIST = 0.9f;
// �d��
static const float TREASURE_GRAVITY = 0.2f;

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CTreasure::CTreasure(LPDIRECT3DDEVICE9 *pDevice, int nPriority, OBJTYPE m_objType) :CSceneAnime(pDevice, nPriority, m_objType)
{
	// ����̏�ԏ�����
	m_TreasureState = TREASURE_STATE_OWNER_NONE;

}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CTreasure ::~CTreasure(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
void CTreasure::Init(D3DXVECTOR3 pos)
{
	// ������
	CSceneAnime::Init(pos, TREASURE_WIDTH, TREASURE_HEIGHT, TREASURE_TEXTURE,
		TREASURE_TEXTURE_X, TREASURE_TEXTURE_Y, TREASURE_ANIME_SPEED, -1);

	SetAutoUpdate(true);
	m_nJumpCount = 0;
	m_vVelocity = D3DXVECTOR3(0, 0, 0);
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CTreasure::Uninit(void)
{
	CScene2D::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CTreasure::Update(void)
{
	CSceneAnime::Update();

	// jump�A�j���[�V�����X�V
	if (m_TreasureState == TREASURE_STATE_JUMPING){
		
		// jump����
		m_vVelocity.y += TREASURE_GRAVITY;
		m_vVelocity.x *= TREASURE_MOVE_RESIST;
			
		// �J�E���g
		m_nJumpCount++;
		if (m_nJumpCount > TREASURE_JUMP_INTERVAL){
			// ���ԃ��Z�b�g
			m_TreasureState = TREASURE_STATE_OWNER_NONE;
			m_vVelocity = D3DXVECTOR3(0, 0, 0);
			m_nJumpCount = 0;
		}

		// ���Z
		m_vPos += m_vVelocity;

		// �G�t�F�N�g�ɍ��W��K�p
		m_pKira->SetPos(m_vPos);
		m_pLight->SetPos(m_vPos);
	}
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CTreasure* CTreasure::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos)
{
	// �쐬
	CTreasure* p = new CTreasure(pDevice);
	p->Init(pos);

	// �G�t�F�N�g�̍쐬
	p->m_pKira = CSceneAnime::Create(
		pDevice,
		pos, TREASURE_KIRA_SIZE.x, TREASURE_KIRA_SIZE.y,
		TEXTURE_TREASURE_KIRAKIRA, TREASURE_KIRA_TEX_X, TREASURE_KIRA_TEX_Y, TREASURE_KIRA_ANIME_SPEED, -1);
	p->m_pLight = CSceneAnime::Create(
		pDevice,
		pos, TREASURE_LIGHT_SIZE.x, TREASURE_LIGHT_SIZE.y,
		TEXTURE_TREASURE_LIGHT, TREASURE_LIGHT_TEX_X, TREASURE_LIGHT_TEX_Y, TREASURE_LIGHT_ANIME_SPEED, -1);

	p->m_pLight->SetDrawFlag(false);
	return p;
}

//*****************************************************************************
// �傫����e�N�X�`�����ŏ��̏�Ԃɖ߂�
// �󕨂��v���C�������Ƃ����ۂɌĂ΂��
//*****************************************************************************
void CTreasure::Reset(D3DXVECTOR3 pos){

	// �G�̕ύX
//	CScene2D::ChangeTexture(TREASURE_TEXTURE);
	CScene2D::SetWidth(TREASURE_WIDTH);
	CScene2D::SetHeight(TREASURE_HEIGHT);
	SetAnimeSpeed(TREASURE_ANIME_SPEED);

	// ���Ƃ�����
	SetFall();
	CManager::PlaySoundA(SOUND_LABEL_SE_DEST_TREASURE);
}

//*****************************************************************************
// �傫����e�N�X�`�����ŏ��̏�Ԃɖ߂�
// �󕨂��v���C�������Ƃ����ۂɌĂ΂��
//*****************************************************************************
void CTreasure::SetIcon(){

	// �G�̕ύX
//	CScene2D::ChangeTexture(TEXTURE_TREASURE_ICON);
	CScene2D::SetWidth(TREASURE_ICON_WIDTH);
	CScene2D::SetHeight(TREASURE_ICON_HEIGHT);
	SetAnimeSpeed(TREASURE_ICON_ANIME_SPEED);

	// �G�t�F�N�g�̕ύX
	m_pLight->SetHeight(150);
}


//*****************************************************************************
// �󕨂̏ꏊ���Z�b�g �G�t�F�N�g�Ƃ����ꏏ�ɃZ�b�g����
//*****************************************************************************
void CTreasure::SetPos(D3DXVECTOR3 pos){

	// ���W�Z�b�g
	m_vPos = pos;
	m_pKira->SetPos(pos);
	m_pLight->SetPos(pos + D3DXVECTOR3(0, 40, 0));
}

//*****************************************************************************
// �󕨂𗎂Ƃ���Ԃɂ���
//*****************************************************************************
void CTreasure::SetFall(){
	int index = mersenne_twister_int(0, TREASURE_FALL_TABLE_NUM - 1);
	m_vVelocity = TREASURE_FALL_VELOCITY[index];
	m_TreasureState = TREASURE_STATE_JUMPING;
}
//----EOF-------