// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/AssetManager.h"
#include "UObject/NoExportTypes.h"
#include "GameStart/VS/MapBaseType.h"
#include "Engine/StreamableManager.h"
#include "GameSystem/GameDataSubsystem.h"
#include "GameSystem/DataTableAssetData.h"
#include "Engine/Classes/Engine/DataTable.h"
#include "ItemBaseStruct.generated.h"

// this->M_UUI_PlayerInformationShow->SetPlayerSuitConst();


//---------------------------------------------------------------------------------------------------------
// 
//    射线结构开始->射线用于一些卡片-武器->来检测老鼠
//---------------------------------------------------------------------------------------------------------

//射线位置
UENUM(BlueprintType)
enum class ELineTracePosition : uint8 {
	//包括当前网格，从左边开始直到末尾
	E_Left  UMETA(DisplayName = "Left"),
	//包括当前网格，从右边开始直到末尾
	E_Right UMETA(DisplayName = "Right"),
	//检测当前左右两边
	E_LeftRight_M UMETA(DisplayName = "LeftRight_M"),
	//包括当前网格，从上边开始直到末尾
	E_Top UMETA(DisplayName = "Top"),
	//包括当前网格，从下边开始直到末尾
	E_Bottom UMETA(DisplayName = "Bottom"),
	//检测当前上下两边
	E_TopBottom_M UMETA(DisplayName = "TopBottom_M")
};

//射线检测类型
UENUM(BlueprintType)
enum class ELineTraceType : uint8 {
	//在格子走的老鼠
	E_MouseGround  UMETA(DisplayName = "MouseGround"),
	//在空中漂浮的老鼠
	E_MouseSky  UMETA(DisplayName = "MouseSky"),
	//陆地和空中
	E_MouseGroundAndSky  UMETA(DisplayName = "MouseGroundAndSky"),
	//在地下的老鼠
	E_MouseUnder  UMETA(DisplayName = "MouseUnder"),
	//在水上的老鼠
	E_MouseWater  UMETA(DisplayName = "MouseWater"),
	//任何类型
	E_MouseVisibility  UMETA(DisplayName = "MouseVisibility"),
};

//游戏属性分类【能量，雷系，水系，火系......】
UENUM(BlueprintType)
enum class EGamePropertyCategory : uint8 {
	//能量系
	Energy  UMETA(DisplayName = "Energy"),
	//近战
	Melee  UMETA(DisplayName = "Melee"),
	//防御
	Defense  UMETA(DisplayName = "Defense"),
	//雷系
	Thunder  UMETA(DisplayName = "Thunder"),
	//火系
	Fire  UMETA(DisplayName = "Fire"),
	//冰系
	Ice  UMETA(DisplayName = "Ice"),
	//超弦系
	Chord  UMETA(DisplayName = "Chord"),
	//魔法系
	Mage UMETA(DisplayName = "Mage"),
	//支持系
	Support UMETA(DisplayName = "Support")
};

//检测范围结构设置
USTRUCT(BlueprintType)
struct FLineCheckSetting {
	GENERATED_USTRUCT_BODY()
public:
	//检测类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELineTraceType M_ELineTraceType = ELineTraceType::E_MouseGround;
	//检测发射位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELineTracePosition M_ELineTracePosition = ELineTracePosition::E_Right;
	//检测长度[-1表示不限制长度]
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CheckLength = -1;
	//检测偏移[如果是左右检测(行偏移)如果是上下检测(列偏移)]
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CheckOffset = 0;
};

//射线偏移结构设置
USTRUCT(BlueprintType)
struct FLineTraceSetting {
	GENERATED_USTRUCT_BODY()
public:
	//开始位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector M_BeginLocation = FVector();
	//结束位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector M_EndLocation = FVector();
	//设置位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELineTracePosition M_ELineTracePosition = ELineTracePosition::E_Right;
	//开始固定格子长度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool M_bEnableSettingGridCount = false;
	//最长几个格子
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_GridCount = 1;
	//射线最终的偏移位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector M_LineTraceOffset = FVector();
	//检测类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELineTraceType M_ELineTraceType = ELineTraceType::E_MouseGround;
};

//---------------------------------------------------------------------------------------------------------
// 
//    射线结构结束
//---------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------
// 
//    技能书结构开始
//---------------------------------------------------------------------------------------------------------

UENUM(BlueprintType)
enum class ESkillBookTipCategory : uint8 {
	//普通卡
	S_Normal UMETA(DisplayName = "Normal"),
	//纪念卡
	S_Style_1 UMETA(DisplayName = "Style_1"),
	//星座卡
	S_Style_2 UMETA(DisplayName = "Style_2"),
	//金卡
	S_Style_3 UMETA(DisplayName = "Style_3"),
};

//技能书数据结构(用于加载展示的结构)
USTRUCT(BlueprintType)
struct FSkillBookData {
	GENERATED_USTRUCT_BODY()
public:
	//技能书名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_Name = FString(TEXT("小笼包"));
	//提示信息
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_Tip = FString(TEXT("提升攻速"));
	//转职卡片名称(同样可以使用当前技能书)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> M_ChangeCardNames;
	//技能书头像名称-每一个等级对应一个名称(路径由程序自动完善)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> M_HeadNames;
	//技能书等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_LV = 0;
	//技能书当前经验值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_CurrentEx = 0;
	//技能书当前需要的总经验值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_CurrentTotalEx = 200;
	//技能书下一个等级需要的加成经验值比例
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_NextAddTotalEx = 1.5f;
	//是否满级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool M_IsMax = false;
	//技能书分类
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillBookTipCategory M_TabCategory = ESkillBookTipCategory::S_Normal;
};


