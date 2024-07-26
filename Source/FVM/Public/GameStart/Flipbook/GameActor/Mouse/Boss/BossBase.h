// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"

//boss 进度条
#include "GameStart/VS/UI/UI_MouseBoss_Slider.h" 

#include "GameStart/VS/MapMeshe.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/Components/MesheControllComponent.h"
#include <Kismet/KismetSystemLibrary.h>

#include "BossBase.generated.h"

/**
 *
 */

class UUI_MouseBoss_Slider;
class UBoxComponent;
class USphereComponent;

UINTERFACE(BlueprintType)
class UBossBaseInter : public UInterface
{
	GENERATED_BODY()
};
class FVM_API IBossBaseInter
{
	GENERATED_BODY()
public:
	//初始化boss进度条
	virtual void InitBoss(const FString& Name, const FString& BossCode) = 0;
};

//状态模式
UCLASS()
class ABossStateManager : public AMouseActor, public IBossBaseInter {
	GENERATED_BODY()
		friend class UBossStateBase;
	friend class ABossBase;
public:
	virtual void InitBoss(const FString& Name, const FString& BossCode) override;
	void ChangeState(class UBossStateBase* NewState);
	class UBossStateBase* Get();
private:
	UPROPERTY()
		class UBossStateBase* State = nullptr;
	UPROPERTY()
		class ABossBase* MouseActor = nullptr;
};

//boss状态基类
UCLASS()
class UBossStateBase : public UObject {
	GENERATED_BODY()
		friend class ABossStateManager;
public:
	virtual	void Init();
	virtual void MouseTick(const float& DeltaTime);
	virtual void MoveingUpdate(const float& DeltaTime);
	virtual void BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType);
	virtual void ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime);
	virtual void InMapMeshe(ELineType CurLineType);
	virtual void AnimPlayEnd();
protected:
	void InitState(ABossStateManager* NewMan);
	class ABossBase* Get();
	void PlayAnim(UPaperFlipbook* Anim1, UPaperFlipbook* Anim2Low);
protected:
	UPROPERTY()
		ABossStateManager* CurMan = nullptr;
};

//通用状态基本类型
UCLASS()
class FVM_API UBossUniformState : public UBossStateBase
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void MouseTick(const float& DeltaTime) override;
	virtual void ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime) override;
public:
	//获取线路
	FLine GetLine();
	//获取网格
	AMapMeshe* GetMeshe(int32 Row, int32 Col);
	//获取网格位置
	FVector GetMesheLocation(int32 Row, int32 Col);
	//获取网格长度
	FLine GetRowAndCol();
	//获取等级
	int32 GetLevel();
	//播放透明度动画
	void PlayOpacity(bool bStart);
	//boss出现
	void BossOutDef();
	//boss退场
	void BossExitDef();
private:
	UPROPERTY()
		UTimeLineClass* TimeLine = nullptr;
};

UCLASS()
class FVM_API ABossBase : public ABossStateManager
{
	GENERATED_BODY()
		friend class ABossStateManager;
private:
	//tick更新速度
	UPROPERTY()
		float M_TickRate = 1.f;
public:
	//更新
	virtual void MouseTick(const float& DeltaTime);
	//初始化
	virtual void BeginPlay() override;
	//老鼠初始化(Base BeginPlay自动调用一次)
	virtual void MouseInit() override;
	//初始化boss进度条
	virtual void InitBoss(const FString& Name, const FString& BossCode) override;
public:
	//基本老鼠类型
	virtual EMouseBaseType GetMouseBaseType() override;
	//移动(每0.02ms自动调用(允许移动的情况下))
	virtual void MoveingUpdate(float DeltaTime) override;
	//停止移动(当设置停止移动时调用一次)
	virtual void MoveingStop() override;
	//开始移动(当设置开始移动时调用一次)
	virtual void MoveingBegin() override;

	//老鼠开始攻击(如果设置了IsAttack为true 会自动调用一次)
	virtual void AttackedBegin() override;
	//老鼠攻击结束(如果设置了IsAttack为false 会自动调用一次)
	virtual void AttackedEnd() override;

	//当老鼠被命中时受到的伤害数值(前提是isHurt为true  调用一次(一般由FlyitemActor命中时调用))
	virtual bool BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType) override;

	//老鼠受到攻击((当设置isHurt为true时)调用一次)
	virtual void BeAttakedBegin() override;

	//当老鼠死亡时(当设置老鼠SetMouseDeath(true)时调用一次)
	virtual void MouseDeathed() override;

	virtual void InMapMeshe(ELineType CurLineType) override;

	//执行buff
	virtual void ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime) override;
	//结束buff
	virtual void ExitBuff(EGameBuffTag BuffTag) override;

	//动画播放结束
	void BossAnimPlayEnd();
public:
	//淡入淡出
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 淡入淡出曲线")
		TSoftObjectPtr<UCurveFloat> BossOpacityCurve =
		TSoftObjectPtr<UCurveFloat>(
			FSoftObjectPath(
				TEXT("CurveFloat'/Game/Resource/BP/GameStart/Item/Mouse/boss/BP_Boss淡入淡出时间曲线.BP_Boss淡入淡出时间曲线'"))
			);

	//默认可生成的老鼠
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss基础能力")
		TArray<FString> SpawnDefMouseNames = {
	TEXT("兰博鼠"),TEXT("兔子鼠"),TEXT("房东鼠"),TEXT("捧花鼠"),TEXT("铁锅鼠"),TEXT("迷球鼠"),
	TEXT("巨人鼠"),TEXT("多宝太郎鼠"),TEXT("黄瓜平民鼠"),TEXT("滑板鼠"),TEXT("跳跳鼠"),TEXT("忍者首领鼠")
	};

	//获取网格控制组件
	UFUNCTION(BlueprintCallable)
		UMesheControllComponent* GetMesheComp();
	//获取地图发射器
	UFUNCTION(BlueprintCallable)
		AMapLauncherBase* GetMapLauncher();
	//获取老鼠等级
	UFUNCTION(BlueprintCallable)
		int32 GetMouseLevel();
	//更新老鼠显示透明度
	UFUNCTION(BlueprintCallable)
		void UpdateBossOpacity(const float& value);
	//生成老鼠
	UFUNCTION(BlueprintCallable)
		void SpawnMouseByName(const FString& MouseName, int32 Row, int32 Col = -1);
	//指定的数组生成
	UFUNCTION(BlueprintCallable)
		void DefSpawnMouseTemp(int32 Level, const TArray<FString>& MouseNames);
	//默认使用自带的老鼠
	UFUNCTION(BlueprintCallable)
		void DefSpawnMouse(int32 Level);
	//设置boss名称
	UFUNCTION(BlueprintCallable)
		void SetBossName(const FString& Name);
private:
	//boss老鼠的血量进度
	UPROPERTY()
		UUI_MouseBoss_Slider* M_BossHP_Progress = nullptr;

	//是否已经初始化
	bool bInit = false;
};

//自带盒体碰撞
UCLASS()
class FVM_API ABossBoxBase : public ABossBase
{
	GENERATED_BODY()
public:
	//检查碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* MMeshe = nullptr;
	//重叠碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* MBody = nullptr;
public:
	ABossBoxBase();
};

//自带球体碰撞
UCLASS()
class FVM_API ABossSphereBase : public ABossBase
{
	GENERATED_BODY()
public:
	//检查碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* MMeshe = nullptr;
	//重叠碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* MBody = nullptr;
public:
	ABossSphereBase();
};
