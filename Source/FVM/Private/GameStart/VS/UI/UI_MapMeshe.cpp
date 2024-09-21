// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/UI/UI_MapMeshe.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"
#include "GameStart/Flipbook/GameActor/GamePlayer.h"

#include "GameStart/VS/Components/CardManagerComponent.h"
#include "GameStart/VS/Components/VSManagerComponent.h"
#include "GameStart/VS/CardCoolDownManager.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/MapMeshe.h"

#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include "GameStart/VS/Components/MesheControllComponent.h"

#include "GameStart/VS/CreateCardBar.h"
#include "GameStart/VS/UI/UI_Card.h"

#include "GameSystem/FVMGameInstance.h"

#include <Kismet/GameplayStatics.h>

#include <Components/Image.h>

void UUI_MapMeshe::PlayCard_PlayBGM()
{
	if (
		this->M_ELineType == ELineType::OnWater
		||
		this->M_ELineType == ELineType::Underwater

		)
	{
		UFVMGameInstance::PlayBGM_S_Static("PlayCardToOnWater", "ItemAudio");
	}
	else {
		UFVMGameInstance::PlayBGM_S_Static("PlayCardToGround", "ItemAudio");
	}
}

void UUI_MapMeshe::PlayCard(
	UCardManagerComponent* _CardMComp,
	UClass* _CardRes,
	FItemCard& _CardData,
	int32 _Price,
	bool ResetCoolDown
)
{
	FTransform Trans = this->M_MapMesheTransform;
	FVector _Location = Trans.GetLocation();
	Trans.SetLocation(_Location);

	ACardActor* Card = Cast <ACardActor>(
		UGameplayStatics::BeginDeferredActorSpawnFromClass(this->M_MapMeshe->GetWorld(), _CardRes, Trans)
		);

	//卡片无效
	if (!IsValid(Card))
	{
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("当前卡片资源无效"));
		}
		return;
	}

	//设置线路
	Card->SetLine(this->GetLine());
	//设置UIMapMesh
	Card->SetUIMapMesh(this);
	//设置UICard
	Card->SetCardUI(_CardMComp->GetUICard(_CardData.ItemName.ToString()));
	//产生阶段结束
	UGameplayStatics::FinishSpawningActor(Card, Trans);

	//初始化绑定移动函数(绑定网格移动)
	if (this->GetMapMeshe())
	{
		this->GetMapMeshe()->AddCardMoveModelBind(Card);
		if (_CardData.M_CardLayer == -1)
		{
			//设置卡片排序优先级
			Card->SetRenderLayer(0);
		}
		else {
			//设置卡片排序优先级
			Card->SetRenderLayer(this->GetCharTranslucency() + _CardData.M_CardLayer + 1);
		}

	}

	//设置当前放置的网格
	AGameMapInstance::GetCardManagerComponent_Static()->SetCurrentCardMapMeshe(this);

	//判断是否是夜间卡片  并且 开启了咖啡粉自动激活
	if (!_CardData.M_CardDay && _CardData.M_AutoEnableCardDay)
	{
		//寻找卡片列表的咖啡粉（如果找到则修改夜间卡片为白天） 
		if (_CardMComp->GetUICard(TEXT("咖啡粉")))
		{
			Card->CardDayTransform(true);
		}
		else if (!AGameMapInstance::GetGameMapInstance()->
			M_MesheControllComponent->
			GetGameMapStructManager()->LevelConfig.LevelIsDay
			)
		{
			//如果本事处于夜晚
			Card->CardDayTransform(true);
		}
	}

	//重置冷却时间
	if (ResetCoolDown)
	{
		if (IsValid(_CardMComp->GetCurrentCoolDownManager()))
		{
			//重置冷却
			_CardMComp->GetCurrentCoolDownManager()->SetCoolDownFinish(false);
		}
	}

	//减去费用
	AGameMapInstance::GetGameMapInstance()->M_ResourceManagerComponent->SubtractFlameNum(_Price);

	//判断卡片是否开启费用线性增加
	if (_CardData.M_CardPriceAutoUp)
	{
		_CardData.M_CardPrice += _CardData.M_CardPriceUpNum;
		UUI_Card* const UICard = _CardMComp->GetUICard(_CardData.ItemName.ToString());
		if (UICard)
		{
			UICard->M_NeedFlame = _CardData.M_CardPrice;
		}
	}

	//添加层级
	if (this->IdAdd(_CardData.M_CardLayer, Card))
	{
		//添加上一次卡片选择(限制到可以被复制的卡片Copy)
		if (_CardData.M_bEnableCopy)
		{
			_CardMComp->M_LastSelectCardClass = _CardData.CardActorResource.TryLoadClass<ACardActor>();
			_CardMComp->M_LastSelectCardName = _CardData.ItemName.ToString();
		}

		//播放音效
		this->PlayCard_PlayBGM();
	}

	//放置完成
	this->PlayFinish(Card);

	if (ResetCoolDown)
	{
		_CardMComp->CancelSelect();
	}
}

