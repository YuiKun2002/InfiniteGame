// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameSystem/Tools/ItemDataTable.h"

void UItemDataTable::SetValue(FTableRowBase* _Value)
{
	this->M_Value = _Value;
}

FTableRowBase* UItemDataTable::GetValue()
{
	return this->M_Value;
}