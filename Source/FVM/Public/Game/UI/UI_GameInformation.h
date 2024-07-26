// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "Engine/DataTable.h"
#include "Data/MapData/MapDataStruct.h"
#include "GameSystem/Tools/ItemLoadManager.h"
#include "UI_GameInformation.generated.h"

/**
 * 情报岛UI
 */

class UImage;
class UButton;
class UTextBlock;
class UScrollBox;
class UUniformGridPanel;

//情报结构
USTRUCT(BlueprintType)
struct FGameInformationStructData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	//物品名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Name;
	//图片
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath Img;
};

UCLASS()
class FVM_API UUI_GameInformationMapItemBox : public UWidgetBase
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;
public:
	UPROPERTY()
		UImage* HeadImg = nullptr;
	UPROPERTY()
		FString ItemName;
public:
	UFUNCTION()
		void SetHiLight(bool bvalue);
	UFUNCTION(BlueprintCallable)
		FString GetName();

};

UCLASS()
class FVM_API UUI_GameInformationBox : public UWidgetBase
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;
protected:
	UPROPERTY()
		UImage* Head = nullptr;
	UPROPERTY()
		UButton* Bg = nullptr;
	UPROPERTY()
		UTextBlock* Title = nullptr;
	UPROPERTY()
		FString Name;
	UPROPERTY()
		FSoftObjectPath ImgPath;
public:
	UPROPERTY()
		class UUI_GameInformation* Cur = nullptr;
public:
	//设置名称
	void SetName(const FString& LName, const FSoftObjectPath& LImgPath);
	//显示
	UFUNCTION(BlueprintCallable)
		virtual	void ShowContent();
};

//卡片显示
UCLASS()
class FVM_API UUI_GameInformationBoxCard : public UUI_GameInformationBox
{
	GENERATED_BODY()
public:
	//显示
	virtual	void ShowContent() override;
private:
	//头像
	UPROPERTY()
		UImage* CardHead = nullptr;
	//卡片类型文本
	UPROPERTY()
		UTextBlock* CardTypeText = nullptr;
	UPROPERTY()
		FString CardType;
	//卡片价格文本
	UPROPERTY()
		UTextBlock* CardPriceText = nullptr;
	UPROPERTY()
		FString CardPrice;
	//卡片介绍文本
	UPROPERTY()
		UTextBlock* CardDesText = nullptr;
	UPROPERTY()
		FString CardDes;
	//是否初始化
	UPROPERTY()
		bool bInit = false;
};

//老鼠显示
UCLASS()
class FVM_API UUI_GameInformationBoxMouse : public UUI_GameInformationBox
{
	GENERATED_BODY()
public:
	//显示
	virtual	void ShowContent() override;
protected:
	//头像
	UPROPERTY()
		UImage* MouseHead = nullptr;
	//老鼠类型文本
	UPROPERTY()
		UTextBlock* MouseTypeText = nullptr;
	UPROPERTY()
		FString MouseType;
	//老鼠血量文本
	UPROPERTY()
		UTextBlock* MouseHPText = nullptr;
	UPROPERTY()
		FString MouseHP;
	//老鼠速度文本
	UPROPERTY()
		UTextBlock* MouseSpeedText = nullptr;
	UPROPERTY()
		FString  MouseSpeed;
	//老鼠介绍文本
	UPROPERTY()
		UTextBlock* MouseDesText = nullptr;
	UPROPERTY()
		FString MouseDes;
	//是否初始化
	UPROPERTY()
		bool bInit = false;
};

