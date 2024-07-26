// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/Flipbook/GameActor/FlameActor.h"

void AFlameActor::SetFlameValue(const int32& _value)
{
	if (_value < 15 || _value > 500)
	{
		this->M_Value = 25;
	}

	this->M_Value = _value;
}

const int32& AFlameActor::GetFlameValue() const
{
	return this->M_Value;
}
