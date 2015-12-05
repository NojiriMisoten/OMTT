//=============================================================================
//
//	CCommandChart�N���X�̐錾(CCommandChart.h)
//
//	Author : �����@�Ȉ�
//
//=============================================================================
#ifndef __CCOMMANDCHART_H__
#define __CCOMMANDCHART_H__

//-----------------------------------------------------------------------------
//	�C���N���[�h
//-----------------------------------------------------------------------------
#include "../../../MAIN/main.h"
#include "CCommandChartUI.h"
#include "../PLAYER/CPlayerManager.h"

class CCommandChartManager;
//-----------------------------------------------------------------------------
//	�}�N����`
//-----------------------------------------------------------------------------
// �R�}���h�̍ő���͐�
static const int MAX_COMMAND_KEEP = 6;
// ���̓��͌��̋Z�̍ő吔
static const int MAX_NEXT_COMMAND_VIEW = 4;

// �v���C���[�̐�
static const int MAX_PLAYER = 2;

// �R�}���h�̓��͐�
// �����Z
static const int COMMAND_INPUT_NUM_MONO = 1;
// ���Z
static const int COMMAND_INPUT_NUM_SMALL = 3;
// ���Z
static const int COMMAND_INPUT_NUM_MIDDLE = 4;
// ��Z
static const int COMMAND_INPUT_NUM_LARGE = 5;
// ���ߋZ
static const int COMMAND_INPUT_NUM_FINISHER = 6;

// �Z�̎��
typedef enum
{
	COMMAND_TYPE_NONE = -1,		// �R�}���h���s����������
	COMMAND_TYPE_CHOP = 0,		// �`���b�v
	COMMAND_TYPE_ELBOW,			// �G���{�[
	COMMAND_TYPE_LARIAT,		// �����A�b�g
	COMMAND_TYPE_ROLLING,		// ���[�����O�G���{�[
	COMMAND_TYPE_SHOULDER,		// �t���C���O�G���{�[
	COMMAND_TYPE_DROPKICK,		// �h���b�v�L�b�N
	COMMAND_TYPE_SLAP,			// �r���^
	COMMAND_TYPE_BACKDROP,		// �o�b�N�h���b�v
	COMMAND_TYPE_STUNNER,		// �X�^�i�[
	COMMAND_TYPE_ROPE,			// ���[�v
	COMMAND_TYPE_FINISHER,		// ���ߋZ
	COMMAND_TYPE_MAX			// �Z�̐�

}COMMAND_TYPE;

// �Z�̑�ʂ������
typedef enum
{
	SKILL_SMALL_ATTACK = 0,
	SKILL_MIDDLE_ATTACK,
	SKILL_BIG_ATTACK,
	SKILL_MAX,
	SKILL_FINISH_ATTACK	// MAX���Œ�̐��Ƃ��Ďg���Ă��邽��
}SKILL_TYPE;

//-----------------------------------------------------------------------------
//	�\���̒�`
//-----------------------------------------------------------------------------
// �R�}���h���
struct COMMAND_INFO
{
	const int m_nCommandLength;
	const COMMAND_TYPE m_CommandType;
	const BUTTON_TYPE* m_Command;
};

// �R�}���hUI�̕ۑ����
struct COMMAND_UI_INFO
{
	COMMAND_UI_INFO* m_NextCommand;		// ���̌��̃{�^�����
	CCommandChartUI* m_pUIType;			// �R�}���h�`���[�gUI�̃|�C���^�̕ێ�
	bool m_isInputButton;				// ���̃{�^���̓��͊m�F
};

//-----------------------------------------------------------------------------
//	�O���錾
//-----------------------------------------------------------------------------
class CCommandName;

//-----------------------------------------------------------------------------
//	�N���X��`
//-----------------------------------------------------------------------------
class CCommandChart
{
public:
	static const int MAX_BEGIN_COMAND_NUM = 5;
	// �R�}���h�`���[�g�̏��
	typedef enum
	{
		MODE_APPEAR = 0,			// �o����
		MODE_INPUT,					// ���͎�t��
		MODE_COMPLETE_COMMAND,		// �R�}���h����
		MODE_VANISH,				// �\���I�t��
		MODE_RESET,
		MODE_ROPE,
		MODE_MAX
	}MODE_COMMAND_CHART;

