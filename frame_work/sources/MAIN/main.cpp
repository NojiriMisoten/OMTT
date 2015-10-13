//=============================================================================
//
// ���C������ [main.cpp]
// Author : ��K�@����
//
//=============================================================================
#include "main.h"
#include "../MANAGER/CManager.h"
#include "../RENDERER/CRenderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
static const char*	CLASS_NAME	= "AppClass";		// �E�C���h�E�̃N���X��
static const char*	WINDOW_NAME	= "�g���W���[�n���^�[����";			// �E�C���h�E�̃L���v�V������
static const int	TIME_INTERVAL = 500;			// ���s�Ԋu
static const int	FPS_COEFFICIENT = 1000;			// FPS�̌W��
static const int	BASE_FPS = 1000 / 60;			// FPS�̊���x
static const int	DRAW_FPS = 1000 / 30;			// FPS�̊���x

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Init(void);
void Update(void);
void Draw(void);
void Uninit(void);

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
static CManager					*g_pManager;
static bool bDrawCounter = true;
//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	// ���������[�N�`�F�b�N
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j

	// �}�l�[�W���[�쐬
	g_pManager = new CManager;

	// FPS�p
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND hWnd;
	MSG msg;
	
	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�̎g����@�\�̐ݒ�
	DWORD style = WS_OVERLAPPEDWINDOW & ~WS_MINIMIZEBOX & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;

	// ��`�쐬
	RECT rc;
	rc.top = 0;
	rc.left = 0;
	rc.bottom = SCREEN_HEIGHT;
	rc.right = SCREEN_WIDTH;

	// �A�W���X�g�E�B���h�E
//	AdjustWindowRect(&rc, style, FALSE);

	long WidthWindow = rc.right - rc.left;
	long HeightWindow = rc.bottom - rc.top;

	// �E�B���h�E�̍쐬
	hWnd = CreateWindowEx(0,
						CLASS_NAME,
						WINDOW_NAME,
						style,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						SCREEN_WIDTH,
						SCREEN_HEIGHT,
						NULL,
						NULL,
						hInstance,
						NULL);


	//�t���[���J�E���g������
	timeBeginPeriod(1);				// ����\��ݒ�
	dwExecLastTime =
		dwFPSLastTime = timeGetTime();
	dwCurrentTime =
		dwFrameCount = 0;

	// �E�C���h�E�̕\��(�����������̌�ɌĂ΂Ȃ��Ƒʖ�)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	bool modeWindow = false;
//	if (MessageBox(NULL, "�t���X�N���[���ŋN�����܂����H", "�E�B���h�E���[�h", MB_YESNO) == IDYES)
//	{
//		//�t���X�N���[���ŏ���������(�E�B���h�E���쐬���Ă���s��)
//		modeWindow = false;
//	}
//	else
//	{
//		//�ʏ�� ����������(�E�B���h�E���쐬���Ă���s��)
//		modeWindow = true;
//	}

	// ����������(�E�B���h�E���쐬���Ă���s��)
	if (FAILED(g_pManager->Init(hInstance, hWnd, modeWindow)))
	{
		return -1;
	}

	// ���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// ���ݎ����擾
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= TIME_INTERVAL)	// 0.5�b���ƂɎ��s
			{
#ifdef _DEBUG
				// FPS�\��
				g_pManager->GetRenderer()->SetFPS(dwFrameCount * FPS_COEFFICIENT / (dwCurrentTime - dwFPSLastTime));
#endif
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) >= BASE_FPS)
			{
				dwExecLastTime = dwCurrentTime;

				// �X�V����
				Update();

				if (bDrawCounter)
				{
					// �`�揈��
					Draw();

					dwFrameCount++;
				}

				bDrawCounter = !bDrawCounter;
#ifdef _DEBUG
				// �o�b�t�@�N���A
				CDebugProc::ClearBuff();
#endif
			}
		}
	}

	
	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �I������
	Uninit();

	timeEndPeriod(1);				// ����\��߂�

	return (int)msg.wParam;
}

//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// �I������
//=============================================================================
void Uninit(void)
{
	g_pManager->Uninit();
	delete g_pManager;
}

//=============================================================================
// �X�V����
//=============================================================================
void Update()
{
	g_pManager->Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void Draw()
{
	g_pManager->Draw();
}

bool GetDrawFlag(void)
{
	return bDrawCounter;
}
//----EOF----