//=============================================================================
//
// CLightManager�N���X [CLightManager.h]
// Author : �˖{�r�F
//
// ���C�g�̐���cpp�Ō��߂āA���̐������C�g��new���悤���Ȃ�
//
// 
//=============================================================================
#ifndef _CLIGHTMANAGER_H_
#define _CLIGHTMANAGER_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../MAIN/main.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CManager;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CLightManager
{
public:
	// ���C�g�̗v�f�܂Ƃ߂�
	class CLight
	{
	public:
		CLight()
		{
			m_Pos = D3DXVECTOR3(0, 0, 0);
			m_Color = D3DXCOLOR(1, 1, 1, 1);
		}
		CLight(D3DXVECTOR3 pos, D3DXCOLOR color)
		{
			m_Pos = pos;
			m_Color = color;
		}
		~CLight(){};

		// ���W�̃Z�b�g
		void SetPos(D3DXVECTOR3 pos)
		{
			m_Pos = pos;
		}
		// ���W�̎擾
		D3DXVECTOR3 GetPos()
		{
			return m_Pos;
		}
		// �F�̃Z�b�g
		void SetColor(D3DXCOLOR color)
		{
			m_Color = color;
		}
		// �F�̎擾
		D3DXCOLOR GetColor()
		{
			return m_Color;
		}

	private:
		// �e���C�g�̍��W
		D3DXVECTOR3 m_Pos;
		// �e���C�g�̐F
		D3DXCOLOR m_Color;
	};

	// �R���X�g���N�^
	CLightManager(CManager* pManager);

	// �f�X�g���N�^
	~CLightManager(void);

	// �I��
	void Uninit(void);

	// ���C�g�̍X�V
	void Update(void);

	// ���C�g�̏��擾
	CLight* GetLight(int index){
		return m_pLight[index];
	}

	// �萔���C�g�̐�
	static const int LIGHT_NUM = 2;
private:

	// �}�l�[�W���[
	CManager* m_pManager;

	// ���C�g�̐�
	int m_LightNum;
	// ���C�g����
	CLight *m_pLight[LIGHT_NUM];

	// ���C�g����]���邽�߂̃J�E���g
	float m_Count;
	// �~�����C�g���Ŋ������p�x
	float m_RadOne;
};

#endif
//----EOF----