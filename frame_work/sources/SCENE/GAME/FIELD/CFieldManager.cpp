//=============================================================================
//
// CFieldManager�N���X [CFieldManager.cpp]
// Author : ��K�@����
//
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "CFieldManager.h"
#include "CField.h"
#include <stdio.h>
#include "CNaviTile.h"


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	char* fieldFile;
	char* naviFile;
}FIELD_INFO;


//*****************************************************************************
// �萔��`
//*****************************************************************************
// �t�B�[���h�t�@�C���̃p�X
static const FIELD_INFO FIELD_INFO_FILE_PATH[CFieldManager::FIELD_MAX] =
{
	{ "data/FIELD/field01.txt", "data/FIELD/NAVI_FILE/navi_01.txt" },
	{ "data/FIELD/field02.txt", "data/FIELD/NAVI_FILE/navi_02.txt" },
	{ "data/FIELD/field03.txt", "data/FIELD/NAVI_FILE/navi_03.txt" },
	{ "data/FIELD/field04.txt", "data/FIELD/NAVI_FILE/navi_04.txt" },
	{ "data/FIELD/field05.txt", "data/FIELD/NAVI_FILE/navi_05.txt" },
	{ "data/FIELD/field06.txt", "data/FIELD/NAVI_FILE/navi_06.txt" },
	{ "data/FIELD/field07.txt", "data/FIELD/NAVI_FILE/navi_07.txt" },
	{ "data/FIELD/field08.txt", "data/FIELD/NAVI_FILE/navi_08.txt" },
};

//=========================================================================
// �ÓI�����o�ϐ�
//=========================================================================
CNaviTile* CFieldManager::m_pNaviTile = NULL;

//=========================================================================
// �R���X�g���N�^
//=========================================================================
CFieldManager::CFieldManager(void)
{
	
}

//=========================================================================
// �f�X�g���N�^
//=========================================================================
CFieldManager::~CFieldManager(void)
{
	
}

//=========================================================================
// �N���G�C�g
//=========================================================================
CField* CFieldManager::CreateField(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType)
{
	CField* p = CField::Create(pDevice, pos, width, height, texType);
	return p;
}

//=========================================================================
// ���[�h�t�B�[���h
//=========================================================================
void CFieldManager::LoadField(LPDIRECT3DDEVICE9 *pDevice, FIELD_TYPE fieldType)
{
	int nFieldNameLength = 0;
	char FieldFilePath[MAX_PATH] = { 0 };
	char FieldName[MAX_PATH] = { 0 };

	// �ǂݍ��ݗp�t�@�C��
	FILE* fpr = fopen(FIELD_INFO_FILE_PATH[fieldType].fieldFile, "r");

	// ���s������
	if (fpr == NULL)
	{
		MessageBox(NULL, "�t�B�[���h�t�@�C���̓ǂݍ��݂Ɏ��s���܂���\n", "�G���[", MB_OK | MB_ICONERROR);
	}

	// ����������
	else
	{
		int ans = 0;
		int ret = 0;
		while (1)
		{
			// ���W�擾
			D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
			fscanf(fpr, "%f", &pos.x);
			fscanf(fpr, "%f", &pos.y);
			fscanf(fpr, "%f", &pos.z);
			
			// ROT�擾
			D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, 0.f);
			fscanf(fpr, "%f", &rot.x);
			fscanf(fpr, "%f", &rot.y);
			fscanf(fpr, "%f", &rot.z);

			// ���擾
			float width = 0.f;
			fscanf(fpr, "%f", &width);

			// �����擾
			float height = 0.f;
			fscanf(fpr, "%f", &height);

			int nTexNameLength = 0;
			char texFilePath[MAX_PATH] = { 0 };
			char texName[MAX_PATH] = { 0 };
			// �g�p�e�N�X�`���̕��������擾
			fscanf(fpr, "%d", &nTexNameLength);

			// �e�N�X�`���g���Ă�Ȃ�
			if (nTexNameLength > 0)
			{
				// �e�N�X�`�����擾
				char temp[MAX_PATH] = { 0 };
				fscanf(fpr, "%s", texName);

				// �p�X�쐬
				strcat(temp, TEX_FOLDER_PATH);	// TEXTURE�t�H���_��
				strcat(temp, texName);				// �t�@�C����
				strcpy(texFilePath, temp);
			}

			else
			{
				// �e�N�X�`�����擾
				char temp[MAX_PATH] = { 0 };
				fscanf(fpr, "%s", texName);
				strcpy(texFilePath, "NULL");
			}

			// �e�N�X�`���^�C�v�擾
			TEXTURE_TYPE texType = CTexture::GetTextureTye(texFilePath);

			// �ۑ��t�@�C�����擾
			char saveName[MAX_PATH] = { 0 };
			fscanf(fpr, "%s", &saveName[0]);

			// �쐬
			CField* p = CreateField(pDevice, pos, width, height, texType);
			
			// ��]���f
			p->SetRot(rot);

			// �I��肩�ǂ������ׂ�
			ret = fscanf(fpr, "%d", &ans);
			if (ret == EOF)
			{
				break;
			}
			int size = sizeof(int);
			size *= -1;
			fseek(fpr, size, SEEK_CUR);
		}
	}
	fclose(fpr);

	// �i�r�t�@�C���ǂݍ���
	m_pNaviTile = CNaviTile::Create(FIELD_INFO_FILE_PATH[fieldType].naviFile);
}

//=========================================================================
// �I��
//=========================================================================
void CFieldManager::Uninit(void)
{
	if (m_pNaviTile)
	{
		m_pNaviTile->Uninit();
		delete m_pNaviTile;
		m_pNaviTile = NULL;
	}
}

//=========================================================================
// �X�V
//=========================================================================
void CFieldManager::Update(void)
{
	
}

//=========================================================================
// ������
//=========================================================================
void CFieldManager::Init(void)
{
	
}


//----EOF----