// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "GunAlien.generated.h"

class AFlyItemActor;
class UAlienAttackLauncherComponent;

//持枪外星人
USTRUCT(BlueprintType)
struct FMouseGunAlienLauncherConfig {
	GENERATED_USTRUCT_BODY()
public:
	//攻击次数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AttackCount = 1;
	//攻击速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeed = 3.f;
	//首次攻击速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FirstAttackSpeed = 0.5f;
	//子弹攻击速度间隔
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SecondAttackSpeed = 0.1f;

	//走动画
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UAssetCategoryName> WalkAnimName;*/
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
	//攻击发射组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAlienAttackLauncherComponent* AttackLauncherComp = nullptr;
	//发射位置组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* LauncherBulletLocationComp = nullptr;
	//射击攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性")
	int32 ShootATK = 5;
	//发射属性
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性")
	FMouseGunAlienLauncherConfig LauncherConfig;
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
	//移动逻辑函数
	virtual void MoveingUpdate(float DeltaTime) override;
	//当老鼠死亡时
	virtual void MouseDeathed() override;
	//进入网格时
	virtual void InMapMeshe(ELineType CurLineType) override;
	//动画播放完毕
	UFUNCTION()
	void OnAnimationPlayEnd(class UTrackEntry* Track);
public:
	FVector GetBulletLauncherLocation();
public:
	//播放走路动画
	void PlayMoveAnim();
	//播放攻击动画
	void PlayAttackAnim();
	//播放发呆动画
	void PlayIdleAnim();
	//检测卡片
	virtual	void CheckCard();
	//设置远程
	void SetShootEnable(bool Value);
	//获取是否进行远程
	bool GetbShootEnabled();
	//获取是否在攻击
	bool GetbShoot();
protected:
	//状态
	UPROPERTY()
	int32 State = -1;
	//检测计时器
	UPROPERTY()
	float Time = 0.f;
	//是否在射击
	UPROPERTY()
	bool bIsShoot = false;
	//启动远程
	UPROPERTY()
	bool bShootEnabled = true;
};
