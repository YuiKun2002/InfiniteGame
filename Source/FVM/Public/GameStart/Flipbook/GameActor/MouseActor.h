// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpineActor.h"
#include "Data/GameBuff.h"
#include <Components/BoxComponent.h>
#include "GameStart/VS/MapBaseType.h"
#include <Kismet/KismetSystemLibrary.h>
#include "GameSystem/GameDataSubsystem.h"
#include "SpineSkeletonAnimationComponent.h"
#include "GameSystem/Item/Card/CardBaseStruct.h"
#include "GameStart/Flipbook/GameActorFlipbookBase.h"
#include "GameStart/VS/Components/Alien/WaterSceneComponent.h"
#include "GameStart/VS/Components/MouseManagerComponent.h"
#include "MouseActor.generated.h"

class UBoxComponent;
class ACardActor;
class AFlyItemActor;
class UMouseManagerComponent;
class UBoxComponent;

#define SpineMouseDeath TEXT("Death")


#define  BINDANIMATION(Anim,Object,Func)\
	Anim->AnimationComplete.AddDynamic(\
	Object,\
	Func\
	)\

//障碍物对象
UCLASS()
class FVM_API AObstacleBase : public AGameActorFlipbookBase
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FMouseDefAnimation_Anim {
	GENERATED_USTRUCT_BODY()
public:
	//发呆动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UAssetCategoryName> IdleAnimRes;
	//受到伤害的发呆动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UAssetCategoryName> IdleDamageAnimRes;
	//走动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UAssetCategoryName> WalkAnimRes;
	//受到伤害的走动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UAssetCategoryName> WalkAnimDamageRes;
	//攻击动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UAssetCategoryName> AttackAnimRes;
	//受到伤害的攻击动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UAssetCategoryName> AttackAnimDamageRes;
	//死亡动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UAssetCategoryName> DeadAnimRes;
};

USTRUCT(BlueprintType)
struct FMouseProperty_HP {
	GENERATED_USTRUCT_BODY()
public:
	//总生命值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_HP = 10.f;
	//老鼠当前生命值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_CurrentHP = 10.f;
	//免伤值【不允许此属性为常驻属性】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_IgnoreHurtValue = 0.0f;
};

USTRUCT(BlueprintType)
struct FMouseProperty_ATK {
	GENERATED_USTRUCT_BODY()
public:
	//老鼠的总攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_ATK = 0.f;
	//老鼠的当前攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_CurrentATK = 0.f;
	//是否在攻击
	UPROPERTY()
	bool IsAttack = false;
	//是否受到伤害
	UPROPERTY()
	bool IsHurt = false;
	//受到的伤害值
	UPROPERTY()
	float HurtValue = 0.f;
};

USTRUCT(BlueprintType)
struct FMouseProperty_State {
	GENERATED_USTRUCT_BODY()
public:
	//移动速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_MoveSpeed = 15.f;
	//当前移动速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_CurrentMoveSpeed = 15.f;
	//当前老鼠死亡后的经验值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_CurrentMouseEx = 1;
	//老鼠的标记
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMouseTag M_MouseTag = EMouseTag::NormalGround;
	//是否可以移动
	UPROPERTY()
	bool M_IsMove = true;
	//是否死亡
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDeath = false;
	//是否死亡
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool MouseDeath = false;
	//是否播放死亡动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPlayDeathAnim = false;
	//是否不可以使用Tick
	UPROPERTY()
	bool IsNotTick = false;
	//当前透明度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurOpacity = 1.f;
};