void UUI_MapMeshe::PlayFinish(ACardActor* NewCard)
{
	NewCard->UpdateCardCollsion();

	UResourceManagerComponent::ResourceAddPlayCard(NewCard->GetCardName());

	//创建放置动画
	AActor* CurAnim = this->GetMapMeshe()->GetWorld()->SpawnActor(
		LoadClass<AActor>(
			0,
			TEXT("Blueprint'/Game/Resource/Texture/Sprite/VS/Map/0/PlayGroundAnim/BP_放置动画.BP_放置动画_C'")
			)
	);

	CurAnim->SetActorLocation(NewCard->GetUIMapMesh()->GetMapMeshe()->GetActorLocation());
}

bool UUI_MapMeshe::PlayPlayer()
{
	//创建角色
	if (AGameMapInstance::GetGameMapInstance()->M_VSManagerComponent->CreatePlayer(
		true,
		UFVMGameInstance::GetPlayerStructManager_Static()->M_PlayerName,
		this->GetLine().Row,
		this->GetLine().Col,
		0,
		FSoftObjectPath(),
		FPlayerSuitItem(),
		TEXT("")
	))
	{
		return true;
	}

	return false;

	/*
	//查询当前位置是否被占用
	if (!this->TestID(0))
	{
		return false;
	}

	//添加ID 0
	this->IdAdd(0, nullptr);
	//生成角色
	if (IsValid(this->M_MapMeshe))
	{
		//根据地形生成不同的平台【水上，水中，岩浆等地形】
		switch (this->M_ELineType)
		{
		case ELineType::OnWater:
		{
			UFVMGameInstance::PlayBGM_S_Static("PlayCardToOnWater", "ItemAudio");

			if (!this->TestID(-1))
			{
				AGameMapInstance::GetGameMapInstance()->SpawnPlayerToMeshe(this->M_MapMeshe, this);
				this->UpdateAllCardsCollision();
				this->bHasChar = true;

				return true;
			}

			FItemCard CardData = UCardDataComponent::GetCardBaseData(TEXT("木盘子"));
			this->PlayCard(
				AGameMapInstance::GetGameMapInstance()->M_CardManagerComponent
				, LoadClass<ACardActor>(0,
					TEXT("Blueprint'/Game/Resource/BP/GameStart/Item/Card/木盘子.木盘子_C'")
					), CardData, 0, false);
		}break;
		case ELineType::Underwater:
		{
			UFVMGameInstance::PlayBGM_S_Static("PlayCardToOnWater", "ItemAudio");

			if (!this->TestID(3))
			{
				AGameMapInstance::GetGameMapInstance()->SpawnPlayerToMeshe(this->M_MapMeshe, this);
				this->UpdateAllCardsCollision();
				this->bHasChar = true;
				return true;
			}

			FItemCard CardData = UCardDataComponent::GetCardBaseData(TEXT("苏打气泡"));
			this->PlayCard(
				AGameMapInstance::GetGameMapInstance()->M_CardManagerComponent
				, LoadClass<ACardActor>(0,
					TEXT("Blueprint'/Game/Resource/BP/GameStart/Item/Card/苏打气泡.苏打气泡_C'")
					), CardData, 0, false);
		}break;
		default:
		{
			UFVMGameInstance::PlayBGM_S_Static("PlayCardToGround", "ItemAudio");
		}break;
		}

		AGameMapInstance::GetGameMapInstance()->SpawnPlayerToMeshe(this->M_MapMeshe, this);
		this->UpdateAllCardsCollision();
		this->GetMapMeshe()->UpdatePlayerLocation();
		this->bHasChar = true;

		return true;
	}

	return false;
	*/
}

