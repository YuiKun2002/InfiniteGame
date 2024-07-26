// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_GamePrepare.h"
#include "GameSystem/GameConfigSubsystem.h"
#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/GameConfigManager.h"
#include "GameSystem/MouseStructManager.h"

#include "Game/UI/UI_TableTextBlock.h"

#include "Game/UI/UI_PlayerBagCardGrid.h"
#include <Components/UniformGridPanel.h>
#include <Components/HorizontalBox.h>
#include <Components/CanvasPanel.h>
#include <Components/VerticalBox.h>
#include <Components/Image.h>

#include <Kismet/KismetSystemLibrary.h>

//角色形象
#include "Game/UI/UI_PlayerShow.h"

UUI_GamePrepare* UUI_GamePrepare::M_GamePrepareStatic = nullptr;

bool UUI_GamePrepare::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	//赋值静态变量
	UUI_GamePrepare::M_GamePrepareStatic = this;
	//UE_LOG(LogTemp, Warning, TEXT("[UUI_GamePrepare::Initialize]: 赋值静态M_GamePrepareStatic"));

	this->M_CardGradeMaxHead = this->GetWidgetComponent<UImage>(this, "Image_62");
	this->M_CardGrid = this->GetWidgetComponent<UUniformGridPanel>(this, "CardGrid");
	this->M_SelectCardList = this->GetWidgetComponent<UHorizontalBox>(this, "SelectCardList");
	this->M_MapHead = this->GetWidgetComponent<UImage>(this, "Map_Head_BG");
	this->M_MouseList = this->GetWidgetComponent<UVerticalBox>(this, "MouseList");

	return true;
}

void UUI_GamePrepare::BeginDestroy()
{
	Super::BeginDestroy();

	UUI_GamePrepare::M_GamePrepareStatic = nullptr;
}

void UUI_GamePrepare::SelectCard(const FString& _CardName, const FItemCard& _CardData)
{
	for (auto Widget : this->M_BagGrid)
	{
		//寻找到你当前点击的卡片
		if (Widget->GetFItemCardData()->ItemName.Equals(_CardName))
		{
			//禁用相关按钮
			Widget->GetButton()->SetIsEnabled(false);
		}
	}

	UUI_PlayerBagCardGrid* M_TempCardGrid = CreateWidget<UUI_PlayerBagCardGrid>(
		this->GetWorld(),
		LoadClass<UUI_PlayerBagCardGrid>(0,
			TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_PlayerBagCardGrid.BPUI_PlayerBagCardGrid_C'")
			));

	M_TempCardGrid->SetFItemCardData(const_cast<FItemCard*>(&_CardData));
	M_TempCardGrid->M_CardTexturePath = _CardData.ItemTexturePath.ToString();
	M_TempCardGrid->UpdateButtonTexture(FString::FormatAsNumber(_CardData.M_CardPrice));
	M_TempCardGrid->PlayBeginAnim();

	//绑定函数 点击之后显示细节面板
	FScriptDelegate CallFunc;
	CallFunc.BindUFunction(M_TempCardGrid, "RemoveCurrentSelectCard");
	M_TempCardGrid->GetButtonClickEvent().Add(CallFunc);
	//添加到列表
	this->M_SelectCardList->AddChildToHorizontalBox(M_TempCardGrid);
	//选择数量+1
	this->M_SelectCardNum++;

	//UE_LOG(LogTemp, Error, TEXT("%s  %d"), __FUNCTION__, _CardData.M_CardLayer);
}

void UUI_GamePrepare::SetCardEnable(const TArray<FString>& _Names, bool _bEnable)
{
	if (!_Names.Num())
		return;

	for (auto Widget : this->M_BagGrid)
	{
		for (auto NamePP = _Names.CreateConstIterator(); NamePP; NamePP++)
		{
			//寻找到你当前点击的卡片
			if (Widget->GetFItemCardData()->ItemName.Equals(*NamePP))
			{
				//禁用相关按钮
				if (_bEnable)
					Widget->GetButton()->SetIsEnabled(true);
				else
					Widget->GetButton()->SetIsEnabled(false);
				break;
			}
		}
	}
}

