//=============================================================================
//
// CTitleクラス [CTitle.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CTITLE_H_
#define _CTITLE_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../MAIN/main.h"
#include "../../PHASE/CPhase.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;
class CTextureAnimator;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTitle : public CPhase
{
public:
	// コンストラクタ
	CTitle(void);

	// デストラクタ
	~CTitle(void);

	//=================================================================
	// 初期化関数
	// 引数: シーンのモード, デバイス
	//=================================================================
	void Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	//=================================================================
	// クリエイト関数
	// 引数: シーンのモード, デバイス
	// 戻り値: このシーンのアドレス
	//=================================================================
	static CTitle* Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);

private:

	//オブジェクトの更新処理
	void ObjectUpdate();
	//オブジェクト生成処理
	void ObjectSet(LPDIRECT3DDEVICE9* pDevice);
	//オブジェクトの移動処理
	void ObjectMove();
	//オブジェクトのUV更新処理
	void ObjectAnimation();
	//オブジェクトの色の更新処理
	void ObjectColorSet();

	//タイトル画面で使用する専用のポリゴンパラメーター
	typedef struct
	{
		CScene2D*	Scene2D;
		D3DXVECTOR3 V3Offset;
		UV_INDEX	UV;
		D3DXCOLOR	Color;
		float		fRotation;
	}POLYGON_PARAMETER;

	POLYGON_PARAMETER	m_BG;
	POLYGON_PARAMETER	m_ScrollBG;
	POLYGON_PARAMETER	m_Fire;
	POLYGON_PARAMETER	m_Logo;
	POLYGON_PARAMETER	m_AnyCongaLogo;

	CTextureAnimator *m_FireAnimatior;

	bool isObject;
};

#endif
//----EOF----