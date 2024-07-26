// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/CardManagerComponent.h"

#include "GameStart/VS/MapMeshe.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/VS/Components/Card/CardDataComponent.h"

#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/GameConfigManager.h"

//玩家背包数据
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/Tools/GameSystemFunction.h"

#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameStart/VS/CardCoolDownManager.h"

#include "GameStart/VS/UI/UI_Card.h"
#include "GameStart/VS/UI/UI_MapMeshe.h"

#include <Kismet/KismetMathLibrary.h>

//铲子UI
#include "GameStart/VS/UI/UI_EradicateCard.h"

#include "GameStart/VS/UI/UI_CardBar.h"
#include <Components/HorizontalBox.h>
#include <Components/Image.h>
#include <Engine/Texture2D.h>

// Sets default values for this component's properties
UCardManagerComponent::UCardManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCardManagerComponent::BeginPlay()
{
	Super::BeginPlay();


	//初始化卡片默认动画
	//加载数据表
	UDataTable* Table = LoadObject<UDataTable>(0,
		TEXT("DataTable'/Game/Resource/BP/Data/CardData/0_CardIdleAnim.0_CardIdleAnim'"));
	TArray<FCard_KeyIdleAnim_Data*> LocalCard_KeyIdleAnim_Data;
	Table->GetAllRows("List", LocalCard_KeyIdleAnim_Data);

	for (auto Data : LocalCard_KeyIdleAnim_Data)
	{
		this->M_CardIdleAnim.Emplace((Data)->M_CardKeyIdleAnim.IdlePath.ToString());
	}
}

void UCardManagerComponent::SetCurrentCardMapMeshe(UUI_MapMeshe* _MapMeshe)
{
	this->M_CurrentPlayCardMapMeshe = _MapMeshe;
}

UUI_MapMeshe* UCardManagerComponent::GetCurrentCardMapMeshe()
{
	return this->M_CurrentPlayCardMapMeshe;
}

UPaperFlipbook* UCardManagerComponent::GetCardIdleAnimByName(const FString& _CardActorName)
{
	//分割字符串
	FString SplitLeft, SplitRight;
	//分割
	_CardActorName.Split(TEXT("_"), &SplitLeft, &SplitRight);
	//将右半段进行数字转换
	int32 LocalResultIndex = FCString::Atoi(*SplitRight);

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, FString(_CardActorName + TEXT("  分割结果：") + FString::FromInt(LocalResultIndex)));
	}

	//获取对应动画
	return LoadObject<UPaperFlipbook>(0, *this->M_CardIdleAnim[LocalResultIndex]);
}


UClass* UCardManagerComponent::GetCurrentSelectActor()
{
	return this->M_CurrentSelectCardInstance;
}

ACardCoolDownManager* UCardManagerComponent::GetCurrentCoolDownManager()
{
	if (this->M_CardName.Find(this->M_CurrentSelectCardName) == -1)
		return nullptr;

	return this->M_CardCoolDownManager[this->M_CardName.Find(this->M_CurrentSelectCardName)];
}

void UCardManagerComponent::SetCard_Image(UUI_Card* _CardUI, const FString& _CardBg)
{
	if (_CardUI)
	{
		//对应卡片的背景路径
		//UWidgetBase::SetButtonStyle();
		UWidgetBase::SetImageBrush(_CardUI->GetCardHead(), _CardBg, FVector(1.f), 1.f, false, FVector(1.f), true);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("卡片背景图片设置失败，因为当前卡片UI无效"));
	}
}

bool UCardManagerComponent::SelectCurrentActor(const FString& _CardName, bool ForceSelect)
{
	//如果不是强制选择卡片
	if (!ForceSelect)
	{
		//如果当前选择的卡片名称和之前选择的卡片名称一样
		if (this->M_CurrentSelectCardName.Equals(_CardName))
		{
			//则取消选择
			this->CancelSelect();

			//卡片名称相同
			//UE_LOG(LogTemp, Error, TEXT("选择的卡片名称相同"));

			return false;
		}

		//则取消选择
		this->CancelSelect();

		//取消对铲子的选择
		this->CancelEradicate();
	}

	//打印当前选择的卡片
	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("当前选择了防御卡：") + _CardName));
	}

	//播放BGM——S
	UFVMGameInstance::PlayBGM_S_Static(TEXT("TakeCard"), TEXT("ItemAudio"));

	//设置选择的名称
	this->M_CurrentSelectCardName = _CardName;

	//非强制选卡
	if (!ForceSelect)
	{
		//获取UICard
		if (this->GetUICard(this->M_CurrentSelectCardName))
		{
			this->GetUICard(_CardName)->SelectColor();
		}
	}

	FItemCard* Data = this->GetCardDataByName(_CardName);
	this->M_CurrentSelectCardInstance = LoadClass<ACardActor>(0, *Data->CardActorResource.ToString());

	if (!this->M_CurrentSelectCardInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("[UCardManagerComponent::SelectCurrentActor]:卡片选择失败,因为加载的对象不存在!"));
		return false;
	}

	//设置当前卡片状态为选择
	this->GetUICard(this->M_CurrentSelectCardName)->SetCardSelect(true);

	return true;
}

