// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Boss/BossBase.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "Boss_LXQS.generated.h"

class UPaperFlipbook;
class APaperFlipbookActor;
/**
 * 龙虾骑士
 */

USTRUCT(BlueprintType)
struct FBoss_LXQSAnim
{
	GENERATED_USTRUCT_BODY();
public:
	//死亡
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Death;
	//默认
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Idle;
	//默认低血
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> IdleLow;
	//移动前后
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> MoveFront;
	//移动前后低血
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> MoveFrontLow;
	//移动上下
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> MoveDown;
	//移动上下低血
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> MoveDownLow;
	//射击1
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Shoot_1;
	//射击1低血
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Shoot_1Low;
	//射击2
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Shoot_2;
	//射击2低血
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Shoot_2Low;
	//预备翻滚
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> RotationMoveBegin;
	//预备翻滚
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> RotationMoveBeginLow;
	//预备翻滚结束
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> RotationMoveEnd;
	//预备翻滚结束
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> RotationMoveEndLow;
	//预备翻滚动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> RotationMove;
	//气泡爆炸
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> BubbleBomb;
	//导弹爆炸
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> MissileBomb;
};

class ABoss_LXQS;

//状态模式龙虾骑士[状态模式实验类]
UCLASS()
class FVM_API ULXQS_StateBase : public UObject
{
	GENERATED_BODY()

		friend class ABoss_LXQS;
public:
	virtual void Init(ABoss_LXQS* OnwerClass);
	virtual void Update(const float& tick);
	virtual void AnimPlayEnd();
	virtual void Finished();
protected:
	class ABoss_LXQS* Onwer = nullptr;
};
//带有时间线的状态
UCLASS()
class FVM_API ULXQS_TimeLineStateBase : public ULXQS_StateBase
{
	GENERATED_BODY()
public:
	//出场位置
	UPROPERTY()
		FVector M_CurrentLocation;
	//到终点的位置
	UPROPERTY()
		FVector M_TargetLocation;
	//目标行
	UPROPERTY()
		int32 TargetRow = 0;
public:
	virtual void Init(class ABoss_LXQS* OnwerClass) override;
	virtual void Update(const float& tick) override;
	virtual void AnimPlayEnd() override;
	virtual void Finished() override;

	UFUNCTION()
		virtual void TimeLineUpdate(float Value);
	UFUNCTION()
		virtual void TimeLineFinish();
public:
	void Start();
	void SetCurve(class UCurveFloat* CurveClass, const FString& _CurName);
private:
	//时间线
	UPROPERTY()
		FTimeline CurTimeLine;

	//float曲线加载代理
	FOnTimelineFloat OnTimelineFloatDelegate;
	//时间轴播放完成
	FOnTimelineEventStatic OnTimeLineFinishDelegate;


};


//出场状态
UCLASS()
class FVM_API ULXQS_BeginState : public ULXQS_TimeLineStateBase
{
	GENERATED_BODY()

public:
	virtual void Init(class ABoss_LXQS* OnwerClass) override;
	virtual void Finished() override;
	virtual void TimeLineFinish() override;
};

//上下移动状态
UCLASS()
class FVM_API ULXQS_MoveDownState : public ULXQS_TimeLineStateBase
{
	GENERATED_BODY()
public:
	virtual void Init(class ABoss_LXQS* OnwerClass) override;
	virtual void Update(const float& tick) override;
	virtual void Finished() override;
	virtual void TimeLineFinish() override;
private:
	UPROPERTY()
		bool bMoveFinish = false;
	UPROPERTY()
		float FinishDalay = 2.f;
};


//发射状态
UCLASS()
class FVM_API ULXQS_ShootMissileState : public ULXQS_StateBase
{
	GENERATED_BODY()
public:
	virtual void Init(class ABoss_LXQS* OnwerClass) override;
	virtual void Update(const float& tick) override;
	virtual void AnimPlayEnd() override;
	virtual void Finished() override;
private:
	//是否射击
	UPROPERTY()
		bool bShoot = false;
	//是否完成射击动作
	UPROPERTY()
		bool bShootFinish = false;
	//列数量
	UPROPERTY()
		int32 ColNum = 0;
	//射击延迟
	UPROPERTY()
		float ShootDelay = 0.1f;
	//射击的目标位置
	UPROPERTY()
		FLine ShootTarget;
};


//泡泡对象
UCLASS()
class FVM_API ALXQS_BubbleBomb : public AGameActorFlipbookBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
		class ACardActor* GetCard();
