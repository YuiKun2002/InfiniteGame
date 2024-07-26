// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameSystem/MouseStructManager.h"
#include "MouseViewEditorRoundNodeGrid.generated.h"

/**
 * 老鼠编辑器，回合节点格子
 */

class UGameMapUI_MouseTab;
class UGameMapUI_MouseViewEditor;
class UMouseViewSelectMouseItem;
class UMouseViewSelectMousePanel;

class UImage;
class UUniformGridPanel;
class UTextBlock;
class UVerticalBox;
class UEditableTextBox;
class UButton;

UCLASS()
class FVM_API UMouseViewEditorRoundNodeGrid : public UUserWidget
{
	GENERATED_BODY()

		friend class UGameMapUI_MouseViewEditor;

public:

	virtual bool Initialize() override;

public:

	//生成老鼠
	UFUNCTION(BlueprintCallable)
		void GenerateNewMouse();

	//初始化
	void InitEditorRoundNodeGrid(const int32& Row, const int32& Col, UGameMapUI_MouseViewEditor* GameMapUIMouseViewEditor);

	//老鼠节点检查
	void CheckMouseNode();

	//更新显示
	void UpdateView();

	void Enable();

	void DisEnable();
private:

	//老鼠编辑视图
	UPROPERTY()
		UGameMapUI_MouseViewEditor* GameMapUI_MouseViewEditor = nullptr;

	//生成老鼠
	UPROPERTY()
		UButton* GenerateMouse = nullptr;

	//显示老鼠的图片
	UPROPERTY()
		UImage* ShowMouseImg = nullptr;

	//行
	UPROPERTY()
		int32 RowIndex = 0;

	//列
	UPROPERTY()
		int32 ColIndex = 0;

	//当前节点的数据
	UPROPERTY()
		FMouseConfigNode MouseConfigNode;
};
