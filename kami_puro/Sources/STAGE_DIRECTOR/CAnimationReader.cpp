//=============================================================================
//
// CAnimationReader�N���X [CAnimationReader.cpp]
// Author : �r���@���
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CAnimationReader.h"
#include <string>
using namespace std;

//=================================================
// �R���X�g���N�^
//=================================================
CAnimationReader::CAnimationReader( void )
{

}

//=================================================
// �f�X�g���N�^
//=================================================
CAnimationReader::~CAnimationReader( void )
{

}

//=================================================
// ������
//=================================================
LRESULT CAnimationReader::Init( char* pFileName )
{
	// �t�@�C���ǂݍ���
	m_pFile = fopen( pFileName, "r" );
	// �G���[����
	if( m_pFile == NULL )
	{
		return E_FAIL;
	}
	return S_OK;
}

//=================================================
// �I��
//=================================================
void CAnimationReader::Uninit( void )
{
	fclose( m_pFile );
}

//=================================================
// �X�V
//=================================================
void CAnimationReader::Read( void )
{
	char c;
	string str;
	// �ꕶ���Âǂ�ł����AEOF�ɓ���������I��
	while( ( c = fgetc( m_pFile ) ) != EOF )
	{
		// �^�O�L��
		if( c == '[' )
		{
			// �^�O�I���L���܂œǂݍ���
			while( ( c = fgetc( m_pFile ) ) != ']' )
			{
				// �����ɑ����Ă����A�^�O�I���܂ł̕�������i�[
				str += str;
			}

			// �Ȃ�̃^�O�H
			{
				if( str == "ANIMATION" )
				{
					ReadAnimation();
				}

				if( str == "CAMERA" )
				{
					ReadCamera();
				}
	
				if( str.compare( 0, 7, "CONTROL" ) == 0 )		// 0�����ڂ���7�����ڂ�"CONTROL"�ƈ�v�����
				{
					int id;
					if( str.length() == ( 7 + 2 ) )
					{
						// ID����
						id = (int)str[6] * 10 + (int)str[7];
					}
					else
					{
						// ID���ꌅ
						id = (int)str[6];
					}
					ReadControl( id );
				}
			
				if( str.compare( 0, 6, "EFFECT" ) == 0 )		// 0�����ڂ���6�����ڂ�"EFFECT"�ƈ�v�����
				{
					int id;
					if( str.length() == ( 6 + 2 ) )
					{
						// ID����
						id = (int)str[5] * 10 + (int)str[6];
					}
					else
					{
						// ID���ꌅ
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