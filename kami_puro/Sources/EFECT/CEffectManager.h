//=============================================================================
//
// �G�t�F�N�g�Ǘ��N���X [CEffectManager.h]
// Author : ��{�F��
//
//=============================================================================
#ifndef _CEFFECTMANAGER_H_
#define _CEFFECTMANAGER_H_
//=============================================================================
// �C���N���[�h
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
// �}�N����`
//=============================================================================

//=============================================================================
// �O���錾
//============================================================================= 

//=============================================================================
// �N���X�錾
//=============================================================================
class CEffectManager
{
	public:
		//�R���X�g���N�^
		CEffectManager( LPDIRECT3DDEVICE9 *m_pDevice );
		//�f�X�g���N�^
		~CEffectManager( );

		//������
		void Init( );
		//�I��
		void Uninit( );
		//�X�V
		void Update( );
		//�`��
		static void Draw( );

		//�}�l�[�W���[�̎擾����
		static ::Effekseer::Manager* GetEffectManager( ){ return( m_pManager ); }
		//�����_���[�̎擾����
		static ::EffekseerRenderer::Renderer* GetEffectRender(){ return( m_pRenderer ); }
		//�f�o�C�X�擾
		static LPDIRECT3DDEVICE9 GetDevice(){ return ( *m_pDevice ); }

	private:
		//�}�l�[�W���[�{��
		static ::Effekseer::Manager          *m_pManager;
		//�����_���[�{��
		static ::EffekseerRenderer::Renderer *m_pRenderer;
		//�f�o�C�X
		static LPDIRECT3DDEVICE9             *m_pDevice;
};
#endif