// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "Game/UI/HeroesData.h"
#include "GameSystem/GameDataSubsystem.h"
#include "GameSystem/Item/ItemBaseStruct.h"
#include "GameSystem/Tools/ItemLoadManager.h"
#include "UI_Heroes.generated.h"

class UImage;
class UButton;
class UTextBlock;
class UUI_HeroItem;

/**
 * 英雄界面
 */
UCLASS()
class FVM_API UUI_Heroes : public UWidgetBase
{
	GENERATED_BODY()
public:
	/************************************************************************/
	/*                                角色界面                               */
	/************************************************************************/
	UPROPERTY()
	TArray<FItemHeroBase> Heroes;
	//角色加载器
	UPROPERTY()
	UItemLoadManager* ItemLoadManagerItem = nullptr;
	//角色滑动区
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* Scroll_Item = nullptr;
	//角色格子界面
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UUniformGridPanel* Uniform_Item = nullptr;
	//创建格子界面
	UFUNCTION()
	UWidget* WidgetCreateInitHeroes(UItemDataTable* _Data, int32 _Index);
	//刷新格子界面
	UFUNCTION()
	void WidgetRefreshUpdateHeroes(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);
	//初始化角色界面
	UFUNCTION(BlueprintCallable)
	void InitHeroes();
	//初始化数据界面
	UFUNCTION(BlueprintCallable)
	void InitData();
	//选择角色
	UFUNCTION(BlueprintCallable)
	void Select();
	//强化
	UFUNCTION(BlueprintCallable)
	void Updage();
	//进化
	UFUNCTION(BlueprintCallable)
	void Evolve();
	//当前选择的角色
	UPROPERTY()
	class UUI_HeroItem* CurHeroItem = nullptr;
public:
	//获取当前选择的角色
	UFUNCTION(BlueprintPure)
	FItemHeroBase GetCurrentHeroData();
	FItemHeroBase& GetCurrentHeroDataByRef();
	//获取细节数据
	UFUNCTION(BlueprintPure)
	void GetHeroDetailData(FItemHeroBase& OutData);
	//获取是否可以强化
	UFUNCTION(BlueprintPure)
	bool GetUpgrade();
	//获取是否可以进化
	UFUNCTION(BlueprintPure)
	bool GetEvolve();
public:
	//英雄名称
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (BindWidget))
	UTextBlock* HeroName;
	//英雄星星等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* HeroStarLevel;
	//英雄稀有度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* HeroGrade;
	//英雄等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* HeroLevel;
	//选择按钮
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* Choose;
};