const TMap<int32, ACardActor*>& UUI_MapMeshe::GetCardDatas()
{
	return this->M_Card_Data;
}

void UUI_MapMeshe::GetAllCardsInstance(TArray<ACardActor*>& OutAllCards)
{
	for (auto& Cur : this->M_Card_Data)
	{
		if (IsValid(Cur.Value))
		{
			OutAllCards.Emplace(Cur.Value);
		}
	}
}

ACardActor* UUI_MapMeshe::GetCardByID(const int32& ID)
{
	ACardActor** Card = this->M_Card_Data.Find(ID);

	if (Card && IsValid(*Card))
	{
		return *Card;
	}

	return nullptr;
}

bool UUI_MapMeshe::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_Gird = this->GetWidgetComponent<UImage>(this, "Meshe");

	this->bPlayer = false;

	//this->SetColor(FVector(1.f), 1.f);

	return true;
}

bool UUI_MapMeshe::IdAdd(const int32 _ID, ACardActor* _Card)
{

	if (!IsValid(_Card) && _ID != 0)
	{
		return false;
	}

	ACardActor** _Reult = this->M_Card_Data.Find(_ID);

	if (!_Reult)
	{
		this->M_Card_Data.Emplace(_ID, _Card);
		return true;
	}

	return false;
}

bool UUI_MapMeshe::TestID(const int32 _ID)
{
	ACardActor** _Reult = this->M_Card_Data.Find(_ID);

	//如果当前卡片存在返回false
	if (_Reult && IsValid(*_Reult))
	{
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__,
				FString(TEXT("卡片ID测试失败,已有卡片:")) + (*_Reult)->GetCardName()
			);
		}

		return false;
	}

	if (_ID == 0 && this->GetPlayer())
	{
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("卡片ID测试失败,存在角色")));
		}

		return false;
	}
	else {
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("卡片ID测试成功,无卡片")));
		}
	}

	return true;
}

void UUI_MapMeshe::KillAllCards()
{
	TMap<int32, ACardActor*> Temp = this->M_Card_Data;
	for (const auto& Cur : Temp)
	{
		//如果角色下方存在底卡，则不会被消灭
		if (Cur.Key == -1)
		{
			if (IsValid(Cur.Value))
			{
				if (this->GetPlayer())
				{
					continue;
				}
			}
		}

		if (IsValid(Cur.Value))
		{
			Cur.Value->UpdateCardState(Cur.Value->GetTotalHP(), 0.f);
			Cur.Value->KillCard();
		}
	}

	this->M_Card_Data.Empty();
}

void UUI_MapMeshe::UpdateCardByLineType(const TMap<int32, ACardActor*>& LastCards)
{
	TMap<int32, ACardActor*> CurKills;

	//标记类型不匹配的卡片
	for (const auto& CurCard : LastCards)
	{
		if (IsValid(CurCard.Value))
		{
			if (CurCard.Value->GetLineType() != this->M_ELineType)
			{
				if (CurCard.Value->GetLineType() != ELineType::All)
				{
					if (this->UpdateCardByMesheType(CurKills, CurCard.Key, CurCard.Value))
					{
						continue;
					}
				}
			}

			if (this->TestID(CurCard.Key))
			{
				if (this->IdAdd(CurCard.Key, CurCard.Value))
				{
					CurCard.Value->SetUIMapMesh(this);
					this->PlayFinish(CurCard.Value);
				}
			}
		}
	}

	//销毁类型不匹配的卡片
	for (const auto& CurCard : CurKills)
	{
		if (IsValid(CurCard.Value))
		{
			CurCard.Value->KillCard();
		}
	}
}

bool UUI_MapMeshe::UpdateCardByMesheType(TMap<int32, ACardActor*>& KillCards, int32 Layer, ACardActor* CurCard)
{
	KillCards.Emplace(Layer, CurCard);

	return true;
}

