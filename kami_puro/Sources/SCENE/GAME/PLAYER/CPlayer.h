//=============================================================================
//
// CPlayer�N���X [CPlayer.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CPLAYER_H_
#define _CPLAYER_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../MAIN/main.h"
#include "../../../BASE_OBJECT/CSceneX.h"
#include "../../../SKINMESH/CSkinMesh.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CManager;
class CPlayer;

//*****************************************************************************
// �A�j���[�V�����R�[���o�b�N�p�n���h���[�N���X��`
// �L�����N�^�[���ƂɎ����Ȃ��Ƃ����Ȃ�
//*****************************************************************************
class CCallBackHandlerPlayer : public ID3DXAnimationCallbackHandler
{
public:
	// �R�[���o�b�N�֐�
	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData);
	CPlayer* m_pMyAddress;	// ���g�̃A�h���X
};

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CPlayer : public CSceneX
{
public:
	typedef enum
	{
		PLAYER_WAIT = 0,
		PLAYER_LARIAT,
		PLAYER_ELBOW_LEFT,
		PLAYER_ELBOW_RIGHT,
		PLAYER_DAMAGE_SMALL,
		PLAYER_CHOP_LEFT,
		PLAYER_CHOP_RIGHT,
		PLAYER_ANIM_MAX
	}PLAYER_ANIM_TYPE;

	typedef enum
	{
		RENDERER_TYPE_NORMAL = 0,
		RENDERER_TYPE_NORMAL_VEC,
		RENDERER_TYPE_DEPTH,
		RENDERER_TYPE_MAX
	}PLAYER_RENDERER_TYPE;

	// �R���X�g���N�^
	CPlayer(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE m_objType = OBJTYPE_X);

	// �f�X�g���N�^
	~CPlayer(void);

	// �쐬
	static CPlayer* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, SKIN_MESH_ANIM_MODEL type, CManager* pManager, int ID);

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �`��
	void DrawNormalRender(void);
	void DrawNormalVecRender(void);
	void DrawToonObjectDepthRender(void);

	// �X�L�����b�V���Ōv�Z�������[���h�}�g���N�X�����ƂɃZ�b�g����
	void SetWorldMtx(D3DXMATRIX* worldMtx, PLAYER_RENDERER_TYPE type);
	void SetWorldMtxForNormalRender(D3DXMATRIX* worldMtx);
	void SetWorldMtxForNormalVecRender(D3DXMATRIX* worldMtx);
	void SetWorldMtxForToonObjectDepthRender(D3DXMATRIX* worldMtx);

	int GetHP(void);
	PLAYER_ANIM_TYPE GetAnimState(void);
	
	// �A�j���[�V�����ύX
	void SetAnimType(int type, double moveRate = DEFFAULT_CHANGE_ANIM_SPD);

	// �A�j���[�V���������Ԃ̏�ԂɎw��
	void SetAnimMortionOfTime(int percent);

	// �A�j���[�V�������x�Z�b�g
	void SetAnimSpd(double spd);

	// �_���[�W����
	void TakeDamage( int damage );

	// �񕜏���
	void TakeHeal( int heal );

private:
	// ������
	void Init(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, SKIN_MESH_ANIM_MODEL type, CManager* pManager, int ID);

	// �ړ�
	D3DXVECTOR3 Move(void);

	// �ړ��t�F�[�Y
	void MovePhase(void);

	// �U���t�F�[�Y
	void AttackPhase(void);

	// �v���C���[�W�����v
	void PlayerJamp(void);

	CManager			*m_pManager;			// �}�l�[�W���[
	D3DXVECTOR3			m_vecFront;				// �O�x�N�g��
	D3DXVECTOR3			m_vecRight;				// �E�x�N�g��
	D3DXVECTOR3			m_vecUp;				// ��x�N�g��
	D3DXVECTOR3			m_DestRot;				// �ڕW�p�x
	D3DXVECTOR3			m_DestPos;				// �ڕW���W
	D3DXVECTOR3			m_MovPos;				// �ړ����x
	D3DXMATRIX			m_OldWorldMtx;			// �ȑO�̃��[���h�}�g���b�N�X

	CSkinMesh				*m_pCSkinMesh;			// �X�L�����b�V���A�j���[�V�����p
	CCallBackHandlerPlayer	*m_pFunc;				// �A�j���[�V�����R�[���o�b�N�p
	CALLBACK_TIMING			*m_pCallBackTimiming;	// �R�[���o�b�N�̃^�C�~���O

	LPDIRECT3DVERTEXSHADER9	*m_pVS;
	LPD3DXCONSTANTTABLE		*m_pVSC;
	LPDIRECT3DPIXELSHADER9	*m_pPS;
	LPD3DXCONSTANTTABLE		*m_pPSC;

	int					m_ID;					// �v���C���[��ID
	int					m_HP;					// �v���C���[��HP�A
	int					m_JampPower;			// �W�����v�̏u�ԓI�ȃp���[
	bool				m_JampFlag;				// �W�����v���邽�߂̃t���O
	PLAYER_ANIM_TYPE	m_AnimState;			// �A�j���̏��
};

#endif
//----EOF----