//一些条件
USTRUCT(BlueprintType)
struct FMouseProperty_Condition {
	GENERATED_USTRUCT_BODY()
public:
	//老鼠的类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELineType M_EMouseType;
	//当前进入的网格类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELineType M_CurrentInType = ELineType::OnGround;
	//当前所在的行
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLine M_Line;
	//所在的回合
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_AttachRound = 0;
	//播放速度
	UPROPERTY()
	float M_PlayRate = 1.f;
	//当前播放速度
	UPROPERTY()
	float M_CurrentPlayRate = 1.f;
	//播放攻击动画
	UPROPERTY()
	bool PlayAnim_Attack = false;
	//被攻击时显示的颜色(时间计时)
	UPROPERTY()
	float M_bHurtColorTime = 0.f;
	//是否可以更换颜色
	UPROPERTY()
	bool M_bColor = false;
};

//代理结构
template<class Type>
struct FuncDelegate {
public:
	FuncDelegate() {}
	FuncDelegate(void(Type::* _Func) ()) :Func(_Func) {}
	inline void SetDelegate(void(Type::* _Func) ()) { this->Func = _Func; }
public:
	void(Type::* Func) () = nullptr;
};

class AMouseActor;

//老鼠基本类型
UENUM()
enum class EMouseBaseType : uint8
{
	Normal, //通常
	Boss, // Boss
	Other //其他
};



UCLASS()
class FVM_API AMouseActor : public ASpineActor
{
	GENERATED_BODY()
public:
	//老鼠名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "老鼠名称")
	FString MouseObjName;
	//外星人名字
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "老鼠名称")
	FString AlienName;
	//老鼠入水淹没百分比
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "水中属性")
	float MouseInWaterRate = 1.f;
	//当前z轴位置下降深度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "水中属性")
	float fMouseInWaterZ = 5.f;
	//当前z轴位置下降深度
	UPROPERTY()
	float fCurInWaterZ = 0.f;
	//动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "默认动画属性")
	FMouseDefAnimation_Anim M_DefAnim_Anim;
	//血量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMouseProperty_HP M_Proper_HP;
	//攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMouseProperty_ATK M_Proper_ATK;
	//状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMouseProperty_State M_Proper_State;
	//条件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMouseProperty_Condition M_Proper_Condition;
	//老鼠buff
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGameBuff* M_Buff = nullptr;
	//老鼠的位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* M_MousePosition = nullptr;
public:
	//解析Buff信息
	UFUNCTION(BlueprintCallable)
	void ParseBuff_Information(const FItem_Buff& _Buff);
	//获取当前生命值
	UFUNCTION(BlueprintCallable)
	float GetCurrentHP();
	//获取总生命值
	UFUNCTION(BlueprintCallable)
	float GetTotalHP();
	//获取当前速度
	UFUNCTION(BlueprintCallable)
	float GetCurrentMoveSpeed();
	//设置当前速度
	UFUNCTION(BlueprintCallable)
	void SetCurrentMoveSpeed(const float _Value);
	//获取原本的速度
	UFUNCTION(BlueprintCallable)
	float GetMoveSpeed();
	//获取当前攻击力
	UFUNCTION(BlueprintCallable)
	float GetCurrentATK();
	//获取原本的攻击力
	UFUNCTION(BlueprintCallable)
	float GetATK();
	//获取是否受到伤害
	UFUNCTION(BlueprintCallable)
	bool GetbIsHurt();
	//获取受到的伤害值
	UFUNCTION(BlueprintCallable)
	float GetHurtValue();
	//获取是否在攻击
	UFUNCTION(BlueprintCallable)
	bool GetbIsAttack() const;
	//获取是否可以移动
	UFUNCTION(BlueprintCallable)
	bool GetbIsMove();
	//获取老鼠路线类型
	UFUNCTION(BlueprintCallable)
	ELineType GetMouseLineType();
	//获取老鼠当前的格子位置
	UFUNCTION(BlueprintCallable)
	FLine& GetMouseLine();
	//获取老鼠的位置
	UFUNCTION(BlueprintCallable)
	USceneComponent* const GetMouseLocation();
	//获取老鼠所在的回合
	UFUNCTION(BlueprintCallable)
	int32 GetMouseRound();
	//获取当前正在攻击的卡片
	UFUNCTION(BlueprintCallable)
	ACardActor* GetCurrentAttackCard();
	//获取当前老鼠是否处于攻击状态
	UFUNCTION(BlueprintCallable)
	bool GetbAttackState() const;
	//获取老鼠的中心位置
	UFUNCTION(BlueprintCallable)
	FVector GetMouseTargetPosition();
	//获取路径
	UFUNCTION(BlueprintCallable)
	FString GetPath(const FString& _SourcePath, const FString& _Name);
	//获取老鼠的标记
	UFUNCTION(BlueprintCallable)
	EMouseTag GetMouseTag();
	//获取老鼠基本类型
	UFUNCTION()
	virtual EMouseBaseType GetMouseBaseType();
	//获取老鼠在其他线路的移动
	UFUNCTION(BlueprintCallable)
	bool GetMouseMoveOtherLine() const;
	//获取移动基础方向
	UFUNCTION(BlueprintCallable)
	float GetMouseBaseMoveDirection();
	//获取伤害免伤值
	UFUNCTION(BlueprintCallable)
	float GetIgnoreHurtValue() const;
	//获取当前的不透明度
	UFUNCTION(BlueprintCallable)
	float GetColorOpacity() const;
	//获取轨道动画
	UFUNCTION(BlueprintCallable)
	UTrackEntry* GetTrackEntry();
