//=============================================================================
//
// �f�o�b�O��񏈗� [CDebugProc.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CDEBUGPROC_H_
#define _CDEBUGPROC_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../MAIN/main.h"

//*****************************************************************************
// �N���X�錾
//*****************************************************************************
class CDebugProc
{
public:
	// �R���X�g���N�^
	CDebugProc(void);

	// �f�X�g���N�^
	~CDebugProc(void);

	//==============================================
	// ����������
	// �߂�l:����
	//==============================================
	HRESULT Init(void);

	// �I������
	void Uninit(void);

	// �`�揈��
	static void Draw(void);

	//=================================================
	// �o�^����
	// ����:�\������������(%d���܂�)�A%d���ɑΉ�����ϐ�
	//==================================================
	static void PrintL(const char *fmt, ...);
	static void PrintR( const char *fmt, ... );
	static void PrintLD( const char *fmt, ... );
	static void PrintRD( const char *fmt, ... );
	static void PrintU( const char *fmt, ... );
	static void PrintD( const char *fmt, ... );

	//==================================================
	// �\�����Ă邩�̃t���O�Z�b�g�b�g����
	// ����:�t���O
	//==================================================
	static void SetDrawDisp(bool bDisp){m_bDisp = bDisp;};

private:
	static const int LENGTH_STRING_BUFF = 1028; // �ő�\��������

	static LPD3DXFONT m_pD3DXFONT;				// �t�H���g�ւ̃|�C���^
	static char m_aStrL[LENGTH_STRING_BUFF];	// �f�o�b�O�\���p�̕�����̃o�b�t�@
	static char m_aStrR[LENGTH_STRING_BUFF];
	static char m_aStrLD[LENGTH_STRING_BUFF];
	static char m_aStrRD[LENGTH_STRING_BUFF];
	static char m_aStrU[LENGTH_STRING_BUFF];
	static char m_aStrD[LENGTH_STRING_BUFF];
	static bool m_bDisp;						// �f�o�b�O�\��ON/OFF
	static int	m_nCounter;						// �������J�E���^�[
};

#endif
//----EOF----