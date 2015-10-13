//=============================================================================
//
// CFieldManagerクラス [CFieldManager.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CFIELDMANAGER_H_
#define _CFIELDMANAGER_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../MAIN/main.h"
#include "../../../TEXTURE/CTexture.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CField;
class CNaviTile;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CFieldManager
{
public:
	typedef enum
	{
		FIELD_01 = 0,
		FIELD_02,
		FIELD_03,
		FIELD_04,
		FIELD_05,
		FIELD_06,
		FIELD_07,
		FIELD_08,
		FIELD_MAX
	}FIELD_TYPE;


	// コンストラクタ
	CFieldManager(void);

	// デストラクタ
	~CFieldManager(void);

	// フィールドクリエイト
	CField* CreateField(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType);

	// ロードフィールド
	void LoadField(LPDIRECT3DDEVICE9 *pDevice, FIELD_TYPE fieldType);

	// 初期化
	void Init(void);

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	static CNaviTile* GetNaviTileAdr(void){ return m_pNaviTile; };

private:
	static CNaviTile* m_pNaviTile;
};

#endif
//----EOF----