// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "Engine/DataTable.h"
#include "UI_Dialogue.generated.h"

class UButton;
class UImage;
class UTextBlock;
class UScrollBox;
class UCanvasPanel;

UENUM(BlueprintType)
enum class EDialogueType : uint8 {
	Main UMETA(DisplayName = "Main"),
	Change UMETA(DisplayName = "Change"),
};


//对话结构
USTRUCT(BlueprintType)
struct FDialogueDataTable : public FTableRowBase {
	GENERATED_USTRUCT_BODY()
public:
	//类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EDialogueType Type;
	//角色名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString CharName;
	//角色形象
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath CharImg;
	//角色位置[左|右]
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bLeft = true;
	//对话内容
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString DialogueContent;
	//对话选项[选项内容，跳转对话]
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, int32> DialogueSelect;
	//跳转对话
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 JumpDialogueIndex = -1;
	//背景
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UTexture2D> DialogueBG;
	//背景显示颜色
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector BGColor = FVector(1.f);
	//音频持续时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DialogueTime = 1.f;
	//播放的音频
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath CharVoicePath;
	//播放的音频2【针对主角】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath Char2VoicePath;
	//背景音乐
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath BGMPath;
};

//对话系统
UCLASS()
class FVM_API UDialogueDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	//ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 DialogueID = 0;
	//剧情标题
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString DialogueTitle;
	//剧情目标位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString DialogueTarget;
	//数据表
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UDataTable> DialogueDataTable;
};

//对话数据集合
USTRUCT(BlueprintType)
struct FDialogueDataAssetDataTable : public FTableRowBase {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UDialogueDataAsset> DialogueDataAsset;
};

/**
 * 对话UI选择
 */
UCLASS()
class FVM_API UUI_DialogueSelect : public UWidgetBase
{
	GENERATED_BODY()

		friend class UUI_Dialogue;

public:
	virtual	bool Initialize() override;
public:
	//选择当前选项
	UFUNCTION(BlueprintCallable)
		void Select();
	//初始化
	void Init(class UUI_Dialogue* Class, const FString& Cont, const int32& Index,const int32& SelectIndex);
private:
	//对话内容
	UPROPERTY()
		UTextBlock* Content = nullptr;
	//跳转索引
	UPROPERTY()
		int32 JumpIndex = -1;
	//对应UI
	UPROPERTY()
		class UUI_Dialogue* UI = nullptr;
	//选择索引
	UPROPERTY()
		int32 DiaSelectIndex = 0;
};

/**
 * 对话UI
 */


 //当选择了选项时
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueSelection, int32, SelectIndex);
//当对话完成时
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueComplete);

UCLASS()
class FVM_API UUI_Dialogue : public UWidgetBase
{
	GENERATED_BODY()

		friend class UUI_DialogueSelect;

public:
	//初始化组件
	virtual	bool Initialize() override;
	virtual void NativeDestruct() override;
	//设置剧本
	void SetDialogue(int32 ID, const TSoftObjectPtr<UDialogueDataAsset>& Datas);
public:
	//当选择了选项时
	UPROPERTY(BlueprintAssignable)
		FOnDialogueSelection OnDialogueSelection;
	//当对话完成时
	UPROPERTY(BlueprintAssignable)
		FOnDialogueComplete OnDialogueComplete;
public:
	//继续对话
	UFUNCTION(BlueprintCallable)
		void ContineDialogue();
	//完成对话
	UFUNCTION(BlueprintCallable)
		void DialogueComplete();
	//设置下一句
	UFUNCTION(BlueprintCallable)
		void SetNextDialogue(const int32& Index);
	//添加选项
	UFUNCTION(BlueprintCallable)
		void AddSelect(const TMap<FString, int32>& SelectItems);
	//对话首次开始
	UFUNCTION(BlueprintImplementableEvent)
		void ExeDialogueFirst();
	//执行对话
	UFUNCTION(BlueprintImplementableEvent)
		void ExeDialogue(const EDialogueType& Type, const int32& CurDiaIndex, const FDialogueDataTable& CurDialogueData);
	//完成对话
	UFUNCTION(BlueprintImplementableEvent)
		void ExeDialogueFinished(const FDialogueDataTable& CurDialogueData);
private:
	//对话主界面
	UPROPERTY()
		UCanvasPanel* MainPanel = nullptr;
	//对话选择界面
	UPROPERTY()
		UCanvasPanel* SelectPanel = nullptr;
	//切换场景界面
	UPROPERTY()
		UCanvasPanel* ChangePanel = nullptr;
private:
	//左边角色
	UPROPERTY()
		UImage* LeftChar_Img = nullptr;
	//右边角色
	UPROPERTY()
		UImage* RightChar_Img = nullptr;
	//角色名称
	UPROPERTY()
		UTextBlock* CharName_Txt = nullptr;
	//文本内容
	UPROPERTY()
		UTextBlock* TextContent_Txt = nullptr;
private:
	//对话选择列表
	UPROPERTY()
		UScrollBox* SelectDialogueList = nullptr;
private:
	//转场文字
	UPROPERTY()
		UTextBlock* ChangeText = nullptr;
private:
	//当前对话标题
	UPROPERTY()
		FString CurDialogueTitle = TEXT("");
	//当前对话的数据表
	UPROPERTY()
		class UDataTable* CurDialogueDataTable = nullptr;
	//当前剧本
	UPROPERTY()
		TArray<FDialogueDataTable> CurDialogue;
	//对话索引
	UPROPERTY()
		int32 CurIndex = 0;
	//当前ID
	UPROPERTY()
		int32 CurID = -1;
private:
	//当前执行的对话
	UPROPERTY()
		TSoftObjectPtr<UDialogueDataAsset> CurDiaData;
	UPROPERTY()
		TSoftObjectPtr<UDataTable> CurDiaDataTable;

	//当前正在对话的剧本
	UPROPERTY()
		FDialogueDataTable CurDialogueSturct;
};
