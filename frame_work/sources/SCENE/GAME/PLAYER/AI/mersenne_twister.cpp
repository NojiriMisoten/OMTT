//=============================================================================
//
// Unit_Tactics[unit_tactics.cpp]
//
// Author : Naoki Nojiri
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <random>
#include "mersenne_twister.h"

std::random_device rnd;		// 非決定的な乱数生成器を生成
std::mt19937 mt(rnd());		//  メルセンヌ・ツイスタの32ビット版、引数は初期シード値

//=============================================================================
// メルセンヌツイスター
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