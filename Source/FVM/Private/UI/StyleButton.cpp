﻿// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "UI/StyleButton.h"
#include "Widgets/SNullWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Input/SButton.h"
#include "Components/ButtonSlot.h"

#define LOCTEXT_NAMESPACE "UMG"

FReply UStyleButton::StyleSlateHandleClicked()
{
	return this->SlateHandleClicked();
}

void UStyleButton::StyleSlateHandlePressed()
{
	this->SlateHandlePressed();

	this->SetRenderScale(this->RenderTransform.Scale - FVector2D(this->ScaleRate));

	//this->PressedState = true;
}

void UStyleButton::StyleSlateHandleReleased()
{
	this->SlateHandleReleased();

	this->SetRenderScale(
		this->RenderTransform.Scale + FVector2D(this->ScaleRate)
	);

	/*if (this->HoveredState)
	{
		this->PressedState = false;
		this->HoveredState = false;
		this->StyleSlateHandleClicked();
	}
	else {
		this->PressedState = false;
	}*/
}

void UStyleButton::StyleSlateHandleHovered()
{
	this->SlateHandleHovered();
}

void UStyleButton::StyleSlateHandleUnhovered()
{
	this->SlateHandleUnhovered();

	/*if (this->PressedState)
	{
		this->HoveredState = true;
	}*/
}

TSharedRef<SWidget> UStyleButton::RebuildWidget()
{
	MyButton = SNew(SButton)
		.OnClicked(BIND_UOBJECT_DELEGATE(FOnClicked, StyleSlateHandleClicked))
		.OnPressed(BIND_UOBJECT_DELEGATE(FSimpleDelegate, StyleSlateHandlePressed))
		.OnReleased(BIND_UOBJECT_DELEGATE(FSimpleDelegate, StyleSlateHandleReleased))
		.OnHovered_UObject(this, &ThisClass::StyleSlateHandleHovered)
		.OnUnhovered_UObject(this, &ThisClass::StyleSlateHandleUnhovered)
		.ButtonStyle(&WidgetStyle)
		.ClickMethod(ClickMethod)
		.TouchMethod(TouchMethod)
		.PressMethod(PressMethod)
		.IsFocusable(IsFocusable)
		;

	if (GetChildrenCount() > 0)
	{
		Cast<UButtonSlot>(GetContentSlot())->BuildSlot(MyButton.ToSharedRef());
	}

	return MyButton.ToSharedRef();
}
