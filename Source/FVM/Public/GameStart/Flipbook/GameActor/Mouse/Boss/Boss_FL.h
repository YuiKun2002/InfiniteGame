// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Boss/BossBase.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "Boss_FL.generated.h"

/**
 * 法老，法老变身
 */

 

UCLASS()
class FVM_API ABoss_FL : public ABossSphereBase
{
	GENERATED_BODY()
public:
	//是否开启基础形态【法老原形】
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "能力 | 默认形态")
		bool bBaseState = true;
	//出现
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 法老原形")
		TSoftObjectPtr<UPaperFlipbook> AnimA_Out;
	//出现残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 法老原形")
		TSoftObjectPtr<UPaperFlipbook> AnimA_OutLow;
	//发呆
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 法老原形")
		TSoftObjectPtr<UPaperFlipbook> AnimA_Idle;
	//发呆残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 法老原形")
		TSoftObjectPtr<UPaperFlipbook> AnimA_IdleLow;
	//伸出手臂
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 法老原形")
		TSoftObjectPtr<UPaperFlipbook> AnimA_InitArm;
	//伸出手臂残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 法老原形")
		TSoftObjectPtr<UPaperFlipbook> AnimA_InitArmLow;
	//手臂发呆
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 法老原形")
		TSoftObjectPtr<UPaperFlipbook> AnimA_ArmIdle;
	//手臂发呆残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 法老原形")
		TSoftObjectPtr<UPaperFlipbook> AnimA_ArmIdleLow;
	//使用右手
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 法老原形")
		TSoftObjectPtr<UPaperFlipbook> AnimA_UseRightArm;
	//使用右手残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 法老原形")
		TSoftObjectPtr<UPaperFlipbook> AnimA_UseRightArmLow;
	//使用左手
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 法老原形")
		TSoftObjectPtr<UPaperFlipbook> AnimA_UseLeftArm;
	//使用左手残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 法老原形")
		TSoftObjectPtr<UPaperFlipbook> AnimA_UseLeftArmLow;
	//退场
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 法老原形")
		TSoftObjectPtr<UPaperFlipbook> AnimA_Exit;
	//退场残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 法老原形")
		TSoftObjectPtr<UPaperFlipbook> AnimA_ExitLow;
	//回血动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 法老原形")
		TSoftObjectPtr<UPaperFlipbook> AnimA_HP;

	//变身
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 法老真身")
		TSoftObjectPtr<UPaperFlipbook> AnimB_Create;
	//死亡
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 法老真身")
		TSoftObjectPtr<UPaperFlipbook> AnimB_Death;
	//发呆
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 法老真身")
		TSoftObjectPtr<UPaperFlipbook> AnimB_Idle;
	//发呆残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 法老真身")
		TSoftObjectPtr<UPaperFlipbook> AnimB_IdleLow;
	//施法1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 法老真身")
		TSoftObjectPtr<UPaperFlipbook> AnimB_Use1;
	///施法1残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 法老真身")
		TSoftObjectPtr<UPaperFlipbook> AnimB_Use1Low;
	//施法2
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 法老真身")
		TSoftObjectPtr<UPaperFlipbook> AnimB_Use2;
	///施法2残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 法老真身")
		TSoftObjectPtr<UPaperFlipbook> AnimB_Use2Low;

	//淡入淡出
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 曲线")
		TSoftObjectPtr<UCurveFloat> MouseOpacityCurve;
	//移动
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 曲线")
		TSoftObjectPtr<UCurveFloat> MouseMoveCurve;

	//棺材箱资产
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "资产")
		TSoftClassPtr<class AFL_CallBox> CallMouseBoxRes;
	//绷带资产
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "资产")
		TSoftClassPtr<class AFL_CallBox> KillCardBoxRes;
	//甲虫洞资产
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "资产")
		TSoftClassPtr<class AFL_CallBox> BugBoxRes;
public:
	//当前存在的盒子
	UPROPERTY()
		TArray<AFL_CallBox*> CurValidBox;
public:
	ABoss_FL();
	//初始化
	virtual void BeginPlay() override;
	//老鼠初始化(Base BeginPlay自动调用一次)
	virtual void MouseInit() override;
	//执行buff
	virtual void ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime) override;
	//当老鼠死亡时(当设置老鼠SetMouseDeath(true)时调用一次)
	virtual void MouseDeathed() override;
