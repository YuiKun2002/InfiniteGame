// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/UI_GameInformation.h"
#include "Engine/DataTable.h"
#include "Data/CardData/MouseDataStruct.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/TaskSubsystem.h"
#include <Components/Button.h>
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include <Components/UniformGridPanel.h>
#include <Components/ScrollBox.h>

bool UUI_GameInformationMapItemBox::Initialize()
{
	Super::Initialize();

	this->HeadImg = this->GetWidgetComponent<UImage>(this, TEXT("Head"));

	return true;
}


void UUI_GameInformationMapItemBox::SetHiLight(bool bvalue)
{
	if (bvalue)
	{
		this->HeadImg->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
	else {
		this->HeadImg->SetColorAndOpacity(FLinearColor(0.2f, 0.2f, 0.2f, 1.f));
	}
}

FString UUI_GameInformationMapItemBox::ToString_Implementation()
{
	return this->ItemName;
}

bool UUI_GameInformationBox::Initialize()
{
	Super::Initialize();

	this->Head = this->GetWidgetComponent<UImage>(this, TEXT("Image_106"));
	this->Bg = this->GetWidgetComponent<UButton>(this, TEXT("Button_18"));
	this->Title = this->GetWidgetComponent<UTextBlock>(this, TEXT("TextBlock_235"));

	return true;
}

void UUI_GameInformationBox::SetName(const FString& LName, const FSoftObjectPath& LImgPath)
{
	this->Name = LName;
	this->ImgPath = LImgPath;
	this->Title->SetText(FText::FromString(LName));
	this->SetImageBrush(this->Head, LImgPath.ToString());
}
void UUI_GameInformationBox::ShowContent() {
	if (IsValid(this->Cur))
	{
		Cur->SelectTitleName->SetText(FText::FromString(this->Name));

		//存储查看历史记录
		UPlayerRecord::AddDaySeeInforCount(this->Name);

		//执行任务
		UTaskSubsystem::GetTaskSubsystemStatic()->ExecuteTasks(this);
	}
}


void UUI_GameInformationBoxCard::ShowContent()
{
	Super::ShowContent();
	if (IsValid(this->Cur))
	{
		if (!this->bInit)
		{
			this->CardTypeText = this->GetWidgetComponent<UTextBlock>(this->Cur, TEXT("TextBlock_1"));
			this->CardPriceText = this->GetWidgetComponent<UTextBlock>(this->Cur, TEXT("TextBlock_3"));
			this->CardDesText = this->GetWidgetComponent<UTextBlock>(this->Cur, TEXT("TextBlock_5"));
			this->CardHead = this->GetWidgetComponent<UImage>(this->Cur, TEXT("CardHead"));
			this->bInit = true;
		}

		if (this->bInit)
		{
			FItemCard CurD;
			if (UCardBaseStruct::SearchCardFromDataTable(this->Name, CurD))
			{
				if (CurD.M_ELineType == ELineType::Sky)
				{
					this->CardType = TEXT("空中");
				}
				else if (CurD.M_ELineType == ELineType::OnWater)
				{
					this->CardType = TEXT("水上");
				}
				else if (CurD.M_ELineType == ELineType::Magma)
				{
					this->CardType = TEXT("岩浆");
				}
				else {
					this->CardType = TEXT("陆地");
				}

				this->CardPrice = FString(FString::FromInt(CurD.M_CardPrice));
				if (CurD.M_CardPriceAutoUp)
				{
					this->CardPrice += TEXT("+");
				}

				this->CardDes = CurD.ItemDescrible + TEXT("\n\r") + CurD.ItemDescrible_2;
			}

			this->CardTypeText->SetText(FText::FromString(this->CardType));
			this->CardPriceText->SetText(FText::FromString(this->CardPrice));
			this->CardDesText->SetText(FText::FromString(this->CardDes));
			this->SetImageBrush(this->CardHead, this->ImgPath.ToString());
		}
	}
}

void UUI_GameInformationBoxMouse::ShowContent()
{
	Super::ShowContent();

	if (IsValid(this->Cur))
	{
		if (!this->bInit)
		{
			this->MouseTypeText = this->GetWidgetComponent<UTextBlock>(this->Cur, TEXT("TextBlock_9"));
			this->MouseSpeedText = this->GetWidgetComponent<UTextBlock>(this->Cur, TEXT("TextBlock_11"));
			this->MouseDesText = this->GetWidgetComponent<UTextBlock>(this->Cur, TEXT("TextBlock_13"));
			this->MouseHPText = this->GetWidgetComponent<UTextBlock>(this->Cur, TEXT("TextBlock_30"));
			this->MouseHead = this->GetWidgetComponent<UImage>(this->Cur, TEXT("MouseHead"));
			this->bInit = true;
		}

		if (this->bInit)
		{
			FMouse_Data CurD;
			TArray<FTableRowBase*> Datas;
			this->Cur->MouseDataTable->GetAllRows(TEXT("MouseList"), Datas);
			for (FTableRowBase* CurData : Datas)
			{
				FMouse_Data* Local = ((FMouse_Data*)(CurData));
				if (Local->M_Mouse.M_MouseName.Equals(this->Name))
				{
					CurD = *Local;
					break;
				}
			}

			if (CurD.M_Mouse.LineType == ELineType::Sky)
			{
				this->MouseType = TEXT("空中");
			}
			else {
				this->MouseType = TEXT("陆地");
			}

			this->MouseHP = FString::FromInt(CurD.M_Mouse.M_MouseBaseHP);
			this->MouseSpeed = FString(FString::FromInt(((int32)(CurD.M_Mouse.M_MouseBaseSpeed))));

			FString CurTemp = CurD.M_Mouse.Des;
			this->MouseDes = CurTemp.Replace(TEXT("</>"), TEXT("\n\r"));


			this->MouseTypeText->SetText(FText::FromString(this->MouseType));
			this->MouseSpeedText->SetText(FText::FromString(this->MouseSpeed));
			this->MouseDesText->SetText(FText::FromString(this->MouseDes));
			this->MouseHPText->SetText(FText::FromString(this->MouseHP));
			this->SetImageBrush(this->MouseHead, this->ImgPath.ToString());
		}
	}
}


void UUI_GameInformationBoxMap::ShowContent()
{
	Super::ShowContent();

	if (IsValid(this->Cur))
	{
		if (!this->bInit)
		{
			this->MapTypeText = this->GetWidgetComponent<UTextBlock>(this->Cur, TEXT("TextBlock_15"));
			this->MapDifText = this->GetWidgetComponent<UTextBlock>(this->Cur, TEXT("TextBlock_28"));
			this->MapLevelText = this->GetWidgetComponent<UTextBlock>(this->Cur, TEXT("TextBlock_22"));
			this->MapCardLevelText = this->GetWidgetComponent<UTextBlock>(this->Cur, TEXT("TextBlock_24"));
			this->MapRoundText = this->GetWidgetComponent<UTextBlock>(this->Cur, TEXT("TextBlock_27"));
			this->MapCardText = this->GetWidgetComponent<UTextBlock>(this->Cur, TEXT("TextBlock_20"));
			this->MapHead = this->GetWidgetComponent<UImage>(this->Cur, TEXT("MapHead"));
			this->bInit = true;
		}

		if (this->bInit)
		{
			FLevelConfig CurD;
			for (const FLevelConfig& LocalConfig : this->Cur->MapLevelConfigs)
			{
				if (LocalConfig.LevelName.Equals(this->Name))
				{
					CurD = LocalConfig;
					break;
				}
			}

			this->LevelItems = CurD.LevelItems;
			this->HiCount = CurD.HiLightCount;

			if (CurD.LevelIsDay)
			{
				this->MapType = TEXT("白天");
			}
			else {
				this->MapType = TEXT("夜晚");
			}

			int32 MapLV = CurD.MouseLevel;
			this->MapLevel = FString::FromInt(MapLV);

			if (MapLV == 1)
			{
				this->MapDif = TEXT("简单");
			}
			else if (MapLV == 2)
			{
				this->MapDif = TEXT("容易");
			}
			else if (MapLV == 3)
			{
				this->MapDif = TEXT("正常");
			}
			else if (MapLV >= 4 && MapLV < 10)
			{
				this->MapDif = TEXT("困难");
			}
			else {
				this->MapDif = TEXT("非常困难");
			}

			this->MapCardLevel = FString::FromInt(CurD.CardGradeMax);
			this->MapRound = FString::FromInt(*(this->Cur->MapRound.Find(this->Name)));
			this->MapCard = CurD.LevelDes;

			this->MapTypeText->SetText(FText::FromString(this->MapType));
			this->MapDifText->SetText(FText::FromString(this->MapDif));
			this->MapLevelText->SetText(FText::FromString(this->MapLevel));
			this->MapCardLevelText->SetText(FText::FromString(this->MapCardLevel));
			this->MapRoundText->SetText(FText::FromString(this->MapRound));
			this->MapCardText->SetText(FText::FromString(this->MapCard));
			this->SetImageBrush(this->MapHead, this->ImgPath.ToString());

			//加载掉落物品
			this->Cur->CurMapItemCount = this->HiCount;
			//清空缓存
			this->Cur->MapItemDatas.Empty();
			//重新加载
			for (const auto& CurData : this->LevelItems)
			{
				FGameInformationStructData Temp;
				Temp.Name = CurData.Key;
				Temp.Img = CurData.Value;
				this->Cur->MapItemDatas.Emplace(Temp);
			}

			this->Cur->TimeLoad_MapM_Manager->UpdateDatatable(this->Cur->MapItemDatas);
			this->Cur->TimeLoad_MapM_Manager->ContinueRun();
		}
	}
}

bool UUI_GameInformation::Initialize()
{
	Super::Initialize();

	this->SelectTitleName = this->GetWidgetComponent<UTextBlock>(this, TEXT("TextBlock_235"));

	this->UniformCard = this->GetWidgetComponent<UUniformGridPanel>(this, TEXT("UniformGridPanel_21"));
	this->ScrollBoxCard = this->GetWidgetComponent<UScrollBox>(this, TEXT("Card"));

	this->UniformMouse = this->GetWidgetComponent<UUniformGridPanel>(this, TEXT("UniformGridPanel"));
	this->ScrollBoxMouse = this->GetWidgetComponent<UScrollBox>(this, TEXT("Mouse"));

	this->UniformMap = this->GetWidgetComponent<UUniformGridPanel>(this, TEXT("UniformGridPanel_1"));
	this->ScrollBoxMap = this->GetWidgetComponent<UScrollBox>(this, TEXT("Maps"));

	this->UniformMapM = this->GetWidgetComponent<UUniformGridPanel>(this, TEXT("UniformGridPanel_120"));
	this->ScrollBoxMapM = this->GetWidgetComponent<UScrollBox>(this, TEXT("ScrollBox_173"));

	return true;
}

UWidget* UUI_GameInformation::WidgetCreate_Card(UItemDataTable* _Data, int32 _Index)
{
	//多态，实例化对应的子类，父类返回，调用接口
	UUI_GameInformationBox* Grid = CreateWidget<UUI_GameInformationBox>(
		this->GetWorld(),
		UAssetManager::GetStreamableManager().LoadSynchronous<UClass>(
			FSoftClassPath(this->LoadPath)));

	Grid->Cur = this;
	Grid->SetName(
		((FGameInformationStructData*)(_Data->GetValue()))->Name,
		((FGameInformationStructData*)(_Data->GetValue()))->Img
	);

	if (_Index == 0)
	{
		Grid->ShowContent();
	}

	return Grid;
}

void UUI_GameInformation::WidgetRefresh(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	Cast<UUI_GameInformationBox>(_UWidget)->SetName(
		((FGameInformationStructData*)(_Data->GetValue()))->Name,
		((FGameInformationStructData*)(_Data->GetValue()))->Img
	);

	if (_Index == 0)
	{
		Cast<UUI_GameInformationBox>(_UWidget)->ShowContent();
	}
}

UWidget* UUI_GameInformation::WidgetCreate_MapItem(UItemDataTable* _Data, int32 _Index)
{
	//多态，实例化对应的子类，父类返回，调用接口
	UUI_GameInformationMapItemBox* Grid = CreateWidget<UUI_GameInformationMapItemBox>(
		this->GetWorld(),
		UAssetManager::GetStreamableManager().LoadSynchronous<UClass>(
			FSoftClassPath(
				TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/GameInfor/BPUI_GameInforMapItem.BPUI_GameInforMapItem_C'"))));

	Grid->ItemName = ((FGameInformationStructData*)(_Data->GetValue()))->Name;
	Grid->SetImageBrush(Grid->HeadImg,
		((FGameInformationStructData*)(_Data->GetValue()))->Img.ToString(),
		FVector(1.f), 1.f, true, FVector(1.0f, 1.0f, 0.f)
	);

	if (this->CurMapItemCount > _Index)
	{
		Grid->SetHiLight(true);
	}
	else {
		Grid->SetHiLight(false);
	}

	return Grid;
}

void UUI_GameInformation::WidgetRefresh_MapItem(UItemDataTable* _Data, int32 _Index, UWidget* _UWidget)
{
	UUI_GameInformationMapItemBox* Grid = Cast<UUI_GameInformationMapItemBox>(_UWidget);
	Grid->ItemName = ((FGameInformationStructData*)(_Data->GetValue()))->Name;
	Grid->SetImageBrush(
		Grid->HeadImg,
		((FGameInformationStructData*)(_Data->GetValue()))->Img.ToString(),
		FVector(1.f), 1.f, true, FVector(1.0f, 1.0f, 0.f)
	);

	if (this->CurMapItemCount > _Index)
	{
		Grid->SetHiLight(true);
	}
	else {
		Grid->SetHiLight(false);
	}
}

void UUI_GameInformation::ShowCard(const FString& SearchKey)
{
	if (!IsValid(this->TimeLoad_Card_Manager))
	{
		this->LoadPath =
			TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/GameInfor/BPUI_GameInforCard.BPUI_GameInforCard_C'");

		this->TimeLoad_Card_Manager = NewObject<UItemLoadManager>(this, TEXT("TimeLoad_Card_Manager_T"));
		this->TimeLoad_Card_Manager->InitWidget_First(
			this->UniformCard,
			this->ScrollBoxCard,
			0.001f);
		this->TimeLoad_Card_Manager->InitRange_Second(5, 7,
			9999,
			128.f, 178.f);
		this->TimeLoad_Card_Manager->OnCreateItem.BindUFunction(this, TEXT("WidgetCreate_Card"));
		this->TimeLoad_Card_Manager->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefresh"));

		//初始化数据
		TArray<FItemBase> AllCards;
		UCardBaseStruct::GetAllCardName(AllCards);
		for (const FItemBase& Item : AllCards)
		{
			FGameInformationStructData Temp;
			Temp.Name = Item.ItemName;
			Temp.Img = Item.ItemTexturePath;
			this->Cards.Emplace(Temp);
		}
	}

	if (SearchKey.Equals(TEXT("")))
	{
		this->TimeLoad_Card_Manager->UpdateDatatable(Cards);
	}
	else {
		this->KeyCards.Empty();
		for (const FGameInformationStructData& Data : this->Cards)
		{
			if (Data.Name.Contains(SearchKey))
			{
				KeyCards.Emplace(Data);
			}
		}
		this->TimeLoad_Card_Manager->UpdateDatatable(this->KeyCards);
	}

	this->TimeLoad_Card_Manager->ContinueRun();
}

void UUI_GameInformation::ShowMouse(const FString& SearchKey)
{
	if (!IsValid(this->TimeLoad_Mouse_Manager))
	{
		this->LoadPath =
			TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/GameInfor/BPUI_GameInforMouse.BPUI_GameInforMouse_C'");

		this->MouseDataTable = LoadObject<UDataTable>(
			nullptr, TEXT("DataTable'/Game/Resource/BP/Data/MouseData/MouseData.MouseData'")
		);

		this->TimeLoad_Mouse_Manager = NewObject<UItemLoadManager>(this, TEXT("TimeLoad_Mouse_Manager_T"));
		this->TimeLoad_Mouse_Manager->InitWidget_First(
			this->UniformMouse,
			this->ScrollBoxMouse,
			0.001f);
		this->TimeLoad_Mouse_Manager->InitRange_Second(4, 4,
			9999,
			222.f, 222.f);
		this->TimeLoad_Mouse_Manager->OnCreateItem.BindUFunction(this, TEXT("WidgetCreate_Card"));
		this->TimeLoad_Mouse_Manager->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefresh"));

		TArray<FMouse_Data> AllMouses;
		UGameSystemFunction::GetDataTableRows(
			TEXT("DataTable'/Game/Resource/BP/Data/MouseData/MouseData.MouseData'"), AllMouses);
		for (const FMouse_Data& Item : AllMouses)
		{
			FGameInformationStructData Temp;
			Temp.Name = Item.M_Mouse.M_MouseName;
			Temp.Img = Item.M_Mouse.MouseHead;
			this->Mouses.Emplace(Temp);
		}
	}

	if (SearchKey.Equals(TEXT("")))
	{
		this->TimeLoad_Mouse_Manager->UpdateDatatable(this->Mouses);
	}
	else {
		this->KeyMouses.Empty();
		for (const FGameInformationStructData& Data : this->Mouses)
		{
			if (Data.Name.Contains(SearchKey))
			{
				this->KeyMouses.Emplace(Data);
			}
		}
		this->TimeLoad_Mouse_Manager->UpdateDatatable(this->KeyMouses);
	}

	this->TimeLoad_Mouse_Manager->ContinueRun();
}

void UUI_GameInformation::ShowMap(const FString& SearchKey)
{
	if (!IsValid(this->TimeLoad_Map_Manager))
	{
		this->LoadPath =
			TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/GameInfor/BPUI_GameInforMap.BPUI_GameInforMap_C'");

		//地图加载
		this->TimeLoad_Map_Manager = NewObject<UItemLoadManager>(this, TEXT("TimeLoad_Map_Manager_T"));
		this->TimeLoad_Map_Manager->InitWidget_First(
			this->UniformMap,
			this->ScrollBoxMap,
			0.001f);
		this->TimeLoad_Map_Manager->InitRange_Second(4, 4,
			9999,
			222.f, 222.f);
		this->TimeLoad_Map_Manager->OnCreateItem.BindUFunction(this, TEXT("WidgetCreate_Card"));
		this->TimeLoad_Map_Manager->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefresh"));

		//地图材料加载
		this->TimeLoad_MapM_Manager = NewObject<UItemLoadManager>(this, TEXT("TimeLoad_MapM_Manager_T"));
		this->TimeLoad_MapM_Manager->InitWidget_First(
			this->UniformMapM,
			this->ScrollBoxMapM,
			0.001f);
		this->TimeLoad_MapM_Manager->InitRange_Second(5, 8,
			9999,
			100.f, 101.f);
		this->TimeLoad_MapM_Manager->SetMainScrollTransfrom(FVector2D(30.f, 0.f));
		this->TimeLoad_MapM_Manager->OnCreateItem.BindUFunction(this, TEXT("WidgetCreate_MapItem"));
		this->TimeLoad_MapM_Manager->OnRefreshItem.BindUFunction(this, TEXT("WidgetRefresh_MapItem"));


		TArray<FGameMapData> AllMaps;
		UGameSystemFunction::GetDataTableRows(
			TEXT("DataTable'/Game/Resource/BP/Data/MapData/GameMapData.GameMapData'"), AllMaps);
		AllMaps.RemoveAt(0);
		for (const FGameMapData& Item : AllMaps)
		{
			FGameInformationStructData Temp;
			Temp.Name = Item.M_FLevelConfig.LevelName;
			Temp.Img = Item.M_FLevelConfig.LevelBGHead;
			this->CurMapDatas.Emplace(Temp);
			MapLevelConfigs.Emplace(Item.M_FLevelConfig);
			this->MapRound.Emplace(Item.M_FLevelConfig.LevelName, Item.M_FMouseConfig.CurRoundConfig.Num());
		}
	}

	if (SearchKey.Equals(TEXT("")))
	{
		this->TimeLoad_Map_Manager->UpdateDatatable(this->CurMapDatas);
	}
	else {
		this->KeyMaps.Empty();
		for (const FGameInformationStructData& Data : this->CurMapDatas)
		{
			if (Data.Name.Contains(SearchKey))
			{
				this->KeyMaps.Emplace(Data);
			}
		}
		this->TimeLoad_Map_Manager->UpdateDatatable(this->KeyMaps);
	}

	this->TimeLoad_Map_Manager->ContinueRun();
}
