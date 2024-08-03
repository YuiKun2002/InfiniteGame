// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_Mail_GiftBox.h"
#include "Game/UI/UI_Mail.h"
#include "Game/UI/UI_Mail_MessageBox.h"
#include "Game/UI/Tips/UI_SelectTip.h"
#include "Data/GameLogSubsystem.h"
#include <Components/Button.h>
#include <Components/Image.h>

bool UUI_Mail_GiftBox::Initialize()
{
	if (!Super::Initialize())
		return false;

	//初始化组件
	this->M_ItemHead = this->GetWidgetComponent<UImage>(this, "Mail_Log");
	this->M_Receive_Butt = this->GetWidgetComponent<UButton>(this, "Receive_Butt");
	this->M_Remove_Butt = this->GetWidgetComponent<UButton>(this, "RemoveMail_Butt");

	this->M_Receive_Butt->OnClicked.AddDynamic(this, &UUI_Mail_GiftBox::Receive);
	this->M_Remove_Butt->OnClicked.AddDynamic(this, &UUI_Mail_GiftBox::Remove);

	return true;
}

void UUI_Mail_GiftBox::SetMailBox(UUI_Mail_MessageBox* _MailBox)
{
	this->M_MailBox = _MailBox;
}

UUI_Mail_MessageBox* const UUI_Mail_GiftBox::GetMailBox()
{
	return this->M_MailBox;
}

void UUI_Mail_GiftBox::Receive()
{
	this->M_Receive_Butt->SetIsEnabled(false);

	this->GetMailBox()->SetMailState(2);
	this->GetMailBox()->SetMailStateTexture(2);

	//设置当前邮件状态（邮件数据来自背包）
	this->GetMailBox()->GetMailData()->M_MailState = this->GetMailBox()->GetMailState();

	//背包寻找数据
	FEquipmentBase* EquipmentData = UItemBaseStruct::GetItemFromBag(
		UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Equipment,
		this->GetMailBox()->GetMailData()->M_MailSendItemName
	);

	FEquipmentBase Data;
	//从数据表提前数据
	if (UEquipmentBaseStruct::SearchEquipmentFromDataTable(
		this->GetMailBox()->GetMailData()->M_MailSendItemName, Data, true, EEquipment::E_Gift)
		)
	{
		//发送给背包
		if (EquipmentData)
		{
			EquipmentData->M_Count += this->GetMailBox()->GetMailData()->M_MailItemCount;
		}
		else {
			Data.M_Count = this->GetMailBox()->GetMailData()->M_MailItemCount;
			//申请ID号
			Data.M_ItemID = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->GetEquipID();
			UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Equipment.Emplace(Data);
		}

		UGameLogSubsystem::AddPlayerGetEquipmentLog(this->GetMailBox()->GetMailData()->M_MailSendItemName,
			this->GetMailBox()->GetMailData()->M_MailItemCount);
	}

	//保存数据
	UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("邮件领取操作")));

	FString Tip = TEXT("领取成功");
	UWidgetBase::CreateTipWidget(Tip);
}

void UUI_Mail_GiftBox::Remove()
{
	this->M_Remove_Butt->SetIsEnabled(false);

	UUI_SelectTip* Tip = CreateWidget<UUI_SelectTip>(this, LoadClass<UUI_SelectTip>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/UI_Tip/UI_SelectTip.UI_SelectTip_C'")));
	Tip->M_Title = FText::FromString(TEXT("邮件删除"));
	Tip->M_Content = FText::FromString(TEXT("确定要把这封信件删除吗？"));
	Tip->M_Button_Ok_Text = FText::FromString(TEXT("删除"));
	Tip->M_Button_Cancel_Text = FText::FromString(TEXT("先等等"));
	FScriptDelegate Func, cFunc;
	Func.BindUFunction(this, "DestroyMail");
	cFunc.BindUFunction(this, "CancelDestroyMail");
	Tip->GetOkButton()->OnClicked.Add(Func);
	Tip->GetCancelButton()->OnClicked.Add(cFunc);
	Tip->AddToViewport();
}

void UUI_Mail_GiftBox::DestroyMail()
{
	UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerMails.RemoveAt(
		this->GetMailBox()->GetMailData()->M_Mail_ID
	);
	//保存数据
	UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("邮件删除操作")));
	//重新加载数据
	this->GetMailBox()->GetMail()->LoadList();

	this->GetMailBox()->GetMail()->InitializeMsgText();
	this->GetMailBox()->RemoveFromParent();
	this->RemoveFromParent();
}

void UUI_Mail_GiftBox::CancelDestroyMail()
{
	this->M_Remove_Butt->SetIsEnabled(true);
}

UButton* UUI_Mail_GiftBox::GetReceiveButton()
{
	return this->M_Receive_Butt;
}

UButton* UUI_Mail_GiftBox::GetRemoveButton()
{
	return this->M_Remove_Butt;
}

UImage* UUI_Mail_GiftBox::GetItemHead()
{
	return this->M_ItemHead;
}

void UUI_Mail_GiftBox::Show()
{
	FEquipmentBase Data;
	if (UEquipmentBaseStruct::SearchEquipmentFromDataTable(this->GetMailBox()->GetMailData()->M_MailSendItemName, Data, true, EEquipment::E_Gift))
	{
		//设置log
		this->M_ItemName = Data.ItemName;
		UWidgetBase::SetImageBrush(this->M_ItemHead, Data.ItemTexturePath.ToString());
	}
	else {
		this->M_ItemHead->SetVisibility(ESlateVisibility::Hidden);
		this->M_ItemName = "";
	}

	//设置当前邮件状态（邮件数据来自背包）
	this->GetMailBox()->GetMailData()->M_MailState = this->GetMailBox()->GetMailState();
	//保存数据
	UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("邮件显示操作")));


	if (this->GetMailBox()->GetMailState() == 2)
		this->M_Receive_Butt->SetIsEnabled(false);

	if (this->GetMailBox()->GetMailState() != 1)
	{
		if (this->GetMailBox()->GetMailState() == 2)
			return;
		this->GetMailBox()->SetMailState(1);
		this->GetMailBox()->SetMailStateTexture(1);
	}


}
