// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "Data/CardData/MouseDataStruct.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "RepairMouse.generated.h"

/**
 * 修理鼠
 */

 //修理鼠
USTRUCT(BlueprintType)
struct FRepairMouseResourceStruct {
	GENERATED_USTRUCT_BODY()
public:
	//防具基础血量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DefenceItemBaseHP = 20.f;
	//防具基础当前血量
	UPROPERTY()
		float DefenceItemCurHP = 20.f;

	//全状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Take;
	//全状态放置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> TakeUse;

	//防御残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> TakeLow;
	//防御残血放置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> TakeLowUse;

	//防具正常，角色残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> TakeCharLow;
	//防具正常，角色残血放置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> TakeCharLowUse;

	//防具和角色残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> TakeAllLow;
	//防具和角色残血放置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> TakeAllLowUse;

	//防具掉正常落，角色
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> TakeFallChar;
	//防具掉落，角色残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> TakeFallCharLow;
	//无防具
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> CharDef;
	//无防具残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> CharLow;

	//全状态攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> TakeAtk;
	//防具残血角色正常攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> TakeLowAtk;
	//防具正常角色残血攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> TakeCharLowAtk;
	//防具残血角色残血攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> TakeCharAllLowAtk;
	//无防具角色正常攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> CharAtk;
	//无防具残血角色正常攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> CharLowAtk;

	//死亡
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Death;

	//是否使用防具
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bItemUse = false;
	//防御道具是否已经掉落
	UPROPERTY()
		bool bItemRemove = false;
};

class UBoxComponent;
class USphereComponent;


//修理鼠的扶梯类
UCLASS()
class FVM_API ARepairMouseEscalator : public AActor
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
		void Init(bool bState, int32 Translucency, FVector Location, class ACardActor* CurCard);
};

UCLASS()
class FVM_API ARepairMouse : public ANormalBase
{
	GENERATED_BODY()

		friend class URepairMouseStateManager;
public:
	//网格碰撞组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* MMesheComponent = nullptr;
	//身体碰撞组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* MBodyComponent = nullptr;
	//修理鼠的手扶梯对象
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftClassPtr<ARepairMouseEscalator> RepairMouseEscalator;
	//老鼠的动画资源
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRepairMouseResourceStruct M_NormalDefenceMouseResourceStruct;
	//可以攻击卡片的什么类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECardCollisionType M_ECardCollisionType = ECardCollisionType::E_CardActor2;
public:
	ARepairMouse();
	//初始化
	virtual void BeginPlay() override;
	virtual void MouseInit() override;
	//更新
	virtual void Tick(float DeltaTime) override;
	virtual void MouseTick(const float& DeltaTime) override;
	//动画播放完毕
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
private:
	//是否启动攻击线
	UPROPERTY()
		bool M_bEnableAttakLine = true;

	//因为受到攻击更新状态
	void UpdateState(const float& HurtValue, const EFlyItemAttackType& AttackType);
	//其他情况更新状态
	void UpdateState();
	//设置动画/*[Anim1 atk & take]  [Anim2 atk & !take] [Anim3 !atk& take] [Anim4 !atk & !take]   */
	void SetAnim(
		const bool& bAtk, const bool& bTake,
		TSoftObjectPtr<UPaperFlipbook> Anim1, TSoftObjectPtr<UPaperFlipbook> Anim2,
		TSoftObjectPtr<UPaperFlipbook> Anim3, TSoftObjectPtr<UPaperFlipbook> Anim4);

	//开始使用扶梯
	bool bUseFirst = false;
	bool bSecond = false;
	bool bState = true;
};