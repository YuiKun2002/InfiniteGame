// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Game/UI/UI_RoomCreate.h"
#include <Components/Image.h>
#include <Components/VerticalBox.h>
#include <Components/TextBlock.h>

#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/GameConfigManager.h"
#include "GameSystem/MouseStructManager.h"

bool UUI_RoomCreateRoomListItem::Initialize()
{
	Super::Initialize();

	this->SexImg = this->GetWidgetComponent<UImage>(this, TEXT("Sex"));
	this->SelectBoxImg = this->GetWidgetComponent<UImage>(this, TEXT("SelectBox"));

	this->RoomNumberText = this->GetWidgetComponent<UTextBlock>(this, TEXT("RoomNum"));
	this->RoomNameText = this->GetWidgetComponent<UTextBlock>(this, TEXT("RoomName"));

	return true;
}

void UUI_RoomCreateRoomListItem::Init(class UUI_RoomCreate* NewCurRoom)
{
	this->CurRoom = NewCurRoom;
}

void UUI_RoomCreateRoomListItem::SetData(FString SNumber, FString SRoomName, int32 ISex)
{
	this->RoomNumberText->SetText(FText::FromString(SNumber));
	this->RoomNameText->SetText(FText::FromString(SRoomName));

	//女生
	if (ISex == 0)
	{
		this->SetImageBrush(this->SexImg,
			TEXT("Texture2D'/Game/Resource/Texture/UI/RoomCreate/58.58'"));
	}

	this->SelectBoxImg->SetVisibility(ESlateVisibility::Collapsed);
}

void UUI_RoomCreateRoomListItem::Select()
{
	if (IsValid(this->CurRoom))
	{
		for (UUI_RoomCreateRoomListItem* Cur : this->CurRoom->Alls)
		{
			Cur->Cancel();
		}
	}

	this->SelectBoxImg->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (this->OnSelctDelegate.IsBound())
	{
		this->OnSelctDelegate.Execute();
	}
}

void UUI_RoomCreateRoomListItem::Cancel()
{
	this->SelectBoxImg->SetVisibility(ESlateVisibility::Collapsed);
}

bool UUI_RoomCreate::Initialize()
{
	Super::Initialize();

	this->MapBg = this->GetWidgetComponent<UImage>(this, TEXT("Image_149"));
	this->MouseDefImg = this->GetWidgetComponent<UImage>(this, TEXT("DefMouseImg"));

	this->ShowRoomList = this->GetWidgetComponent<UVerticalBox>(this, TEXT("VerticalBox_23"));

	return true;
}

void UUI_RoomCreate::Init(FString MapName)
{
	if (IsValid(UFVMGameInstance::GetFVMGameInstance()->LoadGameMapStructManager(MapName)))
	{
		//设置角色即将进入地图的名称
		UGameConfigManager* CurConfig = UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager();
		CurConfig->M_PlayerInGameMapName = MapName;

		//获取当前的地图管理器
		UGameMapStructManager* Cur = UFVMGameInstance::GetFVMGameInstance()->GetGameMapStructManager();
		//初始化原始地图配置数据
		this->SourceData = Cur->LevelConfig;

		//加载背景		
		this->SetImageBrush(this->MapBg, Cur->LevelConfig.LevelBGHead.ToString());

		//加载房间1
		UUI_RoomCreateRoomListItem* Cur1 = this->Gen(TEXT("【默认难度】材料掉落率增加0.3%"));
		Cur1->OnSelctDelegate.BindUObject(this, &UUI_RoomCreate::Room1);
		//加载房间2
		UUI_RoomCreateRoomListItem* Cur2 = this->Gen(TEXT("【简单难度】欢乐大冒险,Let's go!"));
		Cur2->OnSelctDelegate.BindUObject(this, &UUI_RoomCreate::Room2);
		//加载房间3
		UUI_RoomCreateRoomListItem* Cur3 = this->Gen(TEXT("【困难难度】关键材料掉落，掉落率增加0.5%"));
		Cur3->OnSelctDelegate.BindUObject(this, &UUI_RoomCreate::Room3);
		//加载房间4
		UUI_RoomCreateRoomListItem* Cur4 = this->Gen(TEXT("【极限难度】等级限制，关键材料掉落，掉落率增加1%"));
		Cur4->OnSelctDelegate.BindUObject(this, &UUI_RoomCreate::Room4);

		this->Alls.Emplace(Cur1);
		this->Alls.Emplace(Cur2);
		this->Alls.Emplace(Cur3);
		this->Alls.Emplace(Cur4);
		this->ShowRoomList->AddChildToVerticalBox(Cur1);
		this->ShowRoomList->AddChildToVerticalBox(Cur2);
		this->ShowRoomList->AddChildToVerticalBox(Cur3);
		this->ShowRoomList->AddChildToVerticalBox(Cur4);

		//默认选择1
		Cur1->Select();
	}
}

