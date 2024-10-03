// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameStart/VS/MapBaseType.h"
#include "GameplayTags/Classes/GameplayTagContainer.h"
#include "MouseStructManager.generated.h"

//相机坐标
USTRUCT(BlueprintType)
struct FCameraPosition
{
	GENERATED_USTRUCT_BODY()
public:
	//相机坐标1
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector PlayerPosition = FVector(-90.f, 0.f, 165.f);
};

UENUM(BlueprintType)
enum class EMouseTag : uint8
{
	//正常陆地
	NormalGround UMETA(DisplayName = "Ground"),
	//正常水上
	NormalWater UMETA(DisplayName = "Water"),
	//蝙蝠
	FlyCatch UMETA(DisplayName = "FlyCatch"),
	//史诗
	Epic UMETA(DisplayName = "Epic"),
	//首领
	Boss UMETA(DisplayName = "Boss"),
};

//老鼠结构
USTRUCT(BlueprintType)
struct FMouseBase {
	GENERATED_USTRUCT_BODY()
public:
	//老鼠分类
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag MouseCategory;
	//老鼠标记
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMouseTag M_MouseTag = EMouseTag::NormalGround;
	//老鼠名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_MouseName;
	//老鼠头像
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftObjectPath MouseHead;
	//老鼠Actor资源路径(_C)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftClassPath ItemTarget_ActorFilePath;
	//老鼠材质动画展示路径(材质动画路径)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftObjectPath ItemTarget_AnimPath;
	//老鼠的基础血量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_MouseBaseHP = 8;
	//老鼠的基础攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_MouseBaseATK = 1;
	//老鼠基础速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_MouseBaseSpeed = 15;
	//老鼠经验值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_MouseEx = 1;
	//老鼠线路类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELineType LineType;
	//老鼠的描述
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Des;
};


//生成后的老鼠路线设置
USTRUCT(BlueprintType)
struct FMouseLineSetting {
	GENERATED_USTRUCT_BODY()
public:
	//默认
	FMouseLineSetting() {}
	//初始化
	FMouseLineSetting(const FString& _Name, TArray<int32>& _SpawnLine)
		: M_MouseName(_Name), M_MouseSpawnLine(_SpawnLine) {}
public:
	//老鼠名称(该名称对应FMouseRounds里面的老鼠名称集合)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_MouseName = TEXT("平民鼠");
	//指定在{x}线路生成生成当前老鼠【必须填写】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> M_MouseSpawnLine = { 0,1,2,3,4,5 };
};

//老鼠回合
USTRUCT(BlueprintType)
struct FMouseRound {
	GENERATED_USTRUCT_BODY()
public:
	//当前回合中一共有多少只老鼠
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_MouseMax = 20;
	//最多一次性出现的最大个数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_MouseMaxCount = 2;
	//最多一次性出现的最小个数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_MouseMinCount = 1;
	//当前回合中每默认2s生成最小和最大值的老鼠个数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_SpawnTime = 2.f;
	//当回合开始时的一瞬间生成多少只老鼠
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_RoundBeginSpawnMaxCount = 4;
	//回合生效延迟(默认5s之后回合将立刻生效)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_RoundBeginDelay = 5.f;
	//如果开启当老鼠为0时,才会进行下一波操作
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool M_IsEnableMouseCountZero = true;
	//当关闭老鼠为0时，会默认开启30m之后自动进入下一波
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_NextRoundTime = 30.f;
	//老鼠回合结束的启用延迟
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_RoundEndDelay = 5.f;
	//这个回合是否需要Key才能启动
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Key = false;
	//这个回合是否需要badge（徽章）才能启动
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Badge = false;
	//钥匙名称（""）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_KeyName = "";
	//徽章A
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_BadgeAName = "";
	//徽章B
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_BadgeBName = "";
	//老鼠的细节设置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMouseLineSetting> M_MouseLineSetting;
};


/*
-------------------------------------------------------------------------------
									关卡配置
-------------------------------------------------------------------------------
*/

//关卡时间限制
USTRUCT(BlueprintType)
struct FLevelTimeWidthLevelConfig
{
	GENERATED_USTRUCT_BODY()
public:
	//开启关卡时间限制
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnable = false;
	//最长时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxTime = 300.f;
};

//显示配置
USTRUCT(BlueprintType)
struct FLevelKeyConfig
{
	GENERATED_USTRUCT_BODY()
public:
	//是否显示钥匙界面
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bKey = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftObjectPath Key1;
	//名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Key1Name = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftObjectPath Key2;
	//名称2
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Key2Name = TEXT("");

	//物品1
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftObjectPath Item1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftObjectPath Item2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftObjectPath Item3;
};

