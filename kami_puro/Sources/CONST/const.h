//=============================================================================
//
// 定数ファイル [const.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CONST_H_
#define _CONST_H_
//=============================================================================
// インクルード
//=============================================================================
#include "../MAIN/main.h"

#define SAFE_RELEASE(p)			if((p)!=NULL){(p)->Release();(p)=NULL;}
#define SAFE_DELETE(p)			if((p)!=NULL){delete (p);(p)=NULL;}
#define SAFE_DELETE_ARRAY(p)	if((p)!=NULL){delete[] (p);(p)=NULL;}

// MODELフォルダへの相対パス
static const char MODEL_FOLDER_PATH[] = "../data/MODEL/";

// TEXTUREフォルダへの相対パス
static const char TEX_FOLDER_PATH[] = "../data/TEXTURE/";

static const int MAX_LENGTH_FILE_PATH = 255;

// デフォルトのポリゴンカラー
static const D3DXCOLOR DEFAULT_POLYGON_COLOR(1.0f, 1.0f, 1.0f, 1.0f);

// デフォルトのアンビエントライトカラー
static const D3DXCOLOR DEFAULT_AMBIENT_LIGHT_COLOR(0.3f, 0.3f, 0.3f, 1.0f);

// アルファ最大値
static const float MAX_ALPHA = 1.0f;

// アルファ最小値
static const float MIN_ALPHA = 0.f;

// プライオリティの初期値
static const int DEFAULT_PRIORITY = 3;

// 三角形作成ポリゴン数
static const int NUM_POLYGON_CREATE_TRIANGLE = 3;

// デフォルトのアップベクトル
static const D3DXVECTOR3 DEFAULT_UP_VECTOR(0.f, 1.f, 0.f);

// デフォルトのフロントベクトル
static const D3DXVECTOR3 DEFAULT_FRONT_VECTOR(0.f, 0.f, 1.f);

// デフォルトのライトベクトル
static const D3DXVECTOR3 DEFAULT_RIGHT_VECTOR(1.f, 0.f, 0.f);

// テクスチャの最大UV値
static const float MAX_UV_VALUE = 1.0f;

// テクスチャの最小UV値
static const float MIN_UV_VALUE = 0.0f;

// 四角形描画時のポリゴン数
static const int DRAW_SQUARE_PRINITIV_NUM = 2;

// ２Ｄ変換マトリックス
static const D3DXMATRIX SCREEN_PROJ(2.f / (float)SCREEN_WIDTH, 0.0f						  , 0.0f, 0.0f,
									0.0f					 , -2.f / (float)SCREEN_HEIGHT, 0.0f, 0.0f,
									0.0f					 , 0.0f						  , 1.0f, 0.0f,
									-1.0f					 , 1.0f						  , 0.0f, 1.0f);

#endif
//----EOF----