UUI_RoomCreateRoomListItem* UUI_RoomCreate::Gen(FString RoomName)
{
	UUI_RoomCreateRoomListItem* NewUI = CreateWidget<UUI_RoomCreateRoomListItem>(this,
		LoadClass<UUI_RoomCreateRoomListItem>(0,
			TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/RoomCreate/BPUI_RoomCreateRoomListItem.BPUI_RoomCreateRoomListItem_C'")));

	NewUI->Init(this);
	NewUI->SetData(FString::FromInt(
		UGameSystemFunction::GetRandomRange(1000, 9999)
	), RoomName,
		UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerSex
	);

	return NewUI;
}

void UUI_RoomCreate::Room1()
{
	//获取当前的地图管理器
	UGameMapStructManager* Cur = UFVMGameInstance::GetFVMGameInstance()->GetGameMapStructManager();
	Cur->LevelConfig = this->SourceData;
	Cur->LevelConfig.ItemsValue += 4.5;
	//移除忽略项目
	for (const FString& CurIg : this->SourceData.IgnoreItems)
	{
		Cur->LevelConfig.LevelItems.Remove(CurIg);
	}
}

void UUI_RoomCreate::Room2()
{
	//获取当前的地图管理器
	UGameMapStructManager* Cur = UFVMGameInstance::GetFVMGameInstance()->GetGameMapStructManager();
	Cur->LevelConfig = this->SourceData;

	//移除忽略项目
	for (const FString& CurIg : this->SourceData.IgnoreItems)
	{
		Cur->LevelConfig.LevelItems.Remove(CurIg);
	}

	Cur->LevelConfig.MouseLevel = 1;
	Cur->LevelConfig.ItemsCountRate = 1;
}

void UUI_RoomCreate::Room3()
{
	//获取当前的地图管理器
	UGameMapStructManager* Cur = UFVMGameInstance::GetFVMGameInstance()->GetGameMapStructManager();
	Cur->LevelConfig = this->SourceData;
	Cur->LevelConfig.MouseLevel = Cur->LevelConfig.MouseLevel + 1;
	Cur->LevelConfig.ItemsCountRate = 1;
	Cur->LevelConfig.ItemsValue += 150.f * 0.05f;
}

void UUI_RoomCreate::Room4()
{
	//获取当前的地图管理器
	UGameMapStructManager* Cur = UFVMGameInstance::GetFVMGameInstance()->GetGameMapStructManager();
	Cur->LevelConfig = this->SourceData;
	Cur->LevelConfig.CardGradeMax = Cur->LevelConfig.CardGradeMin;
	Cur->LevelConfig.MouseLevel = Cur->LevelConfig.MouseLevel * 2;
	if (Cur->LevelConfig.MouseLevel < 3)
	{
		Cur->LevelConfig.MouseLevel = 3;
	}
	Cur->LevelConfig.ItemsCountRate = 1;
	Cur->LevelConfig.ItemsValue += 150.f * 0.1f;
}

