// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameSystem/MouseStructManager.h"
#include "Data/CardData/MouseDataStruct.h"
#include "GameSystem/Tools/ObjectPoolManager.h"
#include "GameStart/VS/MapBaseType.h"
#include "MouseManagerComponent.generated.h"

class UFVMGameInstance;
class UGameMapStructManager;
class UMouseStructManager;
class UFVMGameInstance;
class AMouseActor;
class UMesheControllComponent;

class UUI_MouseRound;
class UUI_MouseBoss_Slider;

//老鼠线路管理器
UCLASS(BlueprintType)
class FVM_API UMouseLineManager : public UObject
{
	GENERATED_BODY()

	friend class UMouseManagerComponent;

public:
	//添加老鼠
	void AddMouse(const FString& MouseName, AMouseActor* _CurMouse);
	//移除老鼠
	bool RemoveMouse(AMouseActor* _CurMouse);
	//移除老鼠
	bool RemoveMouseByName(const FString& MouseName);
	//查询老鼠
	AMouseActor* FindMouse(const FString& MouseName);
	//排序老鼠位置
	void SortMouseByTick(const float& _Tick);
public:
	//获取陆地最前老鼠
	AMouseActor* GetMouseTopByGorund() const;
	//获取地下最前老鼠
	AMouseActor* GetMouseTopByUnderGorund() const;
	//获取空中最前老鼠
	AMouseActor* GetMouseTopBySky() const;
	//获取所有类型最前的老鼠
	AMouseActor* GetMouseTopByAllType() const;
	//获取陆地老鼠
	const TMap<FString, AMouseActor*>& GetMouseGround();
	//获取地下老鼠
	const TMap<FString, AMouseActor*>& GetMouseUnderGround();
	//获取空中老鼠
	const TMap<FString, AMouseActor*>& GetMouseSky();
	//获取全部
	TMap<FString, AMouseActor*> GetMouseAll();
	//当前行是否存在老鼠
	bool GetMouseExist();
	//消灭所有老鼠
	void KillAllMouse();
private:
	//排序老鼠最前位置
	AMouseActor* SortMouseTopLocation(TMap<FString, AMouseActor*>& _Mouses);
	//移除
	bool RemoveMouseIns(TMap<FString, AMouseActor*>& MouseMap, AMouseActor* _CurMouse);
	//通过名称移除
	bool RemoveMouseInsByName(TMap<FString, AMouseActor*>& MouseMap, const FString& MouseName);
private:
	//更新Tick的时间延迟
	UPROPERTY()
	float UpdateTickTime = 0.2f;
	UPROPERTY()
	float CurTime = 0.f;
	//老鼠时间暂停
	UPROPERTY()
	bool bMouseTimePause = false;

	//陆地最前老鼠
	UPROPERTY(EditAnywhere)
	AMouseActor* CurMouseGroundTop = nullptr;
	//地下最前老鼠
	UPROPERTY(EditAnywhere)
	AMouseActor* CurMouseUnderGroundTop = nullptr;
	//空中最前老鼠
	UPROPERTY(EditAnywhere)
	AMouseActor* CurMouseSkyTop = nullptr;

	//陆地老鼠
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TMap<FString, AMouseActor*> CurMouseGround;
	//地下老鼠
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TMap<FString, AMouseActor*> CurMouseUnderGround;
	//飞行老鼠
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TMap<FString, AMouseActor*> CurMouseSky;
	//水上老鼠
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TMap<FString, AMouseActor*> CurMouseOnWater;
};

//当回合数发生改变时触发
DECLARE_DELEGATE(FOnRoundNodeChanged);
//当进入当前回合最末尾时触发
DECLARE_DELEGATE(FOnInCurRoundEnd);
//当进入下一个回合时触发
DECLARE_DELEGATE(FOnInNextRound);

