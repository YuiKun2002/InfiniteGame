#pragma once
#include "CoreMinimal.h"
#include <Components/BoxComponent.h>
#include "Components/ActorComponent.h"
#include "Engine/Classes/Engine/DataTable.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include "GameStart/Flipbook/GameActor/Card/FunctionCardActor.h"
#include "CardFuncCompImplement.generated.h"

/*
执行规则
1.当任意一个功能返回 false 时，会自动进入下一个功能，【当所有功能执行完毕并且当前动画播放完毕时，卡片结束生命】
2.当使用 OnAnimPlayEnd 功能返回 false 时，【如果没有任何功能，卡片结束生命】

注意：
	1.自动进入下一个功能时，会立刻触发 Execute 函数进行初始化，只有初始化后才会陆续执行其他功能
	2.当所有功能全部执行完毕，最后会通过当前卡片的动画播放状态来决定是否移除卡片。如果播放结束则直接移除反之等待直到播放完成
*/


//卡片生长功能结构
USTRUCT(BlueprintType)
struct FCardFunctionGrowImplementTRB : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//生长动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> GrowAnim;
	//生长完成动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> GrowFinishAnim;
	//生长时间（time-time*rate*grade*grade_rate/time）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float GrowTime = 12.f;
	//生长时间缩短比率（等级和等级提升比率的影响）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float GrowTimeRate = 0.3f;
	//生长音效
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString GrowAudioName = TEXT("MouseC_Grow");
};

//生长功能
UCLASS(BlueprintType, Blueprintable)
class FVM_API UCardFunctionGrow : public UCardFunctionBase
{
	GENERATED_BODY()
public:
	//生成对象
	virtual UCardFunctionBase* MakeNewClass() override;
	//执行
	virtual bool Execute(class UCardFunctionComponent* CardFuncComp)  override;
	//更新
	virtual bool Update(class UCardFunctionComponent* CardFuncComp, const float& DeltaTime) override;
private:
	//开启生长功能
	UPROPERTY()
		bool bCardGrowFunc = false;
	//开始生长
	UPROPERTY()
		bool bCardGrowBegin = false;
	//卡片是否生长
	UPROPERTY()
		bool bCardGrow = false;
	//生长完成
	UPROPERTY()
		bool bCardGrowFinish = false;
	//卡片所需生长时间
	UPROPERTY()
		float fCardGrowTime = 0.f;
	//当前生长功能的数据
	UPROPERTY()
		FCardFunctionGrowImplementTRB CardDataTRB;
};


//功能结构-效果只会执行一次-爆炸效果
USTRUCT(BlueprintType)
struct FCardFunctionBombImplementTRB : public FTableRowBase {
	GENERATED_USTRUCT_BODY();
public:
	//检测老鼠的类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<EMouseCollisionType> MouseCollisionType;
	//直接可以无视血量秒杀的老鼠类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<EMouseTag> MouseTag;
	//爆炸对象约束到本行
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bConstLine = false;
	//攻击老鼠的类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EFlyItemAttackType AttackType = EFlyItemAttackType::Bomb;
	//默认3*3范围
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Radius = 90.f;
	//开始位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector BeginPosition;
	//结束位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector EndPosition;
	//攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BombATK = 10.f;
	//攻击力加成比例
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BombATKRate = 0.5f;
	//额外的buff
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FItem_Buff Buffs;
	//生成对象[爆炸生成]
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftClassPtr<AFunctionActor> OtherShow;
	//爆炸动画名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UAssetCategoryName> BombAnimName;
	//爆炸音效
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString BombAudioName = TEXT("Bomb");
	//老鼠死亡之后生成火苗的数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MouseDeathSpawnFlameCount = 0;
	//一次性并发数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 FlameConcurrencyCount = 1;
};

