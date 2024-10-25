// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameSystem/GameMapStructManager.h"
#include "GameMapUI_MapViewEditor.generated.h"

/**
 *
 */

class UGameMapUI_MapTab;
class UGameMapUI_MapViewEditor;
class UFVMEditUI_GameMapEdit;
class UGameMapUI_MapViewCardItem;
class UGameMapUI_MapViewCardViewPanel;

class USizeBox;
class UImage;
class UTextBlock;
class UVerticalBox;
class UEditableTextBox;
class UButton;
class UCanvasPanel;
class UUniformGridPanel;
class UCanvasPanel;


//地图网格
UCLASS()
class FVM_API UGameMapUI_MapMesheItem : public UUserWidget
{
	GENERATED_BODY()

		friend class UGameMapUI_MapViewEditor;

public:

	virtual bool Initialize() override;


	void InitMapMesheItem(UGameMapUI_MapViewEditor* Class);

public:

	//获取配置
	UFUNCTION(BlueprintCallable)
		FMesheCol GetConfig();

	//更新配置
	UFUNCTION(BlueprintCallable)
		void UpdateConfig(FMesheCol MesheCol);

	//更新视图
	UFUNCTION(BlueprintCallable)
		void UpdateView();

	//选择当前网格
	UFUNCTION(BlueprintCallable)
		void Select();

	void UpdateCardView();

private:
	//地图编辑器
	UPROPERTY()
		UGameMapUI_MapViewEditor* MapViewEditor = nullptr;

	//卡片预览图集合
	UPROPERTY()
		TArray<UGameMapUI_MapViewCardViewPanel*> MapViewCardViews;

	//地图网格界面
	UPROPERTY()
		UCanvasPanel* MapCanvasPanel = nullptr;

	//按钮
	UPROPERTY()
		UButton* CurMeshe = nullptr;

	//是否开启移动的提升图片
	UPROPERTY()
		UImage* bMoveImg = nullptr;

	//背景图片
	UPROPERTY()
		UImage* BgImg = nullptr;

	//列数据
	UPROPERTY()
		FMesheCol CurCol;

	//当前地形
	UPROPERTY()
		ELineType CurLineType = ELineType::All;

	UPROPERTY()
		int32 Row = 0;

	UPROPERTY()
		int32 Col = 0;
};



//地图编辑器
UCLASS()
class FVM_API UGameMapUI_MapViewEditor : public UUserWidget
{
	GENERATED_BODY()

		friend class UGameMapUI_MapTab;
	friend class UGameMapUI_MapMesheItem;
	friend class UGameMapUI_MapViewCardItem;

public:

	virtual bool Initialize() override;

	//初始化编辑器
	UFUNCTION(BlueprintCallable)
		void InitMapViewEditor(UGameMapUI_MapTab* CurGameMapUI_MapTab);

	//获取编辑器
	UFUNCTION(BlueprintCallable)
		UFVMEditUI_GameMapEdit* GetEditor();

	//当初始化时
	UFUNCTION(BlueprintImplementableEvent)
		void OnInit(FGameMapStruct GameMapStruct);

	//当选择网格时
	UFUNCTION(BlueprintImplementableEvent)
		void OnSelectMeshe(FMesheCol MesheCol, int32 Row, int32 Col);

	//更新网格
	UFUNCTION(BlueprintCallable)
		void UpdateMeshe(FMesheCol MesheCol, int32 Row, int32 Col);

	//统一选择修改网格
	UFUNCTION(BlueprintCallable)
		void UniformSelectChangeMeshe(int32 Row, int32 Col, FMesheCol MesheCol);

	//更新视图
	UFUNCTION(BlueprintCallable)
		void UpdateWiew();

	void UpdateWiewIndex(const int32& Row, const int32& Col);

public:
	//新的行列，如果小于之前的行列，将永远被删除
	UFUNCTION(BlueprintCallable)
		void UpdateNewRowAndCol(int32 NewRow, int32 NewCol, FVector2D FirstGridLocation);

private:
	//           557
	//FVector2D(-527.f, -190.f);
	FVector2D Offset = FVector2D(0,0);
	//渲染网格
	void RenderMeshe();

private:

	//地图选项
	UPROPERTY()
		UGameMapUI_MapTab* GameMapUI_MapTab = nullptr;

	//网格集合容器
	UPROPERTY()
		UCanvasPanel* MeshePanel = nullptr;

	//地图背景
	UPROPERTY()
		UImage* Bg = nullptr;

	UPROPERTY()
		USizeBox* BgBox = nullptr;

	//卡片选择界面
	UPROPERTY()
		class UUniformGridPanel* CardsPanel = nullptr;

	//当前选择了卡片种植
	UPROPERTY()
		class UGameMapUI_MapViewCardItem* CurSelectMapViewCardItem = nullptr;
};
