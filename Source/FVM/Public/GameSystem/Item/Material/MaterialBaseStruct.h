// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSystem/Item/ItemBaseStruct.h"
#include "MaterialBaseStruct.generated.h"

UENUM(BlueprintType)
enum class EMaterialType : uint8//[当你添加新材料时，记得将Max改为最新的材料，最后去填写数据表路径]
{
	//卡片合成配方
	E_Blueprint UMETA(DisplayName = "Blueprint"),
	//卡片配方材料
	E_CardSynthesisMaterial UMETA(DisplayName = "CardSynthesisMaterial"),
	//卡片转职材料
	E_CardChangeJobMaterial UMETA(DisplayName = "CardChangeJobMaterial"),
	//香料
	E_Spices UMETA(DisplayName = "Spices"),
	//四叶草
	E_Clover UMETA(DisplayName = "Clover"),
	//防御卡技能书
	E_CardSkillBook UMETA(DisplayName = "CardSkillBook"),
	//兑换券
	E_Ticket UMETA(DisplayName = "Ticket"),
	//强化水晶
	E_Crystal UMETA(DisplayName = "Crystal"),
	//武器开槽钻头
	E_Bit UMETA(DisplayName = "Bit"),
	//关卡钥匙
	E_LevelKey UMETA(DisplayName = "LevelKey"),
	//最大值
	E_Max = E_LevelKey + 1u
};

//道具->材料->底层
USTRUCT(BlueprintType)
struct FMaterialBase : public FItemBase {
	GENERATED_USTRUCT_BODY()
public:
	//数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_Count = 0;
	//材料类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMaterialType M_MaterialType = EMaterialType::E_Blueprint;
};

UCLASS()
class FVM_API UMaterialBaseStruct : public UObject
{
	GENERATED_BODY()
public:
	//源数据获取
	static void GetSourceData(TArray<FMaterialBase*>& _Data, const EMaterialType& _Type);
	//返回所有材料数据
	static void GetAllMaterial(TArray<FMaterialBase>& _Item, int32 _UniformCount = 1);
	//返回所有材料数据
	static void GetAllMaterial(TArray<FItemBase>& _Item, int32 _UniformCount = 1);
public:
	//通过名称获取材料道具
	UFUNCTION(BlueprintCallable)
	static bool SearchMaterailFromDataTable(
		const FString& _MaterailName,
		FMaterialBase& OutputData,
		bool _SelectType = false,
		EMaterialType _Material = EMaterialType::E_Blueprint
	);
	UFUNCTION(BlueprintCallable)
	static bool SearchMaterailFromDataTableByID(
		int32 _MaterailID,
		FMaterialBase& OutputData,
		bool _SelectType = false,
		EMaterialType _Material = EMaterialType::E_Blueprint
	);
public:
	/*-----------------------------------------------材料-----------------------------------------------*/
	/*-----------------------------------------------材料-----------------------------------------------*/

	//创建一个新的材料数据
	static FMaterialBase CreateNewMaterial(const FString& _ItemName, EMaterialType _Type)
	{
		FMaterialBase Item;
		UMaterialBaseStruct::SearchMaterailFromDataTable(_ItemName, Item, true, _Type);
		return Item;
	}

	//获取指定材料通过数组【在限定的(基于材料结构)数组里查询名称，匹配返回】
	template <class TargetType>
	void static GetMaterialArraysData(const FString& _Names, TArray<TargetType>& _Arrays, TargetType& _OutputData, bool& _Result)
	{
		//获取物品数据
		for (auto Items = _Arrays.CreateConstIterator(); Items; ++Items)
			if (_Names.Equals((*Items).ItemName.ToString()))
			{
				_OutputData = (*Items);
				_Result = true;
				break;
			}
	}

	//获取指定材料通过数组【在限定的(基于材料结构)数组里查询名称，匹配返回】
	template <class TargetType>
	void static GetMaterialArraysDataID(int32 _Names, TArray<TargetType>& _Arrays, TargetType& _OutputData, bool& _Result)
	{
		for (const FMaterialBase& Data : _Arrays)
		{
			if (_Names == Data.M_ItemID)
			{
				_OutputData = Data;
				_Result = true;
				break;
			}
		}

		////获取物品数据
		//for (auto Items = _Arrays.CreateConstIterator(); Items; ++Items)
		//{
		//	FString ItemName = (*Items).ItemName.ToString();
		//	FString ItemID = FString::FromInt((*Items).M_ItemID);
		//	/*if (_Names == ItemID)
		//	{
		//		_OutputData = (*Items);
		//		_Result = true;
		//		break;
		//	}*/


		//}
	}

	//根据类型返回数据表数据【指定一个(基于材料基础)的结构类型->根据材料的类型返回对应类型的所有数据】
	template<class MaterialsType = FMaterialBase>
	static void GetMaterialDataTableDatas(EMaterialType _Material, TArray<MaterialsType>& _OutMaterials)
	{
		//被转换的原始数据
		TArray<FMaterialBase*> LocalTempDatas;
		//获取源数据
		UMaterialBaseStruct::GetSourceData(LocalTempDatas, _Material);
		//数据转换
		for (auto Data : LocalTempDatas)
		{
			MaterialsType* TempTrans = (MaterialsType*)Data;
			_OutMaterials.Emplace(*TempTrans);
		}
	}

	//获取指定数据的原结构
	template <class MaterailType = FMaterialBase>
	static bool GetMaterialSourceData(const FString& _Name, MaterailType& _Clover, const EMaterialType& _Type)
	{
		bool _Result;
		TArray<MaterailType> Items;
		UMaterialBaseStruct::GetMaterialDataTableDatas<MaterailType>(_Type, Items);
		UMaterialBaseStruct::GetMaterialArraysData<MaterailType>(_Name, Items, _Clover, _Result);
		return _Result;
	}
	/*-----------------------------------------------材料-----------------------------------------------*/
};
