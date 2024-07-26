// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSystem/Item/Equipment/EquipmentBaseStruct.h"
#include "GemBaseStruct.generated.h"

//---------------------------------------------------------------------------------------------------------
// 
//    宝石结构开始
//---------------------------------------------------------------------------------------------------------


//武器分类
UENUM(BlueprintType)
enum class EPlayerWeaponType : uint8
{
	//主武器
	E_First UMETA(DisplayName = "First"),
	//副武器
	E_Second UMETA(DisplayName = "Second"),
	//超级武器
	E_Super UMETA(DisplayName = "Super")
};


//宝石类型分类
UENUM(BlueprintType)
enum class EWeaponGemType : uint8
{
	//主动技能释放宝石
	E_GemFirst UMETA(DisplayName = "First"),
	//被动技能触发宝石
	E_GemSecond UMETA(DisplayName = "Second")
};




//道具->宝石装备
USTRUCT(BlueprintType)
struct FWeaponGem : public FEquipmentBase {
	GENERATED_USTRUCT_BODY()
public:
	//当前宝石镶嵌的武器类型-默认可以镶嵌到主武器
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EPlayerWeaponType M_EPlayerWeaponType = EPlayerWeaponType::E_First;
	//宝石触发类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EWeaponGemType M_EWeaponGemType = EWeaponGemType::E_GemFirst;
	//对应镶嵌武器的名称(无则任何武器都可以)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_TargetAttachWepaonName = TEXT("");
	//宝石标记
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_GemTag = TEXT("GemTag");
	//启用计时(启动之后可以重复触发，不启用只触发一次)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_EnableTimer = true;
	//宝石技能触发时间（冷却）[需要启用计时]
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_GemTime = 30.f;
	//宝石等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_GemGrade = 0;
	//宝石等级上限
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_GemGradeMax = 10;
};


UCLASS()
class FVM_API UGemBaseStruct : public UObject
{
	GENERATED_BODY()

};
