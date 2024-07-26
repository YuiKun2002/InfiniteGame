// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "Data/CardData/MouseDataStruct.h"
#include "GameStart/VS/MapBaseType.h"
#include "RubbishCardMouse.generated.h"

/**
 * 垃圾车鼠
 */

class UBoxComponent;
class UPaperFlipbook;
class USphereComponent;

//垃圾车鼠
USTRUCT(BlueprintType)
struct FRubbishCardMouseResourceStruct {
	GENERATED_USTRUCT_BODY()
public:
	//跳跃
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Def;
	//跳跃残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> DefLow;
	//死亡
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> Death;
	//垃圾动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> RubbishGen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> RubbishFinish;
};

//障碍物
UCLASS()
class FVM_API ARubbishObstacle : public AObstacleBase
{
	GENERATED_BODY()
public:
	//初始化【动画资源,位置，渲染层，持续时间，伤害卡片的血量倍率】
	void Init(FRubbishCardMouseResourceStruct& Res, const FVector& Location, int32 Translucenty, float Time, float HurtCardRate);
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY()
		UPaperFlipbook* RubbishGen = nullptr;
	UPROPERTY()
		UPaperFlipbook* RubbishFinish = nullptr;
	UPROPERTY()
		float fRate = 0.f;
	UPROPERTY()
		float fTime = 0.f;
	UPROPERTY()
		float fSpawnDelay = 3.f;

	bool bFinish = false;
};

UCLASS()
class FVM_API ARubbishCardMouse : public ANormalBase
{
	GENERATED_BODY()
public:
	ARubbishCardMouse();
	//Tick 更新
	virtual void MouseTick(const float& DeltaTime) override;
	//初始化
	virtual void BeginPlay() override;
	//老鼠初始化
	virtual void MouseInit() override;
	//执行buff
	virtual void ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime);
	//移动(每0.02ms自动调用(允许移动的情况下))
	virtual void MoveingUpdate(float DeltaTime) override;
	//当老鼠被命中时受到的伤害数值(前提是isHurt为true  调用一次(一般由FlyitemActor命中时调用))
	virtual bool BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType) override;
	//当老鼠死亡时(当设置老鼠SetMouseDeath(true)时调用一次)
	virtual void MouseDeathed() override;
	//动画播放完毕
	void AnimationPlayEnd();
	//更新状态
	void UpdateState();

	//生成障碍物[通过蓝图调用]
	UFUNCTION(BlueprintCallable)
		void OnSpawnObstacle(AMapMeshe* CurMeshe);
public:
	//网格碰撞组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* MMesheComponent = nullptr;
	//身体碰撞组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* MBodyComponent = nullptr;
	//老鼠动画资产
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRubbishCardMouseResourceStruct MouseAnimalRes;
	UPROPERTY()
		ARubbishObstacle* LastRubbishObstacle = nullptr;
};
