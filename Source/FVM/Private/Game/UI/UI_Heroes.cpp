// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/UI_Heroes.h"
#include <Components/Image.h>
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include "Game/UI/UI_HeroItem.h"
#include "GameSystem/GameCacheSubsystem.h"


UWidget* UUI_Heroes::WidgetCreateInitHeroes(UItemDataTable* _Data, int32 _Index)
{
	UUI_HeroItem* ItemWidget = CreateWidget<UUI_HeroItem>(this,
		UGameSystemFunction::GetUserInterClassByName(UI_HEROESUINAME,
			TEXT("HeroItem"))
	);

	ItemWidget->SetWeaponData(_Data->GetTransValue<FItemHeroBase>(), this);
	ItemWidget->InitWeaponData();
	return ItemWidget;
}

void UUI_Heroes::WidgetRefreshUpdateHeroes(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	UUI_HeroItem* ItemWidget = Cast<UUI_HeroItem>(_UWidget);
	ItemWidget->SetWeaponData(_Data->GetTransValue<FItemHeroBase>(), this);
	ItemWidget->InitWeaponData();
}

void UUI_Heroes::InitHeroes()
{
	if (!IsValid(this->ItemLoadManagerItem))
	{
		//卡片加载器
		this->ItemLoadManagerItem = NewObject<UItemLoadManager>(this, TEXT("ItemLoadManager_Heroes"));
		this->ItemLoadManagerItem->InitWidget_First(this->Uniform_Item, this->Scroll_Item);
		this->ItemLoadManagerItem->InitRange_Second(4, 3, 800, 180.f, 162.f);
		this->ItemLoadManagerItem->OnCreateItem.BindUFunction(this, TEXT("WidgetCreateInitHeroes"));
		this->ItemLoadManagerItem->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefreshUpdateHeroes"));
	}

	if (!this->Heroes.Num())
	{
		this->Heroes = UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Heroes;
	}

	this->ItemLoadManagerItem->UpdateDatatable(
		this->Heroes
	);
	this->ItemLoadManagerItem->SetLoadItemMaxCount(
		this->Heroes.Num()
	);
	this->ItemLoadManagerItem->ContinueRun();

	this->InitData();
}

void UUI_Heroes::InitData()
{
	//初始化选择界面
	FItemHeroBase PlayerHeroData;
	if (IsValid(this->CurHeroItem))
	{
		this->CurHeroItem->GetHeroData(PlayerHeroData);
	}
	else {
		PlayerHeroData = this->GetCurrentHeroData();
	}

	//设置名称
	this->HeroName->SetText(PlayerHeroData.ItemName);
	//获取缓存对象
	UHeroItemDataAssetCache* CacheData = GetGameDataAssetCache<UHeroItemDataAssetCache>(HEROITEM_HEROITEM);
	//设置星级
	FSoftObjectPath LevelPath = CacheData->GetResource(HEROITEM_ITEMLEVEL, TEXT("道具星星等级"), PlayerHeroData.StarsLevel);
	this->SetImageBrushByTexture(this->HeroStarLevel, TSoftObjectPtr<UTexture2D>(LevelPath));
	//设置稀有度
	FSoftObjectPath RarityPath = CacheData->GetResource(HEROITEM_ITEMLEVEL, TEXT("道具稀有度"), PlayerHeroData.RarityLevel);
	this->SetImageBrushByTexture(
		this->HeroGrade, TSoftObjectPtr<UTexture2D>(RarityPath), FVector(1.f), 1.f, true
	);
	//设置等级
	this->HeroLevel->SetText(FText::FromString(TEXT("LV.") + FString::FromInt(PlayerHeroData.HeroLevel)));
	//设置选择按钮是否显示
	const FItemHeroBase& CurPlayerHeroData = UFVMGameInstance::GetPlayerStructManager_Static()->PlayerData;
	if (CurPlayerHeroData.M_ItemID == PlayerHeroData.M_ItemID)
	{
		this->Choose->SetVisibility(ESlateVisibility::Collapsed);
	}
	else {
		this->Choose->SetVisibility(ESlateVisibility::Visible);
	}
}

