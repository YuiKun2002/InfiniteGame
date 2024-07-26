// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSystem/Item/Equipment/EquipmentBaseStruct.h"
#include "GiftBoxStruct.generated.h"


//---------------------------------------------------------------------------------------------------------
// 
//    礼盒结构开始
//---------------------------------------------------------------------------------------------------------

UENUM(BlueprintType)
enum class EGiftBox_Item_Type : uint8
{
	E_Card UMETA(DisplayName = "Card"),
	E_Equipment UMETA(DisplayName = "Equipment"),
	E_Material UMETA(DisplayName = "Material"),
	E_Coin UMETA(DisplayName = "Coin"),
};


//礼盒->随机范围区间
USTRUCT(BlueprintType)
struct FGiftBox_RandomRange {
	GENERATED_USTRUCT_BODY()
public:
	//礼盒最小(该数字只会被礼盒随机模式影响)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_Min = 0;
	//礼盒最大(该数字只会被礼盒随机模式影响)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_Max = 20;
};

//礼盒->随机范围区间
USTRUCT(BlueprintType)
struct FGiftBox_RandomModeDetail {
	GENERATED_USTRUCT_BODY()
public:
	//最大随机数(开启了随机模式-> 100表示 整个范围的最大值)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_RandomMaxNumber = 100;
	//最大获取个数(开启了随机模式-> 1 表示随机从一下道具随机抽取一个)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_GetGiftMaxCount = 1;
};

//礼盒具体奖励设置
USTRUCT(BlueprintType)
struct FGiftBox_Item {
	GENERATED_USTRUCT_BODY()
public:
	//物品名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_ItemName = FString();
	//物品类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EGiftBox_Item_Type M_ItemType = EGiftBox_Item_Type::E_Card;
	//选择当前类型全部(为true时,当前类型的道具全部都会获取,但是不包括礼盒)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_SelectCurrentTypeAll = false;
	//货币类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_CoinType = 0;
	//卡片等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_CardGrade = 0;
	//物品的数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_ItemCount = 1;
	//礼盒数字(该数字只会被礼盒随机模式影响)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGiftBox_RandomRange M_GiftBox_RandomRange;
};

//道具->礼盒
USTRUCT(BlueprintType)
struct FGiftBox : public FEquipmentBase {
	GENERATED_USTRUCT_BODY()
public:
	//开启随机模式
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_bEnableRandomMode = false;
	//必须开启了随机模式->才会生效
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGiftBox_RandomModeDetail M_FGiftBox_RandomModeDetail;
	//礼盒奖励内容
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FGiftBox_Item> M_GiftContent;
};


//---------------------------------------------------------------------------------------------------------
// 
//    礼盒结构结束
//---------------------------------------------------------------------------------------------------------

//装备-礼盒数据
USTRUCT(BlueprintType)
struct FEquipment_GiftBox_Data : public FItemBaseStructData
{
	GENERATED_USTRUCT_BODY();
public:
	//物品结构
	UPROPERTY(EditDefaultsOnly)
		FGiftBox M_FEquipment;
public:
	virtual bool GetSourceData(FItemBase& OutData) override {
		OutData = M_FEquipment;
		return true;
	}
};

UCLASS()
class FVM_API UGiftBoxStruct : public UObject
{
	GENERATED_BODY()
public:
	static const FString& DataTablePath;
};
