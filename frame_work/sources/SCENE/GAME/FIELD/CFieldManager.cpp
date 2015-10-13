//=============================================================================
//
// CFieldManagerクラス [CFieldManager.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "CFieldManager.h"
#include "CField.h"
#include <stdio.h>
#include "CNaviTile.h"


//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	char* fieldFile;
	char* naviFile;
}FIELD_INFO;


//*****************************************************************************
// 定数定義
//*****************************************************************************
// フィールドファイルのパス
static const FIELD_INFO FIELD_INFO_FILE_PATH[CFieldManager::FIELD_MAX] =
{
	{ "data/FIELD/field01.txt", "data/FIELD/NAVI_FILE/navi_01.txt" },
	{ "data/FIELD/field02.txt", "data/FIELD/NAVI_FILE/navi_02.txt" },
	{ "data/FIELD/field03.txt", "data/FIELD/NAVI_FILE/navi_03.txt" },
	{ "data/FIELD/field04.txt", "data/FIELD/NAVI_FILE/navi_04.txt" },
	{ "data/FIELD/field05.txt", "data/FIELD/NAVI_FILE/navi_05.txt" },
	{ "data/FIELD/field06.txt", "data/FIELD/NAVI_FILE/navi_06.txt" },
	{ "data/FIELD/field07.txt", "data/FIELD/NAVI_FILE/navi_07.txt" },
	{ "data/FIELD/field08.txt", "data/FIELD/NAVI_FILE/navi_08.txt" },
};

//=========================================================================
// 静的メンバ変数
//=========================================================================
CNaviTile* CFieldManager::m_pNaviTile = NULL;

//=========================================================================
// コンストラクタ
//=========================================================================
CFieldManager::CFieldManager(void)
{
	
}

//=========================================================================
// デストラクタ
//=========================================================================
CFieldManager::~CFieldManager(void)
{
	
}

//=========================================================================
// クリエイト
//=========================================================================
CField* CFieldManager::CreateField(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType)
{
	CField* p = CField::Create(pDevice, pos, width, height, texType);
	return p;
}

//=========================================================================
// ロードフィールド
//=========================================================================
void CFieldManager::LoadField(LPDIRECT3DDEVICE9 *pDevice, FIELD_TYPE fieldType)
{
	int nFieldNameLength = 0;
	char FieldFilePath[MAX_PATH] = { 0 };
	char FieldName[MAX_PATH] = { 0 };

	// 読み込み用ファイル
	FILE* fpr = fopen(FIELD_INFO_FILE_PATH[fieldType].fieldFile, "r");

	// 失敗したら
	if (fpr == NULL)
	{
		MessageBox(NULL, "フィールドファイルの読み込みに失敗しました\n", "エラー", MB_OK | MB_ICONERROR);
	}

	// 成功したら
	else
	{
		int ans = 0;
		int ret = 0;
		while (1)
		{
			// 座標取得
			D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
			fscanf(fpr, "%f", &pos.x);
			fscanf(fpr, "%f", &pos.y);
			fscanf(fpr, "%f", &pos.z);
			
			// ROT取得
			D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, 0.f);
			fscanf(fpr, "%f", &rot.x);
			fscanf(fpr, "%f", &rot.y);
			fscanf(fpr, "%f", &rot.z);

			// 幅取得
			float width = 0.f;
			fscanf(fpr, "%f", &width);

			// 高さ取得
			float height = 0.f;
			fscanf(fpr, "%f", &height);

			int nTexNameLength = 0;
			char texFilePath[MAX_PATH] = { 0 };
			char texName[MAX_PATH] = { 0 };
			// 使用テクスチャの文字数を取得
			fscanf(fpr, "%d", &nTexNameLength);

			// テクスチャ使ってるなら
			if (nTexNameLength > 0)
			{
				// テクスチャ名取得
				char temp[MAX_PATH] = { 0 };
				fscanf(fpr, "%s", texName);

				// パス作成
				strcat(temp, TEX_FOLDER_PATH);	// TEXTUREフォルダへ
				strcat(temp, texName);				// ファイル名
				strcpy(texFilePath, temp);
			}

			else
			{
				// テクスチャ名取得
				char temp[MAX_PATH] = { 0 };
				fscanf(fpr, "%s", texName);
				strcpy(texFilePath, "NULL");
			}

			// テクスチャタイプ取得
			TEXTURE_TYPE texType = CTexture::GetTextureTye(texFilePath);

			// 保存ファイル名取得
			char saveName[MAX_PATH] = { 0 };
			fscanf(fpr, "%s", &saveName[0]);

			// 作成
			CField* p = CreateField(pDevice, pos, width, height, texType);
			
			// 回転反映
			p->SetRot(rot);

			// 終わりかどうか調べる
			ret = fscanf(fpr, "%d", &ans);
			if (ret == EOF)
			{
				break;
			}
			int size = sizeof(int);
			size *= -1;
			fseek(fpr, size, SEEK_CUR);
		}
	}
	fclose(fpr);

	// ナビファイル読み込み
	m_pNaviTile = CNaviTile::Create(FIELD_INFO_FILE_PATH[fieldType].naviFile);
}

//=========================================================================
// 終了
//=========================================================================
void CFieldManager::Uninit(void)
{
	if (m_pNaviTile)
	{
		m_pNaviTile->Uninit();
		delete m_pNaviTile;
		m_pNaviTile = NULL;
	}
}

//=========================================================================
// 更新
//=========================================================================
void CFieldManager::Update(void)
{
	
}

//=========================================================================
// 初期化
//=========================================================================
void CFieldManager::Init(void)
{
	
}


//----EOF----