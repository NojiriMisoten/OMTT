//=============================================================================
//
// CPlayer�N���X [CPlayer.h]
// Author : �����@�Ȉ�
//
//=============================================================================
#ifndef __CPLAYER_H__
#define __CPLAYER_H__

#include "../../CSCENE/CSceneAnime.h"
#include "../../../MANAGER/CManager.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
static const float PLAYER_DEFAULT_HP = 500.0f; 	// �v���C���[�̃f�t�H���g�̗̑�
static const float PLAYER_DEFAULT_MP = 300.0f;	// �v���C���[�̃f�t�H���g�̕ό`�p�|�C���g
static const bool PLAYER_MANUAL = TRUE;		  	// �v���C���[����}�j���A��
static const bool PLAYER_COMPUTER = FALSE;	  	// �v���C���[����AUTO
// �v���C���[�̈ړ����x(��)
static const float PLAYER_SPEED = 6.0f;

// �v���C���[���ړ�������ԂɂȂ������̌W��(��)
static const float PLAYER_MODE_SPEED_COEFFICIENT = 1.5f;


#define PLAYER_OPERATION	BOOL				// �v���C���[�̑���t���O

//-----------------------------------------------------------------------------
// �񋓑̒�`
//-----------------------------------------------------------------------------
// �v���C���[�̌`��
typedef enum
{
	PLAYER_MODE_NONE = 0,		// �ʏ���
	PLAYER_MODE_ATTACK,			// �U������
	PLAYER_MODE_SPEED,			// �f��������
	PLAYER_MODE_TRAP,			// �W�Q����
	PLAYER_MODE_MAX
}PLAYER_MODE;

// �v���C���[�̍s��
typedef enum
{
	PLAYER_ACTION_NONE = 0,
	PLAYER_ACTION_WALK,				// ���s
	PLAYER_ACTION_ATTACK,			// �U��
	PLAYER_ACTION_METAMORPHOSE,		// �ό`
	PLAYER_ACTION_THREAD,			// �����o��
	PLAYER_ACTION_KNOCK_BACK,		// ������
	PLAYER_ACTION_DOWN,				// �_�E�����
	PLAYER_ACTION_STOP,				// �~�܂���
	PLAYER_ACTION_MAX
}PLAYER_ACTION;

// �v���C���[�̌����Ă������
typedef enum
{
	PLAYER_DIRECTION_NONE = 0,
	PLAYER_DIRECTION_UP,			// ��
	PLAYER_DIRECTION_DOWN,			// ��
	PLAYER_DIRECTION_RIGHT,			// �E
	PLAYER_DIRECTION_LEFT,			// ��
	PLAYER_DIRECTION_UPPER_RIGHT,	// �E��
	PLAYER_DIRECTION_UPPER_LEFT,	// ����
	PLAYER_DIRECTION_DOWNER_RIGHT,	// �E��
	PLAYER_DIRECTION_DOWNER_LEFT,	// ����
	PLAYER_DIRECTION_MAX
}DIRECTION_PLAYER_FACING;

// �v���C���̌����Ă�����ɑ΂��Ẵx�N�g��
static const D3DXVECTOR3 PLAYER_DIRECTION_VECTOR[] = {
	D3DXVECTOR3(0, 0, 0),
	D3DXVECTOR3(0, -1, 0),
	D3DXVECTOR3(0, +1, 0),
	D3DXVECTOR3(+1, 0, 0),
	D3DXVECTOR3(-1, 0, 0),
	D3DXVECTOR3(+0.5f, -0.5f, 0),
	D3DXVECTOR3(-0.5f, -0.5f, 0),
	D3DXVECTOR3(+0.5f, +0.5f, 0),
	D3DXVECTOR3(-0.5f, +0.5f, 0)
};

// �v���C���̃e�N�X�`��������
static const int PLAYER_WALK_TEXTURE_SEP_X = 6;
static const int PLAYER_WALK_TEXTURE_SEP_Y = 6;

// �v���C���̐��ʃe�N�X�`���C���f�b�N�X �L�����Z���N�g�Ƃ��ł�����
static const int PLAYER_STOP_TEXTURE_MIN = 5;
static const int PLAYER_STOP_TEXTURE_MAX = 5;

static const int PLAYER_TEXTURE_WALK_FRONT_MIN = 6;
static const int PLAYER_TEXTURE_WALK_FRONT_MAX = 9;
static const int PLAYER_TEXTURE_WALK_BACK_MIN = 10;
static const int PLAYER_TEXTURE_WALK_BACK_MAX = 13;
static const int PLAYER_TEXTURE_WALK_LEFT_MIN = 1;
static const int PLAYER_TEXTURE_WALK_LEFT_MAX = 4;