void UUI_Heroes::Select()
{
	UFVMGameInstance::GetPlayerStructManager_Static()->PlayerData = this->GetCurrentHeroData();

	UGameSystemFunction::SaveCurrentPlayerData(
		TEXT("选择当前角色:ATK=>") + FString::FromInt(this->GetCurrentHeroData().ATK) +
		TEXT("等级=>") + FString::FromInt(this->GetCurrentHeroData().HeroLevel)
	);

	this->InitHeroes();
}

void UUI_Heroes::Updage()
{
	FItemHeroBase& Ref = this->GetCurrentHeroDataByRef();
	FItemHeroBase& PlayerRef = UFVMGameInstance::GetPlayerStructManager_Static()->PlayerData;

	if (Ref.M_ItemID == PlayerRef.M_ItemID)
	{
		PlayerRef.HeroLevel++;
	}
	Ref.HeroLevel++;

	for (auto& Data : this->Heroes)
	{
		if (Data.M_ItemID == Ref.M_ItemID)
		{
			Data.HeroLevel++;
			break;
		}
	}


	//反存储到角色列表
	if (!UGameCacheSubsystem::GetNetMode())
	{
		for (auto& Data : UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Heroes)
		{
			if (Ref.M_ItemID == Data.M_ItemID)
			{
				Data = Ref;
				break;
			}
		}
	}
}

bool UUI_Heroes::GetUpgrade()
{
	FItemHeroBase& Ref = this->GetCurrentHeroDataByRef();

	if (Ref.HeroLevel < 50)
	{
		return true;
	}

	return false;
}

void UUI_Heroes::Evolve()
{
	FItemHeroBase& Ref = this->GetCurrentHeroDataByRef();
	FItemHeroBase& PlayerRef = UFVMGameInstance::GetPlayerStructManager_Static()->PlayerData;

	if (Ref.M_ItemID == PlayerRef.M_ItemID)
	{
		PlayerRef.StarsLevel++;
	}
	Ref.StarsLevel++;

	for (auto& Data : this->Heroes)
	{
		if (Data.M_ItemID == Ref.M_ItemID)
		{
			Data.StarsLevel++;
			break;
		}
	}

	//反存储到角色列表
	if (!UGameCacheSubsystem::GetNetMode())
	{
		for (auto& Data : UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerItems_Heroes)
		{
			if (Ref.M_ItemID == Data.M_ItemID)
			{
				Data = Ref;
				break;
			}
		}
	}
}

bool UUI_Heroes::GetEvolve()
{
	FItemHeroBase& Ref = this->GetCurrentHeroDataByRef();

	if (Ref.StarsLevel < 4)
	{
		int32 CurStarsLevel = Ref.HeroLevel / 10;
		if (CurStarsLevel - Ref.StarsLevel > 0)
		{
			return true;
		}
	}

	return false;
}

FItemHeroBase UUI_Heroes::GetCurrentHeroData()
{
	if (IsValid(this->CurHeroItem))
	{
		FItemHeroBase Data;
		this->CurHeroItem->GetHeroData(Data);
		return Data;
	}
	return UFVMGameInstance::GetPlayerStructManager_Static()->PlayerData;
}

FItemHeroBase& UUI_Heroes::GetCurrentHeroDataByRef()
{
	if (IsValid(this->CurHeroItem))
	{
		return this->CurHeroItem->GetHeroDataByRef();
	}
	return UFVMGameInstance::GetPlayerStructManager_Static()->PlayerData;
}

void UUI_Heroes::GetHeroDetailData(FItemHeroBase& OutData)
{
	FItemHeroBase Data = this->GetCurrentHeroDataByRef();

	OutData = UItemHeroDataFunc::Calculate(Data);
}

