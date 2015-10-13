//=============================================================================
//
// CTreasureクラス [CTreasure.cpp]
// Author : 塚本　俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CTreasure.h"
#include "../PLAYER/AI/mersenne_twister.h"
#include "../../../MANAGER/CManager.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
// テクスチャ関連
static const TEXTURE_TYPE TREASURE_TEXTURE = TEXTURE_TREASURE;
static const int TREASURE_TEXTURE_X = 13;
static const int TREASURE_TEXTURE_Y = 1;
// お宝２Ｄ
static const float TREASURE_WIDTH = 70;
static const float TREASURE_HEIGHT = 70;
static const int TREASURE_ANIME_SPEED = 5;
// お宝アイコン２Ｄ
static const float TREASURE_ICON_WIDTH = 40;
static const float TREASURE_ICON_HEIGHT = 40;
static const int TREASURE_ICON_ANIME_SPEED = 3;

// お宝の所持番号（誰も持っていないときの-1）
static const short OWNER_NONE = -1;

// きらきら絵
static const D3DXVECTOR3 TREASURE_KIRA_SIZE = D3DXVECTOR3(80, 80, 0);
static const int TREASURE_KIRA_TEX_X = 8;
static const int TREASURE_KIRA_TEX_Y = 1;
static const int TREASURE_KIRA_ANIME_SPEED = 2;
// 神々しいエフェクト
static const D3DXVECTOR3 TREASURE_LIGHT_SIZE = D3DXVECTOR3(100, 100, 0);
static const int TREASURE_LIGHT_TEX_X = 10;
static const int TREASURE_LIGHT_TEX_Y = 1;
static const int TREASURE_LIGHT_ANIME_SPEED = 4;

// 宝物がジャンプする時間（プレイヤが落としてから次にキャッチできるまでの時間）
static const int TREASURE_JUMP_INTERVAL = 60;

// 宝物を落とす距離
static const float TREASURE_FALL_DIST = 10;

// 宝物を落とす位置のテーブル
static const int TREASURE_FALL_TABLE_NUM = 8;
static const D3DXVECTOR3 TREASURE_FALL_VELOCITY[TREASURE_FALL_TABLE_NUM] = {
	D3DXVECTOR3(TREASURE_FALL_DIST * +0.0f, -8, 0),
	D3DXVECTOR3(TREASURE_FALL_DIST * +0.0f, -4, 0),
	D3DXVECTOR3(TREASURE_FALL_DIST * +1.0f, -6, 0),
	D3DXVECTOR3(TREASURE_FALL_DIST * -1.0f, -6, 0),
	D3DXVECTOR3(TREASURE_FALL_DIST * +0.5f, -7, 0),
	D3DXVECTOR3(TREASURE_FALL_DIST * +0.5f, -7, 0),
	D3DXVECTOR3(TREASURE_FALL_DIST * -0.5f, -5, 0),
	D3DXVECTOR3(TREASURE_FALL_DIST * -0.5f, -5, 0),
};