static const int PLAYER_TEXTURE_ATTACK_FRONT_MIN = 14;
static const int PLAYER_TEXTURE_ATTACK_FRONT_MAX = 17;
static const int PLAYER_TEXTURE_ATTACK_BACK_MIN = 18;
static const int PLAYER_TEXTURE_ATTACK_BACK_MAX = 21;
static const int PLAYER_TEXTURE_ATTACK_LEFT_MIN = 22;
static const int PLAYER_TEXTURE_ATTACK_LEFT_MAX = 25;

static const int PLAYER_TEXTURE_THREAD_FRONT_MIN = 26;
static const int PLAYER_TEXTURE_THREAD_FRONT_MAX = 28;
static const int PLAYER_TEXTURE_THREAD_BACK_MIN = 29;
static const int PLAYER_TEXTURE_THREAD_BACK_MAX = 31;
static const int PLAYER_TEXTURE_THREAD_LEFT_MIN = 32;
static const int PLAYER_TEXTURE_THREAD_LEFT_MAX = 34;

// �v���C���̌����ɑ΂��Ẵe�N�X�`���C���f�b�N�X�̍ŏ��ƍő�
static const short PLAYER_TEXTURE_INDEX_MIN[PLAYER_DIRECTION_MAX] = {
	0,	// �Ȃ�
	PLAYER_TEXTURE_WALK_BACK_MIN,		// ��
	PLAYER_TEXTURE_WALK_FRONT_MIN,		// ��
	PLAYER_TEXTURE_WALK_LEFT_MIN,		// �E
	PLAYER_TEXTURE_WALK_LEFT_MIN,		// ��
	PLAYER_TEXTURE_WALK_BACK_MIN,		// �E��
	PLAYER_TEXTURE_WALK_BACK_MIN,		// ����
	PLAYER_TEXTURE_WALK_FRONT_MIN,		// �E��
	PLAYER_TEXTURE_WALK_FRONT_MIN,		// ����
};
static const short PLAYER_TEXTURE_INDEX_MAX[PLAYER_DIRECTION_MAX] = {
	0,	// �Ȃ�
	PLAYER_TEXTURE_WALK_BACK_MAX,		// ��
	PLAYER_TEXTURE_WALK_FRONT_MAX,		// ��
	PLAYER_TEXTURE_WALK_LEFT_MAX,		// �E
	PLAYER_TEXTURE_WALK_LEFT_MAX,		// ��
	PLAYER_TEXTURE_WALK_BACK_MAX,		// �E��
	PLAYER_TEXTURE_WALK_BACK_MAX,		// ����
	PLAYER_TEXTURE_WALK_FRONT_MAX,		// �E��
	PLAYER_TEXTURE_WALK_FRONT_MAX,		// ����
};

// �v���C����HP�̎c����
enum PLAYER_HP_STATE{
	PLAYER_HP_STATE_NORMAL,
	PLAYER_HP_STATE_LOW,
	PLAYER_HP_STATE_VERY_LOW,
	PLAYER_HP_STATE_DIE,
	PLAYER_HP_STATE_MAX
};
// �v���C����HP�̎c���Ԃ����߂�HP�̒l
static const float PLAYER_HP_STATE_RATIO[] = {
	1.0f,	// PLAYER_NORMAL
	0.3f,	// PLAYER_LOW
	0.1f,	// PLAYER_VERY_LOW
	0.0f	// PLAYER_DIE
};
// �v���C���̎c��HP��Ԃ��Ƃ̐Ԃ��_�ł���J�E���g��
static const short PLAYER_HP_STATE_FLASH_INTERVAL[] = {
	-1,	// PLAYER_NORMAL
	30,	// PLAYER_LOW
	12,	// PLAYER_VERY_LOW
	-1	// PLAYER_DIE
};

// �v���C���̃A�j���X�s�[�h
static const int PLAYER_ANIME_SPEED = 10;
//-----------------------------------------------------------------------------
// �O���錾
//-----------------------------------------------------------------------------
class CAttackManager;
class CThreadManager;
class CTreasure;
class CMp;
class CEffectManager;
class C_CPU_AI;
class CPlayerManager;
class CSceneAnime;
//-----------------------------------------------------------------------------
// �v���C���[�N���X��`
//-----------------------------------------------------------------------------
class CPlayer : public CSceneAnime
{
public:
	// �R���X�g���N�^
	// �����@�f�o�C�X�A�ԍ��A�I�u�W�F�^�C�v
	CPlayer(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_PLAYER, OBJTYPE objType = OBJTYPE_PLAYER);