//老鼠时间管理器
UCLASS()
class FVM_API UMouseTimeManager : public UObject
{
	GENERATED_BODY()
public:
	//当回合数发生改变时触发
	FOnRoundNodeChanged OnRoundNodeChanged;
	//当进入当前回合最末尾时触发
	FOnInCurRoundEnd OnInCurRoundEnd;
	//当进入下一个回合时触发
	FOnInNextRound OnInNextRound;
public:
	//重置
	void Reset();
	//更新时间
	void UpdateTime(const float& _tick, UMouseManagerComponent* const MouseManagerComponent);
	//进入下一个回合
	void InNextRound(const float& _DelayTime);
	//进入下一个回合
	void InNextRound();
	//是否是当前回合的最末尾
	bool GetCurRoundEnd();
	//老鼠是否全部生成完毕
	bool GetAllMouseSpawnEnd();
	//启动计时器
	void Enable();
	//获取最小子节点索引
	int32 GetTimeNode();
	//获取最小大节点索引
	int32 GetTimeNodeRound();
	//获取回合数索引
	int32 GetTimeRound();
	//获取当前回合索引
	int32 GetCurrentTimeRound();
	//设置回合索引
	void SetTimeRound(const int32& Index);
	//设置所有回合结束
	void SetRoundEnd();
	//获取时间
	int32 GetTime();
private:
	//启动时间计时
	UPROPERTY()
	bool bEnable = false;

	//启动进入下一个回合的计时
	UPROPERTY()
	bool bEnableNextRound = false;
	//当前进入下一个回合的时间
	UPROPERTY()
	float CurNextRoundTime = 0.f;
	//进入下一个回合的总时间
	UPROPERTY()
	float CurNextRoundTotalTime = 0.f;
	//是否继续更新当前回合的节点
	UPROPERTY()
	bool bUpdateCurNode = true;

	//进入下一个回合时检测是否还有老鼠存在
	UPROPERTY()
	bool bEnableCheckMouseExist = false;
	//开始下一个回合是否是最后一个会的检测
	UPROPERTY()
	bool bEnableCheckNextRoundIsEnd = false;
	//当前回合是否是最后一个回合
	UPROPERTY()
	bool bCurRoundIsEnd = false;

	//是否将所有老鼠全部生成完毕
	UPROPERTY()
	bool bCurAllMouseSpawnEnd = false;

	//分钟
	UPROPERTY()
	int32 Minute = 0;
	//秒钟
	UPROPERTY()
	int32 Second = 0;
	//当前时间流动
	UPROPERTY()
	float CurTime = 0.f;
	//总时间流动
	UPROPERTY()
	float AllTime = 0.f;
private:
	//最小节点索引
	UPROPERTY()
	int32 TimeNodeIndex = 0;
	//最小大节点索引，它决定了使用那个最小节点组
	UPROPERTY()
	int32 TimeNodeRoundIndex = 0;
	//大节点索引,它决定了当前是第多少波
	UPROPERTY()
	int32 RoundIndex = 0;
	//当前回合
	UPROPERTY()
	int32 CurRoundIndex = 0;
};


