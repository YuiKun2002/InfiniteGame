// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_MainFramework.h"

#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/Tools/GameSystemFunction.h"

#include <Components/Image.h>


bool UUI_MainFramework::Initialize()
{
	if (!Super::Initialize())
		return false;

	//初始化静态变量
	//UUI_MainFramework::M_UUI_MainFrameworkStatic = this;

	//this->M_MailTip = this->GetWidgetComponent<UImage>(this, TEXT("Mail_tip"));

	return true;
}

//资源初始化
void UUI_MainFramework::Init()
{
	//this->M_MailTip = this->GetWidgetComponent<UImage>(this, TEXT("Mail_tip"));

	////默认隐藏
	//if (this->M_MailTip)
	//	this->M_MailTip->SetVisibility(ESlateVisibility::Hidden);

	////检测玩家是否还有邮件未领取
	//for (auto Mail : UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerMails)
	//{
	//	if (Mail.M_MailState == 0)
	//	{
	//		if (IsValid(this->M_MailTip))
	//		{
	//			//显示提示
	//			this->M_MailTip->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	//		}
	//		break;
	//	}
	//}
}
