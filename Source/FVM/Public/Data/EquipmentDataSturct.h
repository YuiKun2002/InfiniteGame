// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Classes/Engine/DataTable.h"
#include "GameSystem/Item/ItemStruct.h"
#include "EquipmentDataSturct.generated.h"


 //装备-邮件
USTRUCT(BlueprintType)
struct FEquipment_FMail_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//物品结构
	UPROPERTY(EditDefaultsOnly)
		FMail M_FEquipment;
};


//装备-玩家补偿数据
USTRUCT(BlueprintType)
struct FPlayerVersionCompensate_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//物品结构
	UPROPERTY(EditDefaultsOnly)
		FPlayerVersionCompensate M_FPlayerVersionCompensate;
};

UCLASS()
class FVM_API UEquipmentDataSturct : public UObject
{
	GENERATED_BODY()

};