//爆炸功能基本类型【不建议直接选择】
UCLASS(BlueprintType, Blueprintable)
class FVM_API UCardFunctionBombBase : public UCardFunctionBase
{
	GENERATED_BODY()
public:
	//生成对象
	virtual UCardFunctionBase* MakeNewClass() override;
	//执行
	virtual bool Execute(class UCardFunctionComponent* CardFuncComp)  override;
public:
	//创建一个爆炸范围
	UFUNCTION()
		void CreateBomb(UCardFunctionComponent* CardFuncComp, bool bCollision);
	//创建一个行爆炸
	UFUNCTION()
		void CreateBombLine(UCardFunctionComponent* CardFuncComp, bool bCollision);
	//攻击单个老鼠
	UFUNCTION()
		void CreateSingleMouse(UCardFunctionComponent* CardFuncComp);
	//检查老鼠是否在范围内
	UFUNCTION()
		bool CheckMouse(UCardFunctionComponent* CardFuncComp);
	//生成火苗
	UFUNCTION()
		void SpawnFlame(class AMouseActor* CurMouse, UCardFunctionComponent* CardFuncComp);
	//资产加载
	UFUNCTION()
		void ResourceLoad(UCardFunctionComponent* CardFuncComp);
private:
	//卡片数据
	FCardFunctionBombImplementTRB CardDataTRB;
protected:
	//当前检测的老鼠
	UPROPERTY()
		class AMouseActor* CurCheckMouse = nullptr;
protected:
	//触发爆炸
	bool bBomb = false;
};

//爆炸功能
UCLASS(BlueprintType, Blueprintable)
class FVM_API UCardFunctionBomb : public UCardFunctionBombBase
{
	GENERATED_BODY()
public:
	//生成对象
	virtual UCardFunctionBase* MakeNewClass() override;
	//执行
	virtual bool Execute(class UCardFunctionComponent* CardFuncComp)  override;
	//动画播放完毕
	virtual bool OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp)  override;
};

//行爆炸功能
UCLASS(BlueprintType, Blueprintable)
class FVM_API UCardFunctionBombLine : public UCardFunctionBombBase
{
	GENERATED_BODY()
public:
	//生成对象
	virtual UCardFunctionBase* MakeNewClass() override;
	//执行
	virtual bool Execute(class UCardFunctionComponent* CardFuncComp)  override;
	//动画播放完毕
	virtual bool OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp)  override;
};

//触碰爆炸功能
UCLASS(BlueprintType, Blueprintable)
class FVM_API UCardFunctionOverlapBomb : public UCardFunctionBombBase
{
	GENERATED_BODY()
public:
	//生成对象
	virtual UCardFunctionBase* MakeNewClass() override;
	//执行
	virtual bool Execute(class UCardFunctionComponent* CardFuncComp)  override;
	//更新
	virtual bool Update(class UCardFunctionComponent* CardFuncComp, const float& DeltaTime) override;
private:
	//检测时间
	UPROPERTY()
		float CheckTime = 0.2f;
	//检测延迟
	UPROPERTY()
		float CheckDelay = 0.2f;

};

//触碰伤害功能
UCLASS(BlueprintType, Blueprintable)
class FVM_API UCardFunctionOverlapAttack : public UCardFunctionBombBase
{
	GENERATED_BODY()
public:
	//生成对象
	virtual UCardFunctionBase* MakeNewClass() override;
	//执行
	virtual bool Execute(class UCardFunctionComponent* CardFuncComp)  override;
	//更新
	virtual bool Update(class UCardFunctionComponent* CardFuncComp, const float& DeltaTime) override;
private:
	//检测时间
	UPROPERTY()
		float CheckTime = 0.2f;
	//检测延迟
	UPROPERTY()
		float CheckDelay = 0.2f;

};

//白天转换夜间功能
UCLASS(BlueprintType, Blueprintable)
class FVM_API UCardFunctionTransfromDayToNight : public UCardFunctionBase
{
	GENERATED_BODY()
public:
	//生成对象
	virtual UCardFunctionBase* MakeNewClass() override;
	//动画播放完毕
	virtual bool OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp)  override;
public:
	//转换
	UFUNCTION()
		bool Transfrom(class UCardFunctionComponent* CardFuncComp, bool bDay);
};

//夜间转换白天功能
UCLASS(BlueprintType, Blueprintable)
class FVM_API UCardFunctionTransfromNightToDay : public UCardFunctionTransfromDayToNight
{
	GENERATED_BODY()
public:
	//生成对象
	virtual UCardFunctionBase* MakeNewClass() override;
	//动画播放完毕
	virtual bool OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp)  override;
};

