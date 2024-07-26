// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "GameSystem/Item/ItemStruct.h"
#include "GameSystem/GlobalDatas.h"

#include "PlayerStructBase.generated.h"

 //地图（结构关联（GameSystemFunction：LoadMap；SetMap））
UENUM(BlueprintType)
enum class EGameVsMap : uint8 {
	//美味镇
	VM_Main UMETA(DisplayName = "Main"),
	//美味岛
	VM_Map_1 UMETA(DisplayName = "Map_1"),
	//火山岛
	VM_Map_2 UMETA(DisplayName = "Map_2"),
	//火山遗迹
	VM_Map_3 UMETA(DisplayName = "Map_3"),
	//浮空岛
	VM_Map_4 UMETA(DisplayName = "Map_4"),
	//海底漩涡
	VM_Map_5 UMETA(DisplayName = "Map_5"),
};

//角色进入的地图
USTRUCT(BlueprintType)
struct FPlayerInMap {
	GENERATED_USTRUCT_BODY()
public:
	//当前玩家进入的地图
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGameVsMap CurrentInMap = EGameVsMap::VM_Main;

	//当前玩家上次进入的地图
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGameVsMap LastInMap = EGameVsMap::VM_Main;

	//当前正在进入的地图UI名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CurrentInMapUIName = TEXT("");
};

USTRUCT(BlueprintType)
struct FPlayerEx {
	GENERATED_USTRUCT_BODY()
public:
	//当前经验值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_CurrentEx = 0.f;
	//当前经验值顶峰
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_CurrentExTop = 90.f;
	//经验值成长
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_ExUp = 10.f;
	//经验值成长率
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_ExUpRate = 0.2f;
	//最大经验值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_ExMax = 10000000.f;
	//角色等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_PlayerGrade = 1;
};

USTRUCT(BlueprintType)
struct FPlayerVIP
{
	GENERATED_USTRUCT_BODY()
public:
	//当前VIP值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_VIP = 0;
	//最大VIP
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 M_MaxVIP = 12;
	//当前进入VIP经验值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_CurrentEx = 0.f;
	//当前VIP最大经验值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_CurrentExTop = 100.f;
	//VIP成长值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_VIP_Up = 2.f;
	//VIP成长率
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_VIP_UpRate = 0.4f;
};

USTRUCT(BlueprintType)
struct FPlayerSign
{
	GENERATED_USTRUCT_BODY()
public:
	//角色等级称号
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_PlayerFlag;
	//角色加入的工会
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_TradeUnionName = TEXT("未加入工会");
	//成就名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_AchievementName = TEXT("");
};

USTRUCT(BlueprintType)
struct FDialogueShowInfor {
	GENERATED_BODY()
public:
	//剧情标题
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DialogueTitle;
	//剧情目标位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DialogueTarget;
};

//角色货币(存储信息)
USTRUCT(BlueprintType)
struct FPlayerCoin {
	GENERATED_USTRUCT_BODY()
public:
	//货币分等级
	//金币
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_Coin_0 = 2000;
	//礼券
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_Coin_1 = 600;
	//点券
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_Coin_2 = 0;
	//威望
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_Coin_3 = 0;
public:
	int32& GetCoinRef(uint8 _Index)
	{
		//添加货币
		switch (_Index)
		{
		case 0u:return this->M_Coin_0;
		case 1u:return this->M_Coin_1;
		case 2u:return this->M_Coin_2;
		case 3u:return this->M_Coin_3;
		}

		return this->M_Coin_0;
	}
};

//角色背包的格子
USTRUCT(BlueprintType)
struct FPlayerBagGird
{
	GENERATED_USTRUCT_BODY()
public:
	//背景图片
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_GridImagePath;
	//容量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_GridCount;
	//第几个界面
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_PanelNumber;
	//第几个背包
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_BagNumber;
	//Vector名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_VectorName;
	//道具名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_BagName;
};


//角色背包的格子组合(武器 防御卡 材料)
USTRUCT(BlueprintType)
struct FPlayerBagGirdGroup
{
	GENERATED_USTRUCT_BODY()
public:
	//默认容量150
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 M_DefGridCount = 150;
	//装备背包格子
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPlayerBagGird> M_EquipmentGrid;
	//卡片背包格子
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPlayerBagGird> M_CardGrid;
	//材料背包格子
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPlayerBagGird> M_MaterialGrid;
private:
	int32 GetBagCount(TArray<FPlayerBagGird>& _Arrays)
	{
		int32 _LCount = 150;
		for (FPlayerBagGird& _bag : _Arrays)
		{
			_LCount += _bag.M_GridCount;
		}
		return _LCount;
	}
public:

