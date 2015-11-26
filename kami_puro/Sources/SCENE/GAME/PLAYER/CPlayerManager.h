//=============================================================================
//
// CPlayerManager�N���X [CPlayerManager.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CPLAYERMANAGER_H_
#define _CPLAYERMANAGER_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../MAIN/main.h"
#include "../../../SKINMESH/CSkinMesh.h"
#include "CPlayer.h"

//*****************************************************************************
// �񋓑�
//*****************************************************************************
typedef enum
{
	PLAYER_1 = 0,
	PLAYER_2,
	PLAYER_MAX
}PLAYER_ID;

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CManager;
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CPlayerManager
{
public:
	// �R���X�g���N�^
	CPlayerManager(CManager* pManager);

	// �f�X�g���N�^
	~CPlayerManager(void);

	// �쐬
	void CreatePlayer(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, SKIN_MESH_ANIM_MODEL type);

	// �I��
	void Uninit(void);

	D3DXVECTOR3& GetPlayerPos(PLAYER_ID ID);

	int GetPlayerHP( PLAYER_ID ID );

	CPlayer::PLAYER_ANIM_TYPE GetPlayerState( PLAYER_ID ID );

	//==================================================
	// �A�j���[�V�����ύX
	// moveRate�͌��݂Ǝ��̃A�j���[�V�����̍�����䗦
	// �O�ŕ�Ԗ��Ńp�b�Ƃ����
	//==================================================
	void SetAnimType(PLAYER_ID ID, int type, double moveRate = 0.0);

	// �A�j���[�V���������Ԃ̏�ԂɎw��
	void SetAnimMortionOfTime(PLAYER_ID ID, int percent);

	// �A�j���[�V�������x�Z�b�g
	void SetAnimSpd(PLAYER_ID ID, double spd);

	// �_���[�W����
	void TakeDamage( PLAYER_ID ID, int damage ) { m_pPlayer[ID]->TakeDamage( damage ); };

	// �񕜏���
	void TakeHeal( PLAYER_ID ID, int heal ) { m_pPlayer[ID]->TakeHeal( heal ); };

	// FINISH�Z�g�p�\�t���O�A�N�Z�T
	bool GetUseFinishFlag(PLAYER_ID ID);
	void SetUseFinishFlag(PLAYER_ID ID, bool flag);

	// ���[�v�t���O�A�N�Z�T
	bool GetRopeFlag(PLAYER_ID ID);
	void SetRopeFlag(PLAYER_ID ID, bool flag);

private:
	static const int PLAYER_NUM = 2;

	CPlayer*	m_pPlayer[PLAYER_MAX];
	CManager*	m_pManager;
};

#endif
//----EOF----