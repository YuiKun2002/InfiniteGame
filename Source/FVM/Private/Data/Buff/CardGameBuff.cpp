﻿// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Data/Buff/CardGameBuff.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"

UBuffObject* UCardGameBuff::GetNewBuffObject(EGameBuffTag NewTag, float NewBuffTime, UBuffDynamicProperty* Property)
{
	UBuffObject* CurNewBuff = nullptr;
	switch (NewTag)
	{
	case EGameBuffTag::DamageRedu:CurNewBuff = NewObject<UCardDamageReduBuff>(); break;
	default: CurNewBuff = NewObject<UBuffObject>(); break;
	}

	//初始化数据
	CurNewBuff->CurTag = NewTag;
	CurNewBuff->CurTime = NewBuffTime;
	CurNewBuff->CurBuffObject = this;

	//如果有属性
	if (IsValid(Property))
	{
		CurNewBuff->DynamicProperty = Property;
	}
	else {
		CurNewBuff->DynamicProperty = UDynamicProperty::MakeDynamicPropertyByClass<
			UBuffDynamicProperty
		>();
	}

	return CurNewBuff;
}

void UCardDamageReduBuff::BuffUpdate()
{
	float Rate = 1.f;
	if (this->GetDynamicProperty()->GetFloatProperty(TEXT("DamageReduRate"), Rate))
	{
		this->GetBuffChar()->SetAttackDefnceRate(Rate);
	}
}

void UCardDamageReduBuff::BuffEnd()
{
	Super::BuffEnd();

	this->GetBuffChar()->SetAttackDefnceRate(1.f);
}

void UCardDamageReduBuff::UpdateColor()
{
	this->GetBuffChar()->SetSpineRenderColor(
		FLinearColor(
			0.9f, 0.5f, 0.5f, 1.f
		)
	);
}

ACardActor* UCardBuffObjectBase::GetBuffChar()
{
	return Cast<ACardActor>(this->GetGameBuff()->GetBuffChar());
}
