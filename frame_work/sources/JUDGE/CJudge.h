//=============================================================================
//
// CJudge�N���X [CJudge.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CJUDGE_H_
#define _CJUDGE_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../MAIN/main.h"
#include "../SCENE/GAME/PLAYER/CPlayerManager.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CJudgeManager;
class CScene2D;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CJudge
{
public:
	// OBB���\����
	typedef struct
	{
		D3DXVECTOR2 pos;		// ���S�̍��W
		D3DXVECTOR2 axisX;		// ������
		D3DXVECTOR2 axisY;		// ������
		float		lengthX;	// ���S����̋���
		float		lengthY;	// ���S����̋���
	}OBB_INFO;

	// �R���X�g���N�^
	CJudge(CJudgeManager* pJudgeManager);

	// �f�X�g���N�^
	~CJudge(void);

	// �t�B�[���h�ƃv���C���[�̂����蔻��
	void ColiFieldxPlayer(void);

	// ��ƃv���C���[�̂����蔻��
	void ColiTreasurexPlayer(void);

	// �U���ƃv���C���̓����蔻��
	void ColiAttackxPlayer(void);

	// ���ƃv���C���̓����蔻��
	void ColiThreadxPlayer(void);

	// �t�B�[���h�Ƒ���ɂȂ鎅�̂����蔻��
	void ColiFieldxThreadOfFoothold(void);

	// �S�[���ƃv���C���̓����蔻��
	void ColiGoalxPlayer(void);

	// �t�B�[���h�ƕ󔠂̓����蔻��
	void ColiFieldxTreasure(void);

private:
	// �����\����
	struct Segment {
		D3DXVECTOR2 s; // �n�_
		D3DXVECTOR2 v; // �����x�N�g���i�����̒������S���̂Ő��K�����Ȃ��悤�ɁI�j
	};
	//=========================================================================
	// ��`�Ƌ�`�̂����蔻��
	// ����:���肵�������̂�OBB���
	// �߂�l�F�^�U
	//=========================================================================
	bool IsOBB(OBB_INFO& rectA, OBB_INFO& rectB);

	//=========================================================================
	// OBB���쐬�֐�
	// ����:�쐬����OBB��������A���W�A���b�g�A���A����
	//=========================================================================
	void CreateOBBInfo(OBB_INFO* outOBB, D3DXVECTOR2* pos, float* rot, float* width, float* height);

	//=========================================================================
	// �����Ɛ����̂����蔻��
	// ����:����1�A����2�A(out)��_�̍��W
	// �߂�l�F�^�U
	//=========================================================================
	bool ColiRayxRay(Segment &seg1, Segment &seg2, D3DXVECTOR2* outPos);

	CJudgeManager*	m_pJudgeManager;
	CScene2D*		m_LastFieldColiPlayer[MAXIMUM_NUMBER_OF_PLAYER];	// �v���C���[���Ō�ɓ������Ă��t�B�[���h�|�C���^
};

#endif
//----EOF----