// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_ButtonTemp.generated.h"

class UButton;
class UTextBlock;
class UUI_Notice;

UCLASS()
class FVM_API UUI_ButtonTemp : public UWidgetBase
{
	GENERATED_BODY()
private:
	UPROPERTY()
		UButton* M_ButtonComp = nullptr;
	UPROPERTY()
		int32 M_Button_Index = -1;
	UPROPERTY()
		UUI_Notice* M_UUI_Notice = nullptr;
	UPROPERTY()
	    UTextBlock* M_TitleText = nullptr;
public:
	//初始化
	bool Initialize() override;
public:
	//设置标题
	void SetTitle(const FString& _title);
	//设置索引
	void SetIndex(int32 _index);
	//设置公告UI
	void SetUINotice(UUI_Notice* _UUI_Notice = nullptr);
public:
	//获取标题
	FString GetTitle();
	//获取索引
	int32 GetIndex();
	//获取按钮
	UButton* GetButton();
public:
	//显示内容
	UFUNCTION()
		void Show();
};