//驱散迷雾一段时间功能
UCLASS(BlueprintType, Blueprintable)
class FVM_API UCardFunctionHiddFogBlock : public UCardFunctionBase
{
	GENERATED_BODY()
public:
	//生成对象
	virtual UCardFunctionBase* MakeNewClass() override;
	//动画播放完毕
	virtual bool OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp)  override;
};

//驱散迷雾功能(3*5范围)【卡片不死永久存在】
UCLASS(BlueprintType, Blueprintable)
class FVM_API UCardFunctionFogBlock_3x5 : public UCardFunctionBase
{
	GENERATED_BODY()
public:
	//生成对象
	virtual UCardFunctionBase* MakeNewClass() override;
	//执行
	virtual bool Execute(class UCardFunctionComponent* CardFuncComp) override;
	//卡片死亡
	virtual void OnDeath(class UCardFunctionComponent* CardFuncComp) override;
};

//驱散迷雾功能(全屏)【卡片不死永久存在】
UCLASS(BlueprintType, Blueprintable)
class FVM_API UCardFunctionFogBlock_Full : public UCardFunctionBase
{
	GENERATED_BODY()
public:
	//生成对象
	virtual UCardFunctionBase* MakeNewClass() override;
	//执行
	virtual bool Execute(class UCardFunctionComponent* CardFuncComp) override;
	//卡片死亡
	virtual void OnDeath(class UCardFunctionComponent* CardFuncComp) override;
};

//功能结构-效果只会执行一次-爆炸效果
USTRUCT(BlueprintType)
struct FCardFunctionActiveTRB : public FTableRowBase {
	GENERATED_USTRUCT_BODY();
public:
	//被激活时翻书动画路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> ActiveAnimPath;
	//激活时，是否开关碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CollisionEnable = false;
};

//卡片被激活功能【只能是夜间卡片被咖啡粉激活时】
UCLASS(BlueprintType, Blueprintable)
class FVM_API UCardFunctionActive : public UCardFunctionBase
{
	GENERATED_BODY()
public:
	//生成对象
	virtual UCardFunctionBase* MakeNewClass() override;
	//执行
	virtual bool Execute(class UCardFunctionComponent* CardFuncComp)  override;
	//更新
	virtual bool Update(class UCardFunctionComponent* CardFuncComp, const float& DeltaTime) override;
private:
	//卡片数据
	FCardFunctionActiveTRB CardDataTRB;
};

//直接冷却当前卡片
UCLASS(BlueprintType, Blueprintable)
class FVM_API UCardFunctionCoodDownFinish : public UCardFunctionBase
{
	GENERATED_BODY()
public:
	//生成对象
	virtual UCardFunctionBase* MakeNewClass() override;
	//动画播放完毕
	virtual bool OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp)  override;
};

//防御卡全部快速冷却完成
UCLASS(BlueprintType, Blueprintable)
class FVM_API UCardFunctionAllCoodDownFinish : public UCardFunctionBase
{
	GENERATED_BODY()
public:
	//生成对象
	virtual UCardFunctionBase* MakeNewClass() override;
	//动画播放完毕
	virtual bool OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp)  override;
};

//复制上一次放置的卡片
UCLASS(BlueprintType, Blueprintable)
class FVM_API UCardFunctionCopyNextCard : public UCardFunctionBase
{
	GENERATED_BODY()
public:
	//生成对象
	virtual UCardFunctionBase* MakeNewClass() override;
	//动画播放完毕
	virtual bool OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp)  override;
};

//随机放置一张防御卡卡片
UCLASS(BlueprintType, Blueprintable)
class FVM_API UCardFunctionRandomPlayCard : public UCardFunctionBase
{
	GENERATED_BODY()
public:
	//生成对象
	virtual UCardFunctionBase* MakeNewClass() override;
	//动画播放完毕
	virtual bool OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp)  override;
};

