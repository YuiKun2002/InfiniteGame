// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameSystem/Item/Equipment/EquipmentBaseStruct.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/FVMGameInstance.h"

template <class DataTableRowType, class EquipmentType = FEquipmentBase>
void GetEquipmentRowDatasPtr(TArray<DataTableRowType>& _Item_Rows, TArray<EquipmentType*>& _OutArrays)
{
	for (auto& Item : _Item_Rows)
	{
		_OutArrays.Emplace((EquipmentType*)(&Item.M_FEquipment));
	}
}

//通过类型来获取对应的装备数据组
template<class _TargetDataTableStruct>
void GetEquipmentByType(TArray<FEquipmentBase*>& _Item, bool _Select, EEquipment _Type, TArray<_TargetDataTableStruct>& _DataArrays, const EEquipment& _ConstType)
{
	if (!_Select || _Select && _Type == _ConstType)
		GetEquipmentRowDatasPtr<_TargetDataTableStruct>(_DataArrays, _Item);
}

class EquipmentTypeFactory {
private:
	UPROPERTY()
	TArray<FEquipmentBase*>& M_ItemBuffer;
public:
	bool M_Select;
	EEquipment M_Type;
public:
	EquipmentTypeFactory() = delete;
	EquipmentTypeFactory(TArray<FEquipmentBase*>& _ItemBuffer, bool _Select, EEquipment _Type) :M_ItemBuffer(_ItemBuffer), M_Select(_Select), M_Type(_Type) {}
public:
	template<class _LTargetDataTableStruct>
	void GetEquipmentType(TArray<_LTargetDataTableStruct>& _Item, const EEquipment& _ConstType)
	{
		GetEquipmentByType(this->M_ItemBuffer, this->M_Select, this->M_Type, _Item, _ConstType);
	}
public:
	static void GetAllDatas(TArray<FEquipmentBase*>& _OutArrays, bool _Select, EEquipment _Type)
	{
		UEquipmentDataAssetCache* ECache = GetGameDataAssetCache<UEquipmentDataAssetCache>(GLOBALASSET_EQUIP);

		EquipmentTypeFactory LocalEquipmentTypeFactory(_OutArrays, _Select, _Type);

		LocalEquipmentTypeFactory.GetEquipmentType(ECache->GetBag(), EEquipment::E_Bag);
		//角色详细数据
		LocalEquipmentTypeFactory.GetEquipmentType(ECache->GetHeroeDetailDatas(), EEquipment::E_PlayerEquipment);
		LocalEquipmentTypeFactory.GetEquipmentType(ECache->GetGiftBox(), EEquipment::E_Gift);
		//主武器详细数据
		LocalEquipmentTypeFactory.GetEquipmentType(ECache->GetWeaponDetailDatas(), EEquipment::E_PlayerWeaponFirst);
		LocalEquipmentTypeFactory.GetEquipmentType(ECache->GetWeaponSecond(), EEquipment::E_PlayerWeaponSecond);
		LocalEquipmentTypeFactory.GetEquipmentType(ECache->GetWeaponSuper(), EEquipment::E_PlayerWeaponSuper);
		//宝石
		LocalEquipmentTypeFactory.GetEquipmentType(ECache->GetGemLaser(), EEquipment::E_WeaponGem);
		LocalEquipmentTypeFactory.GetEquipmentType(ECache->GetGemAttack(), EEquipment::E_WeaponGem);
	}
};


//-----------------------------------------------------------------------------------------
//      装备搜索->指定一个名称和类型，搜索出对应的数据
//-----------------------------------------------------------------------------------------
class SearchPro
{
	virtual void SearchEquipmentType(const FString& _EquipmentName, FEquipmentBase& OutputData, bool _bSelect, EEquipment Type, bool& _bResult, const EEquipment& _ConstType) = 0;
};

class SearchEquipmentToData : public SearchPro
{
public:
	void SearchEquipmentType(const FString& _EquipmentName, FEquipmentBase& OutputData, bool _bSelect, EEquipment Type, bool& _bResult, const EEquipment& _ConstType) {
		if (!_bResult)
			if (!_bSelect || _bSelect && Type == _ConstType)
			{
				TArray<FEquipmentBase> LocalTempItems;
				UEquipmentBaseStruct::GetAllEquipments(LocalTempItems, true, _ConstType);
				UEquipmentBaseStruct::GetEquipmentArraysData<FEquipmentBase>(_EquipmentName, LocalTempItems, OutputData, _bResult);
			}
	}
};


