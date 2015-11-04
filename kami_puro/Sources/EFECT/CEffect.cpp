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
CEffect::CEffect( int maxFrame, char *filename, bool isloop_ )
{
	m_pEffect  = NULL;
	m_handle   = -1;
	FrameCount = 0;
	m_Pos =  D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_Rot =  D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_vScl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
	isPlay    = false;
	isPause   = false;
	isLoop    = isloop_;
	pFileName = filename;
	MaxFrame  = maxFrame;
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
CEffect* CEffect::Create( int maxFrame, char *filename, bool isloop_ )
{
	CEffect* p = new CEffect( maxFrame, filename, isloop_ );
	p->Init( );
	return ( p );
}
//=============================================================================
//初期化処理
//=============================================================================
HRESULT CEffect::Init( )
{
	// エフェクトの読込
	m_pEffect = Effekseer::Effect::Create( CEffectManager::GetEffectManager( ), ( const EFK_CHAR* )pFileName );
	CScene::AddLinkList( CRenderer::TYPE_RENDER_NORMAL );

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CEffect::Uninit( )
{
	// エフェクトの破棄
	ES_SAFE_RELEASE( m_pEffect );
	this->Release( );
}
//=============================================================================
// 更新処理
//=============================================================================
void CEffect::Update( )
{
	if ( isPlay&&!isPause )
	{
		//m_Pos.y += 0.3f;

		//エフェクトの座標を変更
		CEffectManager::GetEffectManager( )->SetLocation( m_handle, m_Pos.x,m_Pos.y,m_Pos.z );
		//エフェクトの向きを変更
		CEffectManager::GetEffectManager( )->SetRotation( m_handle, m_Rot.x, m_Rot.y, m_Rot.z );
		//エフェクトの大きさを変更
		CEffectManager::GetEffectManager( )->SetScale( m_handle, m_vScl.x, m_vScl.y, m_vScl.z );

		// エフェクトの更新処理を行う
		CEffectManager::GetEffectManager( )->BeginUpdate( );
		CEffectManager::GetEffectManager( )->UpdateHandle( m_handle, 1.f );
		CEffectManager::GetEffectManager( )->EndUpdate( );
		//フレームのカウントアップ
		FrameCount++;

		//ループ再生モード
		if( FrameCount == MaxFrame )
		{
			if( isLoop )
			{
				// エフェクトの停止
				CEffectManager::GetEffectManager( )->StopEffect( m_handle );
				// エフェクトの再生
				m_handle = CEffectManager::GetEffectManager( )->Play( m_pEffect, 0, 0, 0 );
				//エフェクトの座標を変更
				CEffectManager::GetEffectManager( )->SetLocation( m_handle, m_Pos.x, m_Pos.y, m_Pos.z );
				//エフェクトの向きを変更
				CEffectManager::GetEffectManager( )->SetRotation( m_handle, m_Rot.x, m_Rot.y, m_Rot.z );
				//エフェクトの大きさを変更
				CEffectManager::GetEffectManager( )->SetScale( m_handle, m_vScl.x, m_vScl.y, m_vScl.z );
				FrameCount = 0;
			}

			else
			{
				// エフェクトの停止
				CEffectManager::GetEffectManager( )->StopEffect( m_handle );
				FrameCount = 0;
				isPlay = false;
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
	m_Pos  = pos;
	m_Rot  = rot;
	m_vScl = scl;

	// エフェクトの再生
	m_handle = CEffectManager::GetEffectManager( )->Play( m_pEffect, 0, 0, 0 );
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