// 移動量の減衰量
static const float TREASURE_MOVE_RESIST = 0.9f;
// 重力
static const float TREASURE_GRAVITY = 0.2f;

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CTreasure::CTreasure(LPDIRECT3DDEVICE9 *pDevice, int nPriority, OBJTYPE m_objType) :CSceneAnime(pDevice, nPriority, m_objType)
{
	// お宝の状態初期化
	m_TreasureState = TREASURE_STATE_OWNER_NONE;

}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CTreasure ::~CTreasure(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CTreasure::Init(D3DXVECTOR3 pos)
{
	// 初期化
	CSceneAnime::Init(pos, TREASURE_WIDTH, TREASURE_HEIGHT, TREASURE_TEXTURE,
		TREASURE_TEXTURE_X, TREASURE_TEXTURE_Y, TREASURE_ANIME_SPEED, -1);

	SetAutoUpdate(true);
	m_nJumpCount = 0;
	m_vVelocity = D3DXVECTOR3(0, 0, 0);
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CTreasure::Uninit(void)
{
	CScene2D::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CTreasure::Update(void)
{
	CSceneAnime::Update();

	// jumpアニメーション更新
	if (m_TreasureState == TREASURE_STATE_JUMPING){
		
		// jump減衰
		m_vVelocity.y += TREASURE_GRAVITY;
		m_vVelocity.x *= TREASURE_MOVE_RESIST;
			
		// カウント
		m_nJumpCount++;
		if (m_nJumpCount > TREASURE_JUMP_INTERVAL){
			// 宝状態リセット
			m_TreasureState = TREASURE_STATE_OWNER_NONE;
			m_vVelocity = D3DXVECTOR3(0, 0, 0);
			m_nJumpCount = 0;
		}

		// 加算
		m_vPos += m_vVelocity;

		// エフェクトに座標を適用
		m_pKira->SetPos(m_vPos);
		m_pLight->SetPos(m_vPos);
	}
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CTreasure* CTreasure::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos)
{
	// 作成
	CTreasure* p = new CTreasure(pDevice);
	p->Init(pos);

	// エフェクトの作成
	p->m_pKira = CSceneAnime::Create(
		pDevice,
		pos, TREASURE_KIRA_SIZE.x, TREASURE_KIRA_SIZE.y,
		TEXTURE_TREASURE_KIRAKIRA, TREASURE_KIRA_TEX_X, TREASURE_KIRA_TEX_Y, TREASURE_KIRA_ANIME_SPEED, -1);
	p->m_pLight = CSceneAnime::Create(
		pDevice,
		pos, TREASURE_LIGHT_SIZE.x, TREASURE_LIGHT_SIZE.y,
		TEXTURE_TREASURE_LIGHT, TREASURE_LIGHT_TEX_X, TREASURE_LIGHT_TEX_Y, TREASURE_LIGHT_ANIME_SPEED, -1);

	p->m_pLight->SetDrawFlag(false);
	return p;
}

//*****************************************************************************
// 大きさやテクスチャを最初の状態に戻す
// 宝物をプレイヤが落とした際に呼ばれる
//*****************************************************************************
void CTreasure::Reset(D3DXVECTOR3 pos){

	// 絵の変更
//	CScene2D::ChangeTexture(TREASURE_TEXTURE);
	CScene2D::SetWidth(TREASURE_WIDTH);
	CScene2D::SetHeight(TREASURE_HEIGHT);
	SetAnimeSpeed(TREASURE_ANIME_SPEED);

	// 落とす処理
	SetFall();
	CManager::PlaySoundA(SOUND_LABEL_SE_DEST_TREASURE);
}

//*****************************************************************************
// 大きさやテクスチャを最初の状態に戻す
// 宝物をプレイヤが落とした際に呼ばれる
//*****************************************************************************
void CTreasure::SetIcon(){

	// 絵の変更
//	CScene2D::ChangeTexture(TEXTURE_TREASURE_ICON);
	CScene2D::SetWidth(TREASURE_ICON_WIDTH);
	CScene2D::SetHeight(TREASURE_ICON_HEIGHT);
	SetAnimeSpeed(TREASURE_ICON_ANIME_SPEED);

	// エフェクトの変更
	m_pLight->SetHeight(150);
}


//*****************************************************************************
// 宝物の場所をセット エフェクトとかも一緒にセットする
//*****************************************************************************
void CTreasure::SetPos(D3DXVECTOR3 pos){

	// 座標セット
	m_vPos = pos;
	m_pKira->SetPos(pos);
	m_pLight->SetPos(pos + D3DXVECTOR3(0, 40, 0));
}

//*****************************************************************************
// 宝物を落とす状態にする
//*****************************************************************************
void CTreasure::SetFall(){
	int index = mersenne_twister_int(0, TREASURE_FALL_TABLE_NUM - 1);
	m_vVelocity = TREASURE_FALL_VELOCITY[index];
	m_TreasureState = TREASURE_STATE_JUMPING;
}
//----EOF-------