const int32 UCardManagerComponent::GetCardFlameNum(const FString& _CardActorName)
{
	for (auto Datas : this->M_CardData)
	{
		if (Datas.ItemName.Equals(_CardActorName))
			return Datas.M_CardPrice;
	}
	return 0;
}

FItemCard& UCardManagerComponent::GetCardData(const FString& _CardActorName)
{
	for (auto PP = this->M_CardData.CreateConstIterator(); PP; PP++)
	{
		if (PP->ItemName.Equals(_CardActorName))
		{
			return this->M_CardData[PP.GetIndex()];
		}
	}

	static FItemCard _StaticTemp;
	_StaticTemp = UCardDataComponent::GetCardBaseData(_CardActorName);
	_StaticTemp.M_CardOverlap = false;
	_StaticTemp.M_CardPrice = 0;

	return _StaticTemp;
}

bool UCardManagerComponent::GetCardDataByName(const FString& _CardName, FItemCard& _CardFItemData)
{
	for (auto& Item : this->M_CardData)
	{
		if (_CardName.Equals(Item.ItemName))
		{
			_CardFItemData = Item;
			return true;
		}
	}
	return false;
}

FItemCard* const UCardManagerComponent::GetCardDataByName(const FString& _CardName)
{
	for (auto& Item : this->M_CardData)
	{
		if (_CardName.Equals(Item.ItemName))
		{
			return &Item;
		}
	}
	return nullptr;
}

UUI_Card* const UCardManagerComponent::GetUICard(const FString& _CardActorName)
{
	for (auto Datas : this->M_UICard)
	{
		if (Datas->M_Card_ActorName.Equals(_CardActorName))
			return Datas;
	}
	return nullptr;
}

const FString& UCardManagerComponent::GetCurrentSelectCardName()
{
	return this->M_CurrentSelectCardName;
}

void UCardManagerComponent::CancelSelect()
{
	//获取UICard
	if (this->GetUICard(this->M_CurrentSelectCardName))
	{
		this->GetUICard(this->M_CurrentSelectCardName)->CancelColor();
		this->GetUICard(this->M_CurrentSelectCardName)->SetCardSelect(false);
		this->GetUICard(this->M_CurrentSelectCardName)->PlaySelectCardAnimation(false);
	}

	if (this->M_CurrentSelectCardInstance)
		this->M_CurrentSelectCardInstance = nullptr;

	//清空名称
	this->M_CurrentSelectCardName.Empty();
}

void UCardManagerComponent::SelectEradicate()
{
	this->M_CurrentSelectEradicate = true;
	this->CancelSelect();

	if (IsValid(this->M_UUI_EradicateCard))
		this->M_UUI_EradicateCard->Select();

	UFVMGameInstance::PlayBGM_S_Static(TEXT("TakeEradicate"), TEXT("ItemAudio"));
}

void UCardManagerComponent::CancelEradicate()
{
	this->M_CurrentSelectEradicate = false;

	if (IsValid(this->M_UUI_EradicateCard))
		this->M_UUI_EradicateCard->Cancel();
}

