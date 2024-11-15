﻿// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Data/GameBuff.h"
#include "CardGameBuff.generated.h"


UCLASS()
class FVM_API UCardBuffObjectBase : public UBuffObject
{
	GENERATED_BODY()
protected:
	class ACardActor* GetBuffChar();
};

//伤害减免buff
UCLASS()
class FVM_API UCardDamageReduBuff : public UCardBuffObjectBase
{
	GENERATED_BODY()
protected:
	virtual void BuffUpdate() override;
	virtual void BuffEnd() override;
	virtual void UpdateColor() override;
};


/**
 * 卡片buff系统
 */
UCLASS()
class FVM_API UCardGameBuff : public UGameBuff
{
	GENERATED_BODY()
	friend class UBuffObject;
protected:
	//生成一个新的buff对象
	virtual	UBuffObject* GetNewBuffObject(EGameBuffTag NewTag, float NewBuffTime, UBuffDynamicProperty* Property) override;
};
