//=============================================================================
//
// エフェクトクラス [efect.h]
// Author : 坂本友希
//
//=============================================================================
#ifndef _CEFECT_H_
#define _CEFECT_H_
//=============================================================================
// インクルード
//=============================================================================
#include "../MAIN/main.h"
#include "CEffectManager.h"
#include "../BASE_OBJECT/CScene.h"

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// 前方宣言
//============================================================================= 

//=============================================================================
// クラス宣言
//=============================================================================
class CEffect : public CScene
{
	public:
		//コンストラクタ
		CEffect( int maxFrame, char *filename, bool isloop_ );
		//デストラクタ
		~CEffect( );

		//クリエイト
		static CEffect* Create( int maxFrame, char *filename, bool isloop_ );
		
		// ポジションアクセサ
		D3DXVECTOR3& GetPos( void ){ return m_Pos; };
		D3DXVECTOR3* GetPosAdr( void ){ return &m_Pos; };
		void AddPos( D3DXVECTOR3& pos ){ m_Pos += pos; }
		void		SetPos( D3DXVECTOR3& pos ){ m_Pos = pos; };
		void		SetPos( float x, float y, float z ){ m_Pos.x = x; m_Pos.y = y; m_Pos.z = z; };

		// ロットアクセサ
		D3DXVECTOR3& GetRot( void ){ return m_Rot; };
		void		SetRot( D3DXVECTOR3& rot ){ m_Rot = rot; };
		void		SetRot( float x, float y, float z ){ m_Rot.x = x; m_Rot.y = y; m_Rot.z = z; };

		// スケールアクセサ
		D3DXVECTOR3& GetScl( void ){ return m_vScl; };
		void		SetScl( D3DXVECTOR3& scl ){ m_vScl = scl; };
		void		SetScl( float x, float y, float z ){ m_vScl.x = x; m_vScl.y = y; m_vScl.z = z; };

		// 再生スピードアクセサ
		void SetPlaySpeed(float value){ m_PlaySpeed = value; }
		float GetPlaySpeed(void){ return (m_PlaySpeed); }
		void AddPlaySpeed(float value){ m_PlaySpeed += value; }

		//初期化
		HRESULT Init( );
		//終了
		void Uninit( );
		//更新
		void Update( );
		//描画
		void DrawNormalRender( );
		//再生
		void Play( D3DXVECTOR3& pos, D3DXVECTOR3& rot, D3DXVECTOR3& scl );

		//一時停止
		void Pause( ){ isPause = true; }
		//一時停止解除
		void UnPause( ){ isPause = false; }
		//停止
		void Stop( );
		//再生中かどうか取得
		bool GetIsPlay( ){ return ( isPlay ); }


	private:
		// 大きさ（スケール）
		D3DXVECTOR3			m_vScl;
		//ファイル名
		char *pFileName;
		//カウント用
		float FrameCount;
		//フレーム数
		int MaxFrame;
		//エフェクトのスピード
		float m_PlaySpeed;
		//エフェクト本体
		::Effekseer::Effect* m_pEffect;
		//エフェクトのハンドル
		::Effekseer::Handle m_handle;
		//ループするかどうか
		bool isLoop;
		//再生中かどうか
		bool isPlay;
		//ポーズ中かどうか
		bool isPause;

};
#endif