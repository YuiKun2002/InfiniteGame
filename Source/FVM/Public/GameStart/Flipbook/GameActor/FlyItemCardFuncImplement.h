#pragma once
#include "CoreMinimal.h"
#include "GameStart/VS/Components/Card/CardFuncCompImplement.h"
#include "FlyItemCardFuncImplement.generated.h"

class UCardFunctionComponent;
class AFlyItemActor;

//飞行物反弹
UCLASS(BlueprintType, Blueprintable)
class FVM_API UFlyItemReboundCardFunc : public UFlyItemCardFunc
{
	GENERATED_BODY()
public:
	virtual UFlyItemCardFunc* MakeNewClass() override;

	virtual void Execute(
		class UCardFunctionComponent* CardFuncComp,
		FCardFunctionTriggerImplementTRB CardData,
		class AFlyItemActor* CurFlyItem) override;
};

//飞行物buff添加
UCLASS(BlueprintType, Blueprintable)
class FVM_API UFlyItemBuffCardFunc : public UFlyItemCardFunc
{
	GENERATED_BODY()
public:
	virtual UFlyItemCardFunc* MakeNewClass() override;

	virtual void Execute(
		class UCardFunctionComponent* CardFuncComp,
		FCardFunctionTriggerImplementTRB CardData,
		class AFlyItemActor* CurFlyItem) override;
};

//飞行物穿越
UCLASS(BlueprintType, Blueprintable)
class FVM_API UFlyItemAcrossCardFunc : public UFlyItemCardFunc
{
	GENERATED_BODY()
public:
	virtual UFlyItemCardFunc* MakeNewClass() override;

	virtual void Execute(
		class UCardFunctionComponent* CardFuncComp,
		FCardFunctionTriggerImplementTRB CardData,
		class AFlyItemActor* CurFlyItem) override;
};

//飞行物攻击力增强
UCLASS(BlueprintType, Blueprintable)
class FVM_API UFlyItemATKCardFunc : public UFlyItemCardFunc
{
	GENERATED_BODY()
public:
	virtual UFlyItemCardFunc* MakeNewClass() override;

	virtual void Execute(
		class UCardFunctionComponent* CardFuncComp,
		FCardFunctionTriggerImplementTRB CardData,
		class AFlyItemActor* CurFlyItem) override;
};