// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Classes/Engine/DataTable.h"
#include "GameStart/VS/MapBaseType.h"
#include "GameSystem/MouseStructManager.h"
#include "MouseDataStruct.generated.h"


 //----------------------------------------------------------------------------------------------------------
 //---------------------------------------------老鼠资源结构
 //----------------------------------------------------------------------------------------------------------

//老鼠基础属性结构
USTRUCT(BlueprintType)
struct FMouseBaseStruct {
	GENERATED_USTRUCT_BODY()
public:
	//名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText M_MouseName;
	//基础速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_BaseSpeed = 15.f;
	//基础攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_BaseATK = 1;
	//基础生命值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_BaseHP = 12;
	//老鼠适应类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<ELineType> M_MouseLineType;
};


//----------------------------------------------------------------------------------------------------------
//---------------------------------------------数据表
//----------------------------------------------------------------------------------------------------------
//所有老鼠基本数据结构
USTRUCT(BlueprintType)
struct FMouse_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//老鼠数据结构
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FMouseBase M_Mouse;
};


UCLASS()
class FVM_API UMouseDataStruct : public UObject
{
	GENERATED_BODY()
public:
	//设置老鼠资源数据(使用key在生成对应的value，Key是老鼠的中文名称，value生成对应的资源路径)
	UFUNCTION(BlueprintCallable)
		static bool SetMouseResDatas(TMap<FString, FString>& _MouseMapRes);
};
