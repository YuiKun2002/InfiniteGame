// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Data/CardData/MouseDataStruct.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalBase.h"
#include "NormalMouse.generated.h"

class UCapsulecomponent;
class UBoxComponent;

UENUM(BlueprintType)
enum class ENormalMouseTypeMode : uint8 {
	//默认
	Def UMETA(DisplayName = "Def"),
	//装饰
	Add UMETA(DisplayName = "Add"),
	//防御
	AddDefence UMETA(DisplayName = "AddDefence"),
	//甲虫
	Bug UMETA(DisplayName = "Bug"),
};

//普通老鼠资源结构(陆地普通无任何装饰)->一般作为老鼠class的一个属性使用
USTRUCT(BlueprintType)
struct FNormalMouseResourceStruct {
	GENERATED_USTRUCT_BODY()
public:
	//正常形态(No Attack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> M_MouseNomalFlipbookAnim;
	//残血形态(No Attack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> M_MouseResidualBloodFlipbookAnim;
	//正常形态(Attack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> M_MouseAttackNomalFlipbookAnim;
	//残血形态(Attack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> M_MouseAttackResidualBloodFlipbookAnim;
	//死亡形态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> M_MouseDeathedFlipbookAnim;
	//被创建形态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> M_MouseCreateFlipbookAnim;
};


//带有一个装饰普通老鼠资源结构(陆地普通无任何装饰)->一般作为老鼠class的一个属性使用
USTRUCT(BlueprintType)
struct FNormalAddMouseResourceStruct {
	GENERATED_USTRUCT_BODY()
public:
	//有头盔行走
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> M_NomalHeadAnim;
	//有头盔【损坏】行走
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> M_NomalHeadLowAnim;
	//有头盔攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> M_NomalHeadATKAnim;
	//有头盔【损坏】攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> M_NomalHeadATKLowAnim;
	//头盔掉落行走
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> M_NomalHeadRemoveAnim;
	//头盔掉落攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> M_NomalHeadATKRemoveAnim;
	//无盔行走
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> M_NomalAnim;
	//无盔低血行走
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> M_NomalLowAnim;
	//无盔攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> M_NomalATKAnim;
	//无盔低血攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> M_NomalATKLowAnim;
	//死亡
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> M_NomalDeathAnim;
	//头盔是否已经掉落
	UPROPERTY()
	bool bHeadRemove = false;
};

//带有一个前防御门的老鼠，一般惧怕穿透类型的防御卡
USTRUCT(BlueprintType)
struct FNormalDefenceMouseResourceStruct {
	GENERATED_USTRUCT_BODY()
public:
	//防具基础血量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefenceItemBaseHP = 20.f;
	//防具基础当前血量
	UPROPERTY()
	float DefenceItemCurHP = 20.f;
	//全状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> Take;
	//防御残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> TakeLow;
	//防具正常，角色残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> TakeCharLow;
	//防具和角色残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> TakeAllLow;
	//防具掉正常落，角色
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> TakeFallChar;
	//防具掉落，角色残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> TakeFallCharLow;
	//无防具
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> CharDef;
	//无防具残血
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> CharLow;

	//全状态攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> TakeAtk;
	//防具残血角色正常攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> TakeLowAtk;
	//防具正常角色残血攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> TakeCharLowAtk;
	//防具残血角色残血攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> TakeCharAllLowAtk;
	//无防具角色正常攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> CharAtk;
	//无防具残血角色正常攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> CharLowAtk;

	//死亡
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> Death;
	//掉落防具后增加移动速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bItemRemoveAddtionSpeed = false;
	//加速倍率
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float fAddtionSpeedRate = 1.2f;

	//防御道具是否已经掉落
	UPROPERTY()
	bool bItemRemove = false;
	//状态管理器
	UPROPERTY()
	class UNormalMouseDefenceStateManager* Manager = nullptr;
};

//甲虫模式
USTRUCT(BlueprintType)
struct FNormalBugMouseResourceStruct {
	GENERATED_USTRUCT_BODY()
public:
	//默认
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> MDef;
	//攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> MATK;
	//死亡
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UPaperFlipbook> MDeath;
	//最大攻击次数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AttackMaxCount = 1;
};

class UBoxComponent;

//普通老鼠状态模式
UCLASS()
class FVM_API UNormalMouseStateManager : public UObject
{
	GENERATED_BODY()

