// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/UI/UI_GameOver.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include "GameSystem/GameMapStructManager.h"
#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/GameMapStructManager.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include <Components/TextBlock.h>
#include "GameSystem/Item/PlayerRecord.h"
#include <Components/UniformGridPanel.h>
#include "GameSystem/TaskSubsystem.h"
#include <Components/Image.h>

bool UUI_GameOver::Initialize()
{
	Super::Initialize();

	this->CoinText = this->GetWidgetComponent<UTextBlock>(this, TEXT("CoinText"));
	this->LevelItemsPanel = this->GetWidgetComponent<UUniformGridPanel>(this, TEXT("UniformGridPanel_65"));

	this->ScoreText = this->GetWidgetComponent<UTextBlock>(this, TEXT("ScoreText"));
	this->ScoreLevelText = this->GetWidgetComponent<UTextBlock>(this, TEXT("ScoreAText"));
	this->TimeText = this->GetWidgetComponent<UTextBlock>(this, TEXT("TimeText"));
	this->BadCardCountText = this->GetWidgetComponent<UTextBlock>(this, TEXT("BadCardCountText"));
	this->ExText = this->GetWidgetComponent<UTextBlock>(this, TEXT("ExText"));
	this->PlayerGradeHead = this->GetWidgetComponent<UImage>(this, TEXT("GradeHead"));


	return true;
}

void UUI_GameOver::ShowOver1()
{
	const TMap<FString, int32>& CurResource = UResourceManagerComponent::GetResource();

	//获得关卡配置
	const FLevelConfig& LevelConfig = UFVMGameInstance::GetFVMGameInstance()->GetGameMapStructManager()->LevelConfig;

	TMap<FString, int32> Coins;
	Coins.Emplace(FString(TEXT("金币")), 0);
	Coins.Emplace(FString(TEXT("礼券")), 1);
	Coins.Emplace(FString(TEXT("点券")), 2);
	Coins.Emplace(FString(TEXT("威望")), 3);

	//添加显示UI
	int32 i = 0;
	for (const auto& Item : CurResource)
	{
		this->MaterialNames.Emplace(Item.Key);

		const FSoftObjectPath* CurPath = LevelConfig.LevelItems.Find(Item.Key);
		if (CurPath)
		{
			UUI_LevelItem* LevelItem = CreateWidget<UUI_LevelItem>(this, LoadClass<UUI_LevelItem>(nullptr,
				TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_Player/BPUI_LevelItem.BPUI_LevelItem_C'")));

			LevelItem->Init(*CurPath, Item.Value);

			this->LevelItemsPanel->AddChildToUniformGrid(LevelItem, i / 16, i % 16);
		}
		else {
			if (Item.Key.Equals(TEXT("金币")))
			{
				CoinText->SetText(FText::FromString(FString::FromInt(Item.Value)));
			}
			else
			{
				if (UFVMGameInstance::GetDebug())
				{
					UGameSystemFunction::FVMLog(__FUNCTION__, Item.Key + TEXT("掉落物不存在，请检查关卡配置!!"));
				}
			}
		}

		//优先发送货币资源
		//发送[货币]
		if (UFVMGameInstance::GetPlayerStructManager_Static()->SendCoin(Item.Key, Item.Value))
		{
			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, Item.Key + TEXT("货币发送成功：") +
					FString::FromInt(Item.Value));
			}
		}
		else {
			//发送道具[材料]
			if (!UGameSystemFunction::SendMaterialToPlayerBag(Item.Key, Item.Value, false))
			{
				//发送[装备]
				if (!UGameSystemFunction::SendEquipmentToPlayerBag(Item.Key, Item.Value, false))
				{
					//发送[防御卡]
					if (!UGameSystemFunction::SendCardToPlayerBag(Item.Key, 0))
					{
						if (UFVMGameInstance::GetDebug())
						{
							UGameSystemFunction::FVMLog(__FUNCTION__, Item.Key +
								TEXT("掉落物不存在，请检查关卡配置!!"));
						}
					}
				}
			}
		}

		if (Item.Key.Equals(TEXT("金币")))
		{
			continue;
		}
		i++;
	}

	UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("关卡结束，保存进度1")));
}

