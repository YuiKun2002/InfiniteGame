// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/UI_TicketShopGrid.h"
#include "Game/UI/Tips/UI_ShopPayTip.h"
#include <Components/Image.h>
#include <Components/TextBlock.h>

bool UUI_TicketShopGrid::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_ItemNameText = this->GetWidgetComponent<UTextBlock>(this, TEXT("ItemName"));
	this->M_TicketCountText = this->GetWidgetComponent<UTextBlock>(this, TEXT("Price_Text"));

	this->M_Ticket_Img = this->GetWidgetComponent<UImage>(this, TEXT("Image_158"));
	this->M_ItemHead_Img = this->GetWidgetComponent<UImage>(this, TEXT("Head"));

	return true;
}


void UUI_TicketShopGrid::SetTicketHead(const FString& _HeadPath)
{
	if (_HeadPath.Equals(""))
		this->M_Ticket_Img->SetVisibility(ESlateVisibility::Collapsed);
	else
		UWidgetBase::SetImageBrush(this->M_Ticket_Img, _HeadPath, FVector(1.f), 1.f, true);
}

void UUI_TicketShopGrid::SetItemHead(const FString& _HeadPath)
{
	UWidgetBase::SetImageBrush(this->M_ItemHead_Img, _HeadPath, FVector(1.f), 1.f, true);
}

void UUI_TicketShopGrid::SetItemData(FItemTicketPrice& _data)
{
	this->M_ItemTicketPrice_Data = _data;
}

void UUI_TicketShopGrid::InitBase()
{
	this->M_ItemNameText->SetText(this->M_ItemTicketPrice_Data.M_ItemName);
	this->M_TicketCountText->SetText(FText::FromString(FString::FromInt(this->M_ItemTicketPrice_Data.M_TicketCount)));
}

void UUI_TicketShopGrid::Anim()
{
	this->PlayAnimation(this->GetWidgetTreeOwningClass()->Animations[0]);
}

void UUI_TicketShopGrid::TicketShowPayPanel()
{
	UUI_ShopPayTip* _PayWidget = CreateWidget<UUI_ShopPayTip>(this,
		LoadClass<UUI_ShopPayTip>(0,
			TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/UI_Tip/UI_BuyItem_Tip.UI_BuyItem_Tip_C'")));

	_PayWidget->OnPayFinish.BindUFunction(this, TEXT("AutoCall_PayFinish"));
	_PayWidget->OnTicketPayFinishOver.BindUFunction(this, TEXT("AutoCall_TicketPayFinishOver"));

	_PayWidget->M_ItemName = this->M_ItemTicketPrice_Data.M_ItemName;
	_PayWidget->M_ItemPirceText = FString::FromInt(this->M_ItemTicketPrice_Data.M_TicketCount);
	_PayWidget->M_ItemDescribe = this->M_ItemTicketPrice_Data.M_ItemDescirbe;
	_PayWidget->M_ItemMoneyType = this->M_ItemTicketPrice_Data.M_TicketName;

	//设置发送值（例如：购买一个商品得到5个道具   购买几次*5 ）
	if (this->M_ItemTicketPrice_Data.M_ItemCount < 1)
		this->M_ItemTicketPrice_Data.M_ItemCount = 1;

	_PayWidget->M_BuyCountSendNums = this->M_ItemTicketPrice_Data.M_ItemCount;

	//赋值价格数据
	_PayWidget->M_FItemTicketPrice = this->M_ItemTicketPrice_Data;
	//赋值购买类型
	_PayWidget->M_EShopPayTipBuyType = EShopPayTipBuyType::E_TicketShopBuy;

	//显示
	_PayWidget->AddToViewport();
}

void UUI_TicketShopGrid::AutoCall_PayFinish()
{
	if (this->OnPayFinish.IsBound())
		this->OnPayFinish.Execute();
}

void UUI_TicketShopGrid::AutoCall_TicketPayFinishOver()
{
	if (this->OnPayFinishOver.IsBound())
		this->OnPayFinishOver.Execute();
}