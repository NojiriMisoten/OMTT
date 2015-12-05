//=============================================================================
//
// エフェクトクラス [efect.h]
// Author : 坂本友希
//
// Create関数呼び出し時引数の最後にpos,rot,sclをセットすると即時再生されます
// ループ再生以外の場合は初期のままだと再生が終了した時に勝手に自信を破棄してしまいます
// Destroy関数で再生終了時に破棄をするようになります
// DontDestroy関数で再生終了時に破棄をしないようになります
// 再生終了まで破棄するのを待てない人はUninit関数を呼べば即時で破棄されます
//
//
//=============================================================================
#ifndef _CEFECT_H_
#define _CEFECT_H_
//=============================================================================
// インクルード
//=============================================================================
#include "../MAIN/main.h"
#include "CEffectManager.h"
#include "CEffectHolder.h"
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
		CEffect(int maxFrame, EFFECT_TYPE filename, bool isloop_);
		//デストラクタ
		~CEffect( );

		//クリエイト
		static CEffect* Create(int maxFrame, EFFECT_TYPE filename, bool isloop_);
		static CEffect* Create(int maxFrame, EFFECT_TYPE filename, bool isloop_, D3DXVECTOR3& pos, D3DXVECTOR3& rot, D3DXVECTOR3& scl, D3DXVECTOR3& moveSpd = D3DXVECTOR3(0, 0, 0), D3DXVECTOR3& rotSpd = D3DXVECTOR3(0, 0, 0));
		
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

		// ポジション速度アクセサ
		D3DXVECTOR3& GetPosSpd(void){ return m_Move_Spd; };
		D3DXVECTOR3* GetPosSpdAdr(void){ return &m_Move_Spd; };
		void AddPosSpd(D3DXVECTOR3& posSpd){ m_Move_Spd += posSpd; }
		void		SetPosSpd(D3DXVECTOR3& posSpd){ m_Move_Spd = posSpd; };
		void		SetPosSpd(float x, float y, float z){ m_Move_Spd.x = x; m_Move_Spd.y = y; m_Move_Spd.z = z; };

		// ロット速度アクセサ
		D3DXVECTOR3& GetRotSpd(void){ return m_Rot_Spd; };
		void		SetRotSpd(D3DXVECTOR3& rotSpd){ m_Rot_Spd = rotSpd; };
		void		SetRotSpd(float x, float y, float z){ m_Rot_Spd.x = x; m_Rot_Spd.y = y; m_Rot_Spd.z = z; };

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
		void DrawNormalRender();
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

		//破棄フラグのセット
		void Destroy(void){ isDestruction = true; }
		void DontDestroy(void){ isDestruction = false; }

		//次再生エフェクトの予約
		void SetNextEffect(EFFECT_TYPE value){ NextEffectType = value; }


	private:
		// 大きさ（スケール）
		D3DXVECTOR3         m_vScl;
		//各スピード
		D3DXVECTOR3         m_Move_Spd;
		D3DXVECTOR3         m_Rot_Spd;
		//ファイル名
		EFFECT_TYPE         EffectType;
		EFFECT_TYPE         NextEffectType;
		//カウント用
		float               FrameCount;
		//フレーム数
		int                 MaxFrame;
		//エフェクトのハンドル
		::Effekseer::Handle m_handle;
		//ループするかどうか
		bool                isLoop;
		//再生中かどうか
		bool                isPlay;
		//ポーズ中かどうか
		bool                isPause;
		//エフェクトのスピード
		float               m_PlaySpeed;
		//自動で破棄するフラグ
		bool isDestruction;

};
#endif