// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSystem/GameDataSubsystem.h"
#include "GameSystem/Item/ItemBaseStruct.h"
#include "CardBaseStruct.generated.h"


//-----------------------------------------   卡片类型->拥有这个类型表示属于那一类

UENUM(BlueprintType)
enum class ECardType : uint8 {
	//攻击型
	E_ATK UMETA(Display = "ATK_Type"),
	//生产型
	E_SPAWN UMETA(Display = "Spawn_Type"),
	//防御型
	E_DEFENCE UMETA(Display = "Defence_Type"),
	//功能型
	E_Function UMETA(Display = "Function_Type"),
	//范围攻击型
	E_RangeATK UMETA(Display = "RangeATK_Type"),


	//卡片最大值【有新类型记得更改】
	E_Max = E_RangeATK + 1u

};

//-----------------------------------------   卡片技能升级类型->拥有这个类型表示会得到这个类型的效果
UENUM(BlueprintType)
enum class ECardSkillType : uint8 {
	//无
	E_NULL UMETA(Display = "NULL_Type"),
	//生产量提升
	E_SpawnFlameNumUp UMETA(Display = "SpawnFlameNumUp_Type"),
	//冷却速度提升
	E_COLDDOWN UMETA(Display = "ColdDown_Type"),
	//攻击速度提升
	E_ATTACKSPEED UMETA(Display = "AttackSpeed_Type"),
};

//-----------------------------------------   卡片星星升级类型->表示每上升一个等级会随之提升某一项数据
UENUM(BlueprintType)
enum class ECardUpGradeType : uint8 {
	//攻击力提升
	E_Atk_Up UMETA(Display = "Atk_Up"),
	//生产速度提升
	E_SpawnSpeed_Up UMETA(Display = "SpawnSpeed_Up"),
	//冷却速度提升
	E_ColdDownSpeed_Up UMETA(Display = "ColdDownSpeed_Up"),
	//防御力(血量)提升
	E_Defence_Up UMETA(Display = "Defence_Up"),
};


//卡片转职
USTRUCT(BlueprintType)
struct FCardChangeJobs {
	GENERATED_USTRUCT_BODY()
public:
	//是否可以转职
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool M_IsChange = false;
	//转职对象
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_ChangeMaterialsName = "";
	//转职类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECardType M_ChangeType = ECardType::E_ATK;
	//转职等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_ChangeGrade = 9;
	//转职关键字
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_ChangeKeyName;
	//转职等级【一，二，三，终...】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_ChangeGradeKeyName = TEXT("一");
};

//等级提升结构
USTRUCT(BlueprintType)
struct FGrade {
	GENERATED_USTRUCT_BODY()
public:
	//当前经验值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_CurrentEx = 0.f;
	//当前经验值最大值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_CurrentTopEx = 100.f;
	//下一次经验值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_NextEx = 150.f;
	//经验值成长率
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_NextExUpRate = 0.2f;
	//当前等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_CurrentGrade = 0;
	//最大等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_MaxGrade = 10;
};

//----------------------------------------------卡片结构-----------------------------------------------------


//卡片碰撞类型
UENUM(BlueprintType)
enum class ECardCollisionType : uint8 {
	//默认(代表卡片所有)
	E_CardActor UMETA(Display = "CardActor"),
	//一般的默认卡
	E_CardActor2 UMETA(Display = "CardActor2"),
	//类似鱼刺的卡片
	E_CardActor3 UMETA(Display = "CardActor3"),
	//无，Tag
	E_None UMETA(Display = "None"),
};

//卡片种类类型
UENUM(BlueprintType)
enum class ECardCategoryTag : uint8 {
	//默认(代表卡片所有)
	Def UMETA(Display = "Def"),
	//管线
	Line UMETA(Display = "Line"),
	//投射物
	Projectile UMETA(Display = "Projectile"),
	//生产
	Spawn UMETA(Display = "Spawn"),
	//炸弹
	Bomb UMETA(Display = "Bomb"),
	//高墙面
	HighBlock UMETA(Display = "HighBlock"),
	//低墙面
	LowBlock UMETA(Display = "LowBlock"),
	//地形卡
	GroundBlock UMETA(Display = "GroundBlock"),
	//管线反弹
	LineRebound UMETA(Display = "LineRebound"),
	//投射加强
	ProjectileUp UMETA(Display = "ProjectileUp")
};