//功能结构-投掷物攻击力增强永久触发直到卡片死亡
USTRUCT(BlueprintType)
struct FCardFunctionProjectileATKImplementTRB : public FTableRowBase {
	GENERATED_USTRUCT_BODY();
public:
	//攻击力加成比例
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ATKAddtion = 1.f;
	//攻击力成长加成比例
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BombATKRate = 0.5f;
};

//投掷物攻击力增强功能【永久生效直到卡片死亡】
UCLASS(BlueprintType, Blueprintable)
class FVM_API UCardFunctionProjectileATK : public UCardFunctionBase
{
	GENERATED_BODY()
public:
	//生成对象
	virtual UCardFunctionBase* MakeNewClass() override;
	//执行
	virtual bool Execute(class UCardFunctionComponent* CardFuncComp)  override;
	//动画播放完毕
	virtual bool OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp)  override;
	//当卡片死亡
	virtual void OnDeath(class UCardFunctionComponent* CardFuncComp)  override;
private:
	//卡片数据
	FCardFunctionProjectileATKImplementTRB CardDataTRB;

	//投掷卡片数据
	UPROPERTY()
		TArray<class AAttackCardActor*> LastProjectileCards;
	//攻击力还原
	UFUNCTION()
		void ClearATK(class UCardFunctionComponent* CardFuncComp);
	//攻击力增强
	UFUNCTION()
		void AddtionATK(class UCardFunctionComponent* CardFuncComp);
};

class UFlyItemCardFunc;
//功能结构-通过碰撞执行的
USTRUCT(BlueprintType)
struct FCardFunctionTriggerImplementTRB : public FTableRowBase {
	GENERATED_USTRUCT_BODY();
public:
	//遇到子弹需要处理的功能
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSet<TSoftClassPtr<UFlyItemCardFunc>> FlyItemFunctionClass;
	//伤害增加比例(M_UpATK影响)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ATK = 1.f;
	//伤害成长增加比例(等级影响)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ATKRate = 1.f;
	//携带buff
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<Buff_Infor, float> BuffSet;
	//更正的动画资源(当子弹穿过时可以更正外观)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftClassPtr<AFlyItemActor> AcrossBulletClass;
	//更正的动画资源(当子弹穿过时可以更正外观2，遇到冰冻buff的子弹)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftClassPtr<AFlyItemActor> DefBulletClass;
};

//------------------------------------------------------------------------------
//子弹卡片功能接口
UINTERFACE(MinimalAPI, BlueprintType)
class UFlyItemCardFunctionInterface : public UInterface {
	GENERATED_BODY()
};
class FVM_API IFlyItemCardFunctionInterface
{
	GENERATED_BODY()
public:
	//生成对象
	virtual class UFlyItemCardFunc* MakeNewClass() = 0;
	//执行
	virtual void Execute(
		class UCardFunctionComponent* CardFuncComp,
		FCardFunctionTriggerImplementTRB CardData,
		class AFlyItemActor* CurFlyItem) = 0;
};
//飞行物基类【空类】
UCLASS(BlueprintType, Blueprintable)
class FVM_API UFlyItemCardFunc : public UObject, public IFlyItemCardFunctionInterface
{
	GENERATED_BODY()
public:
	//生成对象
	virtual UFlyItemCardFunc* MakeNewClass() override;

	virtual void Execute(
		class UCardFunctionComponent* CardFuncComp,
		FCardFunctionTriggerImplementTRB CardData,
		class AFlyItemActor* CurFlyItem) override;
};
//------------------------------------------------------------------------------

//碰撞触发功能【永久存在直到卡片死亡】
UCLASS(BlueprintType, Blueprintable)
class FVM_API UCardFunctionTrigger : public UCardFunctionBase
{
	GENERATED_BODY()
public:
	//生成对象
	virtual UCardFunctionBase* MakeNewClass() override;
	//执行
	virtual bool Execute(class UCardFunctionComponent* CardFuncComp)  override;
	//遇到子弹时
	virtual bool OnOverlapBegin(class UCardFunctionComponent* CardFuncComp, AActor* OverlapObj) override;
private:
	//卡片数据
	FCardFunctionTriggerImplementTRB CardDataTRB;