//老鼠生成器
UCLASS()
class UMouseSpawnManager : public UObject
{
	GENERATED_BODY()
public:
	//生成老鼠
	UFUNCTION(BlueprintCallable)
	AMouseActor* SpawnMouse(
		class UMouseManagerComponent* Comp,
		const FString& _MousePath,//老鼠的类路径
		const FLine& Line,//线路
		const int32& RowMax,//最大行
		bool DeathFroceGameWin,//死亡就游戏结束
		const TArray<FString>& Items,//掉落物
		const FMouseBase& MouseData//老鼠数据
	);
public:
	//创建一只老鼠
	UFUNCTION(BlueprintCallable)
	static AMouseActor* MakeNewMouseByClass(
		class UMouseManagerComponent* Comp,
		const TSoftClassPtr<AMouseActor>& MouseClass,//资源
		const FVector& Location,
		float HP,//血量
		float ATK,//攻击力
		float Speed,//速度
		FLine MouseLine,//线路
		const ELineType& MouseLineType,//类型
		EMouseTag NewTag = EMouseTag::NormalGround,//老鼠标记
		bool bAddManager = true,//添加到老鼠管理器【如果不添加请慎用】,
		FString AlienName = TEXT("Tag")
	);
	//通过老鼠名称[数据表中存在的]创建一只老鼠
	UFUNCTION(BlueprintCallable)
	static bool MakeNewMouseByName(
		UDataTable* MouseDataTable,
		FString MouseName,
		FLine Line,
		FVector Location = FVector::ZeroVector
	);
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UMouseManagerComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class UMouseLineManager;

public:
	static UMouseManagerComponent* M_S_CurrentClass;
public:
	//老鼠管理器
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMouseStructManager* M_UMouseStructManager = nullptr;
	//老鼠回合和Boss的血量UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUI_MouseRound* M_UUI_MouseRound = nullptr;
public:
	// Sets default values for this component's properties
	UMouseManagerComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	//获取时间管理器
	const UMouseTimeManager* const GetMouseTimeManager();
	//通过老鼠名称获取一个资产引用路径
	bool GetMousePathByName(const FString& MouseName, FString& OutMousePath, FMouseBase& OutData);
public:
	//游戏开始
	UFUNCTION(BlueprintCallable)
	void GameStart();
	//游戏结束
	UFUNCTION(BlueprintCallable)
	void GameOver();
	//当回合数更新时
	UFUNCTION()
	void OnRoundNodeChangedCallback();
	//当前回合末尾
	UFUNCTION()
	void OnCurRoundEndCallback();
	//下一个回合开始初始化【提示：最后一波 UI】
	UFUNCTION()
	void InitNextRoundCallBack();
public:
	//显示UI【波数UI】
	UFUNCTION(BlueprintCallable)
	void ShowMouseUI(int32 Round);
	//移除UI【波数UI】
	UFUNCTION(BlueprintCallable)
	void RemoveMouseUI();
	//更新进度值
	UFUNCTION(BlueprintCallable)
	void UpdateRoundProgress();
	//强行进入下一个回合
	UFUNCTION(BlueprintCallable)
	void ForceNextRound();
	//强行进入需要带钥匙开启的回合或者直接到最后一个回合的末尾
	UFUNCTION(BlueprintCallable)
	void ForceNextEndRound();
	//是否是最后一个回合
	UFUNCTION(BlueprintCallable)
	bool IsEndRound();
	//老鼠是否生成完毕
	UFUNCTION(BlueprintCallable)
	bool IsMouseSpawnEnd();
	//当前回合是否存在老鼠
	UFUNCTION(BlueprintCallable)
	bool IsMouseExist();
	//获取线路个数
	UFUNCTION(BlueprintCallable)
	const int32 GetLineCount() const;
	//获取时间
	UFUNCTION(BlueprintCallable)
	int32 GetCurTime();
	//获取当前回合
	UFUNCTION(BlueprintCallable)
	int32 GetCurRound();
	//获取线路最前的老鼠
	UFUNCTION(BlueprintCallable)
	AMouseActor* const GetLineTopFirstMouse(const int32 _LineIndex);
	//获取TopFirst老鼠
	UFUNCTION(BlueprintCallable)
	AMouseActor* const GetTopFirstMouse();
	//更具类型获取最前的老鼠
	UFUNCTION(BlueprintCallable)
	AMouseActor* const GetTopMouseByType(const ELineType& LineType);
	//获取线路管理器
	UFUNCTION(BlueprintCallable)
	UMouseLineManager* GetMouseLineManager(int32 Row);
	//获取当前总波
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentRoundTotal() const;
	//将当前老鼠从线路管理器中移除，被移除的老鼠不再被管理器管理
	UFUNCTION(BlueprintCallable)
	static void RemoveMouse(AMouseActor* _CurMouse);
	//添加老鼠
	UFUNCTION(BlueprintCallable)
	static void AddMouse(AMouseActor* _CurMouse, const int32& Row, bool bInit = true);
	//测试是否老鼠生成则死亡
	UFUNCTION(BlueprintCallable)
	static bool TestMouseSpawnDeath();
	//切换路线【需要手动设置SetMouseLine】
	UFUNCTION(BlueprintCallable)
	static bool ChangeLine(const FString& MouseObjName, const int32& CurRow, const int32& TargetRow);
	//切换路线【不需要需要手动设置SetMouseLine】
	UFUNCTION(BlueprintCallable)
	static bool ForceChangeLine(
		const FString& MouseObjName,
		const int32& CurRow,
		const int32& TargetRow,
		const int32& TargetCol
	);
	//切换老鼠的类型
	UFUNCTION(BlueprintCallable)
	static bool ChangeMouseLineType(
		AMouseActor* _CurMouse,
		int32 CurRow,
		ELineType TargetType,
		class UBoxComponent* CurCollision = nullptr,
		class UShapeComponent* CurBodyCollision = nullptr
	);
	//切换老鼠的类型直接到通道
	static bool ChangeMouseLineTypeToChannel(
		AMouseActor* _CurMouse,
		int32 CurRow,
		ELineType TargetType
	);
	//获取所有存在的老鼠
	UFUNCTION(BlueprintCallable)
	static void GetAllMouse(TArray<AMouseActor*>& OutAllMouse);
	//设置老鼠死亡
	UFUNCTION(BlueprintCallable)
	void SetMouseSpawnKill();
	//设置老鼠时间暂停
	UFUNCTION(BlueprintCallable)
	void SetMouseTimePause();
	//获取当前生成的老鼠所在行
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentSpawnMouseRow();
public:
	/*对象池*/