void UUI_MapMeshe::UpdateCardsLocation(ACardActor* SingleCard /*= nullptr*/)
{
	if (IsValid(SingleCard))
	{
		int32 CurLayer = SingleCard->GetCardData().M_CardLayer;
		SingleCard->SetActorLocation(this->GetMapMeshe()->GetActorLocation());
	}
	else {
		for (const auto& Cur : this->M_Card_Data)
		{
			if (IsValid(Cur.Value))
			{
				int32 CurLayer = Cur.Value->GetCardData().M_CardLayer;
				Cur.Value->SetActorLocation(this->GetMapMeshe()->GetActorLocation());
			}
		}
	}
}

void UUI_MapMeshe::CloseAllCardsCollision()
{
	for (const auto& Cur : this->M_Card_Data)
	{
		if (IsValid(Cur.Value))
		{
			Cur.Value->SetCollisionEnable(false);
		}
	}
}

void UUI_MapMeshe::UpdateAllCardsCollision()
{
	for (const auto& Cur : this->M_Card_Data)
	{
		if (IsValid(Cur.Value))
		{
			Cur.Value->UpdateCardCollsion();
		}
	}
}

FString UUI_MapMeshe::GetCardNameByID(const int32& _Id)
{

	ACardActor** Card = this->M_Card_Data.Find(_Id);


	if (Card && IsValid(*Card))
	{
		return (*Card)->GetCardName();
	}

	return FString(TEXT(""));
}

UUI_Card* UUI_MapMeshe::GetCardUIByID(const int32& _Id)
{
	ACardActor** Card = this->M_Card_Data.Find(_Id);

	if (Card && IsValid(*Card))
	{
		return (*Card)->GetCardUI();
	}

	return nullptr;
}

FString UUI_MapMeshe::GetCardActorNameByID(const int32& _Id)
{
	FItemCard Data;
	if (AGameMapInstance::GetCardManagerComponent_Static()->GetCardDataByName(this->GetCardNameByID(_Id), Data))
	{
		return Data.ItemName.ToString();
	}
	return FString(TEXT(""));
}

FLine UUI_MapMeshe::GetLine()
{
	return this->M_Line;
}

bool UUI_MapMeshe::EradicateCard()
{
	//销毁顺序
	TArray<int8> M_Card_Layer = { 5,4,2,0,1,-1,3 };

	for (auto PP = M_Card_Layer.CreateConstIterator(); PP; PP++)
	{
		ACardActor** _Reult = this->M_Card_Data.Find((*PP));

		if (
			((*PP) == -1
				||
				(*PP) == 3)
			&&
			this->GetPlayer()
			)
		{
			return false;
		}

		if (_Reult && IsValid(*_Reult))
		{
			(*_Reult)->KillCard();

			return true;
		}
	}

	return false;
}

bool UUI_MapMeshe::EradicateCardByID(int32 Id)
{
	ACardActor** CurCard = this->M_Card_Data.Find(Id);

	if (CurCard)
	{
		if (IsValid(*CurCard))
		{
			(*CurCard)->KillCard();

			return true;
		}
	}

	return false;
}

void UUI_MapMeshe::SetColor(FVector _Vec, float _Alpha)
{
	UWidgetBase::SetImageColor(this->M_Gird, _Vec, _Alpha);
}

void UUI_MapMeshe::SetMapMeshe(AMapMeshe* _Meshe)
{
	this->M_MapMeshe = _Meshe;
}

void UUI_MapMeshe::SetPlayer(AGamePlayer* NewPlayerIns)
{
	this->M_PlayerIns = NewPlayerIns;
	this->bPlayer = true;
}

void UUI_MapMeshe::SetLine(FLine CurLine)
{
	this->M_Line = CurLine;
}

void UUI_MapMeshe::SetCardCollisionByID(int32 ID)
{
	ACardActor** CurCard = this->M_Card_Data.Find(ID);
	if (CurCard && IsValid(*CurCard))
	{
		(*CurCard)->SetCollisionEnable(true);
	}
}

