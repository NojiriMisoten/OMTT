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
#include "../../MAIN/main.h"
#include "../../TEXTURE/CTexture.h"
#include "../../CONST/const.h"
#include "../../MATH/math.h"

//*****************************************************************************
// �񋓑�
//*****************************************************************************
// �v���C�I���e�B�̎��
typedef enum
{
	TYPE_PRIORITY_0 = 0,
	TYPE_PRIORITY_BG,					// BG
	TYPE_PRIORITY_FIELD,				// �t�B�[���h
	TYPE_PRIORITY_GOAL,					// �S�[��
	TYPE_PRIORITY_THREAD_OF_FOOTHOLD,	// ����ɂȂ鎅
	TYPE_PRIORITY_THREAD_OF_TRAP,		// 㩂̎�
	TYPE_PRIORITY_TRESURE,				// ��
	TYPE_PRIORITY_PLAYER,				// �v���C���[
	TYPE_PRIORITY_ATTACK,				// �U��
	TYPE_PRIORITY_EFFECT,				// �G�t�F�N�g
	TYPE_PRIORITY_UI,					// UI
	TYPE_PRIORITY_PAUSE,
	TYPE_PRIORITY_FADE,
	TYPE_PRIORITY_MAX
}TYPE_PRIORITY;

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// UV�l�\����
typedef struct
{
	float left;
	float right;
	float top;
	float bottom;
}UV_INDEX;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CScene
{
public:
	// �I�u�W�F�^�C�v
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_2D,					// 2D
		OBJTYPE_PLAYER,				// �v���C���[
		OBJTYPE_ATTACK,				// �U��
		OBJTYPE_THREAD,				// ��
		OBJTYPE_FIELD,				// �t�B�[���h
		OBJTYPE_TREASURE,			// ��
		OBJTYPE_SCORE,				// �X�R�A
		OBJTYPE_GOAL,				// GOAL
		OBJTYPE_MAX					// �I�u�W�F�^�C�v����
	}OBJTYPE;

	//======================================================
	// �R���X�g���N�^
	// ����: �v���C�I���e�B�A�I�u�W�F�^�C�v
	//======================================================
	CScene(int nPriority = DEFAULT_PRIORITY, OBJTYPE m_objType = OBJTYPE_NONE);

	// �f�X�g���N�^
	virtual ~CScene() = 0;

	//======================================================
	// ������
	// ����: ���W�A���A�����A�e�N�X�`���[�^�C�v�A(���������A�c������)
	// �߂�l: ����
	//======================================================
	virtual HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock = 1, int hblock = 1) = 0;
	
	// �I��
	virtual void Uninit(void) = 0;

	// �X�V
	virtual void Update(void) = 0;

	// �`��
	virtual void Draw(void) = 0;

	// �|�W�V�����A�N�Z�T
	virtual D3DXVECTOR3 GetPos(void) = 0;
	virtual void		SetPos(D3DXVECTOR3 pos) = 0;
	virtual void		SetPos(float x,float y, float z) = 0;

	// �I�[���h�|�W�V�����A�N�Z�T
	virtual D3DXVECTOR3 GetOldPos(void) {return D3DXVECTOR3(0.f,0.f,0.f);};
	virtual void		SetOldPos(D3DXVECTOR3 pos) {};
	virtual void		SetOldPos(float x,float y, float z) {};

	// ���b�g�A�N�Z�T
	virtual D3DXVECTOR3 GetRot(void) = 0;
	virtual void		SetRot(D3DXVECTOR3 rot) = 0;
	virtual void		SetRot(float x,float y,float z) = 0;

	// �I�[���h���b�g�A�N�Z�T
	virtual D3DXVECTOR3 GetOldRot(void){return D3DXVECTOR3(0.f,0.f,0.f);};
	virtual void		SetOldRot(D3DXVECTOR3 rot){};
	virtual void		SetOldRot(float x,float y,float z){};

	// �o�E���f�B���O�{�b�N�X�p�A�N�Z�T
	virtual D3DXVECTOR3 GetVertexMin(void){return D3DXVECTOR3(0.f,0.f,0.f);};
	virtual D3DXVECTOR3 GetVertexMax(void){return D3DXVECTOR3(0.f,0.f,0.f);};

	// ���g�����[�X�֐�
	void Release(void);

	// �����N���珜�O�֐�
	void UnLinkList(void);

	//=================================================
	// �I�u�W�F�^�C�v�Q�b�g�֐�
	// �߂�l: �I�u�W�F�^�C�v
	//=================================================
	OBJTYPE GetObjType(void){return m_objType;};
	
	//=================================================
	// �I�u�W�F�^�C�v�Z�b�g�֐�
	// ����: �I�u�W�F�^�C�v
	//=================================================
	void SetObjType(OBJTYPE type);

	//=================================================
	// ���X�g�̐擪�A�h���X�Q�b�g�֐�
	// ����: �v���C�I���e�B
	// �߂�l: �w��v���C�I���e�B�̐擪�A�h���X
	//=================================================
	static CScene* GetTopAddress(int priority){return m_apTop[priority];};

	//=================================================
	// ���g�̎��̃A�h���X�Q�b�g
	// �߂�l: ���̃A�h���X
	//=================================================
	CScene* GetNextAddress(void){return m_pNext;};

	// �S�čX�V
	static void UpdateAll(void);

	//=================================================
	// �w�肵���v���C�I���e�B�̂ݍX�V
	// ����: �v���C�I���e�B
	//=================================================
	static void UpdateChoice(int priority);

	// �S�čX�V
	static void DrawAll(void);

	//=================================================
	// �w�肵���v���C�I���e�B�̂ݕ`��
	// ����: �v���C�I���e�B
	//=================================================
	static void DrawChoice(int priority);

	// �S�ă����[�X
	static void ReleaseAll(void);

private:
	// Z�\�[�g
	static void ZSort(void);

	static CScene		*m_apTop[TYPE_PRIORITY_MAX];	// ���X�g�̐擪�A�h���X
	static CScene		*m_apCur[TYPE_PRIORITY_MAX];	// ���X�g�̏I�[�A�h���X
	static int			m_nNumInList[TYPE_PRIORITY_MAX];// ���X�g�̒��g�̌�
	CScene				*m_pPrev;						// �O�̃I�u�W�F�N�g�ւ̃|�C���^
	CScene				*m_pNext;						// ���̃I�u�W�F�N�g�ւ̃|�C���^
	CScene				*m_pDrawPrev;					// �O�̃I�u�W�F�N�g�ւ̃|�C���^
	CScene				*m_pDrawNext;					// ���̃I�u�W�F�N�g�ւ̃|�C���^
	int					m_nPriority;					// �v���C�I���e�B
	bool				m_bDelete;						// �����t���O
	OBJTYPE				m_objType;						// �^�C�v
};

#endif
//----EOF----