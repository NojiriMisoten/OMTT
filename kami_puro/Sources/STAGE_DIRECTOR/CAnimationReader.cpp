//=============================================================================
//
// CAnimationReaderクラス [CAnimationReader.cpp]
// Author : 池島　大樹
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CAnimationReader.h"
#include <string>
using namespace std;

//=================================================
// コンストラクタ
//=================================================
CAnimationReader::CAnimationReader( void )
{

}

//=================================================
// デストラクタ
//=================================================
CAnimationReader::~CAnimationReader( void )
{

}

//=================================================
// 初期化
//=================================================
LRESULT CAnimationReader::Init( char* pFileName )
{
	// ファイル読み込み
	m_pFile = fopen( pFileName, "r" );
	// エラー処理
	if( m_pFile == NULL )
	{
		return E_FAIL;
	}
	return S_OK;
}

//=================================================
// 終了
//=================================================
void CAnimationReader::Uninit( void )
{
	fclose( m_pFile );
}

//=================================================
// 更新
//=================================================
void CAnimationReader::Read( void )
{
	char c;
	string str;
	// 一文字づつ読んでいき、EOFに当たったら終了
	while( ( c = fgetc( m_pFile ) ) != EOF )
	{
		// タグ記号
		if( c == '[' )
		{
			// タグ終了記号まで読み込む
			while( ( c = fgetc( m_pFile ) ) != ']' )
			{
				// 自分に足していき、タグ終了までの文字列を格納
				str += str;
			}

			// なんのタグ？
			{
				if( str == "ANIMATION" )
				{
					ReadAnimation();
				}

				if( str == "CAMERA" )
				{
					ReadCamera();
				}
	
				if( str.compare( 0, 7, "CONTROL" ) == 0 )		// 0文字目から7文字目が"CONTROL"と一致すれば
				{
					int id;
					if( str.length() == ( 7 + 2 ) )
					{
						// IDが二桁
						id = (int)str[6] * 10 + (int)str[7];
					}
					else
					{
						// IDが一桁
						id = (int)str[6];
					}
					ReadControl( id );
				}
			
				if( str.compare( 0, 6, "EFFECT" ) == 0 )		// 0文字目から6文字目が"EFFECT"と一致すれば
				{
					int id;
					if( str.length() == ( 6 + 2 ) )
					{
						// IDが二桁
						id = (int)str[5] * 10 + (int)str[6];
					}
					else
					{
						// IDが一桁
						id = (int)str[5];
					}
					ReadEffect( id );
				}
			}
		}
	}
}


void CAnimationReader::ReadAnimation( void )
{

}

void CAnimationReader::ReadCamera( void )
{

}

void CAnimationReader::ReadControl( int id )
{

}

void CAnimationReader::ReadEffect( int id )
{

}

//----EOF----