// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Data/PlayerUpdate/PlayerCardUpdate.h"

void UPlayerCardUpdate::Update_Implementation(UPlayerStructManager* PlayerDataIns)
{
	//更新卡片
	for (auto& Item : PlayerDataIns->M_PlayerItems_Card)
	{
		int32 _CardGrade = Item.M_CardGrade;
		if (UCardBaseStruct::SearchCardFromDataTable(Item.ItemName, Item))
		{
			//更新卡片等级
			Item.M_CardGrade = _CardGrade;
		}
	}
}