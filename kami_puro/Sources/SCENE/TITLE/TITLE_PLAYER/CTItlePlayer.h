//=============================================================================
//
// CTitlePlayer�N���X [CTitlePlayer.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CTITLE_PLAYER_H_
#define _CTITLE_PLAYER_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../MAIN/main.h"
#include "../../../BASE_OBJECT/CSceneX.h"
#include "../../../SKINMESH/CSkinMesh.h"
#include "../../GAME/PLAYER/CPlayer.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CManager;
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTitlePlayer : public CPlayer
{
public:
	typedef enum
	{
		DRAW_MODE_GRAY = 0,
		DRAW_MODE_HALF,
		DRAW_MODE_NORMAL,
		DRAW_MODE_MAX,
	}DRAW_MODE;
	// �R���X�g���N�^
	CTitlePlayer(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE m_objType = OBJTYPE_X);

	// �f�X�g���N�^
	~CTitlePlayer(void);

	// �쐬
	static CTitlePlayer* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, D3DXVECTOR3& rot, SKIN_MESH_ANIM_MODEL type, CManager* pManager, int ID);

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �`��
	void DrawNormalRender(void);

	// �X�L�����b�V���Ōv�Z�������[���h�}�g���N�X�����ƂɃZ�b�g����
	void SetWorldMtx(D3DXMATRIX* worldMtx, PLAYER_RENDERER_TYPE type, int blendBoneNum);
	void SetWorldMtxForNormalRender(D3DXMATRIX* worldMtx, int blendBoneNum);

	void SetDrawMode(DRAW_MODE mode){ m_DrawMode = mode; };
private:
	// ������
	void Init(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, D3DXVECTOR3& rot, SKIN_MESH_ANIM_MODEL type, CManager* pManager, int ID);

	int m_DrawMode;
};

#endif
//----EOF----