	friend class UNormalMouseStateBase;

public:
	static UNormalMouseStateManager* MakeNormalMouseStateManager(class ANormalMouse* NewMouseActor);
public:
	void ChangeState(class UNormalMouseStateBase* NewState);
	UNormalMouseStateBase* GetState();
private:
	UPROPERTY()
	class ANormalMouse* MouseActor = nullptr;
	UPROPERTY()
	class UNormalMouseStateBase* MouseState = nullptr;
};


UCLASS()
class FVM_API UNormalMouseStateBase : public UObject
{
	GENERATED_BODY()

	friend class UNormalMouseStateManager;

public:
	//初始化
	virtual void Init();
	//更新
	virtual void MouseTick(const float& DeltaTime);
	//移动逻辑函数
	virtual void MoveingUpdate(float DeltaTime);
	//当老鼠被命中时受到的伤害数值
	virtual bool BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType);
	//老鼠开始攻击
	virtual void AttackedBegin();
	//老鼠攻击结束
	virtual void AttackedEnd();
	//老鼠开始移动
	virtual void MoveingBegin();
	//当老鼠死亡时
	virtual void MouseDeathed();
	//执行buff
	virtual void ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime);
	//动画播放完毕
	virtual void OnAnimationPlayEnd();
protected:
	UFUNCTION()
	ANormalMouse* Get();
private:
	UPROPERTY()
	UNormalMouseStateManager* CurManager = nullptr;
};

//默认老鼠状态
UCLASS()
class FVM_API UMouseStateBase : public UNormalMouseStateBase
{
	GENERATED_BODY()
public:
	//初始化
	virtual void Init() override;
	//更新
	virtual void MouseTick(const float& DeltaTime) override;
	//移动逻辑函数
	virtual void MoveingUpdate(float DeltaTime) override;
	//当老鼠被命中时受到的伤害数值
	virtual bool BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType) override;
	//动画播放完毕
	virtual void OnAnimationPlayEnd() override;
public:
	//是否启动攻击线
	UPROPERTY()
	bool M_bEnableAttakLine = true;
	//当前轨道0动画
	UPROPERTY()
	class UTrackEntry* TrackEntry = nullptr;
	//状态
	UPROPERTY()
	int32 State = 0;
};

//默认模式
UCLASS()
class FVM_API UMouseStateDef : public UMouseStateBase
{
	GENERATED_BODY()
public:
	//初始化
	virtual void Init() override;
	//当老鼠被命中时受到的伤害数值
	virtual bool BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType) override;
	//老鼠开始攻击
	virtual void AttackedBegin() override;
	//老鼠攻击结束
	virtual void AttackedEnd() override;
	//老鼠开始移动
	virtual void MoveingBegin() override;
	//当老鼠死亡时
	virtual void MouseDeathed() override;
	//动画播放完毕
	virtual void OnAnimationPlayEnd() override;
	//动画播放完成
	UFUNCTION()
	void OnAnimationComplet(class UTrackEntry* Track);
protected:
	//默认模式状态
	void ModeDefState();
};

//带装饰的模式
UCLASS()
class FVM_API UMouseStateModAdd : public UMouseStateBase
{
	GENERATED_BODY()
public:
	//初始化
	virtual void Init() override;
	//当老鼠被命中时受到的伤害数值
	virtual bool BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType) override;
	//老鼠开始攻击
	virtual void AttackedBegin() override;
	//老鼠攻击结束
	virtual void AttackedEnd() override;
	//老鼠开始移动
	virtual void MoveingBegin() override;
	//当老鼠死亡时
	virtual void MouseDeathed() override;
	//动画播放完毕
	virtual void OnAnimationPlayEnd() override;
protected:
	void ModeAddState();
};


//带防具的模式
UCLASS()
class FVM_API UMouseStateAddDefence : public UMouseStateBase
{
	GENERATED_BODY()
public:
	//初始化
	virtual void Init() override;
	//移动逻辑函数
	virtual void MoveingUpdate(float DeltaTime) override;
	//当老鼠被命中时受到的伤害数值
	virtual bool BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType) override;
	//老鼠开始攻击
	virtual void AttackedBegin() override;
	//老鼠攻击结束
	virtual void AttackedEnd() override;
	//老鼠开始移动
	virtual void MoveingBegin() override;
	//当老鼠死亡时
	virtual void MouseDeathed() override;
	//动画播放完毕
	virtual void OnAnimationPlayEnd() override;
};

