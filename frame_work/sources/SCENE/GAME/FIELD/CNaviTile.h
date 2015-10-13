//=============================================================================
//
// CNaviTile�N���X [CNaviTile.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CNAVITILE_H_
#define _CNAVITILE_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../MAIN/main.h"

//*****************************************************************************
// �\����
//*****************************************************************************
// �i�r�^�C���\����
typedef enum{
	TILE_MODE_NULL,    // ���T��
	TILE_MODE_OPEN,    // �I�[�v�����
	TILE_MODE_CLOSE,   // �N���[�Y���
	TILE_MODE_STOP,    // ��������֎~
	TILE_MODE_MAX
}TILE_MODE;

typedef struct{
	int x_block;
	int y_block;
}TileIndex;

typedef struct{
	float cost;									// �T���J�n�ꏊ �S�[������̋���
	float heuristic;							// ���ݒn���炱���܂ł̋���
	float score;								// ���v
	TileIndex parent_tile;						// �e�^�C�� ���͂��̃^�C���Ɍ�����
	TILE_MODE search;							// �T��������
	D3DXVECTOR2 centerPos;						// ���S���W
	bool		canMove;						// �ʂ�邩
	bool		staticTile;						// �O������ύX�ł��Ȃ��ꏊ���i�ŏ����瓹�̏��́A�ʂ�邩�ǂ�����ύX�ł��Ȃ��悤�ɂ���j
}NAVI_TILE;
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CNaviTile
{
public:
	// �R���X�g���N�^
	CNaviTile();

	// �f�X�g���N�^
	~CNaviTile();
	
	// �I��
	void	Uninit(void);
	
	//====================================
	// ���[�h�֐�
	// �����F�t�@�C���p�X
	//====================================
	void	Load(char* pNaviTileFileName);

	//====================================
	// �N���G�C�g�֐�
	// �����F�t�@�C���p�X
	// �߂�l�F�A�h���X
	//====================================
	static CNaviTile* Create(char* pNaviTileFileName);
	
	//====================================
	// �ʂ��ꏊ�ݒ�֐�
	// �����F���W�A���A�傫���A�ʂ�邩
	// �l�p�`�Ŕ��肵�܂�
	//====================================
	void	SetCanMove(D3DXVECTOR2 pos, float width, float height, bool canMove);

	//====================================
	// �u���b�N���Q�b�g�֐�
	//====================================
	int GetXBlockNum(void){ return m_nXBlockNum; };
	int GetYBlockNum(void){ return m_nYBlockNum; };

	//====================================
	// �^�C���̍��W�Q�b�g�֐�
	//====================================
	D3DXVECTOR2 GetTilePos(TileIndex tile);

	//====================================
	// �^�C���R�s�[�֐�
	//====================================
	void CopyTile(NAVI_TILE** distTile);

private:
	
	// ����J�����o�ϐ�
	NAVI_TILE**	m_apTile;
	int			m_nXBlockNum;	// m_apTile[m_nYBlockNum][m_nXBlockNum]
	int			m_nYBlockNum;
	float		m_fBlockWidth;	// �u���b�N�̕�
	float		m_fBlockHeight;	// �u���b�N�̍���

};


#endif
//----EOF-----