
#include "Game/UI/UI_ShopGrid.h"
#include "Game/UI/UI_Shop.h"
#include <Components/Image.h>
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include <Components/CanvasPanel.h>
#include "Game/UI/Tips/UI_ShopPayTip.h"
#include "GameSystem/GameUserInterfaceSubsystem.h"
#include <Components/CanvasPanelSlot.h>

bool UUI_ShopGrid::Initialize()
{
	return Super::Initialize();
}

void UUI_ShopGrid::PlayAnimation_1()
{
	if (this->GetWidgetTreeOwningClass()->Animations.Num() > 0)
	{
		this->PlayAnimation(this->GetWidgetTreeOwningClass()->Animations[0]);
	}
}

void UUI_ShopGrid::ShowPayPanel()
{
	UUI_ShopPayTip* _PayWidget = Cast<UUI_ShopPayTip>(
		UGameSystemFunction::GetUserInterInstanceByName(UI_GLOBALUINAME)->GetUI(TEXT("Pay"))
	);

	//物品数据
	_PayWidget->ItemPriceData = this->M_FItemPrice;
	_PayWidget->ItemPriceData.M_ItemNums = 1;

	////物品名称
	//_PayWidget->M_ItemName = this->M_FItemPrice.M_ItemName;
	////物品价格
	//_PayWidget->M_ItemPirceText = FString::FromInt(this->M_FItemPrice.M_ItemPrice);
	////物品ID
	//_PayWidget->M_ItemID = this->M_FItemPrice.M_ItemID;
	////物品描述
	//_PayWidget->M_ItemDescribe = this->M_FItemPrice.M_ItemDescirbe;
	////货币类型
	//_PayWidget->M_ItemMoneyType = this->M_FItemPrice.M_ItemMoneyTypeName;
	////赋值价格数据
	//_PayWidget->M_FItemPrice = this->M_FItemPrice;
	//绑定玩家金额更新函数
	if (this->M_UUI_Shop)
	{
		_PayWidget->SetShopUI(this->M_UUI_Shop);
	}
	//显示
	_PayWidget->AddToViewport(500);
}

UButton* UUI_ShopGrid::GetBuyItemButton()
{
	return this->ItemPay;
}

UImage* UUI_ShopGrid::GetItemHeadImage()
{
	return this->ItemImage;
}

EItemTipsImageType UUI_ShopGrid::GetEItemTipsImageType()
{
	return this->M_EItemTipsImageType;
}

void UUI_ShopGrid::SetItemTipsType(EItemTipsImageType _EItemTipsImageType)
{
	this->M_EItemTipsImageType = _EItemTipsImageType;

	FString _Texture2DPath;
	bool _IsHave = true;
	switch (_EItemTipsImageType)
	{
	case EItemTipsImageType::E_Selling:
		_Texture2DPath = "Texture2D'/Game/Resource/Texture/UI/Game/Shop/T_Shop_6.T_Shop_6'";
		break;
	case EItemTipsImageType::E_New:
		_Texture2DPath = "Texture2D'/Game/Resource/Texture/UI/Game/Shop/T_Shop_7.T_Shop_7'";
		break;
	case EItemTipsImageType::E_HalfPirce:
		_Texture2DPath = "Texture2D'/Game/Resource/Texture/UI/Game/Shop/T_Shop_8.T_Shop_8'";
		break;
	case EItemTipsImageType::E_None:
		_IsHave = false;
		break;
	}

	if (_IsHave)
	{
		UWidgetBase::SetImageBrush(this->ItemTips, _Texture2DPath);
	}
	else {
		this->ItemTips->SetVisibility(ESlateVisibility::Hidden);
	}
}