	// �R���X�g���N�^
	// �����F�f�o�C�X�A�v���C���[�ԍ�
	CCommandChart(LPDIRECT3DDEVICE9* pDevice, PLAYER_ID nID, CCommandChartManager* pCommandManager);

	// �f�X�g���N�^
	~CCommandChart();

	// ������
	void Init(void);

	// �X�V
	void Update(void);

	// �`��
	void Draw(void);

	// �I��
	void Uninit(void);

	// ���g�̐���
	// �����F�f�o�C�X�̃|�C���^�A�����̃v���C���[�ԍ�
	// �߂�l�F���g�̃|�C���^�A�v���C���[�ԍ�
	static CCommandChart* Create(LPDIRECT3DDEVICE9* pDevice, PLAYER_ID nID, CCommandChartManager* pCommandManager);

	// ��ʊO�ւ̃t�F�[�h�A�E�g
	void ScreenOut(void);

	// ��ʓ��ւ̃t�F�[�h�C��
	void ScreenIn(void);

	// �ŏI�I�ɌJ��o���Z�𒲂ׂĕԂ�
	// �߂�l�F�J��o���Z�̃^�C�v
	COMMAND_TYPE UseTechnic(void);

	// �R�}���h���͉\�t���O�̕ύX
	// �����F�R�}���h���͉\�ɂ���̂Ȃ��true�s�ɂ���̂Ȃ��false
	void SetCommandInputFlag(bool isInput){ m_isCommandInput = isInput; }

	void SetCommandChartMode(MODE_COMMAND_CHART mode){ m_CommandChartMode = mode; };
	CCommandChart::MODE_COMMAND_CHART GetCommandChartMode(void) { return m_CommandChartMode; };

	//================================================================
	// �n���R�}���h�����̏�ԂɂȂ�
	// �Z�o������o�g�����[�h�p���Ȃ炱����
	//================================================================
	void ResetCommandList(void);

	//================================================================
	// �ʒu��e�N�X�`���Ȃǂ̉�����Ăׂ鏉����
	// MODE_APPEAR�ɃZ�b�g����O�ɌĂ�łق���
	//================================================================
	void ResetAllCommand(void);

	//********************************************
	//    2015/12/4
	//    �����@�ǋL
	//********************************************
	// ���ݕ\�����Ă���S�ẴR�}���h�̌��ɕ\�����Ă���R�}���h������
	//    �R�}���h�`���[�g�̕K�v�������Ȃ������Ɏg�p
	void AllCandidateInputBackPolygonVanish(void);
	//********************************************
	//    2015/12/4
	//    �����@�ǋL
	//********************************************


private:
	static const int MAX_COMAND_NUM = 5;
	static const int MAX_COMAND_NAME_NUM = 5;
	//*******************�ǋL�J�n11/23�@��K **************************************
	// �R�}���h���̊�b
	typedef struct
	{
		CCommandChartUI*	pCommandUI;
		bool				isEndList;
		D3DXVECTOR3			vAnimationPosDest;
	}BASE_COMMAND;

	// �n���{�^��
	typedef struct
	{
		BASE_COMMAND firstCommand[MAX_BEGIN_COMAND_NUM];
	}BEGIN_COMMAND;

	// �R�}���h���X�g
	typedef struct
	{
		BASE_COMMAND smallAttack[MAX_COMAND_NUM];	// �n���{�^����������
		BASE_COMMAND middleAttack[MAX_COMAND_NUM];
		BASE_COMMAND largeAttack[MAX_COMAND_NUM];
		BASE_COMMAND finishAttack[MAX_COMAND_NUM];
	}COMMAND_LIST;

	// �R�}���h�S��
	typedef struct
	{
		BEGIN_COMMAND	beginCommand;
		COMMAND_LIST	commandList;
	}COMMAND_STRUCT;

	typedef struct
	{
		BUTTON_TYPE	buttonType;
		bool		isUse;
	}KEEP_BUTTON_INFO;
	//*******************�ǋL�I��11/23�@��K **************************************