//关卡配置
USTRUCT(BlueprintType)
struct FLevelConfig
{
	GENERATED_USTRUCT_BODY()
public:
	//关卡标题名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LevelTitleName = FString(TEXT("美味一区"));
	//关卡名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LevelName = TEXT("曲奇岛");
	//关卡介绍
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LevelDes = TEXT("");
	//最多经验值【限制】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxEx = 0;
	//总回合数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalRound = 1;
	//准备时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GameReadyTime = 20.f;
	//卡片等级上线
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CardGradeMax = 16;
	//卡片等级最低限制
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CardGradeMin = 1;
	//老鼠等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MouseLevel = 1;
	//时间限制
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLevelTimeWidthLevelConfig LevelTimeWidthLevelConfig;
	//是否可以携带宠物
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSpineWidthChar = true;
	//初始能量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InitEnergy = 50;
	//地图的头像【2D纹理】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftObjectPath LevelBGHead = FSoftObjectPath(FString(TEXT("")));
	//地图的背景【Sprite】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftObjectPath LevelBGName = FSoftObjectPath(FString(TEXT("")));
	//开场背景音乐
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftObjectPath LevelDefMusic = FSoftObjectPath(FString(TEXT("SoundWave'/Game/Resource/BP/Audio/GameBG/曲奇岛白天.曲奇岛白天'")));
	//精英背景音乐
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftObjectPath LevelUpMusic = FSoftObjectPath(FString(TEXT("SoundWave'/Game/Resource/BP/Audio/GameBG/曲奇岛白天精英.曲奇岛白天精英'")));
	//boss出场背景音乐
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftObjectPath LevelBossMusic = FSoftObjectPath(FString(TEXT("SoundWave'/Game/Resource/BP/Audio/GameBG/曲奇岛白天Boss.曲奇岛白天Boss'")));
	//白天或者夜晚
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool LevelIsDay = true;
	//地图发射器【Actor】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftClassPath LevelLauncherPath;
	//显示Key UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLevelKeyConfig LevelKeyConfig;
	//掉落物品
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, FSoftObjectPath> LevelItems;
	//掉落数值[掉落的概率数值]
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemsValue = 5;
	//掉落个数倍[1倍掉落数量]
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemsCountRate = 1;
	//忽略掉落[本局永不掉落的物品]
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> IgnoreItems;
	//高亮显示个数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HiLightCount = 1;
};


/*
-------------------------------------------------------------------------------
									怪物配置
-------------------------------------------------------------------------------
*/

//老鼠配置节点
USTRUCT(BlueprintType)
struct FMouseConfigNode {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Index = 0;
	//老鼠名称【平民鼠】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CurMouseName = FString();
	//掉落物品
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> LevelItems;
	//当前老鼠死亡立刻结束游戏
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CurMouseDeathForceGameWin = false;
	//老鼠的行列
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLine CurMouseLine = FLine(-1);
	//排除行【当前老鼠不会生成在被排除的行中】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> IgnoreRows;
};

//小时间节点
USTRUCT(BlueprintType)
struct FTimeNode {
	GENERATED_USTRUCT_BODY()
public:
	//一个小时间节点一次性并发创建的老鼠集
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMouseConfigNode> CurMouseNode;
};

//一个大节点包含20个小节点【即1秒解析一个节点】
USTRUCT(BlueprintType)
struct FTimeNodeWidthRound {
	GENERATED_USTRUCT_BODY()
public:
	//所有的小时间节点，且只会解析到第20个节点为止【最多20个】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTimeNode> CurNode;
};

//钥匙徽章配置
USTRUCT(BlueprintType)
struct FRoundKey {
	GENERATED_USTRUCT_BODY()
public:
	//是否需要使用徽章或者钥匙才能开启当前回合
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Enable = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString KeyName = TEXT("");
	//配置了徽章则优先使用徽章
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BadgeNameA = TEXT("");
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BadgeNameB = TEXT("");
};

//当前波【开始解析大节点，最多10个】
USTRUCT(BlueprintType)
struct FRondWidthMouseConfig {

	GENERATED_USTRUCT_BODY()

public:
	//当前时间节点【每20秒执行一个节点】【总共10个】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTimeNodeWidthRound> CurNode;
	//当前波是否需要钥匙或者徽章
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRoundKey RoundKey;
	//是否等当全部老鼠生成完毕时自动进入下一个回合
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AllMouseSpawnFinishAutoNext = false;
	//进入下一个回合的准备时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InNextRoundTime = 0.f;
	//物品掉落
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> LevelItems;
};

//老鼠配置【当准备时间过了之后，开始解析波数】
USTRUCT(BlueprintType)
struct FMouseConfig {
	GENERATED_USTRUCT_BODY()
public:
	//当前波数【根据关卡波数配置决定只解析多少】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRondWidthMouseConfig> CurRoundConfig;
	//所有老鼠名称Key集合
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, int32> AllMouseListMap;
	//所有老鼠IDKey集合
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, FString> AllMouseKeyListMap;
};


//老鼠结构管理器
UCLASS()
class FVM_API UMouseStructManager : public USaveGame
{
	GENERATED_BODY()
public:
	//获取当前波数，得到对应的 回合配置
	bool GetMouseRound(const int32& _CurRound, FRondWidthMouseConfig& _OutCurRound);
	//通过回合配置获得当前的大节点
	bool GetMouseTimeNodeByRound(FRondWidthMouseConfig& _InputRound, const int32& Index, FTimeNodeWidthRound& _OutCurTimeNode);
	//通过大节点获得当前的小节点
	bool GetMouseTimeNodeByTimeNode(FTimeNodeWidthRound& _InputTimeNode, const int32& _Index, FTimeNode& _OutCurNode);

public:
	//涉资配置信息
	void SetMouseConfig(const FMouseConfig& _Config);
	//获取配置信息
	const FMouseConfig& GetMouseConfig();
	//设置行数量
	void SetRowCount(const int32& _Index);
	//获取行数量
	int32 GetRowCount();
	//获取回合总数
	int32 GetRoundTotal();
	//获取精英波数量
	int32 GetUpRoundTotal();
	//获取普通波数量
	int32 GetNormalRoundTotal();
	//获取某回合的子节点数量
	int32 GetRoundNodeCountByRoundIndex(const int32& _Index);
private:
	//行数
	UPROPERTY()
	int32 RowCount = 0;
	//老鼠的配置信息
	UPROPERTY()
	FMouseConfig CurMouseConfig;
};
