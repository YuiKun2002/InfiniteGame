// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/CardData/MouseDataStruct.h"
#include "MouseViewSelectMousePanel.generated.h"

class UGameMapUI_MouseViewEditor;
class UImage;
class UUniformGridPanel;
class UTextBlock;
class UVerticalBox;
class UEditableTextBox;
class UButton;

/**
 *	老鼠选择项
 */
UCLASS()
class FVM_API UMouseViewSelectMouseItem : public UUserWidget
{
	GENERATED_BODY()

		friend class UGameMapUI_MouseViewEditor;
	friend class UMouseViewSelectMousePanel;

public:

	virtual bool Initialize() override;

	void InitSelectMouseItem(class UMouseViewSelectMousePanel* SelectMousePanel, const FMouse_Data& Data);

	UFUNCTION(BlueprintCallable)
		void Select();

	UFUNCTION(BlueprintCallable)
		void Cancel();
public:
	UFUNCTION(BlueprintImplementableEvent)
		void OnSelect(FMouse_Data Data);

	UFUNCTION(BlueprintImplementableEvent)
		void OnCancel();
private:
	//选择项的按钮
	UPROPERTY()
		UButton* CurButton = nullptr;
	//当前项的老鼠数据
	UPROPERTY()
		FMouse_Data MouseData;
	//老鼠选择界面
	UPROPERTY()
		UMouseViewSelectMousePanel* CurSelectMousePanel = nullptr;
};


/**
 *	老鼠选择界面
 */
UCLASS()
class FVM_API UMouseViewSelectMousePanel : public UUserWidget
{
	GENERATED_BODY()

		friend class UGameMapUI_MouseViewEditor;
	friend class UMouseViewSelectMouseItem;

public:

	virtual bool Initialize() override;

	void InitMouseViewSelectMousePanel();

	UFUNCTION(BlueprintCallable)
		void ShowType(const FString& _Type);

public:

	//当选择老鼠项时
	UFUNCTION(BlueprintImplementableEvent)
		void OnSelect(FMouseConfigNode _NewNode);

	//当取消选择时
	UFUNCTION(BlueprintImplementableEvent)
		void OnCancel();

	//设置新的老鼠节点配置
	UFUNCTION(BlueprintCallable)
		void SetNewMouseNode(FMouseConfigNode MouseNode);
	//获取当前老鼠的节点配置
	UFUNCTION(BlueprintCallable)
		FMouseConfigNode GetMouseNode();

	//获取当前的选择项
	UFUNCTION(BlueprintCallable)
		UMouseViewSelectMouseItem* GetMouseViewSelectMouseItem();
private:
	//显示选择老鼠的容器
	UPROPERTY()
		UUniformGridPanel* UniformGridPanel = nullptr;
	//所有老鼠的数据
	UPROPERTY()
		UDataTable* AllMouseDataTable = nullptr;
	//当前选择的老鼠项
	UPROPERTY()
		UMouseViewSelectMouseItem* CurSelectMouseItem = nullptr;
	//当前老鼠统一节点设置
	UPROPERTY()
		FMouseConfigNode CurUnifromMouseNodeSetting;
};
