// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameSystem/MouseStructManager.h"
#include "Data/MapData/Editor/MouseTab/GameMapUI_MouseTab.h"
#include "GameMapUI_MouseViewEditor.generated.h"

/**
 * 编辑器，老鼠可视化编辑窗口
 */

class UFVMEditUI_GameMapEdit;
class UGameMapUI_MouseTab;
class UMouseViewEditorRoundNodeGrid;
class UMouseViewSelectMousePanel;
class UMouseViewSelectMouseItem;

class UImage;
class UHorizontalBox;
class UUniformGridPanel;
class UTextBlock;
class UVerticalBox;
class UEditableTextBox;
class UButton;


class UGameMapUI_MouseViewEditor;


//物品掉落预览项
UCLASS()
class FVM_API UGameMapUI_MouseViewLevelItemView : public UUserWidget
{
	GENERATED_BODY()

		friend class UGameMapUI_MouseViewEditor;


public:

	virtual bool Initialize() override;

	void Init(FString Name, FSoftObjectPath Img,UGameMapUI_MouseViewEditor* Class);

public:
	UFUNCTION()
		void Remove();

private:

	UPROPERTY()
		UGameMapUI_MouseViewEditor* GameMapUI_MouseViewEditor = nullptr;

	UPROPERTY()
		FString ItemName;

	UPROPERTY()
		UButton* Butt = nullptr;

};


//物品掉落选择项
UCLASS()
class FVM_API UGameMapUI_MouseViewLevelItem : public UUserWidget
{
	GENERATED_BODY()

		friend class UGameMapUI_MouseViewEditor;

public:

	virtual bool Initialize() override;

	void Init(UGameMapUI_MouseViewEditor* OWner, const FString& ItemName, FSoftObjectPath Img);

public:

	UFUNCTION(BlueprintCallable)
		void AddLevelItem();

private:

	UPROPERTY()
		UGameMapUI_MouseViewEditor* GameMapUI_MouseViewEditor = nullptr;

	UPROPERTY()
		UTextBlock* ItemNameText = nullptr;

	UPROPERTY()
		UButton* Butt = nullptr;

	UPROPERTY()
		FSoftObjectPath ImgRes;
};



UCLASS()
class FVM_API UGameMapUI_MouseViewEditor : public UUserWidget
{
	GENERATED_BODY()

		friend class UMouseViewEditorRoundNodeGrid;
	friend class UMouseViewSelectMousePanel;
	friend class UMouseViewSelectMouseItem;
	friend class UGameMapUI_MouseViewEditor;
	friend class UGameMapUI_MouseViewLevelItem;
	friend class UGameMapUI_MouseViewLevelItemView;

public:

	virtual bool Initialize() override;

	//初始化编辑数据
	UFUNCTION(BlueprintCallable)
		void InitMouseViewEditor(UFVMEditUI_GameMapEdit* Class, UGameMapUI_MouseTab* OwnerClass);

	//开初始化
	UFUNCTION(BlueprintImplementableEvent)
		void OnInit();
public:
	//获取编辑器
	UFUNCTION(BlueprintCallable)
		UFVMEditUI_GameMapEdit* GetEditor();
	//获取老鼠选项
	UFUNCTION(BlueprintCallable)
		UGameMapUI_MouseTab* GetMouseTab();
private:
	//初始化最小子节点
	void InitRoundNodeGrid();

	//更新文本
	UFUNCTION(BlueprintCallable)
		void UpdateText();

	//检查最小子节点数量是否正确
	void CheckTimeNodeCount();
	//更新老鼠节点显示
	void UpdateView();
	//显示老鼠节点
	void EnableView();
	//关闭老鼠节点
	void DisEnableView();
private:
	//添加波
	UFUNCTION(BlueprintCallable)
		void Node_AddNewCurRound();
	//删除当前波
	UFUNCTION(BlueprintCallable)
		void Node_RemoveCurRound();
	//倒退波索引
	UFUNCTION(BlueprintCallable)
		void LastCurRound();
	//前进波索引
	UFUNCTION(BlueprintCallable)
		void NextCurRound();

	//添加节点
	UFUNCTION(BlueprintCallable)
		void Node_AddNewRoundNode();
	//删除当前节点
	UFUNCTION(BlueprintCallable)
		void Node_RemoveCurRoundNode();
	//倒退节点索引
	UFUNCTION(BlueprintCallable)
		void LastCurRoundNode();
	//前进节点索引
	UFUNCTION(BlueprintCallable)
		void NextCurRoundNode();


	//设置老鼠生成完毕是否自动进入下一个回合
	UFUNCTION(BlueprintCallable)
		void SetAllMouseSpawnFinishAutoNext(bool Enable);
	UFUNCTION(BlueprintCallable)
		bool GetAllMouseSpawnFinishAutoNext();

