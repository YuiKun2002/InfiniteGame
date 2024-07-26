// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameSystem/Item/Equipment/PlayerWepaonStruct.h"

FString UPlayerWepaonStruct::GetDataTablePath(const EEquipment& _Equipment)
{
	switch (_Equipment)
	{
	case EEquipment::E_PlayerWeaponSecond:return TEXT("DataTable'/Game/Resource/BP/Data/PlayerWeapon/Second/PlayerWeaponSecondData.PlayerWeaponSecondData'");
	case EEquipment::E_PlayerWeaponSuper:return TEXT("DataTable'/Game/Resource/BP/Data/PlayerWeapon/Super/PlayerWeaponSuperData.PlayerWeaponSuperData'");
	default:return TEXT("DataTable'/Game/Resource/BP/Data/PlayerWeapon/First/PlayerWeaponFirstData.PlayerWeaponFirstData'");
	}

	return TEXT("");
}