// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/MapData/MapDataStruct.h"
#include "GameMapUI_EditorTab.generated.h"

class UTextBlock;
class UVerticalBox;
class UEditableTextBox;
class UButton;

//主界面
class UFVMEditUI_GameMapEdit;

//数据表显示的行按钮
UCLASS(BlueprintType)
class FVM_API UFVMEditUI_EditRowTableItem : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual bool Initialize() override;

	//初始化数据
	void InitData(class UGameMapUI_EditorTab* _Class);

	//设置行数据
	void SetRowName(const FString& _RowName);

	//配置
	UFUNCTION(BlueprintCallable)
	void Config();

private:
	//行名称
	FName RowName;

	//编辑器选项
	UPROPERTY()
	class UGameMapUI_EditorTab* Class = nullptr;
private:
	//按钮上的标题名称
	UPROPERTY()
	UTextBlock* Text_Title = nullptr;
};


UCLASS(BlueprintType)
class FVM_API UGameMapUI_EditorTab : public UUserWidget
{
	GENERATED_BODY()
public:
	//初始化
	virtual bool Initialize() override;
	//初始化编辑器选项
	void InitEditorTab(UFVMEditUI_GameMapEdit* _Class);
	//初始化【编辑器选项】，更新列表项
	void InitEditorTabListItems();
public:
	//获取编辑器
	UFVMEditUI_GameMapEdit* const GetGameMapEditor();
public:
	//加载数据表的行
	UFUNCTION(BlueprintCallable)
	void LoadRowDatas(FName RowName);
	//添加新的数据行
	UFUNCTION(BlueprintCallable)
	void AddNewRow(FName NewRowName);
	//修改外星人名称
	UFUNCTION(BlueprintCallable)
	void EditAlienName(const FString& SourceName,const FString& TargetName);
private:
	//主界面
	UPROPERTY()
	UFVMEditUI_GameMapEdit* FVMEditUI_GameMapEdit = nullptr;
	//编辑器【做出编辑器选项】

	//右侧显示，地图数据表的所有行按钮
	UPROPERTY()
	UVerticalBox* EditorTab_ListViewPanel = nullptr;
	//新增行命名文字输入
	UPROPERTY()
	UEditableTextBox* EditorTab_NewRowInputText = nullptr;
	//新增行命名保存按钮
	UPROPERTY()
	UButton* EditorTab_NewRowSaveButt = nullptr;
};
