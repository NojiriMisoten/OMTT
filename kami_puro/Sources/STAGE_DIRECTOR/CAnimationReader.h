//=============================================================================
//
// CAnimationReaderクラス [CAnimationReader.cpp]
// Author : 池島　大樹
//
//=============================================================================
#ifndef _CANIMATIONREADER_H_
#define _CANIMATIONREADER_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../MAIN/main.h"
#include <stdio.h>

//*****************************************************************************
// 前方宣言
//*****************************************************************************


//*****************************************************************************
// クラス定義
//*****************************************************************************
class CAnimationReader
{
public:
	typedef struct
	{
		int				id;					// アニメーションID
		int				dur;				// 総フレーム数

	}ANIMATION_DATA;

	typedef struct
	{
		int				duration;			// 総フレーム数
		D3DXVECTOR3		endP;				// 終了視点
		D3DXVECTOR3		endR;				// 終了注視点
		bool			isShake;			// シェイクをする
		float			amplitude;			// シェイク振幅
		int				delay;				// シェイク開始遅延フレーム数
		int				time;				// シェイク継続フレーム
	}CAMERA_CP_DATA;

	typedef struct
	{
		char			*file;				// ファイル名
		D3DXVECTOR3		pos;				// 座標
		D3DXVECTOR3		ang;				// 角度
		D3DXVECTOR3		scl;				// 縮尺
		int				startFrame;			// 開始フレーム
		int				endFrame;			// 終了フレーム
	}EFFECT_DATA;
	
	// コンストラクタ
	CAnimationReader( void );

	// デストラクタ
	~CAnimationReader( void );

	// 初期化
	LRESULT Init( char *pFile );

	// 終了
	void Uninit( void );

	// 読み取り
	void Read( void );

private:
	FILE				*m_pFile;				// 読み込むファイル
	ANIMATION_DATA		m_Animation;			// アニメーション
	CAMERA_CP_DATA		m_ControlPoint[10];		// カメラコントロールポイント
	EFFECT_DATA			m_Effect[10];			// エフェクト

	void ReadAnimation( void );
	void ReadCamera( void );
	void ReadControl( int id );
	void ReadEffect( int id );

};

#endif

//----EOF----