public:
	UPROPERTY()
		AMapMeshe* CurMehshe = nullptr;
};
//发射泡泡状态
UCLASS()
class FVM_API ULXQS_ShootBubbleState : public ULXQS_StateBase
{
	GENERATED_BODY()
public:
	virtual void Init(class ABoss_LXQS* OnwerClass) override;
	virtual void Update(const float& tick) override;
	virtual void AnimPlayEnd() override;
	virtual void Finished() override;
private:
	//是否播放了动画
	UPROPERTY()
		bool bPlay = false;
	//是否播放了动画并且结束
	UPROPERTY()
		bool bPlayEnd = false;
	//前摇延迟
	UPROPERTY()
		float FirstDelay = 0.86f;
	//射击延迟
	UPROPERTY()
		float ShootDelay = 0.1f;
	//射击次数
	UPROPERTY()
		float ShootCount = 5;
	//带有卡片的网格
	UPROPERTY()
		TArray<AMapMeshe*> CardMesheBuffer;
	//需要射击的网格
	UPROPERTY()
		TArray<AMapMeshe*> TargetCardMeshes;
};


//翻滚状态
UCLASS()
class FVM_API ULXQS_RotationMoveState : public ULXQS_TimeLineStateBase
{
	GENERATED_BODY()
public:
	virtual void Init(class ABoss_LXQS* OnwerClass) override;
	virtual void Update(const float& tick) override;
	virtual void AnimPlayEnd() override;
	virtual void Finished() override;
	virtual void TimeLineUpdate(float Value) override;
	virtual void TimeLineFinish() override;
private:
	//预备翻滚结束
	UPROPERTY()
		bool bRotationMove = false;
	//是否是往返翻滚
	UPROPERTY()
		bool bBack = false;
	//是否是结束
	UPROPERTY()
		bool bBackEnd = false;
	//正在翻滚
	UPROPERTY()
		bool bRotation = false;
	//创建延迟
	UPROPERTY()
		float CreateItemDelay = 0.5f;
	//二次翻滚延迟
	UPROPERTY()
		float bBackRotationDelay = 3.f;
};



class UBoxComponent;
class USphereComponent;

UCLASS()
class FVM_API ABoss_LXQS : public ABossBase
{
	GENERATED_BODY()

		friend class ULXQS_StateBase;

public:
	//构造新状态
	template<class StateClass>
	static ULXQS_StateBase* MakeLXQSState(ABoss_LXQS* OnwerClass)
	{
		if (OnwerClass)
		{
			if (OnwerClass->CurStateClass)
			{
				OnwerClass->CurStateClass = nullptr;
			}
		}

		//血量为0 表示死亡
		if (OnwerClass->GetCurrentHP() <= 0.f)
		{
			return nullptr;
		}

		StateClass* CurClass = NewObject<StateClass>();
		CurClass->Init(OnwerClass);
		return CurClass;
	}

	ABoss_LXQS();
	//初始化
	virtual void BeginPlay() override;
	//Tick 更新
	virtual void Tick(float DeltaTime) override;
	//boos初始化
	virtual void MouseInit() override;
	//老鼠死亡
	virtual void MouseDeathed() override;
	//老鼠初始化(资源加载完毕后调用一次)
	void Init();

	//启动碰撞
	void EnableCo();
	//关闭碰撞
	void DisEnableCo();
public:
	UFUNCTION()
		void AnimPlayEnd();
public:
	UFUNCTION()
		void OnResourceLoadFinished();
	UFUNCTION()
		class UMesheControllComponent* GetMesheCtlComp() const;

	virtual void InMapMeshe(ELineType NewType) override;
	//执行buff
	virtual void ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime);
public:
	//用于被检测的碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* CurMesheComp = nullptr;
	//被子弹击中大碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* CurMouseColisionComp = nullptr;
public:
	//龙虾骑士所有的动画资源
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBoss_LXQSAnim LXQSAnim;
	//瞄准图标
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftClassPtr<APaperFlipbookActor> MouseAimLog;
	//翻滚压痕
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftClassPtr<AGameActorFlipbookBase> MouseRotationMoveItem;
	//导弹对象
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftClassPtr<AGameActorFlipbookBase> MouseMissileClass;
	//气泡对象
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftClassPtr<ALXQS_BubbleBomb> MouseBubbleClass;
	//状态模式
	UPROPERTY()
		ULXQS_StateBase* CurStateClass = nullptr;
	//老鼠出场时曲线
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UCurveFloat* StateBeginCurve = nullptr;
	//老鼠上下移动时曲线
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UCurveFloat* StateMoveDownCurve = nullptr;
	//老鼠翻滚移动时曲线
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UCurveFloat* StateRotationMoveCurve = nullptr;
private:
	//网格控制组件
	UPROPERTY()
		class UMesheControllComponent* MesheCtlComp = nullptr;
};