public:
	//设置老鼠标记
	UFUNCTION(BlueprintCallable)
	void SetMouseTag(EMouseTag NewTag);
	//设置材质
	UFUNCTION(BlueprintCallable)
	void SetMaterial(UMaterialInstance* NewMaterial);
	//设置老鼠入水池的深度
	UFUNCTION(BlueprintCallable)
	void SetMouseInWaterRate(float Rate);
	//设置老鼠基础移动方向
	UFUNCTION(BlueprintCallable)
	void SetMouseBaseMoveDirection(bool bMoveFront);
	//设置经验值
	UFUNCTION(BlueprintCallable)
	void SetEx(const int32& _Ex);
	//设置速度还原
	UFUNCTION(BlueprintCallable)
	void SetSpeedReset();
	//设置原本的速度
	UFUNCTION(BlueprintCallable)
	void SetMoveSpeed(const float _Value);
	//设置生命值
	UFUNCTION(BlueprintCallable)
	void SetHP(float HP);
	//设置老鼠攻击力
	UFUNCTION(BlueprintCallable)
	void SetMouseATK(float _Value);
	//设置是否受到伤害
	UFUNCTION(BlueprintCallable)
	void SetbIsHurt(bool _Value);
	//设置受到的伤害值
	UFUNCTION(BlueprintCallable)
	void SetHurtValue(float _Value);
	//设置伤害免伤值
	UFUNCTION(BlueprintCallable)
	void SetIgnoreHurtValue(float Value);
	//设置是否在攻击
	UFUNCTION(BlueprintCallable)
	void SetbIsAttack(bool _Value);
	//设置老鼠线路类型
	UFUNCTION(BlueprintCallable)
	void SetMouseLineType(ELineType _Type);
	//设置老鼠当前的格子位置
	UFUNCTION(BlueprintCallable)
	void SetMouseLine(FLine _Line);
	//设置老鼠的位置
	UFUNCTION(BlueprintCallable)
	void SetMouseLocation(USceneComponent* _Position);
	//获取老鼠是否死亡
	UFUNCTION(BlueprintCallable)
	bool GetMouseIsDeath();
	//设置老鼠是否死亡
	UFUNCTION(BlueprintCallable)
	void SetMouseDeath(bool _Value);
	//设置老鼠的所在回合
	UFUNCTION(BlueprintCallable)
	void SetMouseRound(int32 _Value);
	//设置正在攻击的卡片
	UFUNCTION(BlueprintCallable)
	ACardActor* SetCurrentAttackCard(ACardActor* _Card);
	//设置当前正在攻击的Actor
	UFUNCTION(BlueprintCallable)
	void SetHitSelfFlyActor(AFlyItemActor* _HitSelfFlyActor);
	//设置当前老鼠死亡时，强行进行到最后一个回合并且直接结束
	UFUNCTION(BlueprintCallable)
	void SetCurMouseDeathForceGameWin(bool Enable);
	//设置老鼠专属掉落物品
	UFUNCTION(BlueprintCallable)
	void SetMouseLevelItems(const TArray<FString>& Items);
	//设置老鼠线路平移[如果老鼠使用移动功能，此函数将生效]
	UFUNCTION(BlueprintCallable)
	void SetMouseMoveOtherLine(FLine NewLine);
	//强制设置水中动画的显示
	UFUNCTION(BlueprintCallable)
	void ForceSetWaterAnimShow(ELineType CurLineType);
	//设置老鼠入水的开启或者关闭
	UFUNCTION(BlueprintCallable)
	void SetInWaterAnimEnable(bool Value);
	//设置老鼠碰撞类型(默认老鼠类型->陆地)
	UFUNCTION(BlueprintCallable)
	void SetMouseCollisionType(
		class UPrimitiveComponent* _Collison,
		class UPrimitiveComponent* _MouseBodyOverlap,
		ECollisionChannel _MouseType
	);
	//设置轨道动画
	UFUNCTION(BlueprintCallable)
	void SetTrackEntry(UTrackEntry* NewTrackEntry);
