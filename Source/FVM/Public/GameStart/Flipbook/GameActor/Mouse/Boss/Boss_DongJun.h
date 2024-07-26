// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Boss/BossBase.h"

#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"

#include "Boss_DongJun.generated.h"


class UBoxComponent;

UCLASS()
class UDongJunStateManager : public UObject {
	GENERATED_BODY()

		friend class UDongJunStateBase;

public:
	//生成管理器
	static UDongJunStateManager* MakeDongJunStateManager(class ABoss_DongJun* NewMouseActor);
public:
	void ChangeState(class UDongJunStateBase* NewState);
	class UDongJunStateBase* Get();
private:
	UPROPERTY()
		class UDongJunStateBase* State = nullptr;
	UPROPERTY()
		class ABoss_DongJun* MouseActor = nullptr;
};

//boss状态基类
UCLASS()
class UDongJunStateBase : public UObject {
	GENERATED_BODY()

		friend class UDongJunStateManager;

public:
	virtual	void Init();
	virtual void MouseTick(const float& DeltaTime);
	virtual void MoveingUpdate(const float& DeltaTime);
	virtual void BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType);
	virtual void AnimPlayEnd();
protected:
	void InitState(UDongJunStateManager* NewMan);
	class ABoss_DongJun* Get();
	void PlayAnim(UPaperFlipbook* Anim1, UPaperFlipbook* Anim2Low);
protected:
	UPROPERTY()
		UDongJunStateManager* CurMan = nullptr;
};

//默认状态
UCLASS()
class UDJ_Out : public UDongJunStateBase
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void MouseTick(const float& DeltaTime)override;
	virtual void AnimPlayEnd() override;
private:
	UPROPERTY()
		bool bEnd = false;
	UPROPERTY()
		UTimeLineClass* CurTime = nullptr;
};


//发呆状态
UCLASS()
class UDJ_Idle : public UDongJunStateBase
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType) override;
	virtual void AnimPlayEnd() override;
public:
	void SpawnMouse(int32 Row);
private:
	UPROPERTY()
		int32 Count = 10;
private:
	UPROPERTY()
		bool bAction = false;
	UPROPERTY()
		int32 CurRow = 0;
};

//创建管道
UCLASS()
class UDJ_Create : public UDongJunStateBase
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void AnimPlayEnd() override;
	//生成管道
	void GenChannel(int32 Row);
public:
	UPROPERTY()
		ADJChannel* CurNewChannel = nullptr;
};

//重新修复管道
UCLASS()
class UDJ_RebuildChannel : public UDongJunStateBase
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void AnimPlayEnd() override;
	void Rebuild(ADJChannel* NewChannel);
private:
	UPROPERTY()
		ADJChannel* CurNewChannel = nullptr;
};

//跳入管道
UCLASS()
class UDJ_JumpInChannel : public UDongJunStateBase
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void MouseTick(const float& DeltaTime);
	virtual void AnimPlayEnd() override;
public:
	void InitChannel(class ADJChannel* CurChannel);
private:
	UPROPERTY()
		class ADJChannel* CurChannelObj = nullptr;
	UPROPERTY()
		UTimeLineClass* JumpTime = nullptr;
public:
	UPROPERTY()
		FVector CurLocation;
	UPROPERTY()
		FVector TargetLocation;
	UPROPERTY()
		bool bInit = false;
	UPROPERTY()
		float ConstTime = 5.f;
	UPROPERTY()
		float CurTime = 5.f;
};

//跳跃
UCLASS()
class UDJ_Jump : public UDongJunStateBase
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void MouseTick(const float& DeltaTime);
	virtual void AnimPlayEnd() override;
public:
	//跳跃方式
	static void DefJump(class UTimeLineClass* time, UObject* Obj, float Value);

	//完成动作
	static void DefFinish(class UTimeLineClass* time, UObject* Obj);
	//完成动作
	static void SecondFinish(class UTimeLineClass* time, UObject* Obj);

	static void LastFinish(class UTimeLineClass* time, UObject* Obj);

private:
	void NewDefJump();
	void NewSecondJump();
	void NewLastJump();
private:
	UPROPERTY()
		UTimeLineClass* JumpTimeLine = nullptr;
	UPROPERTY()
		int32 Count = 0;
	UPROPERTY()
		uint8 JumpType = 0u;
	UPROPERTY()
		bool bIdle = false;
public:
	UPROPERTY()
		FVector CurLocation;
	UPROPERTY()
		FVector TarLocation;
	UPROPERTY()
		int32 Col = 0;
};

//跳跃
UCLASS()
class UDJ_JumpEnd : public UDongJunStateBase
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void MouseTick(const float& DeltaTime)override;
private:
	UPROPERTY()
		UTimeLineClass* JumpTimeLine = nullptr;
};

