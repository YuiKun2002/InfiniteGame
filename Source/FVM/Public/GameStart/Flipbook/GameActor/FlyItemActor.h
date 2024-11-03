// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpineActor.h"
#include "UObject/Interface.h"
#include "GameStart/VS/MapBaseType.h"
#include "GameSystem/Tools/ObjectPoolManager.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"
#include "GameStart/VS/Components/Item/ShootLineComponent.h"
#include "FlyItemActor.generated.h"

class AMouseActor;
class ACardActor;
class UActorComponent;
class UBoxComponent;
class USphereComponent;

//飞行物携带属性-BGM
USTRUCT(BlueprintType)
struct FlyItem_Property_AudioPath {
	GENERATED_USTRUCT_BODY()
public:
	//BGM的根目录名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_AudioRootPathName = FString(TEXT("ItemAudio"));
	//BGM的名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_AudioName = FString(TEXT(""));
};

//飞行物携带属性-动画资源(Fly & Split)
USTRUCT(BlueprintType)
struct FlyItem_Property_AnimRes {
	GENERATED_USTRUCT_BODY()
public:
	//飞行物动画资源Fly
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<class UAssetCategoryName> FlyItemDefAnimNameClass;
	//飞行物动画资源Split
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<class UAssetCategoryName> FlyItemSplitAnimNameClass;
public:
	UPROPERTY()
	FString FlyItemDefAnimName;
	UPROPERTY()
	FString FlyItemSplitAnimName;
};

//飞行物数据
USTRUCT(BlueprintType)
struct FlyItem_PropertyData {
	GENERATED_USTRUCT_BODY()
public:
	//当前的攻击线路(如果需要用到该条件)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_Line = 0;
	//攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ATK = 1.f;
	//当前攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurATK = 1.f;
	//第二攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _SecondATK = 0.f;
};

//飞行物条件
USTRUCT(BlueprintType)
struct FlyItem_PropertyCondition {
	GENERATED_USTRUCT_BODY()
public:
	//子弹攻击的类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFlyItemAttackType M_FlyItemAttackType = EFlyItemAttackType::Def;
	//穿透层数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PanetrateLayers = 999;
	//是否约束飞行物到本线路
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool M_bConstraintLine = false;
	//是否可以被其他组件使用(其他组件可以修改数据)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool M_bInfluence = false;
	//是否击中目标
	UPROPERTY()
	bool M_bHitTarget = false;
	//是否是静态飞行物
	UPROPERTY()
	bool M_bStaticFlyItem = false;
};

/*
子弹类
*/
UCLASS()
class FVM_API AFlyItemActor : public ASpineActor, public IObjectPoolInterface
{
	GENERATED_BODY()
public:
	//飞行物
	AFlyItemActor();
	//获取功能组件
	UActorComponent* const GetActor_CardFunction_Component();
	//设置组件
	void SetActor_CardFunction_Component(UActorComponent* _Value);
	//初始化碰撞
	void InitCollision();
	//初始化
	virtual void BeginPlay() override;
	//Tick 更新
	virtual void Tick(float DeltaTime) override;
	//当被对象池创建
	virtual void PoolCreate(class UObjectPoolManager* PoolManager) override;
	//对象初始化
	virtual void PoolInit(class UObjectPoolManager* PoolManager) override;
	//返回对象池
	UFUNCTION(BlueprintCallable)
	virtual bool ReturnPool() override;
	//动画播放完成
	UFUNCTION()
	void AnimComplete(UTrackEntry* Track);
	//初始化
	void Init();
public:
	//设置回池的时间【0表示永久存在】
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "存在时间")
	float ReturnTime = 0.f;
	UPROPERTY()
	float CurReturnTime = 0.f;