public:
	UFUNCTION()
		bool RemoveCallBox(FLine Line);
};

//法老原形状态
UCLASS()
class FVM_API UFL_BaseState : public UBossStateBase
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void MouseTick(const float& DeltaTime) override;
	virtual void ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime) override;
public:
	//获取boss对象
	ABoss_FL* GetA();
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
private:
	UPROPERTY()
		UTimeLineClass* TimeLine = nullptr;
};

//出现
UCLASS()
class FVM_API UFL_Out : public UFL_BaseState
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void AnimPlayEnd();
};

//退出
UCLASS()
class FVM_API UFL_Exit : public UFL_BaseState
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void AnimPlayEnd();
};

//发呆
UCLASS()
class FVM_API UFL_Idle : public UFL_BaseState
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType);
	virtual void AnimPlayEnd();
private:
	UPROPERTY()
		int32 IdleCount = 5;
};
//-----------------------------------------------------------------------------------------------------
//召唤盒子
UCLASS()
class FVM_API AFL_CallBox : public ANormalBase
{
	GENERATED_BODY()
public:
	//生成
	UFUNCTION(BlueprintImplementableEvent)
		void SpawnBox(AMapMeshe* CurMeshe, FVector CurLocation);
	//默认
	UFUNCTION(BlueprintImplementableEvent)
		void OnDefAnim();
	//残血
	UFUNCTION(BlueprintImplementableEvent)
		void OnDefLowAnim();
	//死亡
	UFUNCTION(BlueprintImplementableEvent)
		void OnDeathed();
public:
	AFL_CallBox();
	//受到攻击
	virtual bool BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType) override;
	//执行buff
	virtual void ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime) override;
	//重新类型
	virtual EMouseBaseType GetMouseBaseType() override;
	//老鼠死亡
	virtual void MouseDeathed() override;
};

//老鼠召唤器
UCLASS()
class FVM_API AFL_CallMouseActor : public AActor
{
	GENERATED_BODY()
public:
	AFL_CallMouseActor();
	void Init(UFL_CallMouse* CurState);
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY()
		int32 CallCount = 2;
	UPROPERTY()
		float CallTimeDelay = 2.f;
private:
	UPROPERTY()
		UFL_CallMouse* State = nullptr;
};

//召唤老鼠
UCLASS()
class FVM_API UFL_CallMouse : public UFL_BaseState
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void AnimPlayEnd();
protected:
	UPROPERTY()
		bool bInit = false;
	UPROPERTY()
		bool bFinish = false;
public:
	void SpawnMouse();
};
//-----------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------

//销毁卡片召唤器
UCLASS()
class FVM_API AFL_KillCardActor : public AActor
{
	GENERATED_BODY()
public:
	AFL_KillCardActor();
	void Init(UFL_KillCard* CurState);
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY()
		int32 CallCount = 2;
	UPROPERTY()
		float CallTimeDelay = 2.f;
private:
	UPROPERTY()
		UFL_KillCard* State = nullptr;
};

//销毁卡片
UCLASS()
class FVM_API UFL_KillCard : public UFL_BaseState
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void AnimPlayEnd() override;
protected:
	UPROPERTY()
		bool bInit = false;
	UPROPERTY()
		bool bFinish = false;
public:
	void KillCard();
};
//-----------------------------------------------------------------------------------------------------


//回血
UCLASS()
class FVM_API UFL_ReverseHP : public UFL_BaseState
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void AnimPlayEnd() override;
	virtual void ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime) override;
protected:
	UPROPERTY()
		bool bInit = false;
	UPROPERTY()
		int32 UseCount = 5;
};
//-----------------------------------------------------------------------------------------------------

//甲虫召唤器
UCLASS()
class FVM_API AFL_CallBugActor : public AActor
{
	GENERATED_BODY()
public:
	void Init(UFL_CallBug* CurState);
};

//召唤甲虫
UCLASS()
class FVM_API UFL_CallBug : public UFL_BaseState
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void AnimPlayEnd() override;
};
