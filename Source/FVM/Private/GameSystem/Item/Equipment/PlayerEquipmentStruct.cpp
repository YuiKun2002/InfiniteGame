// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameSystem/Item/Equipment/PlayerEquipmentStruct.h"
#include "Templates/SharedPointer.h"
#include "Data/EquipmentDataSturct.h"

bool UPlayerEquipmentStruct::SearchPlayerSuitByEquipment(FString& _ItemName, EPlayerEquipmentType _EPlayerEquipmentType, FPlayerEquipment& OutData)
{
	UEquipmentDataAssetCache* Cache = GetGameDataAssetCache<UEquipmentDataAssetCache>(GLOBALASSET_EQUIP);
	for (const auto& Datas : Cache->GetPlayerEquipment())
	{
		UE_LOG(LogTemp, Error,
			TEXT("UPlayerStructManager::SearchPlayerSuitByEquipment: 当前物品名称：%s 目标名称：%s"),
			*Datas.M_FEquipment.ItemName.ToString(),
			*_ItemName);

		if (
			Datas.M_FEquipment.ItemName.ToString().Equals(_ItemName) &&
			Datas.M_FEquipment.M_PlayerEquipmentType == _EPlayerEquipmentType
			)
		{
			OutData = Datas.M_FEquipment;
			return true;
		}
	}

	return false;
}