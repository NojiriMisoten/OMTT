//=============================================================================
//
// CLightManagerクラス [CLightManager.h]
// Author : 塚本俊彦
//
// ライトの数をcppで決めて、その数分ライトをnewしようかなと
//
// 
//=============================================================================
#ifndef _CLIGHTMANAGER_H_
#define _CLIGHTMANAGER_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../MAIN/main.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CManager;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CLightManager
{
public:
	// ライトの要素まとめた
	class CLight
	{
	public:
		CLight()
		{
			m_Pos = D3DXVECTOR3(0, 0, 0);
			m_Color = D3DXCOLOR(1, 1, 1, 1);
		}
		CLight(D3DXVECTOR3 pos, D3DXCOLOR color)
		{
			m_Pos = pos;
			m_Color = color;
		}
		~CLight(){};

		// 座標のセット
		void SetPos(D3DXVECTOR3 pos)
		{
			m_Pos = pos;
		}
		// 座標の取得
		D3DXVECTOR3 GetPos()
		{
			return m_Pos;
		}
		// 色のセット
		void SetColor(D3DXCOLOR color)
		{
			m_Color = color;
		}
		// 色の取得
		D3DXCOLOR GetColor()
		{
			return m_Color;
		}

	private:
		// 各ライトの座標
		D3DXVECTOR3 m_Pos;
		// 各ライトの色
		D3DXCOLOR m_Color;
	};

	// コンストラクタ
	CLightManager(CManager* pManager);

	// デストラクタ
	~CLightManager(void);

	// 終了
	void Uninit(void);

	// ライトの更新
	void Update(void);

	// ライトの情報取得
	CLight* GetLight(int index){
		return m_pLight[index];
	}

	// 定数ライトの数
	static const int LIGHT_NUM = 2;
private:

	// マネージャー
	CManager* m_pManager;

	// ライトの数
	int m_LightNum;
	// ライトたち
	CLight *m_pLight[LIGHT_NUM];

	// ライトを回転するためのカウント
	float m_Count;
	// 円をライト数で割った角度
	float m_RadOne;
};

#endif
//----EOF----