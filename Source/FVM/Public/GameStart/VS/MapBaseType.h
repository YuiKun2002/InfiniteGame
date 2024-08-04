// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "GameStart/VS/MapBaseType.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MapBaseType.generated.h"

//颜色
#define SPINE_COLOR_DARK FLinearColor(0.2f,0.2f,0.2f,1.f)
#define SPINE_COLOR_WHITE FLinearColor(1.f,1.f,1.f,1.f)


//渲染层级
namespace TranslucencyConst {

	enum class ETranslucency : uint8
	{
		//静态背景
		EStaticBg,
		//背景板块
		EBgBlock,
		//网格
		EMeshe
	};

	inline int32 GetLayer(const TranslucencyConst::ETranslucency& _ETranslucencyType)
	{
		switch (_ETranslucencyType)
		{
		case TranslucencyConst::ETranslucency::EStaticBg:return 0;
		case TranslucencyConst::ETranslucency::EBgBlock:return 500;
		case TranslucencyConst::ETranslucency::EMeshe:return 1000;
		}
		return -1;
	}
};

inline int32 GetRangeLayer(int32 _INTOffset, const TranslucencyConst::ETranslucency& _ETranslucencyType)
{
	TranslucencyConst::ETranslucency LType = _ETranslucencyType;

	int32 LTranslucency = TranslucencyConst::GetLayer(_ETranslucencyType);

	//如果总量+位移小于总量则等于总量
	if (LTranslucency + _INTOffset < LTranslucency)
	{
		return (LTranslucency);
	}

	//如果总量+位移大于等于下一个总量
	TranslucencyConst::ETranslucency LNextType = (TranslucencyConst::ETranslucency)((uint8)LType + 1u);
	if (TranslucencyConst::GetLayer(LNextType) != -1)
	{
		if (LTranslucency + _INTOffset >= TranslucencyConst::GetLayer(LNextType))
		{
			return  TranslucencyConst::GetLayer(LNextType) - 1;
		}
	}

	return LTranslucency + _INTOffset;
}



//子弹设射击方向
UENUM(BlueprintType)
enum class EShootDirection : uint8 {
	EUp  UMETA(DisplayName = "Up"),
	EDown UMETA(DisplayName = "Down"),
	ELeft UMETA(DisplayName = "Left"),
	ERight UMETA(DisplayName = "Right"),
	ERotation UMETA(DisplayName = "Rotation"),
};

//子弹飞行物携带buff
UENUM(BlueprintType)
enum class Buff_Infor : uint8
{
	//减速
	E_SlowDown,
	//冻结
	E_Freeze,
	//凝固
	E_Solidification,
	//加速
	E_Accelerate,
	//灼烧
	Burn,
};

//子弹几种老鼠的类型
UENUM(BlueprintType)
enum class EFlyItemAttackType : uint8 {
	//正常类型
	Def UMETA(DisplayName = "DefType"),
	//穿透类型
	Panetrate UMETA(DisplayName = "PanetrateType"),
	//投掷物类型
	Projectile UMETA(DisplayName = "ProjectileType"),
	//爆炸类型
	Bomb UMETA(DisplayName = "BombType"),
	//溅射类型
	Sputtering UMETA(DisplayName = "SputteringType")
};

//发送老鼠的信息
USTRUCT(BlueprintType)
struct FItem_Buff {
	GENERATED_USTRUCT_BODY()
public:
	//条件->是否携带Buff
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool M_bCondition = false;
	//多少个buff （key（buff） value（time））
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<Buff_Infor, float> M_Buffers;
};

//方向
UENUM(BlueprintType)
enum class EDirection : uint8 {
	_EUp  UMETA(DisplayName = "Up"),
	_EDown UMETA(DisplayName = "Down"),
	_ELeft UMETA(DisplayName = "Left"),
	_ERight UMETA(DisplayName = "Right"),
};

//当前属于的线路类型
UENUM(BlueprintType)
enum class ELineType : uint8 {
	//只允许陆地
	OnGround  UMETA(DisplayName = "OnGround"),
	//地下
	Underground UMETA(DisplayName = "Underground"),
	//只允许适应空中的对象
	Sky UMETA(DisplayName = "Sky"),
	//只允许适应水上的对象
	OnWater UMETA(DisplayName = "OnWater"),
	//水下
	Underwater UMETA(DisplayName = "UnderWater"),
	//岩浆
	Magma UMETA(DisplayName = "Magma"),
	//水下和陆地
	UnderwaterAndGround UMETA(DisplayName = "UnderwaterAndGround"),
	//空中和陆地
	SkyAndGround UMETA(DisplayName = "SkyAndGround"),
	//管道
	Channel UMETA(DisplayName = "Channel"),
	//允许任何位置
	All UMETA(DisplayName = "All"),
	//不允许被对象使用
	DisEnable UMETA(DisplayName = "DisEnable"),
};

//老鼠的碰撞类型设置
UENUM(BlueprintType)
enum class EMouseCollisionType : uint8 {
	//陆地单位
	MouseGround  UMETA(DisplayName = "MouseGround"),
	//空中单位
	MouseSky UMETA(DisplayName = "MouseSky"),
	//地下老鼠
	MouseUnder UMETA(DisplayName = "MouseUnder"),
	//任何老鼠对象(表示全部)
	MouseActor UMETA(DisplayName = "MouseActor"),
};


//线路
USTRUCT(BlueprintType)
struct FLine
{
	GENERATED_USTRUCT_BODY()
public:
	FLine() {}
	FLine(const int32& _Row, const int32& _Col) :Row(_Row), Col(_Col) {}
	FLine(const int32& _Uniform) :Row(_Uniform), Col(_Uniform) {}

	void SetLine(const int32& _Row, const int32& _Col) {
		this->Row = _Row;
		this->Col = _Col;
	}

	bool operator==(const FLine& _Line)
	{
		if (this->Col == _Line.Col && this->Row == _Line.Row)
			return true;

		return false;
	}

	FLine operator+(const FLine& _Line)
	{
		this->Col += this->Col + _Line.Col;
		this->Row += this->Row + _Line.Row;
		return *this;
	}

	FLine operator-(const FLine& _Line)
	{
		this->Col -= _Line.Col;
		this->Row -= _Line.Row;
		return *this;
	}

	FLine operator= (const FLine& _Line)
	{
		this->Col = _Line.Col;
		this->Row = _Line.Row;
		return *this;
	}

	FLine operator*(float _Value)
	{
		return FLine((int32)(this->Row * _Value), (int32)(this->Col * _Value));
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Row = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Col = 0;
};


//地图的转换
USTRUCT(BlueprintType)
struct FMapTranform {
	GENERATED_USTRUCT_BODY()
public:
	//位置
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector M_Location = FVector(570.f, 0.f, 110.f);
	//旋转
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRotator M_Rotation = FRotator(0.f, 90.f, 0.f);
	//缩放
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector M_Scale = FVector(1., 1.f, 1.f);
};

class UPaperSprite;
//地图精灵资源
USTRUCT(BlueprintType)
struct FMapSprite {
	GENERATED_USTRUCT_BODY()
public:
	//地图的背景
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperSprite* M_MapBackgroundSprite = nullptr;
};


UCLASS()
class FVM_API UMapBaseType : public UObject
{
	GENERATED_BODY()
};
