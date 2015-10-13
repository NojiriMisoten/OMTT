//=============================================================================
//
// CCursol�N���X [CCursol.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CCURSOL_H_
#define _CCURSOL_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../MAIN/main.h"
#include "../../TEXTURE/CTexture.h"
#include "../CSCENE/CScene.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CCursol : public CScene
{
public:
	//======================================================
	// �R���X�g���N�^
	// ����: �f�o�C�X
	//======================================================
	CCursol(LPDIRECT3DDEVICE9 *pDevice);

	// �f�X�g���N�^
	~CCursol();

	// �I���֐�
	void Uninit(void);

	// �X�V
	void Update(void);

	// �`��
	void Draw(void);

	// �`��t���O�Z�b�^�[
	void SetDrawFlag(bool flag){ m_bDraw = flag; };

	//==================================================================
	// �傫���Əꏊ�ύX�֐�
	// ����: ���W�A���A����
	//==================================================================
	void SetVertexPolygon(D3DXVECTOR3 pos, float width, float height);

	//==================================================================
	// �N���G�C�g�֐��֐�
	// ����: �f�o�C�X�A���W�A���A�����A�e�N�X�`���[�A���E�̋���
	// �߂�l: �|�C���^
	//==================================================================
	CCursol* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, float lengthOfLeftAndRight);

	// �|�W�V�����A�N�Z�T
	D3DXVECTOR3 GetPos(void){ return m_vPos;};
	void		SetPos(D3DXVECTOR3 pos);
	void		SetPos(float x,float y, float z);

	// ���b�g�A�N�Z�T
	D3DXVECTOR3 GetRot(void){return m_vRot;};
	void		SetRot(D3DXVECTOR3 rot){m_vRot = rot;};
	void		SetRot(float x,float y,float z){ m_vRot.x = x; m_vRot.y = y; m_vRot.z = z;};

private:
	static const int MENU_CURSOL_OBJ_NUM = 2; // ���E�����ɂP�Âo���̂łQ

	// �g���Ă��Ȃ����z�֐�
	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock = 1, int hblock = 1){return E_FAIL;};
	
	//==================================================================
	// ������
	// ����: ���W�A���A�����A�e�N�X�`���̎�ށA���E�̋����A(���������A�c������)
	// �߂�l: ����
	//==================================================================
	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, float lengthOfLeftAndRight, int wblock = 1, int hblock = 1);

	//=============================================
	// �F�Z�b�g�֐�
	// ����: �F
	//=============================================
	void SetColorPolygon(D3DXCOLOR color);

	LPDIRECT3DDEVICE9		*m_pD3DDevice;								// �f�o�C�X�I�u�W�F�N�g(�`��ɕK�v)
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVtxBuff[MENU_CURSOL_OBJ_NUM];			// ���_���W�����i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	LPDIRECT3DTEXTURE9		m_pD3DTexBuff[MENU_CURSOL_OBJ_NUM];			// �e�N�X�`�������i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	D3DXVECTOR3				m_vPos;										// �|���S���̒����̈ʒu
	D3DXVECTOR3				m_vRot;										// �|���S���̉�]
	float					m_fWidth;									// ��
	float					m_fHeight;									// ����
	float					m_fAlpha;									// �A���t�@�l
	float					m_fLengthOfLeftToRight;						// ���E�̗�������
	float					m_fMoveAlphaSign;							// �A���t�@�ύX�̕���
	bool					m_bDraw;									// �`��t���O
};
#endif
//----EOF----