//地图显示
UCLASS()
class FVM_API UUI_GameInformationBoxMap : public UUI_GameInformationBox
{
	GENERATED_BODY()
public:
	//显示
	virtual	void ShowContent() override;
protected:
	//头像
	UPROPERTY()
		UImage* MapHead = nullptr;
	//地图类型文本
	UPROPERTY()
		UTextBlock* MapTypeText = nullptr;
	UPROPERTY()
		FString MapType;
	//地图难度文本
	UPROPERTY()
		UTextBlock* MapDifText = nullptr;
	UPROPERTY()
		FString  MapDif;
	//地图等级
	UPROPERTY()
		UTextBlock* MapLevelText = nullptr;
	UPROPERTY()
		FString  MapLevel;
	//卡片等级
	UPROPERTY()
		UTextBlock* MapCardLevelText = nullptr;
	UPROPERTY()
		FString  MapCardLevel;
	//关卡波数
	UPROPERTY()
		UTextBlock* MapRoundText = nullptr;
	UPROPERTY()
		FString  MapRound;
	//卡片推荐
	UPROPERTY()
		UTextBlock* MapCardText = nullptr;
	UPROPERTY()
		FString  MapCard;
	//是否初始化
	UPROPERTY()
		bool bInit = false;
protected:
	UPROPERTY()
		TMap<FString, FSoftObjectPath> LevelItems;
	UPROPERTY()
		int32 HiCount = 1;
};

UCLASS()
class FVM_API UUI_GameInformation : public UWidgetBase
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;
public:
	//选择的名称
	UPROPERTY()
		UTextBlock* SelectTitleName = nullptr;
	//老鼠数据表
	UPROPERTY()
		class UDataTable* MouseDataTable = nullptr;

	//地图材料加载器
	UPROPERTY()
		UItemLoadManager* TimeLoad_MapM_Manager = nullptr;
	UPROPERTY()
		TArray<FGameInformationStructData> MapItemDatas;

	//普通材料高亮显示
	UPROPERTY()
		int32 CurMapItemCount = 1;
	//关卡配置数据
	UPROPERTY()
		TArray<FLevelConfig> MapLevelConfigs;
	UPROPERTY()
		TMap<FString, int32> MapRound;
private:
	//加载资源的路径
	UPROPERTY()
		FString LoadPath;

	//卡片加载器
	UPROPERTY()
		UItemLoadManager* TimeLoad_Card_Manager = nullptr;
	//卡片
	UPROPERTY()
		UUniformGridPanel* UniformCard = nullptr;
	//卡片
	UPROPERTY()
		UScrollBox* ScrollBoxCard = nullptr;


	//老鼠加载器
	UPROPERTY()
		UItemLoadManager* TimeLoad_Mouse_Manager = nullptr;
	//老鼠
	UPROPERTY()
		UUniformGridPanel* UniformMouse = nullptr;
	//老鼠
	UPROPERTY()
		UScrollBox* ScrollBoxMouse = nullptr;

	//地图加载器
	UPROPERTY()
		UItemLoadManager* TimeLoad_Map_Manager = nullptr;
	//地图
	UPROPERTY()
		UUniformGridPanel* UniformMap = nullptr;
	//地图
	UPROPERTY()
		UScrollBox* ScrollBoxMap = nullptr;


	//地图材料
	UPROPERTY()
		UUniformGridPanel* UniformMapM = nullptr;
	//地图材料
	UPROPERTY()
		UScrollBox* ScrollBoxMapM = nullptr;
protected:
	//创建界面
	UFUNCTION()
		UWidget* WidgetCreate_Card(UItemDataTable* _Data, int32 _Index);
	//刷新卡片界面
	UFUNCTION()
		void WidgetRefresh(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);

	//创建材料界面
	UFUNCTION()
		UWidget* WidgetCreate_MapItem(UItemDataTable* _Data, int32 _Index);
	//刷新材料界面
	UFUNCTION()
		void WidgetRefresh_MapItem(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget);
public:
	//显示卡片列表
	UFUNCTION(BlueprintCallable)
		void ShowCard(const FString& SearchKey);
	//显示老鼠列表
	UFUNCTION(BlueprintCallable)
		void ShowMouse(const FString& SearchKey);
	//显示地图列表
	UFUNCTION(BlueprintCallable)
		void ShowMap(const FString& SearchKey);
protected:
	UPROPERTY()
		TArray<FGameInformationStructData> Cards;
	UPROPERTY()
		TArray<FGameInformationStructData> KeyCards;

	UPROPERTY()
		TArray<FGameInformationStructData> Mouses;
	UPROPERTY()
		TArray<FGameInformationStructData> KeyMouses;

	UPROPERTY()
		TArray<FGameInformationStructData> CurMapDatas;
	UPROPERTY()
		TArray<FGameInformationStructData> KeyMaps;
};
