// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "GunAlien.generated.h"

class AFlyItemActor;
class UAlienAttackLauncherComponent;

//持枪外星人
USTRUCT(BlueprintType)
struct FMouseGunAlienAnimation_Anim {
	GENERATED_USTRUCT_BODY()
public:
	//走动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UAssetCategoryName> WalkAnimName;
};

//线路基本配置
USTRUCT(BlueprintType)
struct FGunAlienCheckLineTraceBaseConfig {
	GENERATED_USTRUCT_BODY()
public:
	//线路偏移-行
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 OffsetLine = 0;
	//检测卡片类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECardCollisionType CheckCardType = ECardCollisionType::E_CardActor2;
	//检测格子数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CheckGridLength = 1;
};

/**
 * 持枪的外星人
 */
UCLASS()
class FVM_API AGunAlien : public ANormalCapsuleBase
{
	GENERATED_BODY()
public:
	AGunAlien();
public:
	UPROPERTY()
	UAlienAttackLauncherComponent* AttackLauncherComp = nullptr;
	//动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性")
	FMouseGunAlienAnimation_Anim AnimNames;
	//默认子弹
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性")
	TSoftClassPtr<AFlyItemActor> AlienDefBulletClass;
	//线路检测
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性")
	TArray<FGunAlienCheckLineTraceBaseConfig> CheckLineTraceConfig;
public:
	virtual void BeginPlay() override;
	//老鼠初始化
	virtual void MouseInit() override;
	//老鼠更新
	virtual void MouseTick(const float& DeltaTime) override;
	//移动逻辑函数
	virtual void MoveingUpdate(float DeltaTime) override;
	//当老鼠被命中时受到的伤害数值
	virtual bool BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType) override;
	//老鼠开始移动
	virtual void MoveingBegin() override;
	//当老鼠死亡时
	virtual void MouseDeathed() override;
	//进入网格时
	virtual void InMapMeshe(ELineType CurLineType) override;
	//动画播放完毕
	UFUNCTION()
	void OnAnimationPlayEnd(class UTrackEntry* Track);
private:
	//检测卡片
	void CheckCard();
	//播放走路动画
	void PlayMoveAnim();
	//播放攻击动画
	void PlayAttackAnim();
private:
	//状态
	int32 State = -1;
};