	//设置进入下一个回合的准备时间
	UFUNCTION(BlueprintCallable)
		void SetInNextRoundTime(float InNdexTime);
	UFUNCTION(BlueprintCallable)
		float GetInNextRoundTime();

	//设置当前波是否需要钥匙
	UFUNCTION(BlueprintCallable)
		void SetCurRoundKey(FRoundKey Key);
	UFUNCTION(BlueprintCallable)
		FRoundKey GetCurRoundKey();

public:
	//选择一个最小子节点中的老鼠节点
	UFUNCTION(BlueprintCallable)
		void SelectRoundNodeWithMouseNode(UMouseViewEditorRoundNodeGrid* CurMouseViewEditorRoundNodeGrid);
	//更新当前被选择的节点
	UFUNCTION(BlueprintCallable)
		void UpdateCurRoundNodeWidthMouseNode(FMouseConfigNode NewNode);
	//当选择了一个节点时
	UFUNCTION(BlueprintImplementableEvent)
		void OnSelectMouseNode(FMouseConfigNode Node);
	//当选择的节点无效时
	UFUNCTION(BlueprintImplementableEvent)
		void OnSelectMouseNodeIsNull();


	//老鼠节点移除模式
	UFUNCTION(BlueprintCallable)
		void EnableMouseNodeRemove();

	//老鼠节点开启
	UFUNCTION(BlueprintImplementableEvent)
		void OnMouseNodeRemoveOpen();

	//老鼠节点关闭
	UFUNCTION(BlueprintImplementableEvent)
		void OnMouseNodeRemoveClose();

public:
	//初始化掉落物
	void InitLevelItems();
	//更新当前的掉落物显示
	void UpdateLevelItems();
private:
	template<class T>
	void Remove(TArray<T>& Array, int32& Index)
	{
		if (Array.Num() == 0)
		{
			return;
		}

		if (Index < Array.Num())
		{
			//移除索引
			Array.RemoveAt(Index);

			//如果波数量小于等于索引
			if (Array.Num() <= Index)
			{
				//数组最后一个索引将等于现在的索引
				Index = Array.Num() - 1;
				//索引永远不为负
				if (Index < 0)
				{
					Index = 0;
				}
			}
		}

		//更新视图
		this->GameMapUI_MouseTab->UpdateView();
	}

	void Last(int32& LIndex)
	{
		if (LIndex > 0)
		{
			LIndex--;
		}
	}

	void InNext(const int32& Count, int32& LIndex)
	{
		if (LIndex < Count)
		{
			LIndex++;
			if (LIndex == Count)
			{
				LIndex = Count - 1;
			}
		}
	}

public:
	int32 GetCurRoundIndex()
	{
		return this->CurRoundIndex;
	}
	int32 GetCurRoundNodeIndex()
	{
		return this->CurRoundNodeIndex;
	}

	UGameMapUI_MouseTab* const GetGameMapUIMouseTab()
	{
		return this->GameMapUI_MouseTab;
	}

private:
	//编辑器
	UPROPERTY()
		UFVMEditUI_GameMapEdit* FVMEditUI_GameMapEdit = nullptr;
	//老鼠选项卡
	UPROPERTY()
		UGameMapUI_MouseTab* GameMapUI_MouseTab = nullptr;
	//老鼠选择界面
	UPROPERTY()
		UMouseViewSelectMousePanel* MouseViewSelectMousePanel = nullptr;
	//当前被选择了的老鼠配置节点
	UPROPERTY()
		UMouseViewEditorRoundNodeGrid* MouseViewEditorRoundNodeGrid = nullptr;
private:
	//背景图片
	UPROPERTY()
		UImage* MapBg = nullptr;
	//子回合节点的最小子节点
	UPROPERTY()
		UUniformGridPanel* RoundNodeGridPanel = nullptr;
	//回合掉落物总界面
	UPROPERTY()
		UUniformGridPanel* LevelItemsGridPanel = nullptr;
	//是否启动老鼠移除模式
	UPROPERTY()
		bool bEnableMouseNodeRemove = false;
private:
	//总波数
	UPROPERTY()
		int32 TotalRound = 0;
	UPROPERTY()
		UTextBlock* Text_TotalRoundCount = nullptr;
	//当前第x波
	UPROPERTY()
		int32 CurRoundIndex = 0;
	UPROPERTY()
		UTextBlock* Text_CurRoundIndex = nullptr;
	//总节点数
	UPROPERTY()
		int32 TotalRoundNode = 0;
	UPROPERTY()
		UTextBlock* Text_TotalRoundNodeCount = nullptr;
	//当前节点索引
	UPROPERTY()
		int32 CurRoundNodeIndex = 0;
	UPROPERTY()
		UTextBlock* Text_CurRoundNodeIndex = nullptr;
private:
	//需要显示的掉落物品
	UPROPERTY()
		TArray<FSoftObjectPath> LevelItemList;
	//所有界面显示的容器
	UPROPERTY()
		UHorizontalBox* CurShowLevelItemListPanel = nullptr;

};
