// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "UI_Mail_MessageBox.generated.h"

/**
 *
 */

class UImage;
class UButton;
class UUI_Mail;

UCLASS()
class FVM_API UUI_Mail_MessageBox : public UWidgetBase
{
	GENERATED_BODY()
private:
	//按钮状态
	int32 M_MailStateValue = 0;
	//mail
	FMail* M_MailData = nullptr;
private:
	UPROPERTY()
		UButton* M_MailState = nullptr;
	UPROPERTY()
		UImage* M_MailHead = nullptr;
	UPROPERTY()
		UImage* M_MailLog = nullptr;
	UPROPERTY()
		UImage* M_MailStateLog = nullptr;
public:
	UUI_Mail* M_Mail = nullptr;
public:
	//邮件标题
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_MailTitle;
	//邮件发送人
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_SendMailPlayer;
public:
	//打开邮件
	UFUNCTION()
		void OpenMail();
public:
	virtual bool Initialize() override;
	//初始化数据
	void InitData();
	//设置Maildata
	void SetMailData(FMail* _MailData);
	//获取Mail数据
	FMail* const GetMailData();
	//设置Mail
	void SetMail(UUI_Mail* _Mail);
	//获取Mail
	UUI_Mail* const GetMail();
	//设置按钮状态
	void SetMailStateTexture(int32 _MailState);
	//设置按钮状态
	void SetMailState(int32 _MailStateValue);
	//获取按钮状态
	int32 GetMailState();
};