void UUI_MapMeshe::SetUIMesheEnabled(bool value, UObject* CurObj)
{
	this->bMesheEnabled = true;

	if (IsValid(CurObj))
	{
		if (value)
		{
			this->M_MesheDis.Remove(UGameSystemFunction::GetObjectName(CurObj));
			if (this->M_MesheDis.Num() != 0)
			{
				this->bMesheEnabled = false;
			}
		}
		else {
			this->bMesheEnabled = false;
			this->M_MesheDis.Emplace(UGameSystemFunction::GetObjectName(CurObj), CurObj);
		}
	}
	else {
		this->CheckCurMesheEnabled();
	}
}

bool UUI_MapMeshe::CheckCurMesheEnabled()
{
	if (this->M_MesheDis.Num() == 0)
	{
		this->bMesheEnabled = true;
		return true;
	}

	TMap<FString, UObject*> Temp;
	for (const auto& Cur : this->M_MesheDis)
	{
		//如果当前对象有效
		if (IsValid(Cur.Value))
		{
			Temp.Emplace(Cur.Key, Cur.Value);
		}
	}
	//移除之前的所有内容
	this->M_MesheDis.Empty();
	//复制新内容
	this->M_MesheDis = Temp;

	if (this->M_MesheDis.Num() == 0)
	{
		this->bMesheEnabled = true;
		return true;
	}
	else {
		this->bMesheEnabled = false;
		return false;
	}
}

TArray<UObject*> UUI_MapMeshe::GetUIMesheEnabledObjs()
{
	TArray<UObject*> CurObjs;
	for (const auto& Cur : this->M_MesheDis)
	{
		CurObjs.Emplace(Cur.Value);
	}
	return CurObjs;
}

bool UUI_MapMeshe::GetUIMesheEnabled()
{
	return this->CheckCurMesheEnabled();
}

bool UUI_MapMeshe::GetPlayer()
{
	return this->bPlayer;
}

bool UUI_MapMeshe::GetCurMesheIsEnable()
{
	if (IsValid(this->M_MapMeshe))
	{
		return this->M_MapMeshe->GetbUse();
	}
	return false;
}

bool UUI_MapMeshe::EradicateCard(const int32& _CardLayer)
{
	ACardActor** _Reult = this->M_Card_Data.Find(_CardLayer);

	if (_Reult && IsValid(*_Reult))
	{
		//判断卡片是否有线性加价(如果有则减少加价)
		if (FItemCard* const CardData =
			AGameMapInstance::GetCardManagerComponent_Static()->GetCardDataByName(
				(*_Reult)->GetCardName()
			))
		{
			if (
				CardData->M_CardPriceAutoUp
				&&
				AGameMapInstance::GetCardManagerComponent_Static()->GetUICard(CardData->ItemName.ToString())
				)
			{
				AGameMapInstance::GetCardManagerComponent_Static()
					->GetUICard(CardData->ItemName.ToString())
					->M_NeedFlame -= CardData->M_CardPriceUpNum;

				CardData->M_CardPrice -= CardData->M_CardPriceUpNum;
			}
		}

		this->M_Card_Data.Remove(_CardLayer);

		(*_Reult)->CardDeath();

		return true;
	}

	return false;
}

bool UUI_MapMeshe::CreateNewCard(UCardManagerComponent* _CardMComp)
{
	if (!this->CheckCurMesheEnabled())
	{
		return false;
	}

	//调用卡片创建虚函数【子类也实现】
	return  this->CreateCard(
		this,
		_CardMComp,
		_CardMComp->GetCurrentSelectActor(),
		_CardMComp->M_CurrentSelectCardName,
		_CardMComp->GetCardFlameNum(_CardMComp->M_CurrentSelectCardName),
		true,
		true);
}

