// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "LogPanel.generated.h"

/**
 * 聊天窗口
 */

class UButton;
class UImage;
class UScrollBox;
class UVerticalBox;
class ULogItemPanel;
class UEditableTextBox;
class UGameLogSubsystem;

UCLASS()
class FVM_API ULogPanel : public UWidgetBase
{
	GENERATED_BODY()
public:
	//初始化
	virtual bool Initialize() override;
public:
	//打开当前界面
	UFUNCTION(BlueprintCallable)
		void OpenCurrent();
	//打开私聊界面
	UFUNCTION(BlueprintCallable)
		void OpenPrivate();
	//更新显示
	UFUNCTION(BlueprintCallable)
		void UpdateContent();
	//上一页
	UFUNCTION(BlueprintCallable)
		void LastPage();
	//下一页
	UFUNCTION(BlueprintCallable)
		void NextPage();
	//最后一页
	UFUNCTION(BlueprintCallable)
		void EndPage();
private:
	//滚动界面
	UPROPERTY()
		UScrollBox* CurScrollBox = nullptr;
	//输入的内容
	UPROPERTY()
		UEditableTextBox* InputContent = nullptr;
	//列表内容
	UPROPERTY()
		UVerticalBox* CurLogList = nullptr;
	//信息界面
	UPROPERTY()
		TArray<ULogItemPanel*> CurLogItems;
	//子系统
	UPROPERTY()
		UGameLogSubsystem* CurSystem = nullptr;
	//状态
	UPROPERTY()
		bool bCur = false;
	//页数
	UPROPERTY()
		int32 CurPage = 0;
	//当前的消息个数
	UPROPERTY()
		int32 CurLogCount = 0;
	//私有的消息个数
	UPROPERTY()
		int32 PrivetLogCount = 0;
};
