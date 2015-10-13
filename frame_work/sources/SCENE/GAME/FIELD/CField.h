//=============================================================================
//
// CFieldクラス [CField.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CFIELD_H_
#define _CFIELD_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../CSCENE/CScene2D.h"
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CField : public CScene2D
{
public:
	//=======================================================================
	// コンストラクタ
	// 引数:デバイス、プライオリティ、オブジェタイプ
	//=======================================================================
	CField(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_FIELD, OBJTYPE m_objType = OBJTYPE_FIELD);

	// デストラクタ
	~CField();

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	// 描画
	void Draw(void);

	// デス
	void Death(void){ Release(); };

	// ポジションアクセサ
	D3DXVECTOR3 GetPos(void){ return m_vPos; };
	void		SetPos(D3DXVECTOR3 pos){ m_vPos = pos; };
	void		SetPos(float x, float y, float z){ m_vPos.x = x; m_vPos.y = y; m_vPos.z = z; };

	// ロットアクセサ
	D3DXVECTOR3 GetRot(void){ return m_vRot; };
	void		SetRot(D3DXVECTOR3 rot){ m_vRot = rot; };
	void		SetRot(float x, float y, float z){ m_vRot.x = x; m_vRot.y = y; m_vRot.z = z; };

	// サイズアクセサ
	float GetWidth(void){ return m_fWidth; };
	void SetWidth(float width){ m_fWidth = width; };
	float GetHeight(void){ return m_fHeight; };
	void SetHeight(float height){ m_fHeight = height; };

	// 乗ってる
	void Ride(bool ride);

	//=======================================================================
	// クリエイト関数
	// 引数:デバイス、座標、幅、高さ、テクスチャの種類
	// 戻り値:作成したアドレス
	//=======================================================================
	static CField* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType);

private:
	//=======================================================================
	// 初期化関数
	// 引数:座標、幅、高さ、テクスチャの種類、（以下必要なし）（横分割数、縦分割数）
	// 戻り値:作成したアドレス
	//=======================================================================
	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock = 1, int hblock = 1);

	// 当たり判定ステータスセット関数
	void SetJudgeParam(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType);

	D3DXVECTOR3 m_vDefaultPos;
	D3DXVECTOR3 m_vDestPos;
protected:

};
#endif
//----EOF----