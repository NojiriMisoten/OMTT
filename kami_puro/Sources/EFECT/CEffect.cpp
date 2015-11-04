//=============================================================================
//
// �G�t�F�N�g�N���X [efect.cpp]
// Author : ��{�F��
//
//=============================================================================
#include "CEffectManager.h"
#include "CEffect.h"
//=============================================================================
// ���̒�`
//=============================================================================

//=============================================================================
// �}�N����`
//=============================================================================

//=============================================================================
// �\���̒�`
//=============================================================================

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================

//=============================================================================
// �O���[�o���ϐ��̐錾
//=============================================================================

//=============================================================================
//CPose�̃R���X�g���N�^
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
//CPose�̃f�X�g���N�^
//=============================================================================
CEffect::~CEffect( )
{

}
//=============================================================================
//�N���G�C�g
//=============================================================================
CEffect* CEffect::Create( int maxFrame, char *filename, bool isloop_ )
{
	CEffect* p = new CEffect( maxFrame, filename, isloop_ );
	p->Init( );
	return ( p );
}
//=============================================================================
//����������
//=============================================================================
HRESULT CEffect::Init( )
{
	// �G�t�F�N�g�̓Ǎ�
	m_pEffect = Effekseer::Effect::Create( CEffectManager::GetEffectManager( ), ( const EFK_CHAR* )pFileName );
	CScene::AddLinkList( CRenderer::TYPE_RENDER_NORMAL );

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CEffect::Uninit( )
{
	// �G�t�F�N�g�̔j��
	ES_SAFE_RELEASE( m_pEffect );
	this->Release( );
}
//=============================================================================
// �X�V����
//=============================================================================
void CEffect::Update( )
{
	if ( isPlay&&!isPause )
	{
		//m_Pos.y += 0.3f;

		//�G�t�F�N�g�̍��W��ύX
		CEffectManager::GetEffectManager( )->SetLocation( m_handle, m_Pos.x,m_Pos.y,m_Pos.z );
		//�G�t�F�N�g�̌�����ύX
		CEffectManager::GetEffectManager( )->SetRotation( m_handle, m_Rot.x, m_Rot.y, m_Rot.z );
		//�G�t�F�N�g�̑傫����ύX
		CEffectManager::GetEffectManager( )->SetScale( m_handle, m_vScl.x, m_vScl.y, m_vScl.z );

		// �G�t�F�N�g�̍X�V�������s��
		CEffectManager::GetEffectManager( )->BeginUpdate( );
		CEffectManager::GetEffectManager( )->UpdateHandle( m_handle, 1.f );
		CEffectManager::GetEffectManager( )->EndUpdate( );
		//�t���[���̃J�E���g�A�b�v
		FrameCount++;

		//���[�v�Đ����[�h
		if( FrameCount == MaxFrame )
		{
			if( isLoop )
			{
				// �G�t�F�N�g�̒�~
				CEffectManager::GetEffectManager( )->StopEffect( m_handle );
				// �G�t�F�N�g�̍Đ�
				m_handle = CEffectManager::GetEffectManager( )->Play( m_pEffect, 0, 0, 0 );
				//�G�t�F�N�g�̍��W��ύX
				CEffectManager::GetEffectManager( )->SetLocation( m_handle, m_Pos.x, m_Pos.y, m_Pos.z );
				//�G�t�F�N�g�̌�����ύX
				CEffectManager::GetEffectManager( )->SetRotation( m_handle, m_Rot.x, m_Rot.y, m_Rot.z );
				//�G�t�F�N�g�̑傫����ύX
				CEffectManager::GetEffectManager( )->SetScale( m_handle, m_vScl.x, m_vScl.y, m_vScl.z );
				FrameCount = 0;
			}

			else
			{
				// �G�t�F�N�g�̒�~
				CEffectManager::GetEffectManager( )->StopEffect( m_handle );
				FrameCount = 0;
				isPlay = false;
			}
		}
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void CEffect::DrawNormalRender( )
{
	if( isPlay )
	{
		// �G�t�F�N�g�̕`��J�n�������s���B
		CEffectManager::GetEffectRender( )->BeginRendering( );
		// �G�t�F�N�g�̕`����s���B
		CEffectManager::GetEffectManager( )->DrawHandle( m_handle );
		// �G�t�F�N�g�̕`��I���������s���B
		CEffectManager::GetEffectRender( )->EndRendering( );

		// �����_�[�X�e�[�g�p�����[�^���ύX����Ă���\��������̂Ŗ߂��Ƃ�������
	}
}
//=============================================================================
// �Đ�
//=============================================================================
void CEffect::Play( D3DXVECTOR3& pos, D3DXVECTOR3& rot, D3DXVECTOR3& scl )
{
	m_Pos  = pos;
	m_Rot  = rot;
	m_vScl = scl;

	// �G�t�F�N�g�̍Đ�
	m_handle = CEffectManager::GetEffectManager( )->Play( m_pEffect, 0, 0, 0 );
	//�G�t�F�N�g�̍��W��ύX
	CEffectManager::GetEffectManager( )->SetLocation( m_handle, m_Pos.x, m_Pos.y, m_Pos.z );
	//�G�t�F�N�g�̌�����ύX
	CEffectManager::GetEffectManager( )->SetRotation( m_handle, m_Rot.x, m_Rot.y, m_Rot.z );
	//�G�t�F�N�g�̑傫����ύX
	CEffectManager::GetEffectManager( )->SetScale( m_handle, m_vScl.x, m_vScl.y, m_vScl.z );
	//�t���[���J�E���g��0�ɂ���
	FrameCount = 0;
	//�Đ��t���O�I��
	isPlay = true;
	//�|�[�Y�t���O�I�t
	isPause = false;
}
//=============================================================================
// ��~
//=============================================================================
void CEffect::Stop( )
{
	FrameCount = 0;
	isPlay = false;
	isPause = false;
	CEffectManager::GetEffectManager( )->StopEffect( m_handle );
}
