// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/UI_TreviFountain_3.h"
#include "Game/UI/UI_TreviFountain_3Box.h"

#include "GameSystem/PlayerStructManager.h"

#include "GameSystem/Tools/GameSystemFunction.h"

#include "Components/UniformGridPanel.h"

#include <Components/Button.h>
#include <Components/Image.h>
#include <Components/RichTextBlock.h>
#include <Components/CanvasPanel.h>
#include <Components/CanvasPanelSlot.h>
#include <Components/TextBlock.h>

#include "GameSystem/FVMGameInstance.h"

bool UUI_TreviFountain_3::Initialize()
{
	if (!Super::Initialize())
		return false;

	//抽奖结果显示奖励名称
	this->M_URichTextBlock = this->GetWidgetComponent<URichTextBlock>(this, TEXT("RichTextBlock_53"));

	if (this->M_ItemSlotShowPanels.Num() == 0)
	{
		//获取组件
		for (int32 i = 0; i < 12; i++)
		{
			this->M_ItemSlotShowPanels.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, FString(TEXT("Item_") + FString::FromInt(i + 1))));
		}

		//获取抽奖奖励显示界面
		this->M_GetItemPanels.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("ResultPanel")));
		this->M_GetItemPanels.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("Get_R_1")));
		this->M_GetItemPanels.Emplace(this->GetWidgetComponent<UCanvasPanel>(this, TEXT("Get_R_2")));

		this->M_Get_1_Head = this->GetWidgetComponent<UImage>(this, TEXT("Item_Result_1"));
		//获取抽奖奖励图像
		for (int32 i = 0; i < 10; i++)
		{
			this->M_Get_2_Heads.Emplace(this->GetWidgetComponent<UImage>(this, FString(TEXT("Head_") + FString::FromInt(i))));
		}
	}

	return true;
}


void UUI_TreviFountain_3::InitContent()
{
	//初始化数据表
	UGameSystemFunction::GetDataTableRows<FTreviFountainStructData>(TEXT("DataTable'/Game/Resource/BP/Data/TreviFountainData/TreviFount4_Pool_0.TreviFount4_Pool_0'"), this->M_FTreviFountainStructData);

	//确保数据个数大于等于12
	if (this->M_FTreviFountainStructData.Num() < 12)
	{
		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("奖池中解析的奖励个数没有达到12个，请重新设置奖励个数"));
		return;
	}

	//初始化奖励
	for (int32 i = 0; i < 12; i++)
	{
		//创建12个奖励展示盒子并且设置属性
		UUI_TreviFountain_3Box* Box = CreateWidget<UUI_TreviFountain_3Box>(this, LoadClass<UUI_TreviFountain_3Box>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_TreviFountain4_ItemBox.BPUI_TreviFountain4_ItemBox_C'")));
		Box->SetItemName(this->M_FTreviFountainStructData[i].M_FTreviFountainStruct.M_ItemName);
		Box->SetHead(this->M_FTreviFountainStructData[i].M_FTreviFountainStruct.M_ItemHeadPath.ToString());
		Box->SetBgStyle((uint8)(this->M_FTreviFountainStructData[i].M_FTreviFountainStruct.M_ItemQuality));
		UCanvasPanelSlot* LSlot = this->M_ItemSlotShowPanels[i]->AddChildToCanvas(Box);
		FAnchors FA;
		FA.Maximum = FVector2D(0.5f);
		FA.Minimum = FVector2D(0.5f);
		LSlot->SetAnchors(FA);
		LSlot->SetAlignment(FVector2D(0.5f));
		this->M_FTreviFountainItemStructs.Emplace(this->M_FTreviFountainStructData[i].M_FTreviFountainStruct);
	}

	//初始化奖池数据
	this->M_FTreviFountainRandomCategoyStruct.SetRandomItems(this->M_FTreviFountainItemStructs);
	this->M_FTreviFountainRandomCategoyStruct.SetRadom(0.43f, 50, 4.01f, 10);
}

