// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

// #include"GameSystem/Item/Equipment/EquipmentBaseStruct.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSystem/Item/ItemBaseStruct.h"
#include "EquipmentBaseStruct.generated.h"


UENUM(BlueprintType)
enum class EEquipment : uint8
{
	//背包
	E_Bag UMETA(DisplayName = "Bag"),
	//礼盒
	E_Gift UMETA(DisplayName = "Gift"),
	//角色服装
	E_PlayerEquipment UMETA(DisplayName = "PlayerEquipment"),
	//角色主武器
	E_PlayerWeaponFirst UMETA(DisplayName = "WeaponFirst"),
	//角色副武器
	E_PlayerWeaponSecond UMETA(DisplayName = "WeaponSecond"),
	//角色超级武器
	E_PlayerWeaponSuper UMETA(DisplayName = "WeaponSuper"),
	//武器宝石
	E_WeaponGem UMETA(DisplayName = "WeaponGem"),


	//最大值->(武器宝石)
	E_Max = E_WeaponGem
};

//道具->底层
USTRUCT(BlueprintType)
struct FEquipmentBase : public FItemBase {
	GENERATED_USTRUCT_BODY()
public:
	FEquipmentBase() {};
public:
	//是否被使用
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_Used = false;
	//是否可以道具重叠->如果不能重叠那么一个道具占用一个格子
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_IsOverlap = true;
	//道具数量->基于格子开启了重叠才会生效
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_Count = 0;
	//是否是礼盒模式->如果是礼盒模式可以使用【开启功能】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_IsGift = false;
	//道具类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EEquipment M_EquipmentType = EEquipment::E_Bag;
	//性别->当前道具会根据玩家性别进行->如果性别不一致则不能使用道具(默认女)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 M_PlayerUseSex = 0;
	//道具等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_EquipmentGrade = 0;
};

class UPlayerStructManager;

UCLASS()
class FVM_API UEquipmentBaseStruct : public UObject
{
	GENERATED_BODY()
public:
	//通过角色背包里面的装备获取所有的基础数据
	static void GetAllEquipmentsByPlayer(TArray<FEquipmentBase*>& _Item, bool _Select, const TArray<EEquipment>& _Type);
	//获取所有的原始数据，可以指定获取类型
	static void GetAllEquipmentsSourceData(TArray<FEquipmentBase*>& _Item, bool _Select, const TArray<EEquipment>& _Type);
	//通过名称获取装备原始数据
	static bool SearchSourceEquipmentFromDataTable(const FString& _EquipmentName, FEquipmentBase*& OutputData, bool _bSelect = false, EEquipment Type = EEquipment::E_Bag);
public:
	//获取所有装备基础数据
	UFUNCTION(BlueprintCallable)
		static void GetAllEquipments(TArray<FEquipmentBase>& _Item, bool _Select = false, EEquipment _Type = EEquipment::E_Bag);
	//获取所有装备名称
	UFUNCTION(BlueprintCallable)
		static void GetAllEquipmentName(TArray<FItemBase>& _Item, bool _Select = false, EEquipment _Type = EEquipment::E_Bag);
	//通过名称获取装备数据
	UFUNCTION(BlueprintCallable)
		static bool SearchEquipmentFromDataTable(const FString& _EquipmentName, FEquipmentBase& OutputData, bool _bSelect = false, EEquipment Type = EEquipment::E_Bag);
public:
	/*-----------------------------------------------装备-----------------------------------------------*/
	/*-----------------------------------------------装备-----------------------------------------------*/
	//获取指定装备通过数组
	template <class TargetType>
	void static GetEquipmentArraysData(const FString& _Names, TArray<TargetType>& _Arrays, TargetType& _OutputData, bool& _Result)
	{
		//获取物品数据
		for (TargetType& Items : _Arrays)
			if (_Names.Equals(Items.ItemName.ToString()))
			{
				_OutputData = Items;
				_Result = true;
				return;
			}
	}

	//获取指定装备通过数组
	bool static GetMailArraysData(const FString& _Names, TArray<FMail>& _Arrays, FMail& _OutputData)
	{
		//获取物品数据
		for (FMail& Items : _Arrays)
			if (_Names.Equals(Items.M_MailTitle))
			{
				_OutputData = Items;
				return true;
			}
		return false;
	}

	//获取所有装备【DataTableRowType 数据表结构】
	template <class DataTableRowType, class EquipmentType = FEquipmentBase>
	static void GetEquipmentRowDatas(TArray<DataTableRowType>& _Item_Rows, TArray<EquipmentType>& _OutArrays)
	{
		for (auto& Item : _Item_Rows)
		{
			_OutArrays.Emplace(Item.M_FEquipment);
		}
	}

	//将装备数据结构表转换成数据组
	template<class EquipmentDataStructType, class EquipmentType>
	static void EquipmentDataStructTransform(TArray<EquipmentDataStructType>& Datas, TArray<EquipmentType>& _OutCards)
	{
		for (auto& LData : Datas)
		{
			_OutCards.Emplace((*(EquipmentType*)(&LData.M_FEquipment)));
		}
	}
	/*-----------------------------------------------装备-----------------------------------------------*/
	/*-----------------------------------------------装备-----------------------------------------------*/
};
