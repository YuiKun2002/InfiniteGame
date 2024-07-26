// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSystem/Item/Material/MaterialBaseStruct.h"
#include "SynModel_Structures.generated.h"

/**
 * 合成屋的所有结构体集合
 */

 /**
  * 武器宝石镶嵌功能
  */

class UUI_PlayerSynthesis;
class UImage;
class UScrollBox;
class UUniformGridPanel;
class UButton;
class UWidgetBase;

//宝石槽位->1槽-2槽【槽位】
USTRUCT()
struct FSynModel_GemSlot {
	GENERATED_USTRUCT_BODY()
public:
	//头像
	UPROPERTY()
		UImage* M_ItemHead = nullptr;
	//等级
	UPROPERTY()
		UImage* M_ItemGrade = nullptr;
	//背景
	UPROPERTY()
		UImage* M_ItemBorderStyle = nullptr;
	//武器开槽或者卸载宝石
	UPROPERTY()
		UButton* M_ItemAttachmentButton = nullptr;
	//宝石点击
	UPROPERTY()
		UButton* M_ItemClickButton = nullptr;
};


//材料搜索类型绑定
USTRUCT()
struct FMaterialsSerachTypeBind {
	GENERATED_USTRUCT_BODY()
public:
	EMaterialType M_Type;
	TArray<FName> M_BindFnName;
public:
	FMaterialsSerachTypeBind() {}
	FMaterialsSerachTypeBind(const EMaterialType& _Type, const TArray<FName>& _BindFuncName)
	{
		this->M_Type = _Type;
		this->M_BindFnName = _BindFuncName;
	}
};

//关键词排除
USTRUCT()
struct FMaterialsSerachKeyWordsIgnore {
	GENERATED_USTRUCT_BODY()
public:
	FMaterialsSerachKeyWordsIgnore() {};
	FMaterialsSerachKeyWordsIgnore(const TArray<FString>& NewKeyWords) :KeyWords(NewKeyWords) {}

	FMaterialsSerachKeyWordsIgnore operator=(const FMaterialsSerachKeyWordsIgnore& NewKeyWords)
	{
		this->KeyWords = NewKeyWords.GetKeyWords();
		//this->KeyWords.Append(NewKeyWords.GetKeyWords());
		return *this;
	}

public:
	const TArray<FString>& GetKeyWords() const { return this->KeyWords; }
private:
	TArray<FString> KeyWords;
};


UCLASS()
class FVM_API USynModel_Structures : public UObject
{
	GENERATED_BODY()
public:
	//初始化宝石槽位组件绑定【只需要传递5个名称->3个Image和2个Button，与结构顺序匹配】
	static void InitComponentBindGemSlot(UWidgetBase* _widget, FSynModel_GemSlot& _comps, const TArray<FString>& _CompsName);
	//初始化宝石槽位组件按钮绑定【只需要传递两个UFUNCTION函数名称，与结构顺序匹配】
	static void InitComponentBindGemSlotButton(UObject* _Object, FSynModel_GemSlot& _comps, const TArray<FString>& _FunctionName);
};