//卡片关键字映射默认动画
USTRUCT(BlueprintType)
struct FCardKeyIdleAnimation {
	GENERATED_USTRUCT_BODY()
public:
	//副词
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftObjectPath IdlePath;
};

//卡片预览图
USTRUCT(BlueprintType)
struct FCardpreViewHead {
	GENERATED_USTRUCT_BODY()
public:
	//卡片名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CardName;
	//预览动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftObjectPath CardViewAnim;
	//卡片优先级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CardLayer = 0;
	//预览图偏移
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D CardViewOffset;
	//预览图缩放
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D CardViewScale = FVector2D(2.f);
};

//卡片基础底层
USTRUCT(BlueprintType)
struct FItemCard : public FItemBase {
	GENERATED_USTRUCT_BODY()
public:
	FItemCard() {
	}
public:
	//卡片种类
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECardCategoryTag CardCategoryTag = ECardCategoryTag::Def;
	//卡片系
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGamePropertyCategory GamePropertyCategory = EGamePropertyCategory::Energy;
	//卡片属于白天还是夜晚
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool M_CardDay = true;
	//自我替换（自我替换刷新生命）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool M_ReplaceSelf = false;
	//是否允许被复制（将用于幻幻鸡类型的卡片变身）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool M_bEnableCopy = true;
	//携带咖啡粉则自动激活卡片（夜间转换）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool M_AutoEnableCardDay = true;
	//卡片价格
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_CardPrice = 250;
	//冷却时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_CardColdDown = 3.f;
	//卡片星星等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_CardGrade = 0;
	//卡片生命值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_CardHP = 10.f;
	//卡片优先级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_CardLayer = 0;
	//卡片是否可以重叠(覆盖)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool M_CardOverlap = false;
	//卡片重叠对象名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> M_CardOverlapName;
	//卡片价格是否随玩家放置个数线性增加
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool M_CardPriceAutoUp = false;
	//卡片每次增加25火苗
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 M_CardPriceUpNum = 25;
	//卡片转职
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCardChangeJobs M_FCardChangeJobs;
	//卡片类型(用于结构转型)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECardType M_ECardType = ECardType::E_ATK;
	//卡片技能升级后开启的能力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECardSkillType M_ECardSkillType = ECardSkillType::E_ATTACKSPEED;
	//卡片对应的技能书名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_CardSkillBookName = FString(TEXT("NULL"));
	//卡片星星升级类型->表示每上升一个等级会随之提升某一项数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECardUpGradeType M_ECardUpGradeType = ECardUpGradeType::E_Atk_Up;
	//因为等级每次的提升比率
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_M_ECardUpGradeUpRate = 0.f;
	//卡片线路类型->顾名思义就是种植的当前网格，如果网格是【陆地】类型（当前卡片类型必须是:陆地）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELineType M_ELineType = ELineType::OnGround;
	//道具描述2
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemDescrible_2 = FString(TEXT("描述信息待完善"));
	//卡片对应的蓝图实例
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftClassPath CardActorResource;
	//卡片的大头像
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftObjectPath CardActorHead;
	//确定卡片特殊性(标记特殊会在强化中表示高概率卡片)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool M_SpecialCard_SynthesisUpGrade = false;
	//是否启用碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool M_bEnableCardCollision = true;
	//卡片碰撞类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECardCollisionType M_ECardCollisionType = ECardCollisionType::E_CardActor2;
	//卡片碰撞宽度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float M_CardCollsionWidth = 20.f;
	//射线设置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLineTraceSetting> M_LineTraceSettings;
	//网格检测设置
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	TArray<FLineCheckSetting> M_LineCheckSetting;

	//FLineCheckSetting
};

