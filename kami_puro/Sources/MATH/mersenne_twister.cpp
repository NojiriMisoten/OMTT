//=============================================================================
//
// Unit_Tactics[unit_tactics.cpp]
//
// Author : Naoki Nojiri
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <random>
#include "mersenne_twister.h"

std::random_device rnd;		// �񌈒�I�ȗ���������𐶐�
std::mt19937 mt(rnd());		//  �����Z���k�E�c�C�X�^��32�r�b�g�ŁA�����͏����V�[�h�l

//=============================================================================
// �����Z���k�c�C�X�^�[
//-----------------------------------------------------------------------------
float mersenne_twister_float(float low, float high){
	std::uniform_real_distribution<> rand_(low, high);
	return static_cast<float>(rand_(mt));
}

int mersenne_twister_int(int low, int high)
{
	std::uniform_int_distribution<> rand_(low, high);
	return static_cast<int>(rand_(mt));
}

// EOF