//管道
UCLASS()
class FVM_API ADJChannel : public AMouseActor
{
	GENERATED_BODY()
public:
	//网格的碰撞盒子
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* MBoxMesh = nullptr;
	//身体的碰撞盒子
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* MBody = nullptr;
public:
	//管道入口
	UPROPERTY()
		FLine ChannelIn;
	//管道出口
	UPROPERTY()
		FLine ChannelOut;
public:
	ADJChannel();
	//初始化
	virtual void MouseInit() override;
	//受到攻击
	virtual bool BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType) override;
	//更新
	virtual void MouseTick(const float& DeltaTime) override;
	//死亡
	virtual void MouseDeathed() override;
	//受到攻击
	UFUNCTION(BlueprintImplementableEvent)
		void BPBeHit(float CurHP, float TotalHP);
	//初始化管道入口
	UFUNCTION(BlueprintImplementableEvent)
		void InitChannelIn(FLine NewLine);
	//初始化管道出口
	UFUNCTION(BlueprintImplementableEvent)
		void InitChannelOut(FLine NewLine);
	//显示
	UFUNCTION(BlueprintImplementableEvent)
		void Show();
	//隐藏
	UFUNCTION(BlueprintImplementableEvent)
		void Hidde();
	//开始
	void Begin();
	//结束
	UFUNCTION(BlueprintCallable)
		void End();
	//基本老鼠类型
	virtual EMouseBaseType GetMouseBaseType() override;
private:
	UPROPERTY()
		bool bEn = false;
	UPROPERTY()
		float bCheckTime = 0.2f;
	UPROPERTY()
		AMapMeshe* CurMapMeshe = nullptr;
	UPROPERTY()
		AMapMeshe* CurMapMesheIn = nullptr;
};

//洞君
UCLASS()
class FVM_API ABoss_DongJun : public ABossBase
{
	GENERATED_BODY()
public:
	//默认状态动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画|Def")
		FSoftObjectPath M_MouseDef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画|Def")
		FSoftObjectPath M_MouseLowDef;

	//从地上钻出来状态动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画|Out")
		FSoftObjectPath M_MouseOut;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画|Out")
		FSoftObjectPath M_MouseLowOut;

	//修建管道状态动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画|Build")
		FSoftObjectPath M_MouseBuild;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画|Build")
		FSoftObjectPath M_MouseLowBuild;

	//从地上钻下去状态动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画|In")
		FSoftObjectPath M_MouseIn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画|In")
		FSoftObjectPath M_MouseLowIn;

	//从地上钻下去状态动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画|Jump")
		FSoftObjectPath M_MouseJump;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画|Jump")
		FSoftObjectPath M_MouseLowJump;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "老鼠数据表")
		TSoftObjectPtr<UDataTable> MouseDataTable;

	//反应速度，越低反应越快
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "能力")
		int32 IdleCount = 6;

	//生成的老鼠
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "能力")
		TArray<FString> SpawnMouseNames;

	//爆炸资源
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "能力")
		TSoftClassPtr<class AMachineBombAnim> MouseBombRes;

	//老鼠管道资源
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "管道")
		TSoftClassPtr<ADJChannel> MouseChannelRes;

	//老鼠跳入管道的时间曲线
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "曲线")
		TSoftObjectPtr<UCurveFloat> MouseCurveJumpInChannel;


	UPROPERTY()
		UPaperFlipbook* M_MouseDef_res = nullptr;
	UPROPERTY()
		UPaperFlipbook* M_MouseLowDef_res = nullptr;

	UPROPERTY()
		UPaperFlipbook* M_MouseOut_res = nullptr;
	UPROPERTY()
		UPaperFlipbook* M_MouseLowOut_res = nullptr;

	UPROPERTY()
		UPaperFlipbook* M_MouseBuild_res = nullptr;
	UPROPERTY()
		UPaperFlipbook* M_MouseLowBuild_res = nullptr;

	UPROPERTY()
		UPaperFlipbook* M_MouseIn_res = nullptr;
	UPROPERTY()
		UPaperFlipbook* M_MouseLowIn_res = nullptr;

	UPROPERTY()
		UPaperFlipbook* M_MouseJump_res = nullptr;
	UPROPERTY()
		UPaperFlipbook* M_MouseLowJump_res = nullptr;

	//网格的碰撞盒子
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* M_BoxMesh = nullptr;
	//身体的碰撞盒子
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* M_BoxBody = nullptr;
public:
	//管道
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<int32, ADJChannel*> Channels;
	//地图等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MouseLevel = 1;
public:
	ABoss_DongJun();
	//老鼠更新
	virtual void MouseTick(const float& DeltaTime) override;
	//初始化
	virtual void BeginPlay() override;
	//老鼠初始化(Base BeginPlay自动调用一次)
	virtual void MouseInit() override;
	//移动(每0.02ms自动调用(允许移动的情况下))
	virtual void MoveingUpdate(float DeltaTime) override;
	//当老鼠被命中时受到的伤害数值(前提是isHurt为true  调用一次(一般由FlyitemActor命中时调用))
	virtual bool BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType) override;
	//老鼠死亡
	virtual void MouseDeathed() override;
	//执行buff
	virtual void ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime);
	//结束buff
	virtual void ExitBuff(EGameBuffTag BuffTag);
	//动画播放完毕
	void AnimPlayEnd();
	//设置碰撞
	void SetCollision(bool bEnable);
public:
	UMesheControllComponent* GetMeshe();
	UDongJunStateManager* GetMan();
private:
	//状态管理器
	UPROPERTY()
		UDongJunStateManager* StateMan = nullptr;
	UPROPERTY()
		UMesheControllComponent* CurMeshe = nullptr;
};
