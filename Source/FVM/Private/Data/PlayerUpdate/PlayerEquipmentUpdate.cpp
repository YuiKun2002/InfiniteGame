// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Data/PlayerUpdate/PlayerEquipmentUpdate.h"

void UPlayerEquipmentUpdate::Update_Implementation(UPlayerStructManager* PlayerDataIns)
{
	//初始化ID
	PlayerDataIns->UpdateEquipID(PlayerDataIns->M_PlayerBagGirdGroup.GetBagCount(0));
	//更新装备
	for (auto& Item : PlayerDataIns->M_PlayerItems_Equipment)
	{
		int32 _ItemCount = Item.M_IsOverlap ? Item.M_Count : 0;
		bool _Use = Item.M_Used;
		bool _WaitRemove = Item.bWaitRemove;
		int32 _ID = Item.M_ItemID;

		//搜索成功的更新
		if (UEquipmentBaseStruct::SearchEquipmentFromDataTable(Item.ItemName, Item))
		{
			//更新材料个数
			Item.M_Count = _ItemCount;
			//更新使用情况
			Item.M_Used = _Use;
			//是否等待删除
			Item.bWaitRemove = _WaitRemove;
			//赋予ID号
			Item.M_ItemID = _ID;

			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("装备道具：") + Item.ItemName + TEXT("ID：") + FString::FromInt(Item.M_ItemID)));
			}
		}
		else {
			//未查询的 标记(判断名称是否是：煮蛋器投手)【版本降低的可以忽略】
			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("当前装备道具-查询失败：") + Item.ItemName + TEXT("ID：") + FString::FromInt(_ID)));
			}

			//注销ID
			PlayerDataIns->RemoveEquipmentFromPlayerBag(Item);
		}

	}

	//清理掉等待删除的物品
	UGameSystemFunction::ClearWaitingItemsForEquip(PlayerDataIns->M_PlayerItems_Equipment);
}
