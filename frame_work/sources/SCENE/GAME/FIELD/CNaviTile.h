//=============================================================================
//
// CNaviTileクラス [CNaviTile.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CNAVITILE_H_
#define _CNAVITILE_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../MAIN/main.h"

//*****************************************************************************
// 構造体
//*****************************************************************************
// ナビタイル構造体
typedef enum{
	TILE_MODE_NULL,    // 未探索
	TILE_MODE_OPEN,    // オープン状態
	TILE_MODE_CLOSE,   // クローズ状態
	TILE_MODE_STOP,    // 立ち入り禁止
	TILE_MODE_MAX
}TILE_MODE;

typedef struct{
	int x_block;
	int y_block;
}TileIndex;

typedef struct{
	float cost;									// 探索開始場所 ゴールからの距離
	float heuristic;							// 現在地からここまでの距離
	float score;								// 合計
	TileIndex parent_tile;						// 親タイル 次はこのタイルに向かう
	TILE_MODE search;							// 探索したか
	D3DXVECTOR2 centerPos;						// 中心座標
	bool		canMove;						// 通れるか
	bool		staticTile;						// 外部から変更できない場所か（最初から道の所は、通れるかどうかを変更できないようにする）
}NAVI_TILE;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CNaviTile
{
public:
	// コンストラクタ
	CNaviTile();

	// デストラクタ
	~CNaviTile();
	
	// 終了
	void	Uninit(void);
	
	//====================================
	// ロード関数
	// 引数：ファイルパス
	//====================================
	void	Load(char* pNaviTileFileName);

	//====================================
	// クリエイト関数
	// 引数：ファイルパス
	// 戻り値：アドレス
	//====================================
	static CNaviTile* Create(char* pNaviTileFileName);
	
	//====================================
	// 通れる場所設定関数
	// 引数：座標、幅、大きさ、通れるか
	// 四角形で判定します
	//====================================
	void	SetCanMove(D3DXVECTOR2 pos, float width, float height, bool canMove);

	//====================================
	// ブロック数ゲット関数
	//====================================
	int GetXBlockNum(void){ return m_nXBlockNum; };
	int GetYBlockNum(void){ return m_nYBlockNum; };

	//====================================
	// タイルの座標ゲット関数
	//====================================
	D3DXVECTOR2 GetTilePos(TileIndex tile);

	//====================================
	// タイルコピー関数
	//====================================
	void CopyTile(NAVI_TILE** distTile);

private:
	
	// 非公開メンバ変数
	NAVI_TILE**	m_apTile;
	int			m_nXBlockNum;	// m_apTile[m_nYBlockNum][m_nXBlockNum]
	int			m_nYBlockNum;
	float		m_fBlockWidth;	// ブロックの幅
	float		m_fBlockHeight;	// ブロックの高さ

};


#endif
//----EOF-----