	//可以被功能卡片组件使用的功能
	UPROPERTY()
		TSet<class UFlyItemCardFunc*> CardInfluenceFlyItemClass;
};

//卡片猫猫盒功能结构
USTRUCT(BlueprintType)
struct FCardFunctionCatBoxImplementTRB : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//猫猫盒基础伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CatBoxAtkBase = 3.f;
	//基础伤害倍率
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CatBoxAtkRate = 1.f;

	//动画状态[默认状态]
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> State1;
	//动画状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> State2;
	//动画状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> State3;

	//动画状态[攻击状态]
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> StateATK1;
	//动画状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> StateATK2;
	//动画状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> StateATK3;
};

//猫猫盒功能
UCLASS(BlueprintType, Blueprintable)
class FVM_API UCardFunctionCatBox : public UCardFunctionBase
{
	GENERATED_BODY()
public:
	//生成对象
	virtual UCardFunctionBase* MakeNewClass() override;
	//执行
	virtual bool Execute(class UCardFunctionComponent* CardFuncComp)  override;
	//更新
	virtual bool Update(class UCardFunctionComponent* CardFuncComp, const float& DeltaTime) override;
	//动画播放完毕
	virtual bool OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp)  override;
	//判断状态
	bool JudgeState(class UCardFunctionComponent* CardFuncComp);
private:
	//卡片数据
	FCardFunctionCatBoxImplementTRB CardDataTRB;
	//更新延迟数据
	float fUpdateDelay = 0.2;
	//当前生命值
	float fCurHp = 0.f;
};

//雷电长棍面包功能结构
USTRUCT(BlueprintType)
struct FCardFunctionElectricImplementTRB : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//基础伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AtkBase = 50.f;
	//基础伤害倍率
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AtkRate = 1.f;
	//攻击时间间隔
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Time = 4.f;
	//攻击类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<EMouseCollisionType> MouseType;
	//动画状态[默认状态]
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> State1;
	//动画状态[攻击状态]
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> State2;
	//攻击路径动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftClassPtr<AActor> ATKAnim;
};
//雷电长棍面包功能
UCLASS(BlueprintType, Blueprintable)
class FVM_API UCardFunctionElectric : public UCardFunctionBase
{
	GENERATED_BODY()
public:
	//生成对象
	virtual UCardFunctionBase* MakeNewClass() override;
	//执行
	virtual bool Execute(class UCardFunctionComponent* CardFuncComp)  override;
	//更新
	virtual bool Update(class UCardFunctionComponent* CardFuncComp, const float& DeltaTime) override;
	//动画播放完毕
	virtual bool OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp)  override;
	//判断状态
	bool CheckState(class UCardFunctionComponent* CardFuncComp);
	//设置攻击状态
	void SetATK();
	//创建碰撞
	void Create(class UCardFunctionComponent* CardFuncComp,FVector Location,bool bShow);
private:
	//卡片数据
	FCardFunctionElectricImplementTRB CardDataTRB;
	//更新延迟数据
	float fDelay = 0.2;
	//更新延迟数据
	float fCurDelay = 0.2;
	//更新检测
	float fUpdateTime = 0.2;
	//是否在攻击
	bool bATK = false;
private:
	UPROPERTY()
		UCardFunctionElectric* Target = nullptr;
	UPROPERTY()
		ACardActor* CurActor = nullptr;
};


//卡片面粉袋功能结构
USTRUCT(BlueprintType)
struct FCardFunctionFlourbagImplementTRB : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//面粉袋基础伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FlourbagAtkBase = 3.f;
	//基础伤害倍率
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FlourbagAtkRate = 1.f;
	//检测距离
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CheckLength = 100.f;
	//使用次数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 UseCount = 1;
	//攻击类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<ELineType> MouseLineType;
	//攻击的位置，一个位置表示一次范围攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FVector2D> HitLocationOffset;
	//半径
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RangeRadius = 25.f;
	//移动曲线
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<class UCurveFloat> MoveCurve;
	//动画状态[默认状态]
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UAssetCategoryName> State1;
	//攻击状态1
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UAssetCategoryName> StateLeft;
	//攻击状态2
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UAssetCategoryName> StateRight;
};

