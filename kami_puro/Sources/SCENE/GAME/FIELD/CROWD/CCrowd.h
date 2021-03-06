//=============================================================================
//
// CCrowdクラス [CCrowd.h]
// Author : 塚本俊彦
//
//=============================================================================
#ifndef _CCROWD_H_
#define _CCROWD_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../../BASE_OBJECT/CScene3D.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CManager;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CCrowd : public CScene3D
{
public:
	//======================================================
	// コンストラクタ
	// 引数: デバイス、プライオリティ、オブジェタイプ
	//======================================================
	CCrowd(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE m_objType = OBJTYPE_3D);

	// デストラクタ
	~CCrowd();

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	// 描画
	void DrawNormalRender(void);

	//=================================================
	// クリエイト関数
	// 引数: デバイス、座標、幅、高さ、ブロック数、テクスチャタイプ
	// 戻り値: 作ったやつのアドレス
	//=================================================
	static CCrowd* Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos, float width, float height,
		TEXTURE_TYPE texType,
		CManager* pManager);

	void SetTextureOffsetV(float v)
	{
		m_TextureOffsetV = v;
	}
private:
	//======================================================
	// 初期化
	// 引数: 座標、幅、高さ、テクスチャータイプ、(横分割数、縦分割数)
	// 戻り値: 成否
	//======================================================
	void Init(D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType, CManager* pManager);

	// 保存しときたいポインタ
	LPDIRECT3DDEVICE9		*m_pD3DDevice;			// デバイスオブジェクト(描画に必要)
	CManager				*m_pManager;

	// テクスチャのオフセット値（UVのVだけ）
	float m_TextureOffsetV;

};

#endif
//----EOF----