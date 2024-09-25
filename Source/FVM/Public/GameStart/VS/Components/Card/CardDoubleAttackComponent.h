// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/VS/Components/Card/CardAttackComponent.h"
#include "CardDoubleAttackComponent.generated.h"

/**
 * 双重形态攻击
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UCardDoubleAttackComponent : public UCardAttackComponent
{
	GENERATED_BODY()
public:
	//第二发射位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* SecondBulletLauncherPointComp = nullptr;
public:
	//初始化
	virtual void BeginPlay() override;
	//资源加载
	virtual void LoadResource() override;
	//生成投射物并且设置属性
	virtual void Spawn() override;
	//生成投射物并且设置属性
	virtual void SpawnBullet(AFlyItemActor* NewBullet);
	//播放攻击动画
	virtual void PlayAttackAnimation() override;
	//销毁
	virtual void BeginDestroy() override;
	//更新
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	//初始化第二个攻击
	UFUNCTION(BlueprintCallable)
	void InitSecondLaunchBullet(
		TSoftClassPtr<AFlyItemActor> Res,
		TSoftClassPtr<class UAssetCategoryName> AnimName
	);
	//添加随机发射物【新增攻击动作】
	UFUNCTION(BlueprintCallable)
	void AddSecondLaunchBulletByRandomValue(
		int32 RandomValue,
		TSoftClassPtr<AFlyItemActor> Res,
		TSoftClassPtr<class UAssetCategoryName> AnimName
	);
private:
	UPROPERTY()
	bool bSInit = false;
	UPROPERTY()
	bool bSFirst = false;
	UPROPERTY()
	bool bSInitItemSort = false;
	//随机内容集合
	UPROPERTY()
	TArray<FCardOtherItem> SOtherItems;
	//对象池
	UPROPERTY()
	TArray<UObjectPoolManager*> SPool;
	//当前已经初始化完毕的子弹对象【全部的攻击动作】
	UPROPERTY()
	TMap<int32, FCardOtherItemInitFinish> CurSFinishItems;
	//当前生成的对象【当前使用的攻击动作】
	UPROPERTY()
	FCardOtherItem STargetCardOtherItem;
private:
	UPROPERTY()
	class UMouseManagerComponent* MouseComp = nullptr;
	UPROPERTY()
	class UMouseLineManager* LineComp = nullptr;
	UPROPERTY()
	class AMouseActor* CurSky = nullptr;
};
