//=============================================================================
//
// エフェクトクラス [efect.cpp]
// Author : 坂本友希
//
//=============================================================================
#include "CEffectManager.h"
#include "CEffect.h"
//=============================================================================
// 実体定義
//=============================================================================

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// 構造体定義
//=============================================================================

//=============================================================================
// プロトタイプ宣言
//=============================================================================

//=============================================================================
// グローバル変数の宣言
//=============================================================================

//=============================================================================
//CPoseのコンストラクタ
//=============================================================================
CEffect::CEffect(int maxFrame, EFFECT_TYPE filename, bool isloop_)
{
	m_handle   = -1;
	FrameCount = 0;
	m_Pos =  D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_Rot =  D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_Move_Spd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot_Spd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vScl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
	isPlay    = false;
	isPause   = false;
	NextEffectType = EFFECT_MAX;
	isLoop    = isloop_;
	isDestruction = !isloop_;
	EffectType = filename;
	MaxFrame  = maxFrame;
	m_PlaySpeed = 1.0f;

}
//=============================================================================
//CPoseのデストラクタ
//=============================================================================
CEffect::~CEffect( )
{

}
//=============================================================================
//クリエイト
//=============================================================================
CEffect* CEffect::Create(int maxFrame, EFFECT_TYPE filename, bool isloop_)
{
	CEffect* p = new CEffect( maxFrame, filename, isloop_ );
	p->Init( );
	return ( p );
}
//=============================================================================
//クリエイト(即時再生)
//=============================================================================
CEffect* CEffect::Create(int maxFrame, EFFECT_TYPE filename, bool isloop_, D3DXVECTOR3& pos, D3DXVECTOR3& rot, D3DXVECTOR3& scl, D3DXVECTOR3& moveSpd, D3DXVECTOR3& rotSpd)
{
	CEffect* p = new CEffect(maxFrame, filename, isloop_);
	p->Init();
	p->SetPosSpd(moveSpd);
	p->SetRotSpd(rotSpd);
	p->Play(pos, rot, scl);
	return (p);
}
//=============================================================================
//初期化処理
//=============================================================================
HRESULT CEffect::Init( )
{
	CScene::AddLinkList( CRenderer::TYPE_RENDER_NORMAL );

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CEffect::Uninit( )
{
	if (isPlay)
	{
		CEffectManager::GetEffectManager()->StopEffect(m_handle);
		FrameCount = 0;
		isPlay = false;
	}

	this->Release( );
}
//=============================================================================
// 更新処理
//=============================================================================
void CEffect::Update( )
{
	if ( isPlay&&!isPause )
	{
		//スピードを考慮した移動
		m_Pos += m_Move_Spd;
		m_Rot += m_Rot_Spd;

		//エフェクトの座標を変更
		CEffectManager::GetEffectManager( )->SetLocation( m_handle, m_Pos.x,m_Pos.y,m_Pos.z );
		//エフェクトの向きを変更
		CEffectManager::GetEffectManager( )->SetRotation( m_handle, m_Rot.x, m_Rot.y, m_Rot.z );
		//エフェクトの大きさを変更
		CEffectManager::GetEffectManager( )->SetScale( m_handle, m_vScl.x, m_vScl.y, m_vScl.z );

		// エフェクトの更新処理を行う
		CEffectManager::GetEffectManager( )->BeginUpdate( );
		CEffectManager::GetEffectManager()->UpdateHandle(m_handle, m_PlaySpeed);
		CEffectManager::GetEffectManager( )->EndUpdate( );
		//フレームのカウントアップ
		FrameCount += m_PlaySpeed;

		//ループ再生モード
		if( FrameCount >= MaxFrame )
		{
			//破棄フラグがONの場合は無条件で破棄
			if (isDestruction)
			{
				CEffectManager::GetEffectManager()->StopEffect(m_handle);
				FrameCount = 0;
				isPlay = false;
				Uninit();
				return;
			}

			bool isSet=false;

			if (NextEffectType != EFFECT_MAX)
			{
				EffectType = NextEffectType;
				NextEffectType = EFFECT_MAX;
				isSet = true;
			}

			if (isLoop || isSet)
			{
				// エフェクトの停止
				CEffectManager::GetEffectManager( )->StopEffect( m_handle );
				// エフェクトの再生
				m_handle = CEffectManager::GetEffectManager()->Play(CEffectHolder::GetEffect(EffectType), 0, 0, 0);
				//エフェクトの座標を変更
				CEffectManager::GetEffectManager( )->SetLocation( m_handle, m_Pos.x, m_Pos.y, m_Pos.z );
				//エフェクトの向きを変更
				CEffectManager::GetEffectManager( )->SetRotation( m_handle, m_Rot.x, m_Rot.y, m_Rot.z );
				//エフェクトの大きさを変更
				CEffectManager::GetEffectManager( )->SetScale( m_handle, m_vScl.x, m_vScl.y, m_vScl.z );
				FrameCount = 0;
			}

			//ループ再生以外の場合は自信を破棄
			else
			{
				CEffectManager::GetEffectManager()->StopEffect(m_handle);
				FrameCount = 0;
				isPlay = false;
				Uninit();
			}
		}
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void CEffect::DrawNormalRender( )
{
	if( isPlay )
	{
		// エフェクトの描画開始処理を行う。
		CEffectManager::GetEffectRender( )->BeginRendering( );
		// エフェクトの描画を行う。
		CEffectManager::GetEffectManager( )->DrawHandle( m_handle );
		// エフェクトの描画終了処理を行う。
		CEffectManager::GetEffectRender( )->EndRendering( );

		// レンダーステートパラメータが変更されている可能性があるので戻すといいかも
	}
}
//=============================================================================
// 再生
//=============================================================================
void CEffect::Play( D3DXVECTOR3& pos, D3DXVECTOR3& rot, D3DXVECTOR3& scl )
{
	if (isPlay)
	{
		CEffectManager::GetEffectManager()->StopEffect(m_handle);
		FrameCount = 0;
		isPlay = false;
	}


	m_Pos  = pos;
	m_Rot  = rot;
	m_vScl = scl;

	// エフェクトの再生
	m_handle = CEffectManager::GetEffectManager()->Play(CEffectHolder::GetEffect(EffectType), 0, 0, 0);
	//エフェクトの座標を変更
	CEffectManager::GetEffectManager( )->SetLocation( m_handle, m_Pos.x, m_Pos.y, m_Pos.z );
	//エフェクトの向きを変更
	CEffectManager::GetEffectManager( )->SetRotation( m_handle, m_Rot.x, m_Rot.y, m_Rot.z );
	//エフェクトの大きさを変更
	CEffectManager::GetEffectManager( )->SetScale( m_handle, m_vScl.x, m_vScl.y, m_vScl.z );
	//フレームカウントを0にする
	FrameCount = 0;
	//再生フラグオン
	isPlay = true;
	//ポーズフラグオフ
	isPause = false;
}
//=============================================================================
// 停止
//=============================================================================
void CEffect::Stop( )
{
	FrameCount = 0;
	isPlay = false;
	isPause = false;
	CEffectManager::GetEffectManager( )->StopEffect( m_handle );
}
