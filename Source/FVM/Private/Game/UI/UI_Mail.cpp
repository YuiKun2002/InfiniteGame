// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_Mail.h"
#include "Game/UI/UI_Mail_GiftBox.h"
#include "Game/UI/UI_Mail_MessageBox.h"

#include <Components/Image.h>
#include <Components/Button.h>
#include <Components/MultiLineEditableText.h>
#include <Components/HorizontalBox.h>
#include <Components/VerticalBox.h>
#include <Components/TextBlock.h>

void UUI_Mail::SendMsgToSystem()
{
	this->AnalysisMsg(this->M_PlayerInputContent->GetText().ToString());
	this->M_PlayerInputContent->SetText(FText());
}

void UUI_Mail::AnalysisMsg(FString _Value)
{
	if (UGameSystemFunction::AddGameinstructions(this, FString("/mail ") + _Value))
	{
		this->LoadList();
	}
}

void UUI_Mail::AnalysisMail(int32 _Index)
{
	//加载邮件时顺便更改ID
	FMail& _Mail = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerMails[_Index];
	_Mail.M_Mail_ID = _Index;

	//解析
	this->CreateNewMailToList(_Mail);
}

/*

	FMail Mail;
	Mail.M_MailTitle = TEXT("送你一份礼物");
	Mail.M_MailSendItemName = TEXT("冬日阳光");
	Mail.M_MailSendPlayer = TEXT("凉凉");
	Mail.M_MailContent = TEXT("收到礼物之后希望你会开心，今天就忘掉所有的烦恼吧");

	UGameSystemFunction::SendMailToCurrentPlayer(Mail);

*/

void UUI_Mail::LoadList()
{

	//查询邮件是否有以及领取的然后清除掉
	//UGameSystemFunction::ClearWaitingItems<>()

	TArray<FMail>& _Items_ = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerMails;

	TArray<FMail> FinishMail;
	for (auto& Mail : _Items_)
	{
		if (Mail.M_MailState != 2)
		{
			FinishMail.Emplace(Mail);
		}
	}

	UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerMails.Empty();
	UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerMails = FinishMail;


	this->M_MailListBox->ClearChildren();

	UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_0);

	this->GetWorld()->GetTimerManager().SetTimer(this->M_FTimeClip_0.M_FTimerHandle, [&]() {
		//卡片数据
		TArray<FMail>& _Items_ = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerMails;
		//判断索引是否有效
		if (this->M_FTimeClip_0.M_Count < _Items_.Num())
		{

			this->AnalysisMail(this->M_FTimeClip_0.M_Count);

			this->M_FTimeClip_0.M_Count++;
		}
		else {
			UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_0);
		}
		}, 0.02f, true);
}

void UUI_Mail::CloseMailPanel()
{
	//刷新主界面内容
	if (UGameSystemFunction::GetMainFramework())
	{
		UGameSystemFunction::GetMainFramework()->Init();
	}

	UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_0);

	this->RemoveFromParent();
}

bool UUI_Mail::Initialize()
{
	if (!Super::Initialize())
		return false;

	if (!this->M_MailListBox)
	{
		this->M_MailListBox = this->GetWidgetComponent<UVerticalBox>(this, "VerticalBox_117");
		this->M_MailItemBox = this->GetWidgetComponent<UHorizontalBox>(this, "Maill_Gift_Panel");
		this->M_PlayerInputContent = this->GetWidgetComponent<UMultiLineEditableText>(this, "Input_Content_TextLine");
		this->M_PlayerSendMsgButt = this->GetWidgetComponent<UButton>(this, "SendMsg");

		this->M_ConfigList = this->GetWidgetComponent<UTextBlock>(this, "TextBlock_40");

		//绑定按钮事件
		this->M_PlayerSendMsgButt->OnClicked.AddDynamic(this, &UUI_Mail::SendMsgToSystem);

		//初始化信息
		this->InitializeMsgText();
	}

	return true;
}

void UUI_Mail::InitializeMsgText()
{
	this->M_MailReceivePlayerName = TEXT("我");
	this->M_MailThemeTitle = TEXT("小提示");
	this->M_MailContent = TEXT("点券-金币-防御卡等道具获取方法：点击【写邮件】(道具获取方法)\n美食大战老鼠 - 回忆版\n 该游戏是由B站(赖小宇ly)一人独立制作的【二创】复刻类游戏任何更新内容都可以看游戏公告");

	//初始化指令大全
	TArray<FMail> Mail_;
	UEquipmentBaseStruct::GetEquipmentRowDatas<FEquipment_FMail_Data, FMail>(UGlobalDatas::Global_SourceMailData_Mail, Mail_);

	FString LContent;

	for (const auto& _Data_Name : Mail_)
		LContent += _Data_Name.M_MailTitle + TEXT("\n");
	this->M_ConfigList->SetText(FText::FromString(LContent));
}

void UUI_Mail::CreateNewMailToList(FMail& _Mail)
{
	UUI_Mail_MessageBox* MailMsg = CreateWidget<UUI_Mail_MessageBox>(this, LoadClass<UUI_Mail_MessageBox>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_Mail_MessageBox.BPUI_Mail_MessageBox_C'")));
	MailMsg->SetMail(this);
	MailMsg->SetMailData(&_Mail);
	MailMsg->InitData();
	this->M_MailListBox->AddChildToVerticalBox(MailMsg);
}

void UUI_Mail::SetMailContent(const FString& _RecvName, const FString& _ThemeTile, const FString& _Content)
{
	this->M_MailReceivePlayerName = _RecvName;
	this->M_MailThemeTitle = _ThemeTile;
	this->M_MailContent = _Content;
}

void UUI_Mail::AddGiftToPanel(UWidget* _Widget)
{
	this->M_MailItemBox->ClearChildren();
	this->M_MailItemBox->AddChildToHorizontalBox(_Widget);
}