void UUI_GameOver::ShowOver2(bool bWin)
{
	if (bWin)
	{
		//累积
		UPlayerRecord::AddDayLevelWinCount();
		UPlayerRecord::AddLevelWinCount();
	}

	this->bLevelWin = bWin;

	UResourceManagerComponent* Manager = UResourceManagerComponent::GetResourceManagerComponent();
	//存储卡片，老鼠的历史记录
	UPlayerRecord::AddDayCardCount(
		Manager->PlayCards
	);
	UPlayerRecord::AddDayKillMouseCount(
		Manager->KillMouses
	);

	int32 CurMaxEx = UFVMGameInstance::GetFVMGameInstance()->GetGameMapStructManager()->LevelConfig.MaxEx;

	int32 CurEx = Manager->CurEx;
	if (CurMaxEx > 0)
	{
		if (CurEx > CurMaxEx)
		{
			CurEx = CurMaxEx;
			Manager->CurEx = CurMaxEx;
		}
	}

	this->ScoreText->SetText(FText::FromString(FString::FromInt(Manager->CurScore)));

	int32 minute = Manager->CurLevelTime / 60;
	int32 Seconds = Manager->CurLevelTime % 60;

	this->TimeText->SetText(FText::FromString(FString::FromInt(minute) + TEXT(" 分 ") + FString::FromInt(Seconds) + TEXT(" 秒")));
	this->BadCardCountText->SetText(FText::FromString(FString::FromInt(Manager->CurBadCardCount) + TEXT(" 张")));
	this->ExText->SetText(FText::FromString(FString::FromInt(CurEx * (bWin ? 0.9f : 0.2f)) + TEXT(" 经验值")));

	//发送点券和威望
	if (bWin)
	{
		//UFVMGameInstance::GetPlayerStructManager_Static()->AddCoin(CurEx * 0.2f, 1);
		//UFVMGameInstance::GetPlayerStructManager_Static()->AddCoin(CurEx * 0.1f, 2);
		UFVMGameInstance::GetPlayerStructManager_Static()->AddCoin(CurEx * 0.6f, 3);
	}

	//设置关卡等级
	FPlayerLevelGrade NewLevel;
	NewLevel.Init(Manager->CurLevelTime, Manager->CurScore, Manager->CurBadCardCount);
	//初始化并初始化地图评级
	this->ScoreLevelText->SetText(FText::FromString(NewLevel.InitScoreLevel(Manager->CurBadCardCount + (bWin ? 0 : 60))));

	//添加地图评级
	UPlayerStructManager::AddPlayerLevelScore(
		UFVMGameInstance::GetFVMGameInstance()->GetGameMapStructManager()->LevelConfig.LevelName
		, NewLevel);


	//执行任务
	UTaskSubsystem::GetTaskSubsystemStatic()->ExecuteTasks(this);

	//添加角色经验值
	UFVMGameInstance::GetPlayerStructManager_Static()->AddPlayerEx(CurEx * (bWin ? 0.9f : 0.2f));
	//设置角色等级
	FSoftObjectPath PlayerGradePath = UGameSystemFunction::GetPlayerGradeImagePath();
	this->PlayerGradeHead->SetBrushFromTexture(Cast<UTexture2D>(PlayerGradePath.TryLoad()));

	UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("关卡结束，保存进度2")));
}

void UUI_GameOver::ShowOver3()
{
	UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("关卡结束，保存进度3")));
}

bool UUI_GameOver::FindMaterial(const FString& Name)
{
	return MaterialNames.Contains(Name);
}

bool UUI_LevelItem::Initialize()
{
	Super::Initialize();

	this->ItemHead = this->GetWidgetComponent<UImage>(this, TEXT("Head"));
	this->ItemCountText = this->GetWidgetComponent<UTextBlock>(this, TEXT("TextBlock_45"));

	return true;
}

void UUI_LevelItem::Init(const FSoftObjectPath& _path, const int32& Count)
{
	this->ItemHead->SetBrushFromTexture(Cast<UTexture2D>(_path.TryLoad()));
	this->ItemCountText->SetText(FText::FromString(TEXT("x") + FString::FromInt(Count)));
}
