// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSystem/Item/Equipment/EquipmentBaseStruct.h"
#include "PlayerEquipmentStruct.generated.h"


//---------------------------------------------------------------------------------------------------------
// 
//    时装结构开始
//---------------------------------------------------------------------------------------------------------
//玩家时装的装备类型的性别区分
UENUM(BlueprintType)
enum class EPlayerEquipmentType : uint8
{
	//玩家服装(区分套装性别在于后缀的_女)
	E_Men UMETA(DisplayName = "Men"),
	//玩家服装(区分套装性别在于后缀的_男)
	E_Man UMETA(DisplayName = "Man"),
	//玩家服装(区分套装性别在于后缀的_不区分)
	E_Base UMETA(DisplayName = "Base")
};

//玩家时装的对应的插槽位置
UENUM(BlueprintType)
enum class EPlayerEquipmentSlotPosition : uint8
{
	//翅膀
	E_Player_FlyItem UMETA(DisplayName = "FlyItem"),
	//头发
	E_Player_Hair UMETA(DisplayName = "Hair"),
	//着装
	E_Player_Body UMETA(DisplayName = "Body"),
	//头部-脸(无面小白脸)
	E_Player_Head UMETA(DisplayName = "Head"),
	//眼睛
	E_Player_Eye UMETA(DisplayName = "Eye"),
	//脸饰品
	E_Player_Face UMETA(DisplayName = "Face"),
	//眼镜
	E_Player_Glasses UMETA(DisplayName = "Glasses"),
	//帽子
	E_Player_Cap UMETA(DisplayName = "Cap"),
	//套装
	E_Player_Suit UMETA(DisplayName = "Suit"),
};

//道具->玩家装备(时装)
USTRUCT(BlueprintType)
struct FPlayerEquipment : public FEquipmentBase {
	GENERATED_USTRUCT_BODY()
public:
	//服装性别
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EPlayerEquipmentType M_PlayerEquipmentType = EPlayerEquipmentType::E_Man;
	//服装类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EPlayerEquipmentSlotPosition M_EPlayerEquipmentSlotPosition = EPlayerEquipmentSlotPosition::E_Player_Suit;
	//隐藏翅膀功能(当服装类型为Player_Suit时才会生效)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_bHiddenFlyItemByPlayer_Suit = true;
	//服装路径->外形——1-前
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath M_PlayerEq_Path_First;
	//服装路径->外形——2-后
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath M_PlayerEq_Path_Second;
	//服装动画路径1默认动画->不需要结尾_C_Flipbook
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath M_PlayerEq_Animaion_Path_First;
	//服装动画路径2默认动画->不需要结尾_C_Flipbook
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath M_PlayerEq_Animaion_Path_Second;
	//服装动画路径1攻击动画->不需要结尾_C_Flipbook
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath M_PlayerEq_Animaion_Path_First_Attack;
	//服装动画路径2攻击动画->不需要结尾_C_Flipbook
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath M_PlayerEq_Animaion_Path_Second_Attack;
	//服装数值->基础体力加成值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_PlayerEq_HP_Value = 10.f;
	//服装魅力数值->服装提供魅力加成值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_PlayerEq_Charm_Value = 30;
	//坐标偏移
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D M_PlayerEq_Offset = FVector2D(0.f, 0.f);
};

//---------------------------------------------------------------------------------------------------------
// 
//    时装结构结束
//---------------------------------------------------------------------------------------------------------

//装备-玩家服装数据
USTRUCT(BlueprintType)
struct FEquipment_PlayerEquipment_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//物品结构
	UPROPERTY(EditDefaultsOnly)
		FPlayerEquipment M_FEquipment;
};


UCLASS()
class FVM_API UPlayerEquipmentStruct : public UObject
{
	GENERATED_BODY()
public:
	//通过装备类型搜索角色服装原始数据
	static bool SearchPlayerSuitByEquipment(FString& _ItemName, EPlayerEquipmentType _EPlayerEquipmentType, FPlayerEquipment& OutData);
};