bool UUI_MapMeshe::CreateCard(
	UUI_MapMeshe* _Meshe,
	UCardManagerComponent* _CardMComp,
	UClass* _CardRes,
	const FString& _CardActorName,
	int32 _Price,
	bool ResetCoolDown,
	bool _bCheck
)
{
	//如果地形被禁止
	if (!this->CheckCurMesheEnabled())
	{
		AGameMapInstance::GetVSManagerComponent_Static()->ShowText(TEXT("当前卡片不适合放在当前网格"));
		return false;
	}

	if (_bCheck)
	{
		//如果选择了铲卡
		if (_CardMComp->M_CurrentSelectEradicate)
		{
			this->EradicateCard();
			_CardMComp->CancelEradicate();

			AGameActorFlipbookBase* Eradicate_ = this->GetWorld()->SpawnActor<AGameActorFlipbookBase>(
				LoadClass<AGameActorFlipbookBase>(0,
					TEXT("Blueprint'/Game/Resource/BP/GameStart/VS/BPEradicate.BPEradicate_C'")
					),
				this->M_MapMesheTransform);

			Eradicate_->SetAnimationPlayEndDestroy();
			Eradicate_->SetFlipbookRotation(FRotator(0.f, 90.f, 0.f));

			switch (this->M_ELineType)
			{
			case ELineType::OnWater:
			case ELineType::Underwater: {UFVMGameInstance::PlayBGM_S_Static("PlayCardToOnWater", "ItemAudio"); }break;
			default: {UFVMGameInstance::PlayBGM_S_Static("PlayCardToGround", "ItemAudio"); }break;

			}
			return false;
		}

		//如果当前玩家选择了卡牌
		if (_CardMComp->GetCurrentSelectActor())
		{
			return true;
		}

		return false;
	}

	return true;
}

void UUI_MapMeshe::BoxOverlapBeginByAMapMeshe(AActor* _Actor)
{
	if (!IsValid(_Actor))
	{
		return;
	}

	//当老鼠进入网格时设置老鼠排序
	if (Cast<AMouseActor>(_Actor))
	{
		this->OnMouseEnter(Cast<AMouseActor>(_Actor));
	}
}

void UUI_MapMeshe::BoxOverlapEndByAMapMeshe(AActor* _Actor)
{
	//当老鼠进入网格时设置老鼠排序
	if (Cast<AMouseActor>(_Actor))
	{
		if (this->GetLine().Col == 0)
		{
			if (Cast<AMouseActor>(_Actor)->GetCurrentHP() > 0.f)
			{
				if (Cast<AMouseActor>(_Actor)->GetMouseLine().Row == this->M_Line.Row)
				{
					if (Cast<AMouseActor>(_Actor)->GetActorLocation().Y < this->GetMapMeshe()->GetActorLocation().Y)
					{
						Cast<AMouseActor>(_Actor)->InMapMeshe(ELineType::OnGround);
					}
				}
			}
		}
	}
}

void UUI_MapMeshe::TickByAMapMeshe(float DeltaTime)
{
}

AMapMeshe* UUI_MapMeshe::GetMapMeshe()
{
	return this->M_MapMeshe;
}

FVector UUI_MapMeshe::GetMapMeshCurrentTop(int32 _Col)
{
	AMapMeshe* Meshe = AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->GetMapMeshCurrentTop(_Col);
	return Meshe->GetActorLocation();
}

FVector UUI_MapMeshe::GetMapMeshCurrentBottom(int32 _Col)
{
	AMapMeshe* Meshe = AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->GetMapMeshCurrentBottom(_Col);
	return Meshe->GetActorLocation();
}

FVector UUI_MapMeshe::GetMapMeshCurrentLeft(int32 Row)
{
	AMapMeshe* Meshe = AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->GetMapMeshCurrentLeft(Row);
	return Meshe->GetActorLocation();
}

FVector UUI_MapMeshe::GetMapMeshCurrentRight(int32 Row)
{
	AMapMeshe* Meshe = AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->GetMapMeshCurrentRight(Row);
	//FVector A = Meshe->GetWidgetComponentTransform().GetLocation();
	//UE_LOG(LogTemp, Error, TEXT("%d %.2f %.2f %.2f"), Row,A.X, A.Y, A.Z);
	return Meshe->GetActorLocation();
}

