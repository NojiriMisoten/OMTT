//=============================================================================
//
// �G�t�F�N�g�N���X [efect.h]
// Author : ��{�F��
//
//=============================================================================
#ifndef _CEFECT_H_
#define _CEFECT_H_
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../MAIN/main.h"
#include "CEffectManager.h"
#include "../BASE_OBJECT/CScene.h"

//=============================================================================
// �}�N����`
//=============================================================================

//=============================================================================
// �O���錾
//============================================================================= 

//=============================================================================
// �N���X�錾
//=============================================================================
class CEffect : public CScene
{
	public:
		//�R���X�g���N�^
		CEffect( int maxFrame, char *filename, bool isloop_ );
		//�f�X�g���N�^
		~CEffect( );

		//�N���G�C�g
		static CEffect* Create( int maxFrame, char *filename, bool isloop_ );
		
		// �|�W�V�����A�N�Z�T
		D3DXVECTOR3& GetPos( void ){ return m_Pos; };
		D3DXVECTOR3* GetPosAdr( void ){ return &m_Pos; };
		void AddPos( D3DXVECTOR3& pos ){ m_Pos += pos; }
		void		SetPos( D3DXVECTOR3& pos ){ m_Pos = pos; };
		void		SetPos( float x, float y, float z ){ m_Pos.x = x; m_Pos.y = y; m_Pos.z = z; };

		// ���b�g�A�N�Z�T
		D3DXVECTOR3& GetRot( void ){ return m_Rot; };
		void		SetRot( D3DXVECTOR3& rot ){ m_Rot = rot; };
		void		SetRot( float x, float y, float z ){ m_Rot.x = x; m_Rot.y = y; m_Rot.z = z; };

		// �X�P�[���A�N�Z�T
		D3DXVECTOR3& GetScl( void ){ return m_vScl; };
		void		SetScl( D3DXVECTOR3& scl ){ m_vScl = scl; };
		void		SetScl( float x, float y, float z ){ m_vScl.x = x; m_vScl.y = y; m_vScl.z = z; };

		// �Đ��X�s�[�h�A�N�Z�T
		void SetPlaySpeed(float value){ m_PlaySpeed = value; }
		float GetPlaySpeed(void){ return (m_PlaySpeed); }
		void AddPlaySpeed(float value){ m_PlaySpeed += value; }

		//������
		HRESULT Init( );
		//�I��
		void Uninit( );
		//�X�V
		void Update( );
		//�`��
		void DrawNormalRender( );
		//�Đ�
		void Play( D3DXVECTOR3& pos, D3DXVECTOR3& rot, D3DXVECTOR3& scl );

		//�ꎞ��~
		void Pause( ){ isPause = true; }
		//�ꎞ��~����
		void UnPause( ){ isPause = false; }
		//��~
		void Stop( );
		//�Đ������ǂ����擾
		bool GetIsPlay( ){ return ( isPlay ); }


	private:
		// �傫���i�X�P�[���j
		D3DXVECTOR3			m_vScl;
		//�t�@�C����
		char *pFileName;
		//�J�E���g�p
		float FrameCount;
		//�t���[����
		int MaxFrame;
		//�G�t�F�N�g�̃X�s�[�h
		float m_PlaySpeed;
		//�G�t�F�N�g�{��
		::Effekseer::Effect* m_pEffect;
		//�G�t�F�N�g�̃n���h��
		::Effekseer::Handle m_handle;
		//���[�v���邩�ǂ���
		bool isLoop;
		//�Đ������ǂ���
		bool isPlay;
		//�|�[�Y�����ǂ���
		bool isPause;

};
#endif