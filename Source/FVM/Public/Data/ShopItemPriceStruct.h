// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Classes/Engine/DataTable.h"
#include "GameSystem/Item/ItemStruct.h"
#include "ShopItemPriceStruct.generated.h"

class UPlayerStructManager;
class UGameConfigManager;
class UFVMGameInstance;

//货币资源支付成功
DECLARE_DELEGATE(FPayFinish);

//券支付成功并且券个数归0
DECLARE_DELEGATE(FTicketPayFinishOver);

//物品类型
UENUM(BlueprintType)
enum class EItemType : uint8 {
	//装备
	E_Equipment UMETA(DisplayName = "Equipment"),
	//卡片
	E_Card UMETA(DisplayName = "Card"),
	//材料
	E_MaterialItem UMETA(DisplayName = "Material_Item"),
	//特殊的支付对象(背包格子用于扩充玩家背包放置个数)
	E_BagGrid UMETA(DisplayName = "BagGrid"),
};

UENUM(BlueprintType)
enum class EItemTipsImageType : uint8 {
	//热卖
	E_Selling UMETA(DisplayName = "Selling"),
	//新品
	E_New UMETA(DisplayName = "New"),
	//半价
	E_HalfPirce UMETA(DisplayName = "HalfPirce"),
	//空
	E_None UMETA(DisplayName = "None")
};

//物品的结构
USTRUCT(BlueprintType)
struct FItemPrice {
	GENERATED_USTRUCT_BODY()
public:
	//物品名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText M_ItemName;
	//简单的提示信息
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText M_ItemTips;
	//物品的描述信息
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText M_ItemDescirbe;
	//物品类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType M_ItemType;
	//物品的价格
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_ItemPrice;
	//货币的类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 M_ItemMoneyType;
	//货币的类型文字
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText M_ItemMoneyTypeName;
	//展示的纹理图片路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftObjectPath M_ItemHeadTexture2DPath;
	//售卖类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemTipsImageType M_ItemTipsImageType = EItemTipsImageType::E_None;
	//该商品是服装类(显示试衣间)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool M_IsPlayerSuit = false;
	//是否显示
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool M_IsShow = true;
};

//券兑换
USTRUCT(BlueprintType)
struct FItemTicketPrice {
	GENERATED_USTRUCT_BODY()
public:
	//物品名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText M_ItemName;
	//兑换的物品类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType M_ItemType;
	//物品获得数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_ItemCount = 1;
	//物品的纹理图片路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftObjectPath M_ItemHeadTexture2DPath;
	//券图标的纹理图片路径(""表示没有图片显示)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftObjectPath M_TicketHeadTexture2DPath = FSoftObjectPath(TEXT("Texture2D'/Game/Resource/Texture/UI/Game/TicketShop/31.31'"));
	//物品的描述信息
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText M_ItemDescirbe;
	//券兑换名称（自动搜寻玩家背包对应道具）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText M_TicketName;
	//券数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_TicketCount = 5;
	//是否显示
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool M_IsShow = true;
};

//商店券物品的价格
USTRUCT(BlueprintType)
struct FItem_TicketPrice_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//物品结构
	UPROPERTY(EditDefaultsOnly)
	FItemTicketPrice M_FItemPrice;
};

//商店物品的价格
USTRUCT(BlueprintType)
struct FItem_Price_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//物品结构
	UPROPERTY(EditDefaultsOnly)
	FItemPrice M_FItemPrice;
};

UCLASS()
class FVM_API UShopItemPriceStruct : public UObject
{
	GENERATED_BODY()
public:
	static void AddToPlayerBag(UPlayerStructManager* _Player, const FItemPrice& _ItemData, int32 _Count);

	//开启背包界面第几个界面的第几个背包
	static void AddPlayerBagGrid(UPlayerStructManager* _Player, const FItemPrice& _ItemData, int32 _Panel_Index);
private:
	//添加卡片到背包
	static void AddCardToPlayerBag(UPlayerStructManager* _Player, const FItemPrice& _ItemData, int32 _Count);
	//添加装备到背包
	static void AddEquipmentToPlayerBag(UPlayerStructManager* _Player, const FItemPrice& _ItemData, int32 _Count);
	//添加材料到背包
	static void AddMaterialToPlayerBag(UPlayerStructManager* _Player, const FItemPrice& _ItemData, int32 _Count);
};