//面粉袋功能
UCLASS(BlueprintType, Blueprintable)
class FVM_API UCardFunctionFlourbag : public UCardFunctionBase
{
	GENERATED_BODY()
public:
	//生成对象
	virtual UCardFunctionBase* MakeNewClass() override;
	//执行
	virtual bool Execute(class UCardFunctionComponent* CardFuncComp)  override;
	//更新
	virtual bool Update(class UCardFunctionComponent* CardFuncComp, const float& DeltaTime) override;
	//动画播放完毕
	virtual bool OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp)  override;
	//生成检测
	AMouseActor* Gen(class UCardFunctionComponent* CardFuncComp, bool bLeft);
	//初始化
	void Init(class UCardFunctionComponent* CardFuncComp);
public:
	//卡片数据
	UPROPERTY()
		FCardFunctionFlourbagImplementTRB CardDataTRB;
	UPROPERTY()
		class AMouseActor* CurMouse = nullptr;
	UPROPERTY()
		bool bCurStateLeft = true;
	UPROPERTY()
		float fCheckTime = 0.2f;
	UPROPERTY()
		bool bCheckPlay = false;
	UPROPERTY()
		UTimeLineClass* Time = nullptr;
	//当前位置
	UPROPERTY()
		FVector CurLocation;
	//目标位置
	UPROPERTY()
		FVector TargetLocation;

	UPROPERTY()
		bool bEnd = false;
	//组件
	UPROPERTY()
		class UCardFunctionComponent* CurCardFuncComp = nullptr;
};


//卡片汉堡包功能结构
USTRUCT(BlueprintType)
struct FCardFunctionBurgerImplementTRB : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//基础伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AtkBase = 3.f;
	//基础伤害倍率
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AtkRate = 1.f;
	//咀嚼次数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 EattingCount = 9;
	//咀嚼完成后，结束当前功能
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool EattingFinishEndCurFunction = false;
	//可以被秒杀的老鼠对象
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<EMouseTag> CatchMouse;

	//是否有吸附功能[如果开启将不在使用线路检测]
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCatchFunc = false;
	//吸附类型[老鼠线路类型]
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<ELineTraceType> LineTraceType;
	//吸附范围
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float fCatchRange = 90.f;
	//可以吸附对象
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<EMouseTag> CanCatchMouse;

	//动画状态[默认状态]
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> StateDef;
	//动画状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> StateATK;
	//动画状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> StateEat;
	//动画状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> StateFinish;
	//动画状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UPaperFlipbook> StateCatch;
};

//汉堡包功能
UCLASS(BlueprintType, Blueprintable)
class FVM_API UCardFunctionBurger : public UCardFunctionBase
{
	GENERATED_BODY()
public:
	//生成对象
	virtual UCardFunctionBase* MakeNewClass() override;
	//执行
	virtual bool Execute(class UCardFunctionComponent* CardFuncComp)  override;
	//更新
	virtual bool Update(class UCardFunctionComponent* CardFuncComp, const float& DeltaTime) override;
	//动画播放完毕
	virtual bool OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp)  override;
private:
	//启用范围检测
	void EnableRangeATK(class UCardFunctionComponent* CardFuncComp);
	//默认线路检测
	void DisEnableRangeATK(class UCardFunctionComponent* CardFuncComp);
	//单体攻击
	void SingleAttack(class UCardFunctionComponent* CardFuncComp);
	//攻击老鼠
	void HitAllMouse(class UCardFunctionComponent* CardFuncComp);
private:
	//待攻击对象
	UPROPERTY()
		TArray<AMouseActor*> MouseBuffer;
private:
	//卡片数据
	FCardFunctionBurgerImplementTRB CardDataTRB;
	//检测时间
	float fTime = 0.f;
	float fConstCheckTime = 0.2f;
	//攻击延迟
	float fAttackTime = 0.f;
	float fConstAttackTime = 0.65f;
	bool bHit = false;
	//状态
	uint8 CurState = 0u;// 1攻击 2咀嚼 3吸附 4完成
	//咀嚼次数
	int32 iEattingCount = 0;
};