void UUI_GamePrepare::CancelCardNum()
{
	this->M_SelectCardNum--;
}

int32 UUI_GamePrepare::GetSelectCardNums()
{
	return this->M_SelectCardNum;
}

void UUI_GamePrepare::LoadCardList()
{
	UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_PlayerGamePrepareSelectCardData.Empty();

	//得到关卡配置
	const FLevelConfig& LLevelData = UFVMGameInstance::GetFVMGameInstance()->GetGameMapStructManager()->LevelConfig;
	int32 LCurGradeMax = LLevelData.CardGradeMax > 16 ? 16 : LLevelData.CardGradeMax;
	if (LCurGradeMax < 0)
		LCurGradeMax = 0;
	//初始化卡片等级限制图片
	UWidgetBase::SetImageBrush(this->M_CardGradeMaxHead, UGameSystemFunction::GetCardGradeImagePath(LCurGradeMax));

	this->_CardItems_.Empty();
	//复制卡片数据
	this->_CardItems_ = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Card;
	//得到背包格子数量
	this->M_CardBagCount = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup.GetBagCount(1);

	this->M_CardGrid->ClearChildren();

	this->GetWorld()->GetTimerManager().ClearTimer(this->M_FTimeClip.M_FTimerHandle);
	this->M_FTimeClip.M_Count = 0;

	this->GetWorld()->GetTimerManager().SetTimer(this->M_FTimeClip.M_FTimerHandle, [&]() {
		//卡片数据
	 //	TArray<FItemCard>& _Items_ = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Card;

		//得到关卡配置
		const FLevelConfig& LevelData = UFVMGameInstance::GetFVMGameInstance()->GetGameMapStructManager()->LevelConfig;

		int32 CurGradeMax = LevelData.CardGradeMax > 16 ? 16 : LevelData.CardGradeMax;
		if (CurGradeMax < 0)
			CurGradeMax = 0;

		//判断索引是否有效
		if (this->M_FTimeClip.M_Count < _CardItems_.Num() && this->M_FTimeClip.M_Count < M_CardBagCount)
		{
			UUI_PlayerBagCardGrid* M_TempCardGrid = CreateWidget<UUI_PlayerBagCardGrid>(this->GetWorld(), LoadClass<UUI_PlayerBagCardGrid>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_PlayerBagCardGrid.BPUI_PlayerBagCardGrid_C'")));

			//卡片大于限制等级，则将超越限制的卡片，改为限制等级
			if (_CardItems_[this->M_FTimeClip.M_Count].M_CardGrade > CurGradeMax)
			{
				_CardItems_[this->M_FTimeClip.M_Count].M_CardGrade = CurGradeMax;
			}

			M_TempCardGrid->M_CardTexturePath = _CardItems_[this->M_FTimeClip.M_Count].ItemTexturePath.ToString();
			M_TempCardGrid->SetFItemCardData(&_CardItems_[this->M_FTimeClip.M_Count]);
			M_TempCardGrid->UpdateButtonTexture(FString::FormatAsNumber(_CardItems_[this->M_FTimeClip.M_Count].M_CardPrice));
			M_TempCardGrid->PlayBeginAnim();

			//查询当前创建的新卡片是否被玩家选择(如果选择了则禁用按钮功能)
			for (auto CardDatas : this->M_CardDatas_Copy)
			{
				if ((CardDatas).ItemName.Equals(_CardItems_[this->M_FTimeClip.M_Count].ItemName))
				{
					M_TempCardGrid->GetButton()->SetIsEnabled(false);
					break;
				}
			}

			//绑定函数 点击之后显示细节面板
			FScriptDelegate CallFunc;
			CallFunc.BindUFunction(M_TempCardGrid, "SelectCurrentCard");
			M_TempCardGrid->GetButtonClickEvent().Add(CallFunc);
			//将widget添加到格子中
			this->M_BagGrid.Emplace(M_TempCardGrid);
			this->M_CardGrid->AddChildToUniformGrid(M_TempCardGrid, (this->M_FTimeClip.M_Count / 9), this->M_FTimeClip.M_Count - 9 * (this->M_FTimeClip.M_Count / 9));
			this->M_FTimeClip.M_Count++;
		}
		else {
			this->GetWorld()->GetTimerManager().ClearTimer(this->M_FTimeClip.M_FTimerHandle);
			this->M_FTimeClip.M_Count = 0;
		}

		}, 0.01f, true);
}

void UUI_GamePrepare::LoadMouseList()
{

}

void UUI_GamePrepare::CloseCurrentPanel()
{
	this->RemoveFromParent();

	this->GetWorld()->GetTimerManager().ClearTimer(this->M_FTimeClip.M_FTimerHandle);
	this->GetWorld()->GetTimerManager().ClearTimer(this->M_FTimeClip_Mouse.M_FTimerHandle);

	//将选卡的数据保存到游戏配置结构当中去
	UGameConfigSubsystem::GetGameConfigSubsystemStatic()->GetGameConfigInstance()->M_PlayerGamePrepareSelectCardData =
		this->M_CardDatas_Copy;
	UGameConfigSubsystem::GetGameConfigSubsystemStatic()->SaveLocalGameConfigInstance();

	this->M_FTimeClip.M_Count = 0;
}

void UUI_GamePrepare::InitPanelData()
{
	if (UFVMGameInstance::GetFVMGameInstance())
	{
		//获取角色形象界面
		this->M_UUI_PlayerShow = CreateWidget<UUI_PlayerShow>(this,
			LoadClass<UUI_PlayerShow>(0,
				TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BP_PlayerShow.BP_PlayerShow_C'")));

		if ((this->GetWidgetComponent<UCanvasPanel>(this, "PlayerShow"))->GetAllChildren().IsEmpty())
		{
			if (IsValid(UFVMGameInstance::GetPlayerStructManager_Static()))
			{
				if (
					UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerSuit.M_bPlayerSuit
					&&
					!UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerSuit.M_ShowPlayerSuit

					||

					!UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerSuit.M_bPlayerSuit

					)
				{
					M_UUI_PlayerShow->SetRenderScale(FVector2D(1.2f));
				}
			}

			(this->GetWidgetComponent<UCanvasPanel>(this, "PlayerShow"))->AddChildToCanvas(this->M_UUI_PlayerShow);
		}

		//加载关卡配置
		const FLevelConfig& LevelData =
			UFVMGameInstance::GetFVMGameInstance()->GetGameMapStructManager()->LevelConfig;

		//设置角色即将进入地图的名称
		UGameConfigManager* CurConfig = UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager();
		//加载地图配置
		UFVMGameInstance::GetFVMGameInstance()->LoadGameMapStructManager(CurConfig->M_PlayerInGameMapName);

		//重新设置关卡配置
		UFVMGameInstance::GetFVMGameInstance()->GetGameMapStructManager()->LevelConfig = LevelData;

		//获取老鼠配置
		UMouseStructManager* const MouseData =
			UFVMGameInstance::GetFVMGameInstance()->GetMouseStructManager();

		if (MouseData)
		{
			this->M_MouseNameList.Empty();

			this->M_MapRound = MouseData ?
				FString::FromInt(MouseData->GetRoundTotal()) + TEXT("波") : TEXT("? 波");
			//this->M_MouseNameListSize = MouseData->M_MouseNames.Num();
			//加载老鼠的图片
			// MouseData->M_MouseNames;
			//this->LoadMouseList();
		}
		//加载地图相关描述
		this->M_MapName = LevelData.LevelName;
		this->M_MainMapName = LevelData.LevelTitleName;
		this->M_MapDayState = LevelData.LevelIsDay ? TEXT("白天") : TEXT("夜晚");
		this->M_PlayerName = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerName;

		//加载地图头像
		UWidgetBase::SetImageBrush(this->M_MapHead, LevelData.LevelBGHead.ToString());
	}
}