void UUI_TreviFountain_3::Get_1()
{
	if (!UFVMGameInstance::GetPlayerStructManager_Static()->ReduceCoin(150, 2u))
	{
		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("你当前点券不足，不能就行单抽"));
		this->CreateTipWidget(TEXT("点券不足"));

		return;
	}

	for (auto Panel : this->M_GetItemPanels)
		Panel->SetVisibility(ESlateVisibility::Collapsed);

	this->M_GetItemPanels[0]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->M_GetItemPanels[1]->SetVisibility(ESlateVisibility::Visible);

	//this->PlayAnimation(this->GetWidgetTreeOwningClass()->Animations[1]);

	//抽奖1次
	TArray<FTreviFountainItemStruct> Datas;
	UTreviFountainStruct::GetRandomItems(this->M_FTreviFountainRandomCategoyStruct, 1, 100, Datas);
	//发送抽奖的道具
	UTreviFountainStruct::SendItemToPlayerBag(Datas[0]);

	//发送三叶草
	FTreviFountainItemStruct LItem;
	LItem.M_ItemCount = 1;
	LItem.M_ItemName = TEXT("三级四叶草");
	LItem.M_ItemType = ETreviFountainItemType::TT_Material;
	UTreviFountainStruct::SendItemToPlayerBag(LItem);

	UWidgetBase::SetImageBrush(this->M_Get_1_Head, Datas[0].M_ItemHeadPath.ToString(), FVector(1.f), 1.f, true, FVector(1.f, 1.f, 0.f));

	this->M_URichTextBlock->SetText(FText::FromString(this->AddText(Datas[0].M_ItemName + FString(TEXT("x")) + FString::FromInt(Datas[0].M_ItemCount), Datas[0].M_ItemQuality, true)));
}

void UUI_TreviFountain_3::Get_t10()
{
	if (!UFVMGameInstance::GetPlayerStructManager_Static()->ReduceCoin(1350, 2u))
	{
		if (UFVMGameInstance::GetDebug())
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("你当前点券不足，不能就行10连抽"));
		this->CreateTipWidget(TEXT("点券不足"));

		return;
	}

	for (auto Panel : this->M_GetItemPanels)
		Panel->SetVisibility(ESlateVisibility::Collapsed);

	this->M_GetItemPanels[0]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->M_GetItemPanels[2]->SetVisibility(ESlateVisibility::Visible);

	//this->PlayAnimation(this->GetWidgetTreeOwningClass()->Animations[2]);

	//抽奖10次
	TArray<FTreviFountainItemStruct> Datas;
	UTreviFountainStruct::GetRandomItems(this->M_FTreviFountainRandomCategoyStruct, 10, 100, Datas);

	this->M_TextContent = TEXT("");
	FString LResult_Content = TEXT("");

	for (int32 i = 0; i < 10; i++)
	{
		UWidgetBase::SetImageBrush(this->M_Get_2_Heads[i], Datas[i].M_ItemHeadPath.ToString(), FVector(1.f), 1.f, true, FVector(1.4f, 1.4f, 0.f));
		LResult_Content = this->AddText(Datas[i].M_ItemName + FString(TEXT("x")) + FString::FromInt(Datas[i].M_ItemCount), Datas[i].M_ItemQuality);
		UTreviFountainStruct::SendItemToPlayerBag(Datas[i]);
	}

	//发送三叶草
	FTreviFountainItemStruct LItem;
	LItem.M_ItemCount = 10;
	LItem.M_ItemName = TEXT("三级四叶草");
	LItem.M_ItemType = ETreviFountainItemType::TT_Material;
	UTreviFountainStruct::SendItemToPlayerBag(LItem);

	this->M_URichTextBlock->SetText(FText::FromString(LResult_Content));
}

FString UUI_TreviFountain_3::AddText(const FString& _Content, ETreviFountainItemQualityType _Quality, bool _Reset)
{
	if (_Reset)
		this->M_TextContent = TEXT("");

	switch (_Quality)
	{
	case ETreviFountainItemQualityType::TQT_Normal:
		this->M_TextContent += FString(TEXT("<t_30_blue>")) + _Content + FString(TEXT("</>")) + '\n';
		return this->M_TextContent;
	case ETreviFountainItemQualityType::TQT_Excellent:
		this->M_TextContent += FString(TEXT("<t_30_purple>")) + _Content + FString(TEXT("</>")) + '\n';
		return this->M_TextContent;
	case ETreviFountainItemQualityType::TQT_Epic:
		this->M_TextContent += FString(TEXT("<t_30_gold>")) + _Content + FString(TEXT("</>")) + '\n';
		return this->M_TextContent;
	}

	return FString();
}
