// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除
#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "Engine/Classes/Engine/DataTable.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "Game/UI/UI_ButtonTemp.h"
#include "UI_Notice.generated.h"

//公告结构
USTRUCT(BlueprintType)
struct FGameNotice {
	GENERATED_USTRUCT_BODY()
public:
	//按钮标题名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_ButtonTitle = FString(TEXT("选项卡1"));
	//内容标题名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_ContentTitle = FString(TEXT("文本"));
	//内容
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_Content = FString(TEXT("<t>我妻由乃</>"));
};

//公告结构体数据表
USTRUCT(BlueprintType)
struct FGameNoticeDataTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//攻击类型的卡片
	UPROPERTY(EditDefaultsOnly)
	FGameNotice M_FGameNotice;
};

class UVerticalBox;
class UScrollBox;
class URichTextBlock;
class UUniformGridPanel;

class UItemLoadManager;
class UItemDataTable;

UCLASS()
class FVM_API UUI_Notice : public UWidgetBase
{
	GENERATED_BODY()
private:
	//列表
	UPROPERTY()
	UScrollBox* M_TableListGridList = nullptr;
	//列表格子
	UPROPERTY()
	UUniformGridPanel* M_Load_Uniform_Grid = nullptr;
	//按钮集合(组)
	UPROPERTY()
	TArray<UUI_ButtonTemp*> M_ButtonTempGroup;
	//按钮集合数据(组)
	UPROPERTY()
	TArray<FGameNoticeDataTable> M_ButtonTempGroupDatas;
	//富文本组件
	UPROPERTY()
	URichTextBlock* M_URichTextBlock = nullptr;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_Title;
private:
	//物品加载器
	UPROPERTY()
	UItemLoadManager* M_UItemLoadManager = nullptr;
	//是否初始化
	UPROPERTY()
	bool bInit = false;
private:
	UPROPERTY()
	TArray<FGameNoticeDataTable> LData;
public:
	//初始化
	bool Initialize() override;

	//创建加载
	UFUNCTION()
	UWidget* BeginLoadList(UItemDataTable* _item, int32 _index);
	//更新加载
	UFUNCTION()
	void UpdateList(UItemDataTable* _item, int32 _Data_Index, UWidget* _Widget);
	//预更新
	UFUNCTION()
	void BeginUpdate();

	//添加到组
	void AddToGroup(FGameNoticeDataTable _Data, UUI_ButtonTemp* _table_button = nullptr);
	//点击选项卡
	void ClickTab(int32 _index);
public:
	//开始
	UFUNCTION(BlueprintCallable)
	void Begin();
	//关闭界面
	UFUNCTION(BlueprintCallable)
	void bClosePanel();
};
