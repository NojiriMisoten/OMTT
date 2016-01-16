//=============================================================================
//
// CJumpEffectBillbordクラス [CJumpEffectBillbord.h]
// Author : 塚本俊彦
//
// 加算合成するよ
// 
//=============================================================================
#ifndef _CJUMPEFFECTBILLBORD_H_
#define _CJUMPEFFECTBILLBORD_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../RENDERER/CRenderer.h"
#include "../../../BASE_OBJECT/CScene2D.h"

class CCameraManager;
class CUiManager;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CJumpEffectBillbord : public CScene2D
{
public:

	CJumpEffectBillbord(LPDIRECT3DDEVICE9 *pDevice);
	~CJumpEffectBillbord(void);

	void Update();
	void Uninit();
	void DrawUIRender();

	// 高さ、Y座標、左端の座標、右端の座標、デバイス
	static CJumpEffectBillbord *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3& pos,
		TEXTURE_TYPE texType, CCameraManager* pCamera, CUiManager* pUiManager, int playerNum);


private:
	// 初期化
	void Init(D3DXVECTOR3& pos,
		TEXTURE_TYPE texType);

	CCameraManager* m_pCameraManager;
	CUiManager* m_pUiManager;

	// 透過のための色情報
	D3DXCOLOR m_Color;
	// 飛ぶ先を決めるためのプレイヤ番号
	int m_PlayerNum;
	// 飛ぶための一定カウント
	int m_FlyCount;
	// 飛んでるステートフラグ
	bool m_isFly;
	// 飛ぶときの座標補間用タイム0~1
	float m_FlyTime;
	// 3D座標でプレイヤの位置を取得
	D3DXVECTOR3 m_Pos3D;
	// 2D座標で描画
	D3DXVECTOR3 m_Pos2D;
	// 飛部時の保管用座標
	D3DXVECTOR3 m_Pos2DStart;
	D3DXVECTOR3 m_Pos2DEnd;
	// 初期のXの移動量　だんだん減衰させる
	float m_Velo;
	// 歓声ゲージなのHPゲージなのフラグ
	bool m_isCrowd;
	// だんだん小さくなるための大きさ
	float m_Size;
};

#endif
//----EOF----