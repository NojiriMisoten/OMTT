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
//CPose�̃f�X�g���N�^
//=============================================================================
CEffect::~CEffect( )
{

}
//=============================================================================
//�N���G�C�g
//=============================================================================
CEffect* CEffect::Create(int maxFrame, EFFECT_TYPE filename, bool isloop_)
{
	CEffect* p = new CEffect( maxFrame, filename, isloop_ );
	p->Init( );
	return ( p );
}
//=============================================================================
//�N���G�C�g(�����Đ�)
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
//����������
//=============================================================================
HRESULT CEffect::Init( )
{
	CScene::AddLinkList( CRenderer::TYPE_RENDER_NORMAL );

	return S_OK;
}
//=============================================================================
// �I������
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
// �X�V����
//=============================================================================
void CEffect::Update( )
{
	if ( isPlay&&!isPause )
	{
		//�X�s�[�h���l�������ړ�
		m_Pos += m_Move_Spd;
		m_Rot += m_Rot_Spd;

		//�G�t�F�N�g�̍��W��ύX
		CEffectManager::GetEffectManager( )->SetLocation( m_handle, m_Pos.x,m_Pos.y,m_Pos.z );
		//�G�t�F�N�g�̌�����ύX
		CEffectManager::GetEffectManager( )->SetRotation( m_handle, m_Rot.x, m_Rot.y, m_Rot.z );
		//�G�t�F�N�g�̑傫����ύX
		CEffectManager::GetEffectManager( )->SetScale( m_handle, m_vScl.x, m_vScl.y, m_vScl.z );

		// �G�t�F�N�g�̍X�V�������s��
		CEffectManager::GetEffectManager( )->BeginUpdate( );
		CEffectManager::GetEffectManager()->UpdateHandle(m_handle, m_PlaySpeed);
		CEffectManager::GetEffectManager( )->EndUpdate( );
		//�t���[���̃J�E���g�A�b�v
		FrameCount += m_PlaySpeed;

		//���[�v�Đ����[�h
		if( FrameCount >= MaxFrame )
		{
			//�j���t���O��ON�̏ꍇ�͖������Ŕj��
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
				// �G�t�F�N�g�̒�~
				CEffectManager::GetEffectManager( )->StopEffect( m_handle );
				// �G�t�F�N�g�̍Đ�
				m_handle = CEffectManager::GetEffectManager()->Play(CEffectHolder::GetEffect(EffectType), 0, 0, 0);
				//�G�t�F�N�g�̍��W��ύX
				CEffectManager::GetEffectManager( )->SetLocation( m_handle, m_Pos.x, m_Pos.y, m_Pos.z );
				//�G�t�F�N�g�̌�����ύX
				CEffectManager::GetEffectManager( )->SetRotation( m_handle, m_Rot.x, m_Rot.y, m_Rot.z );
				//�G�t�F�N�g�̑傫����ύX
				CEffectManager::GetEffectManager( )->SetScale( m_handle, m_vScl.x, m_vScl.y, m_vScl.z );
				FrameCount = 0;
			}

			//���[�v�Đ��ȊO�̏ꍇ�͎��M��j��
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
	if (isPlay)
	{
		CEffectManager::GetEffectManager()->StopEffect(m_handle);
		FrameCount = 0;
		isPlay = false;
	}


	m_Pos  = pos;
	m_Rot  = rot;
	m_vScl = scl;

	// �G�t�F�N�g�̍Đ�
	m_handle = CEffectManager::GetEffectManager()->Play(CEffectHolder::GetEffect(EffectType), 0, 0, 0);
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
