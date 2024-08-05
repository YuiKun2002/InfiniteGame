// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/VS/Components/Card/CardLauncherComponent.h"
#include "GameSystem/Tools/ObjectPoolManager.h"
#include "GameStart/VS/MapBaseType.h"
#include "CardAttackComponent.generated.h"

class AAttackCardActor;
class AMapMouseMesheManager;
class UMesheControllComponent;

//攻击动作
USTRUCT()
struct FCardOtherItem {
	GENERATED_USTRUCT_BODY()
public:
	FCardOtherItem() {};
	FCardOtherItem(
		int32 PoolIndex,
		int32 RValue,
		TSoftClassPtr<AFlyItemActor> Res,
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
 * 卡片攻击组件【一般直线攻击卡片】小笼包，三线，双向等这种类型的基础卡片
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UCardAttackComponent : public UCardLauncherComponent
{
	GENERATED_BODY()
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//生成投射物并且设置属性
	virtual void Spawn() override;
	//播放攻击动画
	virtual void PlayAttackAnimation() override;
	//播放默认动画
	virtual void PlayIdleAnimation() override;
	//销毁
	virtual void BeginDestroy() override;
public:

	//资源加载
	UFUNCTION(BlueprintCallable)
	virtual void LoadResource() override;

	//添加随机发射物【新增攻击动作】
	UFUNCTION(BlueprintCallable)
	bool AddLaunchRadomItem(
		int32 RandomValue,
		TSoftClassPtr<AFlyItemActor> Res,
		TSoftClassPtr<class UAssetCategoryName> AnimName
	);

	//当动画播放完毕时触发
	UFUNCTION()
	void OnAnimationComplete(class UTrackEntry* Track);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	//攻击卡
	UPROPERTY(EditAnywhere)
	AAttackCardActor* AttackCardActor = nullptr;
private:
	UPROPERTY()
	float time = 0.f;
	UPROPERTY()
	bool bFirst = false;
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
	//当前发呆动画
	UPROPERTY()
	FString TargetIdleAnimationName;
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