public:
	//可击打类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlyItemActorProperty | AttackType")
	TArray<ELineType> M_AttackType;
	//可击打卡片类型【优先级最高】
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlyItemActorProperty | AttackType")
	ECardCollisionType M_AttackCardType = ECardCollisionType::E_None;
	//携带的buff
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlyItemActorProperty | AddBuffer")
	FGameBuffInfor M_FItem_Buff;
	//飞行物条件
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlyItemActorProperty | Condition")
	FlyItem_PropertyCondition M_FlyCondition;
	//处于边界外被销毁
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlyItemActorProperty | Condition")
	bool M_OutsideAutoDestroy = true;
	//初始化BGM
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlyItemActorProperty | Audio | Begin")
	FlyItem_Property_AudioPath M_FlyItem_Property_AudioBegin;
	//结束BGM
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlyItemActorProperty | Audio | End")
	FlyItem_Property_AudioPath M_FlyItem_Property_AudioEnd;
	//静态对象池(线路约束，限制到本行)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlyItemActorProperty | StaticItems")
	bool M_StaticFlyItemLocationConstLine = true;
	//静态对象池(是否只创建一次，如果遇到穿透类型，可能多次创建)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlyItemActorProperty | StaticItems")
	bool bCreateStaticItemOnceOverride = false;
	//静态对象池(有几个就会创建几个)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlyItemActorProperty | StaticItems")
	TArray<TSoftClassPtr<AFlyItemActor>> M_StaticFlyItemClass;
	//对象死亡静态对象池(有几个就会创建几个)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlyItemActorProperty | StaticItems")
	TArray<TSoftClassPtr<AFlyItemActor>> M_TargetDeathStaticFlyItemClass;
	//飞行物动画资源
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlyItemActorProperty | FlyItemAnimationRes")
	FlyItem_Property_AnimRes M_FlyItem_Property_AnimRes;
public:
	//飞行物碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* M_SphereCollision = nullptr;
	//是否返回了对象池
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bReturnPool = true;
public:
	//设置浮动模式
	UFUNCTION(BlueprintCallable)
	void SetFloatModeEnable(bool _Value);
	//设置飞行物约束
	UFUNCTION(BlueprintCallable)
	void SetFlyConstraintLine(bool _value);
	//设置可以被其他对象修改(影响)
	UFUNCTION(BlueprintCallable)
	void SetFlyInfluence(bool _value);
	//设置攻击力
	UFUNCTION(BlueprintCallable)
	void SetATK(float _value);
	//设置当前攻击力
	UFUNCTION(BlueprintCallable)
	void SetCurATK(float _value);
	//设置第二攻击力
	UFUNCTION(BlueprintCallable)
	void SetSecondATK(float _value);
	//设置线路
	UFUNCTION(BlueprintCallable)
	void SetLine(int32 _line);
	//设置对象Actor
	UFUNCTION(BlueprintCallable)
	void SetObjectActorLocation(AActor* _MouseActor);
	//设置目标击中状态
	UFUNCTION(BlueprintCallable)
	void SetTargetHitState(bool _Value);
	//碰撞的开启与关闭
	UFUNCTION(BlueprintCallable)
	void SetCollisionEnabled(bool _Value);
	//设置回池时间
	UFUNCTION(BlueprintCallable)
	void SetReturnPoolTime(float Time = 0.f);
	//设置动画轨道
	UFUNCTION()
	void SetTrackEntry(class UTrackEntry* Track);
	//设置上次的对象
	UFUNCTION(BlueprintCallable)
	void SetLastObjectActor(AActor* ObjectActor);
	//设置父项
	UFUNCTION(BlueprintCallable)
	void SetFlyParentActor(AActor* NewParent);
public:
	//重置Buff
	UFUNCTION(BlueprintCallable)
	void ResetBuff(
		const TMap<EGameBuffTag, float>& Buffs,
		const TMap<EGameBuffTag, TSubclassOf<UBuffDynamicProperty>>& CurBuffPropertys
	);
	//添加可以攻击类型
	void AddAttackType(TArray<ELineType>& _types);
	//添加可以攻击类型
	UFUNCTION(BlueprintCallable)
	void AddAttackType(ELineType _type);
public:
	//获取父项
	UFUNCTION(BlueprintPure)
	class AActor* GetFlyParentActor();
	//获取动画轨道
	UFUNCTION(BlueprintPure)
	class UTrackEntry* GetTrackEntry();
	//获取是否提前击中了老鼠
	UFUNCTION(BlueprintCallable)
	bool GetFirstHitResult() const;
	//获取是否处于边界之外被销毁
	UFUNCTION(BlueprintCallable)
	bool GetFlyOutsideAutoDestroy();
	//获取约束
	UFUNCTION(BlueprintCallable)
	bool GetFlyConstraintLine();
	//获取攻击力
	UFUNCTION(BlueprintCallable)
	float GetATK();
	//获取当前攻击力
	UFUNCTION(BlueprintCallable)
	float GetCurATK();
	//获取第二攻击力
	UFUNCTION(BlueprintCallable)
	float GetSecondATK();
	//获取线路
	UFUNCTION(BlueprintCallable)
	int32 GetLine();
	//当前飞行物是否可以被影响
	UFUNCTION(BlueprintCallable)
	bool GetFlyInfluence();
	//获取对象的位置
	UFUNCTION(BlueprintCallable)
	FVector GetObjectActorLocation();
	//获取buff
	UFUNCTION(BlueprintCallable)
	FGameBuffInfor& GetBuff();
	//获取对象的ActorLocation
	UFUNCTION(BlueprintCallable)
	AActor* const GetObjectActor();
	UFUNCTION(BlueprintCallable)
	AActor* const GetLastObjectActor();
	//获取可以攻击的类型
	UFUNCTION(BlueprintCallable)
	TArray<ELineType>& GetAttackType();
	//目标是否被击中
	UFUNCTION(BlueprintCallable)
	bool GetDidTargetIsHit();
