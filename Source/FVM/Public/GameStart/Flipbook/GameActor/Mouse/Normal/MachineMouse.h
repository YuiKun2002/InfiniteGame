// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "MachineMouse.generated.h"

/**
 * 机器鼠
 */

USTRUCT(BlueprintType)
struct FMachineMouseResourceStruct {
	GENERATED_USTRUCT_BODY()
public:
	//正常形态(No Attack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> Def;
	//正常形态(No Attack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> DefLow;
	//正常形态(No Attack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> Idle;
	//正常形态(No Attack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> IdleLow;
	//死亡(No Attack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> Death;
};

//机器鼠爆炸动画
UCLASS()
class FVM_API AMachineBombAnim : public AGameActorFlipbookBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnInit();
};

UCLASS()
class FVM_API AMachineMouse : public ANormalCapsuleByWidgetBase
{
	GENERATED_BODY()
public:
	//爆炸范围
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BombRadius = 20.f;
	//检测卡片类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECardCollisionType CheckCardType = ECardCollisionType::E_CardActor2;
public:
	virtual void MouseTick(const float& DeltaTime) override;
	//初始化
	virtual void BeginPlay() override;
	//老鼠初始化
	virtual void MouseInit() override;
	//攻击
	virtual void AttackedBegin() override;
	//攻击结束
	virtual void AttackedEnd() override;
	//开始移动
	virtual void MoveingBegin() override;
	//移动(每0.02ms自动调用(允许移动的情况下))
	virtual void MoveingUpdate(float DeltaTime) override;
	//当老鼠死亡时(当设置老鼠SetMouseDeath(true)时调用一次)
	virtual void MouseDeathed() override;
	//动画播放完毕
	UFUNCTION()
	void AnimationPlayEnd(class UTrackEntry* Track);
	//播放走
	void PlayWalk();
	//播放攻击
	void PlayAttack();
	//生成爆炸
	UFUNCTION(BlueprintImplementableEvent)
	void OnSpawnBomb();
private:
	UPROPERTY()
	int32 State = -1;
	UPROPERTY()
	bool bAttackLine = true;
};
