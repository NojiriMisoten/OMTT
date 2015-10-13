//=============================================================================
//
// CNaviTileクラス [CNaviTile.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CNaviTile.h"
#include <stdio.h>

//*****************************************************************************
// マクロ
//*****************************************************************************

//*****************************************************************************
// スタティックメンバ変数
//*****************************************************************************

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CNaviTile::CNaviTile(void)
{
	m_apTile = NULL;
	m_nXBlockNum = 1;
	m_nYBlockNum = 1;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CNaviTile::~CNaviTile(void)
{
}

//*****************************************************************************
// 終了
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
// クリエイト
//*****************************************************************************
CNaviTile* CNaviTile::Create(char* pNaviTileFileName)
{
	CNaviTile* p = new CNaviTile;

	// 読み込み用ファイル
	FILE* fpr = fopen(pNaviTileFileName, "r");

	// 失敗したら
	if (fpr == NULL)
	{
#ifdef _DEBUG
		MessageBox(NULL, "ナビタイルファイル読み込み失敗\n", "エラー", MB_OK | MB_ICONERROR);
#endif
	}

	// 成功したら
	else
	{
		// 分割数取得
		fscanf(fpr, "%d", &p->m_nXBlockNum);
		fscanf(fpr, "%d", &p->m_nYBlockNum);

		// ブロックの幅高さ取得
		float width = (float)SCREEN_WIDTH / p->m_nXBlockNum;
		float height = (float)SCREEN_HEIGHT / p->m_nYBlockNum;
		p->m_fBlockWidth = width;
		p->m_fBlockHeight = height;
		D3DXVECTOR2 pos(width * 0.5f, height * 0.5f);

		// タイル作成
		p->m_apTile = new NAVI_TILE*[p->m_nYBlockNum];
		for (int i = 0; i < p->m_nYBlockNum; ++i)
		{
			p->m_apTile[i] = new NAVI_TILE[p->m_nXBlockNum];
			for (int n = 0; n < p->m_nXBlockNum; ++n)
			{
				// 通れる場所の設定
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

				// 座標の設定
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
// 通れる場所設定関数
//====================================
void CNaviTile::SetCanMove(D3DXVECTOR2 pos, float width, float height, bool canMove)
{
	// 左上と右下算出
	D3DXVECTOR2 vertexLeftTop(pos.x - width * 0.5f, pos.y - height * 0.5f);
	D3DXVECTOR2 vertexRightDown(pos.x + width * 0.5f, pos.y + height * 0.5f);

	for (int i = 0; i < m_nYBlockNum; ++i)
	{
		for (int n = 0; n < m_nXBlockNum; ++n)
		{
			// 道なら変更なし
			if (m_apTile[i][n].staticTile)
			{
				continue;
			}
			D3DXVECTOR2 tileLeftTop(m_apTile[i][n].centerPos.x - m_fBlockWidth * 0.25f, m_apTile[i][n].centerPos.y - m_fBlockHeight * 0.25f);
			D3DXVECTOR2 tileRightDown(m_apTile[i][n].centerPos.x + m_fBlockWidth * 0.25f, m_apTile[i][n].centerPos.y + m_fBlockHeight * 0.25f);

			// 左の判定
			if (vertexRightDown.x < tileLeftTop.x)
			{
				continue;
			}
			// 右の判定
			if (vertexLeftTop.x > tileRightDown.x)
			{
				continue;
			}
			// 上の判定
			if (vertexRightDown.y < tileLeftTop.y)
			{
				continue;
			}
			// 下の判定
			if (vertexLeftTop.y > tileRightDown.y)
			{
				continue;
			}

			// 変更
			m_apTile[i][n].canMove = canMove;
		}
	}
}

//===================================================
// タイルの座標ゲット関数
//===================================================
D3DXVECTOR2 CNaviTile::GetTilePos(TileIndex tile)
{
	return m_apTile[tile.y_block][tile.x_block].centerPos;
}

//===================================================
// タイルコピー関数
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