bool UUI_MapMeshe::BeginPlayCard(
	FItemCard& _Card,
	UCardManagerComponent* _CardMComp,
	UClass* _CardRes,
	int32 _Price,
	bool ResetCoolDown
)
{
	//判断卡片是否重叠并且查询是否存在相同索引
	if (_Card.M_CardOverlap && !this->TestID(_Card.M_CardLayer))
	{
		//判断当前卡片重叠范围的名称是否匹配			
		//查询卡片 
		ACardActor** _Reult = this->M_Card_Data.Find(_Card.M_CardLayer);

		FString LOverlapCardName;

		//如果查询成功并且这个卡片类型和当前网格类型匹配
		if (_Reult && IsValid(*_Reult))
		{
			for (auto Name : _Card.M_CardOverlapName)
			{
				if ((*_Reult)->GetCardName().Equals(Name))
				{
					//匹配成功
					//寻找当前索引所对应的卡片(销毁)
					(*_Reult)->KillCard();


					//放置卡片
					this->PlayCard(_CardMComp, _CardRes, _Card, _Price, ResetCoolDown);

					return true;
				}

				LOverlapCardName += Name + TEXT(" ");
			}
		}

		AGameMapInstance::GetVSManagerComponent_Static()->ShowText(
			FString(TEXT("当前卡片需要在{")) + LOverlapCardName + TEXT("} 这些卡的基础上放置")
		);
	}
	else {
		//卡片无需重叠
		if (!_Card.M_CardOverlap && this->TestID(_Card.M_CardLayer))
		{
			this->PlayCard(_CardMComp, _CardRes, _Card, _Price, ResetCoolDown);

			return true;
		}
	}

	AGameMapInstance::GetVSManagerComponent_Static()->ShowText(TEXT("无法放置当前防御卡"));

	return false;
}

void UUI_MapMeshe::CardTypeDebug(const FString& _CardName, ELineType _LineType)
{
	if (!this->CheckCurMesheEnabled())
	{
		AGameMapInstance::GetVSManagerComponent_Static()->ShowText(TEXT("当前地形被占用"));

		return;
	}

	switch (_LineType)
	{
	case ELineType::OnGround:
		AGameMapInstance::GetVSManagerComponent_Static()->ShowText(TEXT("当前卡片适合放在-陆地上"));
		break;
	case ELineType::Sky:
		AGameMapInstance::GetVSManagerComponent_Static()->ShowText(TEXT("当前卡片适合放在-空中"));
		break;
	case ELineType::OnWater:
		AGameMapInstance::GetVSManagerComponent_Static()->ShowText(TEXT("当前卡片适合放在-水上"));
		break;
	case ELineType::Underwater:
		AGameMapInstance::GetVSManagerComponent_Static()->ShowText(TEXT("当前卡片适合放在-水下"));
		break;
	case ELineType::Magma:
	{
		if (_CardName.Equals(TEXT("棉花糖")))
			AGameMapInstance::GetVSManagerComponent_Static()->ShowText(TEXT("当前卡片适合放在-岩浆或者填补缺失的云朵"));
		else
			AGameMapInstance::GetVSManagerComponent_Static()->ShowText(TEXT("当前卡片适合放在-岩浆"));
	}
	break;
	case ELineType::UnderwaterAndGround:
		AGameMapInstance::GetVSManagerComponent_Static()->ShowText(TEXT("当前卡片适合放在-陆地或者水下"));
		break;
	case ELineType::SkyAndGround:
		AGameMapInstance::GetVSManagerComponent_Static()->ShowText(TEXT("当前卡片适合放在-空中或者陆地"));
		break;
	default:
		break;
	}
}

int32 UUI_MapMeshe::GetTranslucency()
{
	return this->M_Translucency;
}

int32 UUI_MapMeshe::GetCharTranslucency()
{
	if (this->bHasChar)
	{
		return this->M_PlayerIns->GetPlayerRenderLayerToCardLayer();
	}

	return this->GetTranslucency();
}

void UUI_MapMeshe::SetTranslucency(const int32& _Translucency)
{
	this->M_Translucency = _Translucency;
	this->TranslucencyChange(_Translucency);
}

void UUI_MapMeshe::OnMouseEnter(AMouseActor* Mouse)
{
	if (Mouse->GetCurrentHP() > 0.f)
	{
		if (Mouse->GetMouseLine().Row == this->M_Line.Row)
		{
			Mouse->SetRenderLayer(this->GetTranslucency());
			Mouse->InMapMeshe(this->M_ELineType);
		}
	}
}

void UUI_MapMeshe::OnMouseEnd(AMouseActor* Mouse)
{

}