//---------------------------------------------------------------------------------------------------------
// 
//    技能书结构开始
//---------------------------------------------------------------------------------------------------------

//角色版本补偿
USTRUCT(BlueprintType)
struct FPlayerVersionCompensate
{
	GENERATED_USTRUCT_BODY()
public:
	//发送状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool M_SendState = false;
	//发送的邮件名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_SendMailName = TEXT("");
};


//邮件结构
USTRUCT(BlueprintType)
struct FMail {
	GENERATED_USTRUCT_BODY()
public:
	//邮件发送人
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_MailSendPlayer;
	//邮件标题(输入的指令)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_MailTitle;
	//邮件主题(显示的标题)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_MailTheme;
	//邮件内容
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_MailContent;
	//邮件发送道具名称(对应道具名称)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_MailSendItemName;
	//道具数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_MailItemCount = 1;
	//邮件状态(0未读，1未领取，2以领取)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_MailState = 0;
	//邮件类型（true 系统  false付费）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool M_MailType = true;
	//金额
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_MailPayNum = 10;
	//付费类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_MailPayType = 0;
	//付费的类型名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_MailPayTypeName = TEXT("金币");
	//ID个数（保证邮件的唯一）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_Mail_ID = 0;
};

USTRUCT(BlueprintType)
struct FItemBase {
	GENERATED_USTRUCT_BODY()
public:
	FItemBase() {}
	FItemBase(const FText& _Name) :ItemName(_Name) {}
	FItemBase(const FText& _Name, const FText& _IDs, const FString _TPath) : ItemName(_Name), ItemDescrible(_IDs), ItemTexturePath(_TPath) {}
public:
	//道具名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemName;
	//道具描述
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemDescrible;
	//显示的纹理路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftObjectPath ItemTexturePath = FSoftObjectPath(FString(TEXT("Texture2D'/Game/Resource/Texture/Sprite/VS/Sprite/Card/CardBg/TCard_0.TCard_0'")));
	//是否被认为是等待删除的对象
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bWaitRemove = false;
	//物品ID号(可以不用设置)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_ItemID = -1;
	//背包ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BagID;
};

//Key,Value资源对象
USTRUCT(BlueprintType)
struct FItemResourceBase {
	GENERATED_USTRUCT_BODY()
public:
	//资源
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, FSoftObjectPath> Resources;
};

//Key,Value资源对象
USTRUCT(BlueprintType)
struct FItemResourceData : public FTableRowBase {
	GENERATED_USTRUCT_BODY()
public:
	//资源
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemResourceBase Resource;
};

//物品结构
USTRUCT(BlueprintType)
struct FItemBaseStructData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	virtual bool GetSourceData(FItemBase& OutData)
	{
		return false;
	}
};

//获取数据表原生数据
template<typename Type>
TArray<Type>& GetDataTableSourceData(
	TArray<Type>& InData,
	DataTableAssetData<Type>& InTable,
	FName DataName,
	FName Name
) {
	if (InData.Num())
	{
		return InData;
	}
	else {
		InTable.InitDataTable(
			UGameDataSubsystem::GetGameDataSubsystemStatic()->GetAsset()->
			GetDataByName(DataName)->GetDataByName(Name)
		);

		InTable.GetValues(InData);

		InTable.InitDataTable(nullptr);
	}

	return InData;
}


class UGameDataAssetCache;
UCLASS()
class FVM_API UItemBaseDataAssetCache : public  UGameDataAssetCache
{
	GENERATED_BODY()
private:
	//纹理资源
	UPROPERTY()
	TArray<FItemResourceData> TextureDatas;
	UPROPERTY()
	TSet<int32> TextureRanges;
private:
	//数据表加载器
	DataTableAssetData<FItemResourceData> SourceDatas;
	void GetResource(int32 ID, TSet<int32>& Ranges, TArray<FItemResourceData>& Datas, FItemResourceBase& Data);
public:
	//纹理资源
	UFUNCTION()
	void GetTextureResource(int32 ID, FItemResourceBase& Data);
};

UCLASS()
class FVM_API UItemBaseStruct : public UObject
{
	GENERATED_BODY()
public:
	//全局静态流加载实例句柄
	//static TSharedPtr<FStreamableHandle> M_Globle_StreamableHandle;
	//全局静态物品基础对象
	//static UItemBaseStruct* M_Globle_UItemBaseStruct_Ptr;


	//获取资源信息
	UFUNCTION(BlueprintPure)
	static bool GetTextureResource(int32 ID, FSoftObjectPath& SoftObjectPath);

public:
	//资源加载
	//UFUNCTION()
	//	void ResourceLoad();
public:
	//获取数据项【根据一个数组(基于FItemBase)进行名称匹配，成功返回ref】
	template<class Type>
	static Type* GetMetaByName(TArray<Type>& _Value, const FString& _Name)
	{
		for (auto& _Item : _Value)
			if (_Item.ItemName.ToString().Equals(_Name))
			{
				return &_Item;
			}

		return nullptr;
	}

	//从背包中寻找对应装备
	template <class ItemType>
	static inline ItemType* GetItemFromBag(TArray<ItemType>& _Player, const FString& _ItemName)
	{
		return UItemBaseStruct::GetMetaByName<ItemType>(_Player, _ItemName);
	}
};