	// �f�X�g���N�^
	~CPlayer();

	// ������
	// �����@���W�A���A�����A�e�N�X�`��
	void Init(D3DXVECTOR3 pos,
		float fWidth,
		float fHeight,
		TEXTURE_TYPE texture,
		BOOL playerOperation,
		CAttackManager *pAttackManager,
		CThreadManager *pThreadManager,
		CEffectManager *pEffectManager,
		short sPlayerNumber,
		bool *bPlayerControl,
		CPlayerManager* pPlayerMnager);

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �`��
	void Draw(void);

	// �N���G�C�g
	// �����@�f�o�C�X�A�ԍ��A���W�A���A�����A�e�N�X�`��,�v���C���[����t���O,�U���}�l�[�W���[,�v���C���[�ԍ�
	static CPlayer* Create(LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos,
		float fWidth,
		float fHeight,
		TEXTURE_TYPE texture,
		BOOL playerOperation,
		CAttackManager *pAttackManager,
		CThreadManager *pThreadManager,
		CEffectManager *pEffectManager,
		short sPlayerNumber,
		bool *bPlayerControl,
		CPlayerManager* pPlayerMnager);

	// ���݂̕ό`��Ԃ̎擾
	// �߂�l�@�v���C���[�̌��݂̏��
	PLAYER_MODE GetPlayerMode(void){ return m_Mode; };

	// �����Ԃֈڍs
	//  �����A�߂�l�@����
	//  �v���C���[�̍s����Ԃ�����ɕύX���邾��
	void SetPlyerKnockBack(void);

	// �_�E����Ԃֈڍs
	//  �����A�߂�l�@����
	//  �v���C���[�̍s����Ԃ��_�E���ɕύX���邾��
	void SetPlayerDown(void);

	// �󕨂��E���Ƃ��̏��� �󕨂��A�C�R���ɕύX
	void SetTreasure(CTreasure *pTreasure);

	// �󕨂��̂Ă鎞�̏��� �A�C�R����󕨂ɕύX
	void FallTreasure();

	// �v���C���̌������ԂŃe�N�X�`�����X�V����
	void UpdatePlayerAnimation(void);

	// �v���C���̌�����ύX����֐�
	void SetFace(DIRECTION_PLAYER_FACING value);

	// �̗̓Z�b�^�[
	void AddHp(float fPoint);

	// MP�Z�b�^�[
	void AddMp(float fPoint);

	// MP�����p�֐�
	void MPReduce(void);

	// �v���C���ԍ��Q�b�^�[
	short GetPlayerNum(){
		return m_sNumber;
	}

	// �v���C���ێ��󔠃C���X�^���X�Q�b�^�[
	CTreasure *GetTreasure(){
		return m_pTreasure;
	}

	// �I�[���h�|�W�V�����A�N�Z�T
	D3DXVECTOR3 GetOldPos(void) { return m_vPosOld; }

	// �ݑ��Z�b�g
	void SetSlowSpeed(bool bSlowSpeed){ m_bSlowSpeed = bSlowSpeed;}

	// �v���C���[�}�l�[�W���[�Q�b�^�[
	CPlayerManager* GetPlayerManager(void){ return m_pPlayerManager; };

private:
	//---------------------------------
	// �֐�
	//---------------------------------
	// �ړ�����
	void Move(void);

	// �U��
	void Attack(void);

	// �ό`
	// �����@�ό`����^�C�v
	void Metamorphose(void);

	// ������
	void SpidersThread(void);

	// �ό`�A�j���[�V����
	void MetamorphoseAnimation(void);

	// �����Ԃ̏���
	void KnockBack(void);

	// �_�E����Ԃ̏���
	void PlayerDown(void);

	// ���G��Ԃł̏���
	void Matchless(void);

	// �v���C���[�̃e�N�X�`����UV�l�̐؂�ւ�(U�̐؂�ւ�)
	void ChangeTextureFaceU(void);

	// �v���C���[���ړ��`�Ԃ̍ۂ̍U���ł̈ړ�
	void Rush(void);

	// �v���C����HP�c��X�e�[�g�Ǘ�
	void UpdatePlayerHpState(void);
	void UpdatePlayerRed(void);

	// �v���C���̓ݑ���ԊǗ�
	void UpdateSlow();

	//---------------------------------
	// �ϐ�
	//---------------------------------
	float					m_fMoveSpeedY;		// �v���C���[��X�����̈ړ���
	float					m_fHP;				// �v���C���[�̗̑�
	float					m_fMoveSpeedX;		// �v���C���[��X�����̈ړ���
	float					m_fMP;				// �v���C���[�̕ό`�p�̃|�C���g
	BOOL					m_bOperation;		// �v���C���[�̑���t���O
	D3DXVECTOR3				m_vPosOld;			// �P�t���[���O�̍��W
	D3DXVECTOR3				m_vPosDest;			// �P�t���[���O�̍��W
	D3DXVECTOR3				m_vRotDest;			// �ړI�̊p�x
	PLAYER_MODE				m_Mode;				// ���݂̃v���C���[�̌`��
	PLAYER_MODE				m_ModeDest;			// �ړI�̃v���C���[�̌`��
	PLAYER_ACTION			m_Action;			// �v���C���[�����ݍs���Ă���s��
	short					m_sNumber;			// �}�l�[�W���Ɋ���U��ꂽ�v���C���[�ԍ�
	DIRECTION_PLAYER_FACING	m_PlayerFacing;		// �v���C���[�̌����Ă������
	DIRECTION_PLAYER_FACING	m_PlayerFacingOld;	// �v���C���[�̌����Ă������
	short					m_nTextureIndex;	// �v���C���e�N�X�`���̌��݂̃C���f�b�N�X
	short					m_nTextureCount;	// �e�N�X�`���A�j���[�V�����̃J�E���g
	short					m_sAnimTime;		// �ό`���̃A�j���[�V�����̎���
	short					m_sKnockBackTime;	// �m�b�N�o�b�N���̎���
	short					m_sDownTime;		// �_�E�����̎���
	short					m_sMatchlessTime;	// ���G��Ԃ̎���
	short					m_sKnockBackCount;	// �����ԂɂȂ�����
	short					m_sRushTime;		// �ړ��`�Ԃł̍U�����̃^�C�}�[
	bool					m_bMatchless;		// ���G��Ԃ��ǂ����̔���
	bool					m_bMetamorphose;	// �ό`���Ă����Ԃ��̃t���O
	bool					m_bSpeedAttack;		// �ړ��`�Ԃł̍U�������ǂ����̃t���O
	bool*					m_bPlayerControl;	// �v���C�����R���g���[���ł��邩�t���O
	bool					m_bSlowSpeed;		// �ݑ���ԂɂȂ��Ă��邩�ǂ����̃t���O
	CSceneAnime*			m_pSlow2D;			// �ݑ���Ԃ̃}�[�N
	int						m_nSlowCount;		// �ݑ���ԂɂȂ�����J�E���g���n�߂�
	CAttackManager*			m_pAttackManager;	// �U���}�l�[�W���[
	CThreadManager*			m_pThreadManager;	// ���}�l�[�W���[
	CEffectManager*			m_pEffectManager;	// �G�t�F�N�g�}�l�[�W���[
	CTreasure*				m_pTreasure;		// �󕨂��E�������̕󕨃|�C���^
	CMp*					m_pMp;				// MP�Q�[�W
	C_CPU_AI*				m_pAI;				// AI
	CPlayerManager*			m_pPlayerManager;	// �v���C���[�}�l�[�W���[
	int						m_nCoolTime;		// �U���Ȃǂ̃N�[���^�C���i�����Ȃ��j

	// �Ԃ�����n
	short					m_nRedCount;		// �Ԃ����邽�߂ɃJ�E���g
	bool					m_bRed;				// �Ԃ���Ԃ��t���O
	PLAYER_HP_STATE			m_HpState;			// �v���C���̎c��HP���
	// �V�F�[�_�[
	IDirect3DPixelShader9 *m_pPS;
	LPD3DXCONSTANTTABLE m_pPSC;

	// �e�N�X�`���A�j���[�V�����p�^�C�v
	enum TEXTURE_ANIMATION_TYPE{
		TEXTURE_ANIMATION_TYPE_WALK = 0,
		TEXTURE_ANIMATION_TYPE_ATTACK,
		TEXTURE_ANIMATION_TYPE_THREAD,
		TEXTURE_ANIMATION_TYPE_MAX,
	};
	// �e�N�X�`���A�j���[�V���������݂̂�����
	void SetTextureAnimeAttack();
	void SetTextureAnimeThread();
	// �e�N�X�`���A�j���[�V�����p�^�C�v�i�����A�U���A���j
	TEXTURE_ANIMATION_TYPE m_TextureAnimationType;
	// ���݂̃A�j���[�V�����\��̃C���f�b�N�X�̍ő�
	int m_nTextureIndexMax;
};

#endif // __CPLAYER_H__

// EOF