class UPlayerStructManager;

//卡片基础数据结构
UCLASS()
class FVM_API UCardBaseStruct : public UObject
{
	GENERATED_BODY()
public:
	//获取卡片源数据
	static void GetAllCardsByType(ECardType _Type, TArray<FItemCard*>& _CardDatas);
public:
	//返回所有卡片的名称
	UFUNCTION(BlueprintCallable)
	static void GetAllCardName(TArray<FItemBase>& _CardName);
	//根据类型返回数据表路径
	UFUNCTION(BlueprintCallable)
	static FString GetCardDataTablePath(ECardType _Type);
	//搜索卡片查询所有的卡片数据表
	UFUNCTION(BlueprintCallable)
	static bool SearchCardFromDataTable(
		const FString& _CardName,
		FItemCard& OutputData,
		bool _SelectCardType = false,
		ECardType _CardType = ECardType::E_ATK,
		int32 _UniformGrade = 0
	);
	//通过卡片ID查询所有的卡片数据表
	UFUNCTION(BlueprintCallable)
	static bool SearchCardFromDataTableByID(
		int32 ID,
		FItemCard& OutputData,
		bool _SelectCardType = false,
		ECardType _CardType = ECardType::E_ATK,
		int32 _UniformGrade = 0
	);
	//返回所有卡片的数据
	UFUNCTION(BlueprintCallable)
	static void GetAllCardsData(TArray<FItemCard>& _CardDatas, int32 _UniformGrade = 0);
	//创建所有的卡片到背包
	UFUNCTION(BlueprintCallable)
	static void CreateAllCardToBag(UPlayerStructManager* _Player, int32 _UniformCardGrade);
public:
	/*-----------------------------------------------卡片-----------------------------------------------*/
	/*-----------------------------------------------卡片-----------------------------------------------*/

	//卡片名称匹配
	template<class Type = FItemCard, class OutType = FItemCard>
	static void  GetCardArraysData(const FString& _CardName, TArray<Type*>& _Arrays, OutType& OutputData, bool& _Result)
	{
		//查询卡片
		for (auto _Card = _Arrays.CreateConstIterator(); _Card; ++_Card)
		{
			if ((*_Card).ItemName.Equals(_CardName))
			{
				OutputData = (*_Card);
				_Result = true;
				break;
			}
		}
	}

	//将卡片数据结构表转换成数据组
	template<class CardDataStructType, class CardType>
	static void CardDataStructTransform(TArray<CardDataStructType>& Datas, TArray<CardType>& _OutCards)
	{
		for (auto& LData : Datas)
		{
			_OutCards.Emplace((*(CardType*)(&LData.M_FItemCard)));
		}
	}

	//获取卡片数据
	template<class CardType>
	static void GetCardDatas(ECardType _Type, TArray<CardType>& _OutCards, int32 _UniformGrade = 0)
	{
		TArray<FItemCard*> Local;
		UCardBaseStruct::GetAllCardsByType(_Type, Local);

		for (auto LocalData : Local)
		{
			//进行【例如：】指针的类型转换->FItemCard*->FCard_ATK_Data*->FCard_ATK_Data（防止数据切割）
			_OutCards.Emplace((*(CardType*)(LocalData)));
		}

		if (_UniformGrade > 0)
			for (auto& LData : _OutCards)
			{
				//设置卡片等级
				LData.M_CardGrade = _UniformGrade;
			}
	}

	//返回一个指定类型的卡片数据
	template<class CardType = FItemCard>
	static void GetCardData(CardType& _CardData, const FString& _Name, const ECardType& _ECardType, const int32& _CardGrade)
	{
		//获取具体卡片数据
		TArray<CardType> Cards;
		UCardBaseStruct::GetCardDatas<CardType>(_ECardType, Cards, _CardGrade);

		bool _Result;
		UCardBaseStruct::GetCardArraysData<CardType>(_Name, Cards, _CardData, _Result);
	}
	/*-----------------------------------------------卡片-----------------------------------------------*/
	/*-----------------------------------------------卡片-----------------------------------------------*/
};
