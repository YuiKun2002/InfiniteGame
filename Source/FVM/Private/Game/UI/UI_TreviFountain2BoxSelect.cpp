// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/UI_TreviFountain2BoxSelect.h"

#include "Game/UI/UI_TreviFountain2.h"

#include <Components/Image.h>
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include <Components/CanvasPanelSlot.h>
#include <Components/SizeBox.h>

bool UUI_TreviFountain2BoxSelect::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_Head = this->GetWidgetComponent<UButton>(this, TEXT("Button_44"));
	this->M_Select = this->GetWidgetComponent<UButton>(this, TEXT("Select"));
	this->M_USizeBox = this->GetWidgetComponent<USizeBox>(this, TEXT("SizeBox_36"));

	this->M_SelectContent = this->GetWidgetComponent<UTextBlock>(this, TEXT("TextBlock_22"));


	//绑定选择按钮
	this->M_Select->OnClicked.AddDynamic(this, &UUI_TreviFountain2BoxSelect::Select_Item);

	return true;
}

UButton* const UUI_TreviFountain2BoxSelect::GetButton()
{
	return this->M_Select;
}

void UUI_TreviFountain2BoxSelect::SetUI_TreviFountain2(UUI_TreviFountain2* _UUI_TreviFountain2)
{
	this->M_UUI_TreviFountain2 = _UUI_TreviFountain2;
}

void UUI_TreviFountain2BoxSelect::SetHead(const FString& _HeadPath)
{
	UWidgetBase::SetButtonStyle(this->M_Head, _HeadPath);
	UWidgetBase::SetBoxResetSize(this->M_USizeBox, _HeadPath);
}

void UUI_TreviFountain2BoxSelect::SetItemName(const FString& _Name)
{
	this->M_ItemName = _Name;
}

const FString& UUI_TreviFountain2BoxSelect::GetItemName()
{
	return this->M_ItemName;
}

void UUI_TreviFountain2BoxSelect::Select_Item()
{
	//是否选择取消选择道具
	if (this->M_bSelect)
	{
		//重新记录祈愿次数
		UButton* LSelect = UWidgetBase::CreateSelectTipWidget(TEXT("是否选择取消保底自选锁定，如果取消被记录的次数将会重置为0"));
		//绑定选择
		LSelect->OnClicked.AddDynamic(this, &UUI_TreviFountain2BoxSelect::Cancel_Item);
	}
	else {

		//如果道具被选择->其他的道具全部关闭选择
		//重新记录祈愿次数
		//重新标记道具

		this->M_UUI_TreviFountain2->SetCustonSelectItemName(this->M_ItemName);
		this->M_UUI_TreviFountain2->SetCustonSelectItemCount(0);
		this->M_UUI_TreviFountain2->SetCustonSelectItemEnable(true);

		//更新文字
		this->M_UUI_TreviFountain2->UpdateCustomSelectText("");

		//将其他选择全部禁用
		for (auto Box : this->M_UUI_TreviFountain2->GetCustomBoxSelectItems())
		{
			Box->GetButton()->SetIsEnabled(false);
		}
		//显示当前按钮
		this->GetButton()->SetIsEnabled(true);
		//更新当前文字
		this->M_SelectContent->SetText(FText::FromString(TEXT("取消")));

		this->M_bSelect = true;
	}

}

void UUI_TreviFountain2BoxSelect::Cancel_Item()
{
	//将其他选择全部开启
	for (auto& Box : this->M_UUI_TreviFountain2->GetCustomBoxSelectItems())
	{
		Box->GetButton()->SetIsEnabled(true);
	}

	this->M_UUI_TreviFountain2->SetCustonSelectItemName("");
	this->M_UUI_TreviFountain2->SetCustonSelectItemCount(0);
	this->M_UUI_TreviFountain2->SetCustonSelectItemEnable(false);
	//更新当前文字
	this->M_SelectContent->SetText(FText::FromString(TEXT("选择")));
	//更新文字选项
	this->M_UUI_TreviFountain2->UpdateCustomSelectText("");

	this->M_bSelect = false;
}
