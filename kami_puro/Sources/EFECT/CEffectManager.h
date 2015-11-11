//=============================================================================
//
// エフェクト管理クラス [CEffectManager.h]
// Author : 坂本友希
//
//=============================================================================
#ifndef _CEFFECTMANAGER_H_
#define _CEFFECTMANAGER_H_
//=============================================================================
// インクルード
//=============================================================================
#include "../MAIN/main.h"


#include "Effekseer/include/Effekseer.h"
#include "Effekseer/include/EffekseerRendererDX9.h"
#include "Effekseer/include/EffekseerSoundXAudio2.h"

#if _DEBUG
#pragma comment(lib, "../Sources/EFECT/Effekseer/lib/Effekseer.Debug.lib" )
#pragma comment(lib, "../Sources/EFECT/Effekseer/lib/EffekseerRendererDX9.Debug.lib" )
#pragma comment(lib, "../Sources/EFECT/Effekseer/lib/EffekseerSoundXAudio2.Debug.lib" )
#else
#pragma comment(lib, "Effekseer.Release.lib" )
#pragma comment(lib, "EffekseerRendererDX9.Release.lib" )
#pragma comment(lib, "EffekseerSoundXAudio2.Release.lib" )
#endif

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// 前方宣言
//============================================================================= 

//=============================================================================
// クラス宣言
//=============================================================================
class CEffectManager
{
	public:
		//コンストラクタ
		CEffectManager( LPDIRECT3DDEVICE9 *m_pDevice );
		//デストラクタ
		~CEffectManager( );

		//初期化
		void Init( );
		//終了
		void Uninit( );
		//更新
		void Update( );
		//描画
		static void Draw( );

		//マネージャーの取得処理
		static ::Effekseer::Manager* GetEffectManager( ){ return( m_pManager ); }
		//レンダラーの取得処理
		static ::EffekseerRenderer::Renderer* GetEffectRender(){ return( m_pRenderer ); }
		//デバイス取得
		static LPDIRECT3DDEVICE9 GetDevice(){ return ( *m_pDevice ); }

	private:
		//マネージャー本体
		static ::Effekseer::Manager          *m_pManager;
		//レンダラー本体
		static ::EffekseerRenderer::Renderer *m_pRenderer;
		//デバイス
		static LPDIRECT3DDEVICE9             *m_pDevice;
};
#endif