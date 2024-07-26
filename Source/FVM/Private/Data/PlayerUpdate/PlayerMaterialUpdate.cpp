// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Data/PlayerUpdate/PlayerMaterialUpdate.h"

void UPlayerMaterialUpdate::Update_Implementation(UPlayerStructManager* PlayerDataIns)
{
	//更新材料
	for (auto& Item : PlayerDataIns->M_PlayerItems_Material)
	{
		int32 _ItemCound = Item.M_Count;
		UMaterialBaseStruct::SearchMaterailFromDataTable(Item.ItemName, Item, true, Item.M_MaterialType);
		//更新材料个数
		Item.M_Count = _ItemCound;
	}
}