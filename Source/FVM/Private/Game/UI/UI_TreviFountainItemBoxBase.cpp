// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/UI_TreviFountainItemBoxBase.h"

bool UUI_TreviFountainItemBoxBase::Initialize()
{
	if (!Super::Initialize())
		return false;
	return true;
}

void UUI_TreviFountainItemBoxBase::SetItemName(const FString& _ItemName)
{
	this->M_ItemName = _ItemName;
}

FString UUI_TreviFountainItemBoxBase::ToString_Implementation()
{
	return this->M_ItemName;
}