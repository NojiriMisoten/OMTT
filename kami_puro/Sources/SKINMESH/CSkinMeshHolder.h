//=============================================================================
//
// �X�L�����b�V������ [CSkinMeshHolder.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CSKINMESHHOLDER_H_
#define _CSKINMESHHOLDER_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../MAIN/main.h"
#include "CSkinMesh.h"
#include "../SCENE/GAME/PLAYER/CPlayer.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CSkinMesh;

//*****************************************************************************
// �񋓑̐錾
//*****************************************************************************
typedef enum
{
	SKIN_MESH_INFO_PLAYER_1 = 0,
	SKIN_MESH_INFO_PLAYER_2,
	SKIN_MESH_INFO_MAX
}SKIN_MESH_INFO;

//*****************************************************************************
// �N���X�錾
//*****************************************************************************
class CSkinMeshHolder
{
public:
	// �R���X�g���N�^
	CSkinMeshHolder(void);

	// �f�X�g���N�^
	~CSkinMeshHolder(void);

	// �I��
	static void Uninit(void);

	// �N���G�C�g
	static void Create(LPDIRECT3DDEVICE9 *pDevice);

	// �X�L�����b�V�����Q�b�g
	static CSkinMesh* GetSkinMesh(SKIN_MESH_INFO type){ return m_pSkinMeshHolder[type]; };

private:
	// �R�[���o�b�N�^�C�~���O�̏�����
	static void InitCallBackTiming(void);

	static CSkinMesh				*m_pSkinMeshHolder[SKIN_MESH_INFO_MAX];
	static CCallBackHandlerPlayer	*m_pFunc[SKIN_MESH_INFO_MAX];			// �A�j���[�V�����R�[���o�b�N�p
	static CALLBACK_TIMING			m_pCallBackTimiming[CPlayer::PLAYER_ANIM_MAX];	// �R�[���o�b�N�̃^�C�~���O
};

#endif
//----EOF----