// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Data/MapData/Editor/MapTab/GameMapUI_MapViewCardItem.h"
#include "Data/MapData/Editor/MapTab/GameMapUI_MapViewEditor.h"
#include <Components/Button.h>

bool UGameMapUI_MapViewCardViewPanel::Initialize()
{
	Super::Initialize();

	return true;
}



bool UGameMapUI_MapViewCardItem::Initialize()
{
	Super::Initialize();

	this->CurCardBut = Cast<UButton>(this->GetWidgetFromName(FName(TEXT("Card"))));

	return true;
}

void UGameMapUI_MapViewCardItem::Init(const FString& CardName, const FSoftClassPath& ClassPath, const int32& CardLayer, const FSoftObjectPath& Card2dAnim, const FSoftObjectPath& Texture2dHead)
{
	this->MCardName = CardName;
	this->MClassPath = ClassPath;
	this->MCardLayer = CardLayer;
	this->MCard2dAnim = Card2dAnim;
	this->MTexture2dHead = Texture2dHead;


	FButtonStyle Style;
	FLinearColor Color = FLinearColor(FVector(1.f, 1.f, 1.f));
	Style.Normal.SetResourceObject(Texture2dHead.TryLoad());
	Style.Normal.TintColor = Color;
	Style.Hovered.SetResourceObject(Texture2dHead.TryLoad());
	Style.Hovered.TintColor = Color * 0.7f;
	Style.Pressed.SetResourceObject(Texture2dHead.TryLoad());
	Style.Pressed.TintColor = Color * 0.5;
	Style.Disabled.SetResourceObject(Texture2dHead.TryLoad());

	this->CurCardBut->SetStyle(Style);
}

const FString& UGameMapUI_MapViewCardItem::GetCardName() const
{
	return this->MCardName;
}

const int32& UGameMapUI_MapViewCardItem::GetCardLayer() const
{
	return this->MCardLayer;
}

const FSoftClassPath& UGameMapUI_MapViewCardItem::GetCardClassPath() const
{
	return this->MClassPath;
}

const FSoftObjectPath& UGameMapUI_MapViewCardItem::GetCardHeadPath() const
{
	return this->MTexture2dHead;
}

const FSoftObjectPath& UGameMapUI_MapViewCardItem::GetCard2DAnimPath() const
{
	return this->MCard2dAnim;
}

void UGameMapUI_MapViewCardItem::Select()
{
	if (IsValid(this->GameMapUI_MapViewEditor))
	{
		if (IsValid(this->GameMapUI_MapViewEditor->CurSelectMapViewCardItem))
		{
			this->GameMapUI_MapViewEditor->CurSelectMapViewCardItem->Cancel();
		}

		if (this->GameMapUI_MapViewEditor->CurSelectMapViewCardItem == this)
		{
			this->GameMapUI_MapViewEditor->CurSelectMapViewCardItem = nullptr;
			return;
		}

		this->GameMapUI_MapViewEditor->CurSelectMapViewCardItem = this;

		this->CurCardBut->SetBackgroundColor(FLinearColor(FVector(0.3f)));
	}
}

void UGameMapUI_MapViewCardItem::Cancel()
{
	this->CurCardBut->SetBackgroundColor(FLinearColor(FVector(1.f, 1.f, 1.f)));
}


