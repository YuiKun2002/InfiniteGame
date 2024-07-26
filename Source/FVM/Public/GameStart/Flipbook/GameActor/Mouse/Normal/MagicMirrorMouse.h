// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "GameSystem/Item/Card/CardBaseStruct.h"
#include "MagicMirrorMouse.generated.h"

/**
 * 魔镜老鼠
 */

USTRUCT(BlueprintType)
struct FMagicMirrorMouseResourceStruct {
	GENERATED_USTRUCT_BODY()
public:
	//每多少秒吸取一次【在动画时长的范围内】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TakeSpeed = 0.25f;
	//每次吸取多少能量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 TakeEnergyCount = 5;
	//正常形态(No Attack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Def;
	//残血形态(No Attack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> DefLow;
	//攻击(Attack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> ATK;
	//残血形态(Attack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> ATKLow;
	//吸取
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Take;
	//吸取残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> TakeLow;
	//死亡形态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Death;
};

UCLASS()
class FVM_API AMagicMirrorMouse : public ANormalBase
{
	GENERATED_BODY()
public:
	//网格碰撞组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* MMesheComponent = nullptr;
	//身体碰撞组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USphereComponent* MBodyComponent = nullptr;
	//动画资源
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "老鼠动画资源")
		FMagicMirrorMouseResourceStruct AnimRes;
	//可以攻击卡片的什么类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECardCollisionType MECardCollisionType = ECardCollisionType::E_CardActor2;
public:
	AMagicMirrorMouse();
	//初始化
	virtual void BeginPlay() override;
	//初始化
	virtual void MouseInit() override;
	//更新
	virtual void Tick(float DeltaTime) override;
	//动画播放完毕
	UFUNCTION()
		void OnAnimationPlayEnd();
public:
	//移动逻辑函数
	virtual void MoveingUpdate(float DeltaTime) override;
public:
	//当老鼠被命中时受到的伤害数值
	virtual bool BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType) override;
	//老鼠开始攻击
	virtual void AttackedBegin() override;
	//老鼠攻击结束
	virtual void AttackedEnd() override;
	//老鼠开始移动
	virtual void MoveingBegin() override;
	//当老鼠死亡时
	virtual void MouseDeathed() override;

	//执行buff
	virtual void ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime);
	//结束buff
	virtual void ExitBuff(EGameBuffTag BuffTag);
public:
	//更新状态
	void UpdateState();
private:
	UPROPERTY()
		class UResourceManagerComponent* CurRes = nullptr;

	//攻击线
	bool bAttackLine = true;
	//吸取
	bool bTake = false;
	//被控制
	bool bControll = false;
	//吸取时间延迟
	float fTakeDelay = 15.f;
	//吸取速度
	float fTakeSpeed = 0.15f;
};