	//获取背包容量（0表示获取装备背包 1表示卡片背包 2表示材料背包）
	int32 GetBagCount(uint8 _Index)
	{
		switch (_Index)
		{
			//卡片
		case 1:return GetBagCount(this->M_CardGrid);
			//材料
		case 2:return GetBagCount(this->M_MaterialGrid);
			//装备
		default:return GetBagCount(this->M_EquipmentGrid);
		}
	}

	//获取角色背包
	TArray<FPlayerBagGird*> GetUseBagArrays()
	{
		TArray<FPlayerBagGird*> Items;

		for (FPlayerBagGird& Equipment : this->M_EquipmentGrid)
			if (!Equipment.M_VectorName.Equals(""))
				Items.Emplace(&Equipment);

		for (FPlayerBagGird& Card : this->M_CardGrid)
			if (!Card.M_VectorName.Equals(""))
				Items.Emplace(&Card);

		for (FPlayerBagGird& Material : this->M_MaterialGrid)
			if (!Material.M_VectorName.Equals(""))
				Items.Emplace(&Material);

		return Items;
	}

	static void ClearData(FPlayerBagGird& _Grid)
	{
		//背景图片
		_Grid.M_GridImagePath = "";
		//容量
		_Grid.M_GridCount = 0;
		//第几个界面
		_Grid.M_PanelNumber = -1;
		//第几个背包
		_Grid.M_BagNumber = -1;
		//Vector名称
		_Grid.M_VectorName = "";
		//道具名称
		_Grid.M_BagName = "";
	}
};

USTRUCT(BlueprintType)
struct FPlayerWepaon
{
	GENERATED_USTRUCT_BODY()
public:
	//当前是否有武器装备
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool M_bUse = false;
	//武器名称
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString M_WeaponName = TEXT("");
	//武器头像路径
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString M_WeaponHeadPath = TEXT("");
	//武器资源路径
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString M_WeaponResource_C_Path = TEXT("");
	//武器ID号
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 M_WeaponID = 0;
};

//角色武器
USTRUCT(BlueprintType)
struct FPlayerWepaons
{
	GENERATED_USTRUCT_BODY()
public:
	//主武器
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPlayerWepaon M_PlayerFirstWeapon;
	//副武器
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPlayerWepaon M_PlayerSecondWeapon;
	//超级武器
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPlayerWepaon M_PlayerSuperWeapon;
};

//服装道具
USTRUCT(BlueprintType)
struct FPlayerSuitItem
{
	GENERATED_USTRUCT_BODY()
public:
	//当前是否有服装装备
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool M_bUse = false;
	//服装名称
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString M_SuitName = TEXT("");
	//服装头像路径
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSoftObjectPath M_SuitHeadPath;
	//服装展示图片路径1
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSoftObjectPath M_SuitFirstShowPath;
	//服装展示图片路径2
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSoftObjectPath M_SuitSecondShowPath;
	//服装默认动画1
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSoftObjectPath M_SuitFirstDefAnimPath;
	//服装默认动画2
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSoftObjectPath M_SuitSecondDefAnim2Path;
	//服装攻击动画1
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSoftObjectPath M_SuitFirstAttackAnimPath;
	//服装攻击动画2
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSoftObjectPath M_SuitSecondAttackAnim2Path;
	//服装体力加成
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float M_Suit_HP_Value = 10.f;
	//服装魅力加成
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 M_Suit_Charm_Value = 10;
	//坐标偏移
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D M_PlayerEq_Offset = FVector2D(0.f, 0.f);
	//服装ID号
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 M_ItemID = -1;
public:
	void CopyValue(const FPlayerEquipment& _Value, bool _b_Use)
	{
		this->M_bUse = _b_Use;
		this->M_Suit_Charm_Value = _Value.M_PlayerEq_Charm_Value;
		this->M_Suit_HP_Value = _Value.M_PlayerEq_HP_Value;
		this->M_SuitFirstShowPath = _Value.M_PlayerEq_Path_First;
		this->M_SuitSecondShowPath = _Value.M_PlayerEq_Path_Second;
		this->M_SuitName = _Value.ItemName;
		this->M_SuitHeadPath = _Value.ItemTexturePath;

		this->M_SuitFirstDefAnimPath = _Value.M_PlayerEq_Animaion_Path_First;
		this->M_SuitSecondDefAnim2Path = _Value.M_PlayerEq_Animaion_Path_Second;
		this->M_SuitFirstAttackAnimPath = _Value.M_PlayerEq_Animaion_Path_First_Attack;
		this->M_SuitSecondAttackAnim2Path = _Value.M_PlayerEq_Animaion_Path_Second_Attack;

		this->M_PlayerEq_Offset = _Value.M_PlayerEq_Offset;

		this->M_ItemID = _Value.M_ItemID;
	}
};

