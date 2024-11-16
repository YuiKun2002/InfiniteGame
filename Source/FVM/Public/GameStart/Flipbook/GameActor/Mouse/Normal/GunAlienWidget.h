// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/GunAlien.h"
#include "GunAlienWidget.generated.h"

/**
 * 持枪外星人，界面
 */
UCLASS()
class FVM_API AGunAlienWidget : public ANormalCapsuleByWidgetBase
{
	GENERATED_BODY()
public:
	AGunAlienWidget();
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
	//触发buff时
	virtual void ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime);
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