	//获取灼烧buff的对象池管理器
	UFUNCTION(BlueprintCallable)
	UObjectPoolManager* GetBurnBuffFxObjPoolManager();
	//获取凝固buff的对象池管理器
	UFUNCTION(BlueprintCallable)
	UObjectPoolManager* GetSolidifBuffFxObjPoolManager();
protected:
	//初始化老鼠配置
	void InitMouseStructManager();
	//初始化老鼠线路管理器
	void InitMouseLineManager();
	//初始化时间配置
	void InitTimer();
	//初始化老鼠资源配置
	void InitMouseResource();
	//初始化老鼠生成器
	void InitMouseSpawnManager();
private:
	//开启时间计时器的计数
	float CurGameReadyTime = 0;
	//当前游戏是否结束
	bool CurGameIsOver = false;
	//当前游戏是否胜利
	bool CurGameIsWin = true;
	//是否秒杀老鼠
	bool bKillMouse = false;
	//当前总波数
	int32 CurRoundTotal = 0;
	//当前生成的老鼠行
	int32 CurSpawnRow = 0;
private:
	//老鼠线路管理器
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TArray<UMouseLineManager*> MouseLineManager;
	//时间计时器
	UPROPERTY()
	UMouseTimeManager* MouseTimeManager = nullptr;
	//老鼠生成器
	UPROPERTY()
	UMouseSpawnManager* MouseSpawnManager = nullptr;
	//所有的老鼠数据集合
	UPROPERTY()
	TArray<FMouse_Data> AllMouseData;
	//对象池管理器[灼烧buff对象池]
	UPROPERTY()
	UObjectPoolManager* BurnBuffFxObjPoolManager = nullptr;
	//对象池管理器[凝固buff对象池]
	UPROPERTY()
	UObjectPoolManager* SolidiBuffFxObjPoolManager = nullptr;
	//老鼠路径集合
	UPROPERTY()
	TMap<FString, FMouseBase> MousePaths;
};