//角色服装
USTRUCT(BlueprintType)
struct FPlayerSuit
{
	GENERATED_USTRUCT_BODY()
public:
	//是否装备了套装（没有装备套装显示基础服装，装备套装隐藏基础服装的显示）
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool M_bPlayerSuit = false;
	//隐藏翅膀功能
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool M_HiddenFlyItem = false;
	//使用选择隐藏角色套装显示
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool M_ShowPlayerSuit = true;
public:
	//套装
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPlayerSuitItem M_Suit;
public:
	//身体
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPlayerSuitItem M_Body;
	//头
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPlayerSuitItem M_Head;
	//头发
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPlayerSuitItem M_Hair;
	//脸饰
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPlayerSuitItem M_Face;
	//翅膀
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPlayerSuitItem M_FlyItem;
	//眼镜
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPlayerSuitItem M_Glasses;
	//眼睛
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPlayerSuitItem M_Eye;
	//帽子
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPlayerSuitItem M_Cap;
public:
	//角色武器
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPlayerWepaons M_PlayerWeapons;
};

//游戏角色结构
USTRUCT(BlueprintType)
struct FGamePlayerData : public FTableRowBase {
	GENERATED_USTRUCT_BODY()
public:
	//角色名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PlayerName;
	//角色服装数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayerEquipment Suit;
	//角色主武器名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PlayerFirstWeaponName;
};

//关卡结算信息
USTRUCT(BlueprintType)
struct FPlayerLevelGrade
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Time = 9999;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Score = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString ScoreLevel = TEXT("F");
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 BadCardCount = 9999;
public:
	//初始化
	void Init(int32 _Time, int32 _Score, int32 _BadCardCount)
	{
		if (this->Time > _Time)
		{
			this->Time = _Time;
		}

		if (this->Score < _Score)
		{
			this->Score = _Score;
		}

		if (this->BadCardCount > _BadCardCount)
		{
			this->BadCardCount = _BadCardCount;
		}
	}

	//初始化分数等级
	FString InitScoreLevel(int32 _BadCardCount)
	{
		FString Level = TEXT("");

		if (_BadCardCount >= 0 && _BadCardCount < 10)
		{
			Level = TEXT("SSS");
		}
		else if (_BadCardCount >= 10 && _BadCardCount < 15)
		{
			Level = TEXT("SS");
		}
		else if (_BadCardCount >= 15 && _BadCardCount < 20)
		{
			Level = TEXT("S");
		}
		else if (_BadCardCount >= 20 && _BadCardCount < 30)
		{
			Level = TEXT("A+");
		}
		else if (_BadCardCount >= 30 && _BadCardCount < 45)
		{
			Level = TEXT("A");
		}
		else if (_BadCardCount >= 45 && _BadCardCount < 50)
		{
			Level = TEXT("B+");
		}
		else if (_BadCardCount >= 50 && _BadCardCount < 65)
		{
			Level = TEXT("B");
		}
		else if (_BadCardCount >= 60 && _BadCardCount < 70)
		{
			Level = TEXT("C");
		}
		else {
			Level = TEXT("E");
		}

		//之前的卡片损坏小于现在的，数据不做修改
		if (this->BadCardCount < _BadCardCount)
		{
			return Level;
		}
		else {
			this->BadCardCount = _BadCardCount;
			this->ScoreLevel = Level;

			return Level;
		}

		return Level;
	}
};

//兑换商城的存储信息【富可敌国】
USTRUCT(BlueprintType)
struct FPlayerExchangeAndSplit
{
	GENERATED_USTRUCT_BODY()

public:
	//星星个数
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 StartCount = 1;
	//加成比例
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AddedRate = 0.f;
	//文字提示
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString TextLogResource;
	//显示版面位置
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D PanelTranlation;
};


UCLASS()
class FVM_API UPlayerStructBase : public UObject
{
	GENERATED_BODY()
};
