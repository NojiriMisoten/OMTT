//=============================================================================
//
// CSceneAnime�N���X [CSceneAnime.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CSCENEANIME_H_
#define _CSCENEANIME_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../MAIN/main.h"
#include "../CSCENE/CScene.h"
#include "../CSCENE/CScene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
static const short	INFINIT_LOOP = -1;					// �������[�v������


//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CSceneAnime : public CScene2D
{
public:
	//======================================================
	// �R���X�g���N�^
	// ����: �f�o�C�X�A�v���C�I���e�B
	//======================================================
	CSceneAnime(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_EFFECT, OBJTYPE objType = OBJTYPE_NONE);

	// �f�X�g���N�^
	~CSceneAnime();

	//======================================================
	// �������@���[�v�Ȃ�
	// ����: ���W�A���A�����A�e�N�X�`���[�^�C�v�A�e�N�X�`�����������A�e�N�X�`���c�������A�I������
	// �߂�l: ����
	//======================================================
	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int sepalateX,int sepalateY, int endTime);

	//======================================================
	// �������@���[�v�Ȃ�
	// ����: ���W�A���A�����A�e�N�X�`���[�^�C�v�A�e�N�X�`�����������A�e�N�X�`���c�������A���[�v�񐔁i-1�Ŗ����j
	// �߂�l: ����
	//======================================================
	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int sepalateX,int sepalateY, int animSpd, int loopNum);
	
	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �`��
	void Draw(void);

	// �A�j���X�s�[�h�̃Z�b�^�[
	void SetAnimeSpeed(int value){
		m_nChangeTime = value;
	}

	// �e�N�X�`���̃Z�b�^
	void SetTexture(TEXTURE_TYPE tex, int x, int y){
		m_pD3DTexBuff = CTexture::GetTexture(tex);
		m_nSepalateNumX = x;
		m_nSepalateNumY = y;
	}

	//======================================================
	// �A�j���[�V�����ԍ��ݒ�
	// ����: �A�j���[�V�����ԍ�
	//======================================================
	void SetIndex(int nIdx);

	//======================================================
	// �A�j���[�V�����ԍ��ݒ�
	// ����: �A�j���[�V�����ԍ�,���]���邩
	//======================================================
	void SetIndex(int nIdx, bool reverse);

	//======================================================
	// �����X�V���邩
	// ����: �t���O
	//======================================================
	void SetAutoUpdate(bool flag){m_bAutoUpdate = flag;};

	//======================================================
	// �`�悷�邩
	// ����: �t���O
	//======================================================
	void SetDrawFlag(bool flag){m_bDraw = flag;};

	//======================================================
	// �N���G�C�g�֐��@���[�v�Ȃ�
	// ����: �f�o�C�X�A���W�A���A�����A�e�N�X�`���[�^�C�v�A�e�N�X�`�����������A�e�N�X�`���c�������A�I������
	// �߂�l: �������̃A�h���X
	//======================================================
	static CSceneAnime* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int sepalateX,int sepalateY, int endTime);
	
	//======================================================
	// �N���G�C�g�֐��@���[�v����
	// ����: �f�o�C�X�A���W�A���A�����A�e�N�X�`���[�^�C�v�A�e�N�X�`�����������A�e�N�X�`���c�������A���[�v�񐔁i-1�Ŗ����j
	// �߂�l: �������̃A�h���X
	//======================================================
	static CSceneAnime* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int sepalateX, int sepalateY, int animSpd, int loopNum, int priority = TYPE_PRIORITY_EFFECT);

protected:
	// ���[�v�Ȃ��X�V
	void Update_Normal(void);

	// ���[�v����X�V
	void Update_Loop(void);
	
	LPDIRECT3DDEVICE9	*m_pD3DDevice;			// �f�o�C�X�I�u�W�F�N�g(�`��ɕK�v)
	float				m_fLeft;				// UV�l(��)
	float				m_fRight;				// UV�l(�E)
	float				m_fTop;					// UV�l(��)
	float				m_fBottom;				// UV�l(��)
	int					m_nWidth;				// ����
	int					m_nHeight;				// �c��
	int					m_nSepalateNumX;		// �摜������(��)�E�E�Eex:���ɂT����ł�Ȃ�T
	int					m_nSepalateNumY;		// �摜������(�c)
	int					m_nIdx;					// �����ԍ�
	int					m_nDoAnimCounter;		// �A�j���[�V�������ԊǗ��p
	int					m_nEndTime;				// �I������
	int					m_nMaxLoopNum;			// �ő僋�[�v��
	int					m_nCurLoopNum;			// ���[�v��
	int					m_nChangeTime;			// �؂�ւ�����
	int					m_nMaxIdx;				// idx�̍ő�l
	bool				m_bAutoUpdate;			// �����X�V���邩
	bool				m_bDraw;				// �`�悷�邩
};
#endif
//----EOF----