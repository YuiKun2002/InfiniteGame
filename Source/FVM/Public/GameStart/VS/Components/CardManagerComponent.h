// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameSystem/Item/ItemStruct.h"
#include "GameSystem/Tools/DynamicProperty.h"
#include "CardManagerComponent.generated.h"

class UUI_CardBar;
class ACardActor;
class ACardCoolDownManager;
//卡片
class UUI_Card;
//铲子
class UUI_EradicateCard;
//网格
class UUI_MapMeshe;
//翻书动画
class UPaperFlipbook;

DECLARE_DELEGATE_OneParam(CardManagerDynamicPropertyDelegate, UDynamicProperty*);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UCardManagerComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	//卡片管理器属性变动
	CardManagerDynamicPropertyDelegate OnCardManagerDynamicPropertyChangeDelegate;
	//卡片管理器动态属性
	UPROPERTY()
	UDynamicProperty* DynamicProperty = nullptr;
public:
	//卡片数据
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FItemCard> M_CardData;
	//卡片列表
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FString> M_CardName;
	//UIcard数据
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UUI_Card*> M_UICard;
	//从卡片列表选择索引作为快捷创建
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<int32> M_SpecialCardListIndex;
	//卡片冷却
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<ACardCoolDownManager*> M_CardCoolDownManager;
	//当前选择的卡片
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString M_CurrentSelectCardName;

	//上一次选择的卡片
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UClass* M_LastSelectCardClass;
	//上一次选择的卡片
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString M_LastSelectCardName;

	//当前是否选择了铲子
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool M_CurrentSelectEradicate = false;
	//当前选中的卡片实例
	UPROPERTY(EditAnywhere)
	UClass* M_CurrentSelectCardInstance;
	//3DUI的Actor容器
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* M_3DUIActor = nullptr;
	//卡片选择界面
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUI_CardBar* M_UUI_CardBar = nullptr;
	//当前玩家放置卡片发位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUI_MapMeshe* M_CurrentPlayCardMapMeshe = nullptr;
	//铲子UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUI_EradicateCard* M_UUI_EradicateCard = nullptr;
	//卡片默认动画路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> M_CardIdleAnim;
public:
	// Sets default values for this component's properties
	UCardManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	//设置当前卡片网格
	void SetCurrentCardMapMeshe(UUI_MapMeshe* _MapMeshe);
	//设置一张卡片的背景
	UFUNCTION(BlueprintCallable)
	void SetCard_Image(UUI_Card* _CardUI, const FString& _CardBg);
public:
	//获取当前卡片网页
	UUI_MapMeshe* GetCurrentCardMapMeshe();
	//获取卡片默认动画->通过CardActorName
	UFUNCTION(BlueprintCallable)
	UPaperFlipbook* GetCardIdleAnimByName(const FString& _CardActorName);
	//获取当前选中的Actor对象
	UFUNCTION(BlueprintCallable)
	UClass* GetCurrentSelectActor();
	//获取当前选中卡片的冷却
	UFUNCTION(BlueprintCallable)
	ACardCoolDownManager* GetCurrentCoolDownManager();
	//获取卡片的火苗数量
	UFUNCTION(BlueprintCallable)
	const int32 GetCardFlameNum(const FString& _CardActorName);
	//获取卡片数据
	UFUNCTION(BlueprintCallable)
	FItemCard& GetCardData(const FString& _CardActorName);
	//获取卡片数据通过卡片名称获取
	UFUNCTION(BlueprintCallable)
	bool GetCardDataByName(const FString& _CardName, FItemCard& _CardFItemData);
	//获取UICard
	UFUNCTION(BlueprintCallable)
	UUI_Card* const GetUICard(const FString& _CardActorName);
	//获取当前选择的卡片名称（通过UI选择卡片->选择卡片的名称）
	UFUNCTION(BlueprintCallable)
	const FString& GetCurrentSelectCardName();
	//获取指定卡片，是否冷却完成
	UFUNCTION(BlueprintCallable)
	bool GetCardCoolDownFinish(const int32& _Index);
	//卡片UI数组
	UFUNCTION(BlueprintCallable)
	const TArray<UUI_Card*>& GetUICardArrays();
	//通过索引获取卡片UI
	UFUNCTION(BlueprintCallable)
	UUI_Card* GetUICardArraysByIndex(int32 Index);
	//获取卡片栏
	UFUNCTION(BlueprintCallable)
	UUI_CardBar* GetUICardBar();
	//设置卡片冷却时间
	UFUNCTION(BlueprintCallable)
	void SetCardCoolDownByTag(EGamePropertyCategory Tag, float Rate = 1.f);
	//设置卡片费用
	UFUNCTION(BlueprintCallable)
	void SetCardPriceByTag(EGamePropertyCategory Tag, float Rate = 1.f);
	//改变属性
	UFUNCTION(BlueprintCallable)
	void ChangeProperty();
	//获取属性
	UFUNCTION(BlueprintPure)
	UDynamicProperty* GetDynamicProperty();
	//获取卡片管理器
public:
	//获取卡片原始数据
	FItemCard* const GetCardDataByName(const FString& _CardName);
public:
	//添加卡片（到卡片列表存储起来）
	UFUNCTION(BlueprintCallable)
	void AddCard(const FItemCard& _CardData);
	//添加特别的卡片(必须从卡片列表中选取)
	UFUNCTION(BlueprintCallable)
	bool AddSpecialCard(const int32& _CardListIndex);
	//添加冷却时间（将卡片的冷却时间添加到列表存储）
	UFUNCTION(BlueprintCallable)
	void AddCoolDown(const float& _Time);
public:
	//取消当前选择的卡片（通过UI选择卡片->取消）
	UFUNCTION(BlueprintCallable)
	void CancelSelect();
	//取消铲子功能
	UFUNCTION(BlueprintCallable)
	void CancelEradicate();
public:
	//游戏开始
	UFUNCTION(BlueprintCallable)
	void GameStart();
	//游戏结束
	UFUNCTION(BlueprintCallable)
	void GameOver();
public:
	//隐藏卡片栏
	UFUNCTION(BlueprintCallable)
	void HiddenCardBar();
public:
	//选择一张卡片（通过UI选择卡片->选择）
	UFUNCTION(BlueprintCallable)
	bool SelectCurrentActor(const FString& _CardActorName, bool ForceSelect = false);
	//选择铲子功能
	UFUNCTION(BlueprintCallable)
	void SelectEradicate();
	//显示卡片栏
	UFUNCTION(BlueprintCallable)
	void ShowCardBar();
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