public:
	//播放开始动画
	UFUNCTION(BlueprintCallable)
	void PlayAnim_Fly();
	//播放结束动画
	UFUNCTION(BlueprintCallable)
	void PlayAnim_Split();
	//击中结果
	UFUNCTION(BlueprintCallable)
	void HitEnd(UPrimitiveComponent* _UBoxComp);
	//击中(由老鼠或者其他对象调用)
	UFUNCTION(BlueprintCallable)
	void Hit();
	//当击中目标时
	UFUNCTION(BlueprintImplementableEvent)
	void OnHit(EFlyItemAttackType AttackType);
	//当从对象池拿取时
	UFUNCTION(BlueprintImplementableEvent)
	void OnPoolInit();
	//当初始化时
	UFUNCTION(BlueprintImplementableEvent)
	void OnInit();
	//当被返回时
	UFUNCTION(BlueprintImplementableEvent)
	void OnReturn();
	//创建一个静态物品->作用可以进行二次伤害（范围伤害等）
	UFUNCTION(BlueprintCallable)
	void CreateStaticItem(TSoftClassPtr<AFlyItemActor> CurFlyItemActor);
	//创建一个flyActor直线
	UFUNCTION(BlueprintCallable)
	void CreateFlyActor_ShootLine(
		TSoftClassPtr<AFlyItemActor> _FlyActorPath_C,
		FTargetNode _Node,
		int32 _LineOffset,
		float _Time = 10.f,
		FVector Offset = FVector(0.f),
		bool _IsbConstaintLine = true,
		FString ObjPoolID = TEXT(""),
		bool InheritProperty = false
	);
	//创建一个flyActor斜线
	UFUNCTION(BlueprintCallable)
	void CreateFlyActor_ShootLine_Slash(
		TSoftClassPtr<AFlyItemActor> _FlyActorPath_C,
		float _RotationAngle = 45.f,
		float _Time = 10.f,
		FVector Offset = FVector(0.f),
		FString ObjPoolID = TEXT("")
	);
	//飞行物交替
	UFUNCTION(BlueprintCallable)
	AFlyItemActor* FlyItemActorSwap(AFlyItemActor* _FlyActor);
public:
	//更新旋转360
	UFUNCTION(BlueprintCallable)
	void UpdateRatation360(float _deltaTime, USceneComponent* _Comp);
public:
	//与老鼠发生重叠
	UFUNCTION(BlueprintCallable)
	void HitMouse_OverlapBegin(AActor* _Mouse);
	//与卡片发生重叠
	UFUNCTION(BlueprintCallable)
	void HitCard_OverlapBegin(AActor* _Card);
public:
	//当飞行物开始时与其他对象重叠
	UFUNCTION()
	void OnBoxOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	//当飞行物与其他对象重叠结束时
	UFUNCTION()
	void OnBoxOverlapEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);
private:
	//击中对象的位置
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AActor* M_ObjectActorLocation = nullptr;
	//当前击中的对象
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AActor* M_CurrentHitObjectActor = nullptr;
	//上一次击中的对象
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AActor* M_LastHitObjectActor = nullptr;
	//飞行物数据
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FlyItem_PropertyData M_FlyData;
	//FristHit首次击中结果，投掷物判断【防止二次击中】
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bFirstHitResult = false;
	//发射者
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AActor* ParentActor = nullptr;
private:
	//防止卡片的功能组件（某些功能）重复使用飞行物
	UPROPERTY()
	UActorComponent* M_AActorComponent_CardFunction = nullptr;
	//如果开启了浮动模式（子弹将不在受行限制）
	UPROPERTY()
	bool M_bFloatMode = false;
	//对象池管理器
	UPROPERTY()
	UObjectPoolManager* CurPoolManager = nullptr;
	//动画轨道
	UPROPERTY()
	class UTrackEntry* AnimTrackEntry = nullptr;
};