//----------------------------------------------------------------------------------------------------------------------
void UEquipmentBaseStruct::GetAllEquipmentsByPlayer(TArray<FEquipmentBase*>& _Item, bool _Select, const TArray<EEquipment>& _Type)
{
	//但凡是直接获取角色背包装备数据->都必须控制在背包的最大个数中

	int32 LocalMax = UFVMGameInstance::GetPlayerStructManager_Static()->GetBagMaxCount(0);
	int32 LocalIndex = 0;


	//当选择了只会，只会更具选择的类型，添加
	if (_Select)
	{
		for (auto& Item : UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Equipment)
		{
			if (LocalIndex < LocalMax)
				for (const auto& LType : _Type)
					if (Item.M_EquipmentType == LType)
					{
						//如果当前物品ID -1  先申请ID号，如果成功了，则直接添加，如果不成功就忽略
						if (Item.M_ItemID == -1)
							UFVMGameInstance::GetPlayerStructManager_Static()->AddEquipmentToPlayerBag(Item);

						if (Item.M_ItemID != -1)
							_Item.Emplace(&Item);
						break;
					}

			LocalIndex++;
		}

	}
	else {

		//直接全部添加
		for (auto& Item : UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Equipment)
		{
			if (LocalIndex < LocalMax)
			{
				//尝试申请
				if (Item.M_ItemID == -1)
					UFVMGameInstance::GetPlayerStructManager_Static()->AddEquipmentToPlayerBag(Item);

				//申请成功才会添加
				if (Item.M_ItemID != -1)
					_Item.Emplace(&Item);
			}
			LocalIndex++;
		}
	}
}


void UEquipmentBaseStruct::GetAllEquipmentsSourceData(TArray<FEquipmentBase*>& _Item, bool _Select, const TArray<EEquipment>& _Type)
{
	if (_Type.Num() != 0)
		for (const auto& Type : _Type)
			EquipmentTypeFactory::GetAllDatas(_Item, _Select, Type);
	else
		EquipmentTypeFactory::GetAllDatas(_Item, false, EEquipment::E_Bag);
}

//获取所有装备名称
void UEquipmentBaseStruct::GetAllEquipmentName(TArray<FItemBase>& _Item, bool _Select, EEquipment _Type)
{
	TArray<FEquipmentBase> LocalAllDatas;
	UEquipmentBaseStruct::GetAllEquipments(LocalAllDatas, _Select, _Type);

	for (auto& Data : LocalAllDatas)
		_Item.Emplace((FItemBase)(Data));
}

//获取所有装备基础数据
void UEquipmentBaseStruct::GetAllEquipments(TArray<FEquipmentBase>& _Item, bool _Select, EEquipment _Type)
{
	TArray<FEquipmentBase*> LItemBuffer;
	EquipmentTypeFactory::GetAllDatas(LItemBuffer, _Select, _Type);

	for (auto& Data : LItemBuffer)
		_Item.Emplace(*Data);
}

//通过名称获取装备数据
bool UEquipmentBaseStruct::SearchEquipmentFromDataTable(const FString& _EquipmentName, FEquipmentBase& OutputData, bool _bSelect, EEquipment Type)
{
	FEquipmentBase* Data = nullptr;
	if (UEquipmentBaseStruct::SearchSourceEquipmentFromDataTable(_EquipmentName, Data, _bSelect, Type))
	{
		OutputData = *Data;
		return true;
	}

	return false;
}

bool UEquipmentBaseStruct::SearchSourceEquipmentFromDataTable(const FString& _EquipmentName, FEquipmentBase*& OutputData, bool _bSelect, EEquipment Type)
{
	TArray<FEquipmentBase*> LItemBuffer;
	EquipmentTypeFactory::GetAllDatas(LItemBuffer, _bSelect, Type);

	for (auto& Data : LItemBuffer)
		if (_EquipmentName.Equals(Data->ItemName.ToString()))
		{
			OutputData = Data;

			return true;
		}

	return false;
}
