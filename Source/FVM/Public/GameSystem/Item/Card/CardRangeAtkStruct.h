// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/GameBuff.h"
#include "GameSystem/Item/Card/CardBaseStruct.h"
#include "CardRangeAtkStruct.generated.h"

//范围攻击矩阵
USTRUCT(BlueprintType)
struct FCardRangeATK_Matrix {
	GENERATED_USTRUCT_BODY()
public:
	//允许创建一个矩阵
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_bEnable = false;
	//翻书动画的路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_AnimPath = "";
	//行
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_RowCount = 1;
	//列
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_ColCount = 1;
	//位置偏移
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector M_Offset;
};

//范围攻击音效
USTRUCT(BlueprintType)
struct FCardRangeATK_Audio {
	GENERATED_USTRUCT_BODY()
public:
	//允许使用音效
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_bEnable = false;
	//音效名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_AudioName = "";
};

//范围攻击类型防御卡
USTRUCT(BlueprintType)
struct FItemCardRangeATK : public FItemCard {
	GENERATED_USTRUCT_BODY()
public:
	//卡片基础攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_CardATKBase = 0.f;
	//卡片攻击力加成比率
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_CardATKRate = 0.f;
	//半径(卡片攻击范围)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_ATKRadius = 45.f;
	//攻击速度(攻击延迟)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_ShootSpeed = 2.f;
	//首次投射时间 {延迟}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_FirstShootSpeed = 0.5f;
	//投射延迟
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_ShootDelay = 0.1f;
	//投射多少发
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_ProjectionItemCount = 1;
	//是否自动攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_AutoAttack = false;
	//是否启用计时
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_bEnabledTime = false;
	//卡片存在的时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_CardLifeTime = 60.f;
	//攻击老鼠的类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EFlyItemAttackType AttackType = EFlyItemAttackType::Def;
	//携带buff
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameBuffInfor M_FItem_Buff;
	//检测类型(决定当前卡片可以攻击的老鼠类型)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ELineTraceType M_ELineTraceType;
	//---------------------------------------功能部分--------------------------------
	//创建矩阵动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FCardRangeATK_Matrix M_FCardRangeATK_Matrix;
	//创建音效
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FCardRangeATK_Audio M_FCardRangeATK_Audio;
};

UCLASS()
class FVM_API UCardRangeAtkStruct : public UObject
{
	GENERATED_BODY()

};