	//*************************************
	// �֐�
	//*************************************
	// �R�}���h����
	void InputCommand(void);

	// �R�}���h�̃`�F�b�N
	void CheckCommand(void);

	// �E��L�[�J�n�̃R�}���h�`���[�g�̐���
	void CreateRightUpTechnicCommand(void);

	// ����L�[�J�n�̃R�}���h�`���[�g�̐���
	void CreateLeftUpTechnicCommand(void);

	// �����L�[�J�n�̃R�}���h�`���[�g�̐���
	void CreateLeftDownTechnicCommand(void);

	// �E���L�[�J�n�̃R�}���h�`���[�g�̐���
	void CreateRightDownTechnicCommand(void);

	// FINISH�R�}���h�`���[�g�̐���
	void CreateFinishTechnicCommand(void);

	// �R�}���hUI�������ꂽ��Ԃɂ��邩���肵�ĉ�����Ă����Ԃɂ���̂ł���Ή����ꂽ��Ԃɂ���
	void CommandUIInput(BUTTON_TYPE button);

	// ���[�v�R�}���h���Z�b�g
	void SetRopeCommand(void);

	// FINISH�R�}���h�̎n���{�^���\�����邩
	void isAppearFinishBeginCommand(void);

	// FINISH�Z�������Ă邩
	bool isCompleteFinishSkill(void);

	//*******************�ǋL�J�n11/23�@��K **************************************
	// �n���R�}���h�̍쐬 
	// ���������̂݌ĂԂ��ƁI�I
	void InitCreateBeginCommand(void);
	
	// ���ɓ��͂�����̃R�}���h�̍쐬 
	// ���������̂݌ĂԂ��ƁI�I
	void InitCreateCommandList(void);

	// �R�}���h�`���[�g�\��
	void AppearanceCommandChart(void);

	// �R�}���h�`���[�g����
	void VanishCommandChart(void);

	//================================================================
	// ���ɓ��͂�����̃R�}���h�̃��Z�b�g
	// �e�N�X�`���ς��Ă邾��
	// �R�}���h�̃^�C�v�͂��̎������ŕς��(InputUIUVChange()�ŕς��Ă�)
	// ��ډ����ꂽ��Ɉ��Ă�
	//================================================================
	void ResetNextCommand(void);

	// �R�}���h����
	void VanishCommand(void);

	// �Z�������I������ĂȂ��Z������
	void VanishOtherSkill(SKILL_TYPE completeSkill);

	// �����ꂽ�{�^���ێ��֐�
	void KeepPushButton(bool isPushRightUp, bool isPushRightDown, bool isPushLeftUp, bool isPushLeftDown);

	//=================================================================
	// ������������֐�
	// BUTTON_TYPE_5(�㓯������)�ABUTTON_TYPE_6(����������)
	// �ȊO�͈����œ���Ă�false�����Ԃ��܂���
	//=================================================================
	bool GetSameTimePushButton(BUTTON_TYPE type);

	// �ێ����Ă��R�}���h�j��
	void RefleshKeepCommand();
	//*******************�ǋL�I��11/23�@��K **************************************
	static const int MAX_KEEP_COMMAND_NUM = 10;

	//*************************************
	// �ϐ�
	//*************************************
	// �R�}���h�ێ��p�ϐ�
	BUTTON_TYPE m_aCommandKeep;
	KEEP_BUTTON_INFO m_KeepButton[MAX_KEEP_COMMAND_NUM];
	// �f�o�C�X�̕ێ�
	LPDIRECT3DDEVICE9* m_pD3DDevice;
	// �������̋Z���\���pUI�̃|�C���^�̕ێ�
	CCommandName* m_apCommandName[MAX_COMAND_NAME_NUM];
	// �R�}���h�`���[�g�̔w�ʂɐ�������|���S��
	CScene2D* m_pBackPolygon;
	// ���͌��UI��\������x���W
	float m_fPosX;
	// �\�����Ă���R�}���h�������܂ł̃J�E���g
	int m_nCommandDeathCnt;
	// �ێ����̃R�}���h��
	int m_nKeepCommandNum;
	// ������ID
	PLAYER_ID  m_MyID;
	// �R�}���h���͉\���ǂ����̃t���O
	bool m_isCommandInput;

