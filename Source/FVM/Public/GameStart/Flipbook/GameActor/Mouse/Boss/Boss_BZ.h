// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Boss/BossBase.h"
#include "Boss_BZ.generated.h"

/**
 *
 */
UCLASS()
class FVM_API ABoss_BZ : public ABossSphereBase
{
	GENERATED_BODY()
public:
	//当前初始状态【true 冰渣 false 火渣】
	UPROPERTY()
		bool bDoubleState = true;
	//出现
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 冰渣")
		TSoftObjectPtr<UPaperFlipbook> AnimA_Out;
	//出现残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 冰渣")
		TSoftObjectPtr<UPaperFlipbook> AnimA_OutLow;
	//退场
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 冰渣")
		TSoftObjectPtr<UPaperFlipbook> AnimA_Exit;
	//退场残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 冰渣")
		TSoftObjectPtr<UPaperFlipbook> AnimA_ExitLow;
	//使用冰球
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 冰渣")
		TSoftObjectPtr<UPaperFlipbook> AnimA_IceSphere;
	//使用冰球
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 冰渣")
		TSoftObjectPtr<UPaperFlipbook> AnimA_IceSphereLow;
	//推冰球
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 冰渣")
		TSoftObjectPtr<UPaperFlipbook> AnimA_PushIceSphere;
	//推冰球
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 冰渣")
		TSoftObjectPtr<UPaperFlipbook> AnimA_PushIceSphereLow;
	//法杖攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 冰渣")
		TSoftObjectPtr<UPaperFlipbook> AnimA_Atk;
	//法杖攻击残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 冰渣")
		TSoftObjectPtr<UPaperFlipbook> AnimA_AtkLow;
	//死亡
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 冰渣")
		TSoftObjectPtr<UPaperFlipbook> AnimA_Death;

	//出现
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 火渣")
		TSoftObjectPtr<UPaperFlipbook> AnimB_Out;
	//出现残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 火渣")
		TSoftObjectPtr<UPaperFlipbook> AnimB_OutLow;
	//退场
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 火渣")
		TSoftObjectPtr<UPaperFlipbook> AnimB_Exit;
	//退场残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 火渣")
		TSoftObjectPtr<UPaperFlipbook> AnimB_ExitLow;
	//使用火球
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 火渣")
		TSoftObjectPtr<UPaperFlipbook> AnimB_FireSphere;
	//使用火球残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 火渣")
		TSoftObjectPtr<UPaperFlipbook> AnimB_FireSphereLow;
	//推火球
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 火渣")
		TSoftObjectPtr<UPaperFlipbook> AnimB_PushFireSphere;
	//推火球残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 火渣")
		TSoftObjectPtr<UPaperFlipbook> AnimB_PushFireSphereLow;
	//法杖攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 火渣")
		TSoftObjectPtr<UPaperFlipbook> AnimB_Atk;
	//法杖攻击残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 火渣")
		TSoftObjectPtr<UPaperFlipbook> AnimB_AtkLow;
	//死亡
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "动画 | 火渣")
		TSoftObjectPtr<UPaperFlipbook> AnimB_Death;
public:
	//冰渣构造
	ABoss_BZ();
	//初始化
	virtual void BeginPlay() override;
	//老鼠初始化(Base BeginPlay自动调用一次)
	virtual void MouseInit() override;
	//执行buff
	virtual void ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime) override;
	//当老鼠死亡时(当设置老鼠SetMouseDeath(true)时调用一次)
	virtual void MouseDeathed() override;
public:
	//生成子弹
	UFUNCTION(BlueprintImplementableEvent)
		void OnSpawnBullet(bool CurState,const FVector& CurLocation, const FVector& TargetLocation);
	//生成球体
	UFUNCTION(BlueprintImplementableEvent)
		void OnSpawnElementSphere(bool CurState, FLine Line, const FVector& TargetLocation);
};

//冰渣通用状态基本类型
UCLASS()
class FVM_API UBZ_BaseState : public UBossUniformState
{
	GENERATED_BODY()
public:
	ABoss_BZ* GetBZ();
	virtual void BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType) override;
	virtual void ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime) override;
};

//入场
UCLASS()
class FVM_API UBZ_OutState : public UBZ_BaseState
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void AnimPlayEnd() override;
};

//退场
UCLASS()
class FVM_API UBZ_ExitState : public UBZ_BaseState
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void MouseTick(const float& DeltaTime) override;
	virtual void AnimPlayEnd() override;
private:
	UPROPERTY()
		float fTime = 10.f;
};

//魔法攻击
UCLASS()
class FVM_API UBZ_MagicATKState : public UBZ_BaseState
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void AnimPlayEnd() override;
private:
	UPROPERTY()
		int32 bCount = 3;
};

//元素球体攻击
UCLASS()
class FVM_API UBZ_ElementSphereATKState : public UBZ_BaseState
{
	GENERATED_BODY()
public:
	virtual	void Init() override;
	virtual void AnimPlayEnd() override;
private:
	UPROPERTY()
		int32 Count = 5;
	UPROPERTY()
		bool bInit = false;
};