public:
	//进入网格
	UFUNCTION(BlueprintCallable)
	void BPInMapMeshe(ELineType CurLineType);
	//关闭碰撞
	UFUNCTION(BlueprintCallable)
	void ClosedBoxComponent(UPrimitiveComponent* _Box_Comp);
	//开启碰撞
	UFUNCTION(BlueprintCallable)
	void EnableBoxComponent(UPrimitiveComponent* _Box_Comp);
	//攻击防御卡
	UFUNCTION(BlueprintCallable)
	bool AttackCard();
	//当前老鼠被消灭
	UFUNCTION(BlueprintCallable)
	void MouseKill();
	//开始移动
	UFUNCTION(BlueprintCallable)
	bool MoveStart();
	//移动停止
	UFUNCTION(BlueprintCallable)
	bool MoveStop();
	//更新生命值
	UFUNCTION(BlueprintCallable)
	bool UpdateHP(float _HurtValue);
	//禁用强制移动
	UFUNCTION(BlueprintCallable)
	void DisEnableForceMove();
	//移动
	UFUNCTION(BlueprintCallable)
	void UpdateMove(const float& DeltaTime);
	//关闭入水出水动画
	UFUNCTION(BlueprintCallable)
	void CloseInWaterTimeLine();
public:
	//添加攻击线条
	UFUNCTION(BlueprintCallable)
	void AddAttackLine(
		const FVector& _BeginOffset,
		const FVector& _EndOffset,
		FColor _Color,
		ECollisionChannel _ECollisionChannel,
		float DeltaTime,
		bool DirectionFront = true
	);
	UFUNCTION(BlueprintCallable)
	void AddAttackLineFunc(
		const ECollisionChannel& CollisionType,
		const float& DeltaTime,
		bool DirectionFront = true
	);
	//添加老鼠检测，卡片
	UFUNCTION(BlueprintCallable)
	void AddAttackCardUpdate();
	//添加水组件
	UFUNCTION(BlueprintCallable)
	void AddWaterComponent(UWaterSceneComponent* WaterComp);