void UCardManagerComponent::AddCard(const FItemCard& _CardData)
{
	this->M_CardName.Emplace(_CardData.ItemName);

	if (this->M_UUI_CardBar)
	{
		UUI_Card* CardBoxWidget = CreateWidget<UUI_Card>(this->GetWorld(),
			LoadClass<UUI_Card>(nullptr,
				TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_Player/UICardBad/CardBoxHorizental.CardBoxHorizental_C'")));

		CardBoxWidget->M_CardIndex = this->M_CardName.Num() - 1;
		CardBoxWidget->M_NeedFlame = _CardData.M_CardPrice;
		CardBoxWidget->SetCardName(_CardData.ItemName);
		CardBoxWidget->SetCardGrade(_CardData.M_CardGrade);//设置等级
		CardBoxWidget->M_Card_ActorName = _CardData.ItemName;//该名称不是卡片实例的名称是文件名称
		CardBoxWidget->bEnableCopy = _CardData.M_bEnableCopy;//是否可以被复制
		CardBoxWidget->CardActorSource = _CardData.CardActorResource;//资产类

		if (UFVMGameInstance::GetDebug())
		{
			FString LContent =
				FString(TEXT("当前卡片[") +
					_CardData.ItemName +
					TEXT("]加入放置栏位")
				);

			UGameSystemFunction::FVMLog(__FUNCTION__, LContent);
		}


		this->SetCard_Image(CardBoxWidget, _CardData.CardActorHead.ToString());

		//当前冷却
		float _CardClodDown = 0.f;

		//计算冷却
		if (_CardData.M_ECardUpGradeType == ECardUpGradeType::E_ColdDownSpeed_Up)
		{
			_CardClodDown =
				_CardData.M_CardColdDown -
				(_CardData.M_CardColdDown *
					_CardData.M_CardGrade *
					(_CardData.M_M_ECardUpGradeUpRate / 100.f)
					);

			if (UFVMGameInstance::GetDebug())
			{
				FString LContent =
					FString(TEXT("当前卡片[") +
						_CardData.ItemName +
						TEXT("]等级提升技能：冷却速度=>") +
						FString::SanitizeFloat(_CardClodDown)
					);

				UGameSystemFunction::FVMLog(__FUNCTION__, LContent);
			}
		}
		else
			_CardClodDown = _CardData.M_CardColdDown;


		//技能书->技能冷却提升
		if (_CardData.M_ECardSkillType == ECardSkillType::E_COLDDOWN)
		{
			int32 Lv = UGameSystemFunction::GetPlayerSkillBooksLv(_CardData.M_CardSkillBookName);
			float _Result = _CardClodDown - _CardClodDown * Lv * 0.1f;
			_CardClodDown = _Result > 0.f ? _Result : 0.f;

			if (UFVMGameInstance::GetDebug())
			{
				FString Content =
					FString(TEXT("当前卡片[") +
						_CardData.M_CardSkillBookName +
						TEXT("]技能书技能：减少冷却速度=>") +
						FString::SanitizeFloat(_CardClodDown)
					);

				UGameSystemFunction::FVMLog(__FUNCTION__, Content);
			}
		}

		//设置冷却
		this->AddCoolDown(_CardClodDown);
		this->M_UUI_CardBar->AddCard(CardBoxWidget);
		this->M_UICard.Emplace(CardBoxWidget);
	}
}

bool UCardManagerComponent::AddSpecialCard(const int32& _CardListIndex)
{
	if (this->M_SpecialCardListIndex.Num() < 4)
	{
		this->M_SpecialCardListIndex.Emplace(_CardListIndex);

		return true;
	}

	//UKismetMathLibrary::Lerp();

	return false;
}

void UCardManagerComponent::AddCoolDown(const float& _Time)
{
	ACardCoolDownManager* M_ACardCoolDownManager = this->GetWorld()->SpawnActor<ACardCoolDownManager>();
	M_ACardCoolDownManager->SetCoolDownTime(_Time);
	this->M_CardCoolDownManager.Emplace(M_ACardCoolDownManager);
}

void UCardManagerComponent::HiddenCardBar()
{
	if (this->M_UUI_CardBar->IsInViewport())
		this->M_UUI_CardBar->RemoveFromParent();
}

void UCardManagerComponent::ShowCardBar()
{
	if (!this->M_UUI_CardBar->IsInViewport() && !FVM_VS_GamePause())
		this->M_UUI_CardBar->AddToViewport();
}

bool UCardManagerComponent::GetCardCoolDownFinish(const int32& _Index)
{
	//获取卡片冷却管理器
	return this->M_CardCoolDownManager[_Index]->GetIsCoolDownFinish();
}

const TArray<UUI_Card*>& UCardManagerComponent::GetUICardArrays()
{
	return this->M_UICard;
}

UUI_Card* UCardManagerComponent::GetUICardArraysByIndex(int32 Index)
{
	return this->M_UICard[Index];
}

UUI_CardBar* UCardManagerComponent::GetUICardBar()
{
	return this->M_UUI_CardBar;
}

// Called every frame
void UCardManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCardManagerComponent::GameStart()
{
	//加载卡片栏UI
	if (!this->M_UUI_CardBar)
	{
		this->M_UUI_CardBar = CreateWidget<UUI_CardBar>(this->GetWorld(), LoadClass<UUI_CardBar>(nullptr, TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_Player/UICardBar.UICardBar_C'")));
		if (!this->M_UUI_CardBar)
		{
			UE_LOG(LogTemp, Error, TEXT("卡片栏界面加载失败，路径有误!"));
			return;
		}
		this->M_UUI_CardBar->Init();
		this->M_UUI_CardBar->AddToViewport(0);
	}

	if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		//初始化卡片
		for (auto CardDatas : UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_PlayerGamePrepareSelectCardData)
		{
			this->AddCard(CardDatas);
			this->M_CardData.Emplace(CardDatas);
		}
	}
}

void UCardManagerComponent::GameOver()
{
	this->HiddenCardBar();
}