//甲虫模式
UCLASS()
class FVM_API UMouseStateBug : public UMouseStateBase
{
	GENERATED_BODY()
public:
	//初始化
	virtual void Init() override;
	//老鼠开始攻击
	virtual void AttackedBegin() override;
	//老鼠攻击结束
	virtual void AttackedEnd() override;
	//当老鼠死亡时
	virtual void MouseDeathed() override;
	//动画播放完毕
	virtual void OnAnimationPlayEnd() override;
	//执行buff
	virtual void ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime);
	UPROPERTY()
	int32 ATKCount = 1;
};


/**
 *普通陆地老鼠
 */
UCLASS()
class FVM_API ANormalMouse : public ANormalBase
{
	GENERATED_BODY()

	friend class UNormalMouseDefenceStateManager;

public:
	//网格碰撞组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* MesheComp = nullptr;
	//身体碰撞组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* BodyComp = nullptr;
public:
	//老鼠的类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "老鼠模式")
	ENormalMouseTypeMode M_NormalMouseTypeMode = ENormalMouseTypeMode::Def;
	//老鼠的动画资源   默认模式
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "老鼠模式 | Def")
	FNormalMouseResourceStruct M_MouseResource;
	//老鼠的动画资源   装饰模式
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "老鼠模式 | Add")
	FNormalAddMouseResourceStruct M_NormalAddMouseResourceStruct;
	//老鼠的动画资源   防具模式
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "老鼠模式 | Defence")
	FNormalDefenceMouseResourceStruct M_NormalDefenceMouseResourceStruct;
	//老鼠的动画资源   甲虫模式
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "老鼠模式 | Bug")
	FNormalBugMouseResourceStruct M_NormalBugMouseResourceStruct;
	//可以攻击卡片的什么类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECardCollisionType M_ECardCollisionType = ECardCollisionType::E_CardActor2;
	//状态管理器
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "状态管理器")
	UNormalMouseStateManager* Manager = nullptr;
public:
	//构造
	ANormalMouse();
	//初始化
	virtual void BeginPlay() override;
	//更新
	virtual void Tick(float DeltaTime) override;
	//老鼠更新
	virtual void MouseTick(const float& DeltaTime) override;
	//移动逻辑函数
	virtual void MoveingUpdate(float DeltaTime) override;
	//当老鼠被命中时受到的伤害数值
	virtual bool BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType) override;
	//老鼠开始攻击
	virtual void AttackedBegin() override;
	//老鼠攻击结束
	virtual void AttackedEnd() override;
	//老鼠开始移动
	virtual void MoveingBegin() override;
	//当老鼠死亡时
	virtual void MouseDeathed() override;
	//进入网格时
	virtual void InMapMeshe(ELineType CurLineType) override;
	//执行buff
	virtual void ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime)	override;
	//结束buff
	virtual void ExitBuff(EGameBuffTag BuffTag)	override;
	//动画播放完毕
	UFUNCTION()
	void OnAnimationPlayEnd();
};

UCLASS()
class FVM_API UNormalMouseDefenceStateManager :public UObject {
	GENERATED_BODY()

	friend class ANormalMouse;
public:
	//创建管理器
	static UNormalMouseDefenceStateManager* MakeNormalMouseDefenceStateManager(class ANormalMouse* CurMouse);
	//因为受到攻击更新状态
	void UpdateState(const float& HurtValue, const EFlyItemAttackType& AttackType);
	//其他情况更新状态
	void UpdateState();
	//设置动画/*[Anim1 atk & take]  [Anim2 atk & !take] [Anim3 !atk& take] [Anim4 !atk & !take]   */
	void SetAnim(
		const bool& bAtk, const bool& bTake,
		TSoftObjectPtr<UPaperFlipbook> Anim1, TSoftObjectPtr<UPaperFlipbook> Anim2,
		TSoftObjectPtr<UPaperFlipbook> Anim3, TSoftObjectPtr<UPaperFlipbook> Anim4);
private:
	UPROPERTY()
	ANormalMouse* NormalMouse = nullptr;
};
