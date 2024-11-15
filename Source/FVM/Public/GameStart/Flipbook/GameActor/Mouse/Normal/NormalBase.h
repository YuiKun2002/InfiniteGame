// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"
#include "GameStart/Flipbook/GameActor/ObstacleActor.h"
#include "Data/CardData/MouseDataStruct.h"
#include "NormalBase.generated.h"

/**
 *一般的老鼠基本类型
 */
UCLASS()
class FVM_API ANormalBase : public AMouseActor
{
	GENERATED_BODY()
public:
	//盒体碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "老鼠属性")
	FVector BoxCompSize = FVector(20.f);
	//盒体碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "老鼠属性")
	FVector CollisionOffset = FVector(0.f, 0.f, 17.f);
	//身体碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "老鼠属性")
	FVector BodyCollisionOffset = FVector(0.f, 0.f, 17.f);
public:
	ANormalBase();

	//设置扶梯移动
	UFUNCTION(BlueprintCallable)
	void SetEscalatorMove(int32 NextTranslucenty, bool bEnbale = true);
public:
	//老鼠目前的行为
	virtual void SetMouseMoveOtherLineFunc(FLine NewLine) override;
	virtual void UpdateMoveFunc(const float& DeltaTime) override;
	//移动(每0.02ms自动调用(允许移动的情况下))
	virtual void MoveingUpdate(float DeltaTime) override;
	//停止移动(当设置停止移动时调用一次)
	virtual void MoveingStop() override;
	//开始移动(当设置开始移动时调用一次)
	virtual void MoveingBegin() override;

	//老鼠开始攻击(如果设置了IsAttack为true 会自动调用一次)
	virtual void AttackedBegin() override;
	//老鼠攻击结束(如果设置了IsAttack为false 会自动调用一次)
	virtual void AttackedEnd() override;

	//当老鼠被命中时受到的伤害数值(前提是isHurt为true  调用一次(一般由FlyitemActor命中时调用))
	virtual bool BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType) override;

	//老鼠受到攻击((当设置isHurt为true时)调用一次)
	virtual void BeAttakedBegin() override;

	//当老鼠死亡时(当设置老鼠SetMouseDeath(true)时调用一次)
	virtual void MouseDeathed() override;

	//执行buff
	virtual void ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime);
	//结束buff
	virtual void ExitBuff(EGameBuffTag BuffTag);

	virtual void InMapMeshe(ELineType CurLineType);
public:
	//Tick 更新
	virtual void Tick(float DeltaTime) override;
	//老鼠更新
	virtual void MouseTick(const float& DeltaTime) override;
	//初始化
	virtual void BeginPlay() override;
	//老鼠初始化
	virtual void MouseInit() override;
	//播放灰烬动画
	void PlayBombEffAnim();
	//获取老鼠是否因为爆炸死亡
	bool GetMouseDeathByBomb() const;
	//是否播放爆炸动画
	bool GetPlayPlayBombEffAnim();
public:
	//死亡时是否是爆炸
	UPROPERTY()
	bool M_MouseDeathByBomb = false;
private:
	//冰块
	UPROPERTY()
	AActor* IceBlock = nullptr;
	//启动扶梯移动
	UPROPERTY()
	bool bEnableEscalatorMove = false;
	//时间启动顺序
	UPROPERTY()
	bool bTimePlay = true;
	//当前被记录的z高度
	UPROPERTY()
	float fConstZValue = 0.f;
	UPROPERTY()
	float fCurZValue = 0.f;
	//固定移动时间
	UPROPERTY()
	float fConstMoveTime = 0.5f;
	UPROPERTY()
	float fMoveTime = 0.f;
	//速度基数
	UPROPERTY()
	float fSpeedBaseValue = 0.f;
	//设置渲染层
	UPROPERTY()
	int32 iNextTranslucenty = 0;
	//开启强行降落
	UPROPERTY()
	bool bForceCloseEscalator = false;
};

//自带球体碰撞
UCLASS()
class FVM_API ANormalSphereBase : public ANormalBase
{
	GENERATED_BODY()
public:
	//检查碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* MMeshe = nullptr;
	//重叠碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* MBody = nullptr;
public:
	ANormalSphereBase();
};

//自带胶囊体碰撞
UCLASS()
class FVM_API ANormalCapsuleBase : public ANormalBase
{
	GENERATED_BODY()
public:
	//检查碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* MesheComp = nullptr;
	//重叠碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* BodyComp = nullptr;
public:
	ANormalCapsuleBase();
	virtual void BeginPlay() override;
};

//自带胶囊体碰撞
UCLASS()
class FVM_API ANormalCapsuleByWidgetBase : public ANormalBase
{
	GENERATED_BODY()
public:
	//检查碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* MesheComp = nullptr;
	//重叠碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* BodyComp = nullptr;
	//界面组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWidgetComponent* WidgetRenderComponent = nullptr;
public:
	//构造
	ANormalCapsuleByWidgetBase();
	//脚本构造
	virtual void OnConstruction(const FTransform& Transform) override;
	//开始
	virtual void BeginPlay() override;
	//水动画
	virtual void OnInWater(bool State) override;
	//更新率
	virtual void SetTickRate(float NewRate) override;
public:
	//设置动画
	virtual	UTrackEntry* SetAnimation(int32 TrackIndex, FString AnimationName, bool Loop = true) override;
	//设置渲染的颜色
	virtual void SetSpineRenderColor(FLinearColor SpineColor) override;
	//初始化Spine显示
	virtual void InitSpineShow() override;
	//旋转Spine
	virtual void SetRotation(float Angle) override;
	//渲染层
	virtual	void SetRenderLayer(int32 RenderLayer) override;
public:
	//获取渲染层
	virtual int32 GetRenderLayer() override;
	//获取颜色
	virtual FLinearColor GetSpineRenderColor() const override;
public:
	//入水界面偏移位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NormalCapsuleByWidgetBase|WidgetZ")
	float WidgetZ = 0.f;
private:
	//遮罩界面对象
	UPROPERTY()
	class USpineWidgetWaterAlienMask* SpineWidgetWaterAlienMask = nullptr;
	//层级
	UPROPERTY()
	int32 Layer = 0;
	//遮罩对象颜色
	UPROPERTY()
	FLinearColor MaskColor;
	//当前界面偏移
	UPROPERTY()
	FVector CurWidgetZ;
};