public:
	//老鼠目前的行为

	//老鼠移动方法，包含了强行换行的移动功能
	virtual void SetMouseMoveOtherLineFunc(FLine NewLine);
	virtual void UpdateMoveFunc(const float& DeltaTime);
	//移动(每0.02ms自动调用(允许移动的情况下))
	virtual void MoveingUpdate(float DeltaTime);
	//停止移动(当设置停止移动时调用一次)
	virtual void MoveingStop();
	//开始移动(当设置开始移动时调用一次)
	virtual void MoveingBegin();

	//老鼠开始攻击(如果设置了IsAttack为true 会自动调用一次)
	virtual void AttackedBegin();
	//老鼠攻击结束(如果设置了IsAttack为false 会自动调用一次)
	virtual void AttackedEnd();

	//当老鼠被命中时受到的伤害数值(前提是isHurt为true  调用一次(一般由FlyitemActor命中时调用))
	UFUNCTION(BlueprintCallable)
	virtual bool BeHit(
		UObject* CurHitMouseObj,
		float _HurtValue,
		EFlyItemAttackType AttackType
	);

	//老鼠受到攻击((当设置isHurt为true时)调用一次)
	virtual void BeAttakedBegin();

	//当老鼠死亡时(当设置老鼠SetMouseDeath(true)时调用一次)
	virtual void MouseDeathed();

	//进入网格时
	virtual void InMapMeshe(ELineType CurLineType);

	//排序设置
	virtual void SetRenderLayer(int32 _Layer) override;
public:
	//Tick 更新
	virtual void Tick(float DeltaTime) override;
	//初始化
	virtual void BeginPlay() override;

	//老鼠初始化(Base BeginPlay自动调用一次)
	virtual void MouseInit();
	//老鼠更新【受到buff的影响】
	virtual void MouseTick(const float& DeltaTime);

	//执行buff
	virtual void ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime);
	//结束buff
	virtual void ExitBuff(EGameBuffTag BuffTag);
public:
	//构造
	AMouseActor();

	//获取老鼠管理器
	UFUNCTION()
	UMouseManagerComponent* GetMouseManager();

	//外星人死亡销毁【请使用Spine代理】
	UFUNCTION()
	void AlienDeadAnimationCompelet(class UTrackEntry* Track);
private:
	void UpdateColor();
private:
	//老鼠管理器
	UPROPERTY()
	UMouseManagerComponent* M_MouseManager = nullptr;
	//当前老鼠死亡是否立刻结束游戏
	UPROPERTY()
	bool CurMouseDeathForceGameWin = false;
	//指定掉落物品
	UPROPERTY()
	TArray<FString> CurMouseLevelItems;
private:
	//正在攻击的卡片
	UPROPERTY(EditAnywhere)
	ACardActor* M_CurrentAttackCardActor = nullptr;
	//击中老鼠的子弹
	UPROPERTY()
	AFlyItemActor* M_HitSelfFlyActor = nullptr;
private:
	//攻击线的延迟
	UPROPERTY()
	float fAttackLineDelay = 0.05f;
	//是否启动其他线路移动
	UPROPERTY()
	bool bForceMoveOtherLine = false;
	UPROPERTY()
	bool bEnableForceMove = false;
	//老鼠上下行的移动位置
	UPROPERTY()
	FVector2D MouseMoveLineScaleValue;
	//移动时间更新
	UPROPERTY()
	float bMoveTime = 0.f;
	//移动的总时间
	UPROPERTY()
	float bConstMoveTime = 0.5f;
	//基础移动方向
	UPROPERTY()
	float fBaseMoveDirection = 1.f;
	//是否初始化
	UPROPERTY()
	bool bMouseInit = false;
private:
	//入水的时间轴
	UPROPERTY()
	class UTimeLineClass* InWaterTimeLine = nullptr;
	//动画轨道
	UPROPERTY()
	UTrackEntry* CurAnimTrackEntry = nullptr;
protected:
	//入水开启或者关闭
	UPROPERTY()
	bool bInWaterAnimState = true;
	//水动画
	UPROPERTY()
	TArray<UWaterSceneComponent*> WaterComps;
};
