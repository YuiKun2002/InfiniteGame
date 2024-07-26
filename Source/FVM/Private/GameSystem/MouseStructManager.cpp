// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/MouseStructManager.h"

bool UMouseStructManager::GetMouseRound(const int32& _CurRound, FRondWidthMouseConfig& _OutCurRound)
{
	if (_CurRound < this->CurMouseConfig.CurRoundConfig.Num())
	{		
		_OutCurRound = this->CurMouseConfig.CurRoundConfig[_CurRound];

		if (_CurRound - 1 >= 0)
		{
			//置空回合，释放内存
			FRondWidthMouseConfig Temp;
			this->CurMouseConfig.CurRoundConfig[_CurRound - 1] = Temp;
		}

		return true;
	}

	return false;
}

bool UMouseStructManager::GetMouseTimeNodeByRound(FRondWidthMouseConfig& _InputRound, const int32& Index, FTimeNodeWidthRound& _OutCurTimeNode)
{
	if (Index < _InputRound.CurNode.Num())
	{
		_OutCurTimeNode = _InputRound.CurNode[Index];

		return true;
	}

	return false;
}

bool UMouseStructManager::GetMouseTimeNodeByTimeNode(FTimeNodeWidthRound& _InputTimeNode, const int32& _Index, FTimeNode& _OutCurNode)
{
	if (_Index > 19 || _Index < 0)
		return false;

	if (_Index < _InputTimeNode.CurNode.Num())
	{
		_OutCurNode = _InputTimeNode.CurNode[_Index];

		return true;
	}

	return false;
}

void UMouseStructManager::SetMouseConfig(const FMouseConfig& _Config)
{
	this->CurMouseConfig = _Config;
}

const FMouseConfig& UMouseStructManager::GetMouseConfig()
{
	return this->CurMouseConfig;
}

void UMouseStructManager::SetRowCount(const int32& _Index)
{
	this->RowCount = _Index;
}

int32 UMouseStructManager::GetRowCount()
{
	return this->RowCount;
}

int32 UMouseStructManager::GetRoundTotal()
{
	return this->CurMouseConfig.CurRoundConfig.Num();
}

int32 UMouseStructManager::GetUpRoundTotal()
{
	int32 NormalCount = 0;
	for (const auto& Cur : this->CurMouseConfig.CurRoundConfig)
	{
		NormalCount++;
		if (Cur.RoundKey.Enable)
		{
			break;
		}
	}

	return this->GetRoundTotal() - NormalCount;
}

int32 UMouseStructManager::GetNormalRoundTotal()
{
	int32 NormalCount = 0;
	for (const auto& Cur : this->CurMouseConfig.CurRoundConfig)
	{
		NormalCount++;
		if (Cur.RoundKey.Enable)
		{
			break;
		}
	}
	return NormalCount;
}

int32 UMouseStructManager::GetRoundNodeCountByRoundIndex(const int32& _Index)
{
	if (_Index < 0 || _Index >= this->CurMouseConfig.CurRoundConfig.Num())
	{
		return 0;
	}

	return this->CurMouseConfig.CurRoundConfig[_Index].CurNode.Num();
}
