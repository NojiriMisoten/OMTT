//=============================================================================
//
// CScene�N���X [CScene.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CSCENE_H_
#define _CSCENE_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../MAIN/main.h"
#include "../TEXTURE/CTexture.h"
#include "../CONST/const.h"
#include "../MATH/math.h"
#include "../RENDERER/CRenderer.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CScene
{
public:
	typedef void (CScene::*FUNC)(void);

	// �I�u�W�F�^�C�v
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_2D,					// 2D
		OBJTYPE_3D,					// 3D
		OBJTYPE_X,					// X���f��
		OBJTYPE_ENEMY,				// �G
		OBJTYPE_PLAYER,				// �v���C���[
		OBJTYPE_FIELD,				// �t�B�[���h
		OBJTYPE_LIGHT,				// ���C�g
		OBJTYPE_MAX					// �I�u�W�F�^�C�v����
	}OBJTYPE;

	//======================================================
	// �R���X�g���N�^
	// ����: �v���C�I���e�B�A�I�u�W�F�^�C�v
	//======================================================
	CScene(OBJTYPE m_objType = OBJTYPE_NONE);

	// �f�X�g���N�^
	virtual ~CScene() = 0;

	//======================================================
	// ������
	// ����: ���W�A���A�����A�e�N�X�`���[�^�C�v�A(���������A�c������)
	// �߂�l: ����
	//======================================================
	void Init(void){};
	
	// �I��
	virtual void Uninit(void) = 0;

	// �X�V
	virtual void Update(void) = 0;

	// �`��
	virtual void Draw(void);

	// �|�W�V�����A�N�Z�T
	virtual D3DXVECTOR3& GetPos(void){ return m_Pos; };
	virtual void		SetPos(D3DXVECTOR3& pos){ m_Pos = pos; };
	virtual void		SetPos(float& x, float& y, float& z){ m_Pos.x = x; m_Pos.y = y; m_Pos.z = z; };

	// ���b�g�A�N�Z�T
	virtual D3DXVECTOR3& GetRot(void){ return m_Rot; };
	virtual void		SetRot(D3DXVECTOR3& rot){ m_Rot = rot; };
	virtual void		SetRot(float& x, float& y, float& z){ m_Rot.x = x; m_Rot.y = y; m_Rot.z = z; };
	
	// ���g�����[�X�֐�
	void Release(void);

	// �����N�ɒǉ��֐�
	void AddLinkList(CRenderer::TYPE_RENDER_TEX priority);

	// �����N���珜�O�֐�
	void UnLinkList(CRenderer::TYPE_RENDER_TEX priority);

	//=================================================
	// �I�u�W�F�^�C�v�Q�b�g�֐�
	// �߂�l: �I�u�W�F�^�C�v
	//=================================================
	OBJTYPE GetObjType(void){return m_objType;};
	
	//=================================================
	// �I�u�W�F�^�C�v�Z�b�g�֐�
	// ����: �I�u�W�F�^�C�v
	//=================================================
	void SetObjType(OBJTYPE type){m_objType = type;};

	//=================================================
	// ���X�g�̐擪�A�h���X�Q�b�g�֐�
	// ����: �v���C�I���e�B
	// �߂�l: �w��v���C�I���e�B�̐擪�A�h���X
	//=================================================
	static CScene* GetTopAddress(int priority){return m_apTop[priority];};

	//=================================================
	// ���g�̎��̃A�h���X�Q�b�g
	// ����: �v���C�I���e�B
	// �߂�l: ���̃A�h���X
	//=================================================
	CScene* GetNextAddress(int priority){ return m_pNext[priority]; };

	// �S�čX�V
	static void UpdateAll(void);

	//=================================================
	// �w�肵���v���C�I���e�B�̂ݍX�V
	// ����: �v���C�I���e�B
	//=================================================
	static void UpdateChoice(int priority);

	// �S�ĕ`��
	static void DrawAll(void);

	// �S�ĉ��
	static void ReleaseAll(void);

	// CRenderer.h��enum�Ɠ������ԂɊ֐��|�C���^�ɓ����
	virtual void DrawToonObjectDepthRender(void){};
	virtual void DrawNormalRender(void){};
	virtual void DrawNormalVecRender(void){};
	virtual void DrawUIRender(void){};
	virtual void DrawShadowRender(void){};
	virtual void DrawBaseHighLuminanceRender(void){};
	virtual void DrawFadeRender(void){};

	//=======================================================================
	// �����`��t���O�Z�b�g�֐�
	// ����:�^�U�l
	//=======================================================================
	void SetDrawFlag(bool flag){ m_AutoDraw = flag; };

	//=======================================================================
	// �����`��t���O�Q�b�g�b�g�֐�
	// �߂�l:�^�U�l
	//=======================================================================
	bool GetDrawFlag(void){ return m_AutoDraw; };
private:
	// �S�Ẵ��X�g����O���
	void AllUnLinkList(void);

	static const int MAX_LIST_NUM = CRenderer::TYPE_RENDER_MAX - 7;	// ���X�g�̐�

	static CScene				*m_apTop[MAX_LIST_NUM];	// ���X�g�̐擪�A�h���X
	static CScene				*m_apCur[MAX_LIST_NUM];	// ���X�g�̏I�[�A�h���X
	CScene						*m_pPrev[MAX_LIST_NUM];	// �O�̃I�u�W�F�N�g�ւ̃|�C���^
	CScene						*m_pNext[MAX_LIST_NUM];	// ���̃I�u�W�F�N�g�ւ̃|�C���^
	bool						m_bDelete;				// �����t���O
	OBJTYPE						m_objType;				// �^�C�v
	bool						m_bInList[MAX_LIST_NUM];// ���X�g�o�^���Ă��邩
	bool						m_bDoUpdateFlag;		// ���ɍX�V������

	// �֐��|�C���^�e�[�u��
	static FUNC m_apDrawFunc[MAX_LIST_NUM];

protected:
	D3DXVECTOR3					m_Pos;		// ���W
	D3DXVECTOR3					m_Rot;		// �p�x
	bool		m_AutoDraw;				// �����ŕ`�悷�邩
};

#endif
//----EOF----