	// �R�}���h���X�g
	COMMAND_STRUCT	m_CommandInfo;

	// �R�}���h�`���[�g�̏��
	MODE_COMMAND_CHART	m_CommandChartMode;

	// ���������R�}���h
	COMMAND_TYPE		m_CompleteCommand;
	// ������������\��R�}���h
	COMMAND_TYPE		m_DestCompleteCommand;

	// ������҂����ԊǗ��p�iFINSH�Z�̓��������̗P�\�҂��߁j
	int		m_WiatCompleteCommandTimer;
	
	// ���������X�L��
	SKILL_TYPE	m_CompleteSkill;

	//================================================================
	// ������J������A�j���[�V�����@�˖{
	//================================================================
	// �A�j���[�V����������ۂɕK�v�ȃf�[�^
	struct CAnimeData
	{
		D3DXVECTOR3 m_Pos;
		D3DXVECTOR3 m_PosDest;
		float m_Width;
		float m_Height;
		float m_WidthDest;
		float m_HeightDest;
		// ���̊֐��ŏ��������ăA�j������
		void Init(
			D3DXVECTOR3 pos, D3DXVECTOR3 posDest,
			float width, float widthDest,
			float height, float heightDest)
		{
			m_Pos = pos;
			m_PosDest = posDest;
			m_Width = width;
			m_Height = height;
			m_WidthDest = widthDest;
			m_HeightDest = heightDest;
		}
		// 0~1�̃^�C���ŕۊǂ�����
		float GetEasingWidth(float time)
		{
			return EasingInterpolation(m_Width, m_WidthDest, time);
		}
		// 0~1�̃^�C���ŕۊǂ�������
		float GetEasingHeight(float time)
		{
			return EasingInterpolation(m_Height, m_HeightDest, time);
		}
		// 0~1�̃^�C���ŕۊǂ������WX
		float GetEasingPosX(float time)
		{
			return EasingInterpolation(m_Pos.x, m_PosDest.x, time);
		}
		// 0~1�̃^�C���ŕۊǂ������WY
		float GetEasingPosY(float time)
		{
			return EasingInterpolation(m_Pos.y, m_PosDest.y, time);
		}
	};
	// �A�j���[�V�����p�̏������֐� ���̃N���X��Init�ŌĂ�
	void InitAnime();
	// �X�V�ŌĂԁB�A�j���[�V�����̍X�V�܂Ƃ�
	void UpdateAnime();
	// �J�����������̃A�j���[�V�����J�n�֐�
	void StartAnimeClose();
	void StartAnimeOpen();

	// �A�j���[�V���������邩�t���O
	bool m_isAnime;
	// �J���A�j���[�V�����̕��� �A�j�����I��������̑J�ڂɕK�v
	bool m_isAnimeOpen;
	// �A�j���[�V�����p�̕ۊǂ���Ƃ��Ɏg���^�C��
	float m_AnimeCount;
	// �A�j���[�V�����f�[�^�@�w�i�̃|���S��
	CAnimeData m_Back;
	// �A�j���[�V�����f�[�^�@���U��
	CAnimeData m_CommandSmall[MAX_COMAND_NUM];
	// �A�j���[�V�����f�[�^�@���U��
	CAnimeData m_CommandMiddle[MAX_COMAND_NUM];
	// �A�j���[�V�����f�[�^�@��U��
	CAnimeData m_CommandLarge[MAX_COMAND_NUM];
	// �A�j���[�V�����f�[�^�@FINISH�U��
	CAnimeData m_CommandFinish[MAX_COMAND_NUM];
	// �A�j���[�V�����f�[�^�@�킩��Ȃ�
	CAnimeData m_CommandFirst[MAX_BEGIN_COMAND_NUM];
	// �A�j���[�V�����f�[�^�@�Z��
	CAnimeData m_CommandName[MAX_COMAND_NAME_NUM];
	// �R�}���h�}�l�[�W���[�̃A�h���X
	CCommandChartManager*	m_pCommandManager;
};

#endif

//EOF