//=============================================================================
//
// CNaviTile�N���X [CNaviTile.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CNaviTile.h"
#include <stdio.h>

//*****************************************************************************
// �}�N��
//*****************************************************************************

//*****************************************************************************
// �X�^�e�B�b�N�����o�ϐ�
//*****************************************************************************

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CNaviTile::CNaviTile(void)
{
	m_apTile = NULL;
	m_nXBlockNum = 1;
	m_nYBlockNum = 1;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CNaviTile::~CNaviTile(void)
{
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CNaviTile::Uninit(void)
{
	for (int i = 0; i < m_nYBlockNum; ++i)
	{
		delete[] m_apTile[i];
	}
	delete[] m_apTile;
}

//*****************************************************************************
// �N���G�C�g
//*****************************************************************************
CNaviTile* CNaviTile::Create(char* pNaviTileFileName)
{
	CNaviTile* p = new CNaviTile;

	// �ǂݍ��ݗp�t�@�C��
	FILE* fpr = fopen(pNaviTileFileName, "r");

	// ���s������
	if (fpr == NULL)
	{
#ifdef _DEBUG
		MessageBox(NULL, "�i�r�^�C���t�@�C���ǂݍ��ݎ��s\n", "�G���[", MB_OK | MB_ICONERROR);
#endif
	}

	// ����������
	else
	{
		// �������擾
		fscanf(fpr, "%d", &p->m_nXBlockNum);
		fscanf(fpr, "%d", &p->m_nYBlockNum);

		// �u���b�N�̕������擾
		float width = (float)SCREEN_WIDTH / p->m_nXBlockNum;
		float height = (float)SCREEN_HEIGHT / p->m_nYBlockNum;
		p->m_fBlockWidth = width;
		p->m_fBlockHeight = height;
		D3DXVECTOR2 pos(width * 0.5f, height * 0.5f);

		// �^�C���쐬
		p->m_apTile = new NAVI_TILE*[p->m_nYBlockNum];
		for (int i = 0; i < p->m_nYBlockNum; ++i)
		{
			p->m_apTile[i] = new NAVI_TILE[p->m_nXBlockNum];
			for (int n = 0; n < p->m_nXBlockNum; ++n)
			{
				// �ʂ��ꏊ�̐ݒ�
				int move = 0;
				p->m_apTile[i][n].canMove = false;
				p->m_apTile[i][n].staticTile = false;
				p->m_apTile[i][n].search = TILE_MODE_NULL;
				fscanf(fpr, "%d", &move);
				if (move == 1)
				{
					p->m_apTile[i][n].canMove = true;
					p->m_apTile[i][n].staticTile = true;
				}

				// ���W�̐ݒ�
				p->m_apTile[i][n].centerPos = pos;

				p->m_apTile[i][n].score = 0;
				p->m_apTile[i][n].cost = 0;
				p->m_apTile[i][n].heuristic = 0;
				p->m_apTile[i][n].parent_tile.x_block = 0;
				p->m_apTile[i][n].parent_tile.y_block = 0;
				pos.x += width;
			}
			pos.x = width * 0.5f;
			pos.y += height;
		}
	}
	fclose(fpr);

	return p;
}

//====================================
// �ʂ��ꏊ�ݒ�֐�
//====================================
void CNaviTile::SetCanMove(D3DXVECTOR2 pos, float width, float height, bool canMove)
{
	// ����ƉE���Z�o
	D3DXVECTOR2 vertexLeftTop(pos.x - width * 0.5f, pos.y - height * 0.5f);
	D3DXVECTOR2 vertexRightDown(pos.x + width * 0.5f, pos.y + height * 0.5f);

	for (int i = 0; i < m_nYBlockNum; ++i)
	{
		for (int n = 0; n < m_nXBlockNum; ++n)
		{
			// ���Ȃ�ύX�Ȃ�
			if (m_apTile[i][n].staticTile)
			{
				continue;
			}
			D3DXVECTOR2 tileLeftTop(m_apTile[i][n].centerPos.x - m_fBlockWidth * 0.25f, m_apTile[i][n].centerPos.y - m_fBlockHeight * 0.25f);
			D3DXVECTOR2 tileRightDown(m_apTile[i][n].centerPos.x + m_fBlockWidth * 0.25f, m_apTile[i][n].centerPos.y + m_fBlockHeight * 0.25f);

			// ���̔���
			if (vertexRightDown.x < tileLeftTop.x)
			{
				continue;
			}
			// �E�̔���
			if (vertexLeftTop.x > tileRightDown.x)
			{
				continue;
			}
			// ��̔���
			if (vertexRightDown.y < tileLeftTop.y)
			{
				continue;
			}
			// ���̔���
			if (vertexLeftTop.y > tileRightDown.y)
			{
				continue;
			}

			// �ύX
			m_apTile[i][n].canMove = canMove;
		}
	}
}

//===================================================
// �^�C���̍��W�Q�b�g�֐�
//===================================================
D3DXVECTOR2 CNaviTile::GetTilePos(TileIndex tile)
{
	return m_apTile[tile.y_block][tile.x_block].centerPos;
}

//===================================================
// �^�C���R�s�[�֐�
//===================================================
void CNaviTile::CopyTile(NAVI_TILE** distTile)
{
	for (int i = 0; i < m_nYBlockNum; ++i)
	{
		for (int n = 0; n < m_nXBlockNum; ++n)
		{
			distTile[i][n] = m_apTile[i][n];
		}
	}
}
//----EOF-------