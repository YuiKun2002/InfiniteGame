// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameSystem/Tools/ObjectPoolManager.h"
#include "GameStart/VS/Components/Card/CardLauncherComponent.h"
#include "AttackLauncherComponent.generated.h"

#define SpineCardAnimationState_Idle TEXT("Idle")
#define SpineCardAnimationState_Attack TEXT("Attack")

//攻击动作
USTRUCT()
struct FCardOtherItem {
	GENERATED_USTRUCT_BODY()
public:
	FCardOtherItem() {};
	FCardOtherItem(
		int32 PoolIndex,
		int32 RValue,
		TSoftClassPtr<class AFlyItemActor> Res,
		FString AnimName
	);

	int32 GetValue() const;
	int32 GetIndex() const;
	UClass* GetRes() const;
	FString GetAnim() const;
private:
	//子弹类型
	UPROPERTY()
	UClass* CurRes = nullptr;
	//攻击动画
	UPROPERTY()
	FString ActionAnim;
	//触发值[随机值] 100为100%触发
	UPROPERTY()
	int32 CurValue = 100;
	//对应对象池的索引
	UPROPERTY()
	int32 CurPoolIndex = -1;
};

//攻击动作初始化完成
USTRUCT()
struct FCardOtherItemInitFinish {
	GENERATED_USTRUCT_BODY()
public:
	FCardOtherItemInitFinish() {}
	FCardOtherItemInitFinish(const TArray<FCardOtherItem>& Items) : Buffer(Items) { this->ConstCount = Items.Num(); }
	//所有的攻击动作
	const TArray<FCardOtherItem>& GetItems() const { return this->Buffer; }
	int32 GetCount() const { return this->ConstCount; }
private:
	//攻击动作数组
	UPROPERTY()
	TArray<FCardOtherItem> Buffer;
	UPROPERTY()
	int32 ConstCount = 0;
};


/**
 * 攻击发射器组件
 * 拥有【随机攻击特性】20%发射子弹A  30%发射子弹B  50%发射子弹C
 */
UCLASS()
class FVM_API UAttackLauncherComponent : public UCardLauncherComponent
{
	GENERATED_BODY()
protected:
	//游戏开始
	virtual void BeginPlay() override;
	//游戏更新
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//加载资源
	virtual void LoadResource() override;
	//生成投射物并且设置属性
	virtual void Spawn() override;
	//生成子弹
	virtual void SpawnBullet(AFlyItemActor* NewBullet);
	//播放攻击动画
	virtual void PlayAttackAnimation() override;
	//播放默认动画
	virtual void PlayIdleAnimation() override;
	//销毁
	virtual void BeginDestroy() override;
public:

	//初始化默认子弹对象
	UFUNCTION(BlueprintCallable)
	void InitLaunchBulletByDef(
		TSoftClassPtr<AFlyItemActor> BulletRes,
		const TSubclassOf<class UAssetCategoryName>& IdleName = nullptr,
		const TSubclassOf<class UAssetCategoryName>& AttackName = nullptr
	);

	//重新初始化默认攻击动画名称
	UFUNCTION(BlueprintCallable)
	virtual	void ReInitDefAttackAnimName(TSubclassOf<class UAssetCategoryName> AttackName);

	//重新初始化默认待机动画名称
	UFUNCTION(BlueprintCallable)
	virtual void ReInitDefIdleAnimName(TSubclassOf<class UAssetCategoryName> IdleName);

	//添加发射随机武器
	UFUNCTION(BlueprintCallable)
	virtual bool AddLaunchBulletByRandomValue(
		int32 RandomValue,
		TSoftClassPtr<AFlyItemActor> BulletRes,
		TSoftClassPtr<class UAssetCategoryName> AttackAnimName
	);

	//添加默认动画
	UFUNCTION(BlueprintCallable)
	void AddIdleAnimName(
		const TArray<TSubclassOf<class UAssetCategoryName>>& IdleAnimNames
	);

	//获取Idle动画
	UFUNCTION(BlueprintPure)
	FString GetIdleAnimName();
	//获取Attack动画
	UFUNCTION(BlueprintPure)
	FString GetAttackAnimName();
private:
	//是否初始化
	UPROPERTY()
	bool bInit = false;
	//首次发射
	UPROPERTY()
	bool bFirst = false;
	//初始化物品排序
	UPROPERTY()
	bool bInitItemSort = false;
	//随机数
	UPROPERTY()
	int32 RandomNumber = 0;
	//随机内容集合
	UPROPERTY()
	TArray<FCardOtherItem> OtherItems;
	//对象池
	UPROPERTY()
	TArray<UObjectPoolManager*> Pool;
	//当前已经初始化完毕的子弹对象【全部的攻击动作】
	UPROPERTY()
	TMap<int32, FCardOtherItemInitFinish> CurFinishItems;
	//当前生成的对象【当前使用的攻击动作】
	UPROPERTY()
	FCardOtherItem TargetCardOtherItem;
	//当前发呆动画组合
	UPROPERTY()
	TArray<FString> TargetIdleAnimationNames;
protected:
	//初始化发射器
	void InitLaunch(
		bool& InitItemSort,
		const TArray<FCardOtherItem>& Items,
		TMap<int32, FCardOtherItemInitFinish>& Out
	);
	//发射
	void LauncherItem(
		const TArray<FCardOtherItem>& Items,
		const TMap<int32, FCardOtherItemInitFinish>& OtherItemInitFinish,
		FCardOtherItem& Out
	);
	//初始化随机数
	void SetRandom();
	int32 GetRandom();
};
