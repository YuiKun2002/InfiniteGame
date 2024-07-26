// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/VS/UI/Meshe/UIMesheWidthUnderWater.h"

#include "GameStart/VS/UI/Meshe/UIMesheWidthAll.h"

#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameStart/Flipbook/GameActor/GamePlayer.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"

#include "GameStart/VS/Components/CardManagerComponent.h"
#include "GameStart/VS/Components/VSManagerComponent.h"
#include "GameStart/VS/CardCoolDownManager.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/MapMeshe.h"

#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/VS/Components/Item\ShootLineComponent.h"

#include "GameStart/VS/CreateCardBar.h"
#include "GameStart/VS/UI/UI_Card.h"

#include "GameSystem/FVMGameInstance.h"

#include <Kismet/GameplayStatics.h>

#include <Components/Image.h>

bool UUIMesheWidthUnderWater::Initialize()
{
	Super::Initialize();

	return true;
}

bool UUIMesheWidthUnderWater::CreateCard(UUI_MapMeshe* _Meshe, UCardManagerComponent* _CardMComp, UClass* _CardRes, const FString& _CardActorName, int32 _Price, bool ResetCoolDown, bool _bCheck)
{
	if (Super::CreateCard(_Meshe, _CardMComp, _CardRes, _CardActorName, _Price, ResetCoolDown, _bCheck))
	{
		return UUIMesheWidthAll::CreateCard(
			_Meshe,
			_CardMComp,
			_CardRes,
			_CardActorName,
			_Price,
			ResetCoolDown,
			[&](UCardManagerComponent* _MComp)->bool {

				//获取卡片数据，判断卡片类型
				FItemCard& CardData = _CardMComp->GetCardData(_CardActorName);

				if (
					CardData.M_ELineType == ELineType::OnGround ||
					CardData.M_ELineType == ELineType::UnderwaterAndGround ||
					CardData.M_ELineType == ELineType::SkyAndGround ||
					CardData.M_ELineType == ELineType::Underwater
					)
				{
					//放置卡片，如果放置失败则什么都不管
					return this->BeginPlayCard(CardData, _CardMComp, _CardRes, CardData.M_CardPrice, ResetCoolDown);
				}

				//打印Log
				this->CardTypeDebug(CardData.ItemName, CardData.M_ELineType);

				return false;
			});
	}

	return false;
}

void UUIMesheWidthUnderWater::TickByAMapMeshe(float DeltaTime)
{
	Super::TickByAMapMeshe(DeltaTime);

	if (!this->M_MapMeshUpdate.M_IsbUpdate)
	{
		return;
	}

	this->M_MapMeshUpdate.M_Time += DeltaTime;

	if (this->M_MapMeshUpdate.M_Time > 1.f)
	{
		this->M_MapMeshUpdate.M_Time = 0.f;

		bool LResult = false;
		for (auto& Card : this->M_MapMeshUpdate.GetUpdateCards())
		{
			if (IsValid(Card))
			{
				//更新卡片血量
				//更新最新状态
				if (Card->UpdateCardState(1, 0))
				{
					Card->KillCard();
				}

				LResult = true;

				break;
			}
		}

		//如果列表为空
		if (!LResult)
		{
			this->M_MapMeshUpdate.GetUpdateCards().Empty();
			this->M_MapMeshUpdate.M_Time = 0.f;
			this->M_MapMeshUpdate.M_IsbUpdate = false;

			//隐藏泡泡
			if (IsValid(this->WaterBubbles))
			{
				this->WaterBubbles->SetActorHiddenInGame(true);
			}
		}
		else {
			//如果列表中不为空，但是没有气泡，则显示气泡
			if (!IsValid(this->GetCardByID(3)))
			{
				//显示泡泡
				if (IsValid(this->WaterBubbles))
				{
					this->WaterBubbles->SetActorHiddenInGame(false);
				}

				this->M_MapMeshUpdate.M_IsbUpdate = true;
			}
		}
	}
}

bool UUIMesheWidthUnderWater::UpdateCardByMesheType(TMap<int32, ACardActor*>& KillCards, int32 Layer, ACardActor* CurCard)
{
	switch (CurCard->GetLineType())
	{
	case ELineType::OnGround:
	case ELineType::SkyAndGround:
	case ELineType::UnderwaterAndGround:
		return false;
	}

	KillCards.Emplace(Layer, CurCard);

	return true;
}

void UUIMesheWidthUnderWater::PlayFinish(ACardActor* NewCard)
{
	Super::PlayFinish(NewCard);

	FItemCard CardData = NewCard->GetCardData();

	//加载泡泡
	if (!IsValid(this->WaterBubbles))
	{
		this->WaterBubbles = this->GetMapMeshe()->GetWorld()->SpawnActor<AActor>(
			LoadClass<AActor>(nullptr,
				TEXT("Blueprint'/Game/Resource/BP/GameStart/MapLauncher/5/t/BP_AWaterBubbles.BP_AWaterBubbles_C'")
				),
			this->GetMapMeshe()->GetActorTransform()
			);

		this->WaterBubbles->SetActorHiddenInGame(true);
	}

	if (
		CardData.M_ELineType == ELineType::OnGround ||
		CardData.M_ELineType == ELineType::UnderwaterAndGround ||
		CardData.M_ELineType == ELineType::SkyAndGround ||
		CardData.M_ELineType == ELineType::Underwater
		)
	{
		//清空更新情况
		this->M_MapMeshUpdate.GetUpdateCards().Empty();

		//是否需要显示泡泡
		bool bShowBubble = true;
		//更新网格
		for (auto CurCard : this->GetCardDatas())
		{
			const ACardActor* CurrentCard = CurCard.Value;

			//如果value存在，但是是nullptr ,表示是角色占用位
			if (!CurrentCard)
			{
				//player
				continue;
			}

			if (
				CurrentCard->GetLineType() != ELineType::Underwater
				&&
				CurrentCard->GetLineType() != ELineType::UnderwaterAndGround
				)
			{
				//添加更新
				this->M_MapMeshUpdate.AddCardUpdate(const_cast<ACardActor*>(CurrentCard));
				//设置MapMeshe网格更新
				this->M_MapMeshUpdate.M_IsbUpdate = true;
			}
			else {
				if (CurrentCard->GetCardID() == 3)
				{
					bShowBubble = false;
					//添加更新
					this->M_MapMeshUpdate.AddCardUpdateAt(const_cast<ACardActor*>(CurrentCard), 0);
					//设置MapMeshe网格更新
					this->M_MapMeshUpdate.M_IsbUpdate = true;
				}
				else if ( //海底卡片，则无需更新
					CurrentCard->GetLineType() == ELineType::Underwater
					||
					CurrentCard->GetLineType() == ELineType::UnderwaterAndGround

					)
				{
					bShowBubble = false;
				}
			}
		}

		if (bShowBubble)
		{
			//显示泡泡
			if (IsValid(this->WaterBubbles))
			{
				this->WaterBubbles->SetActorHiddenInGame(false);
			}
		}
		else {
			//显示泡泡
			if (IsValid(this->WaterBubbles))
			{
				this->WaterBubbles->SetActorHiddenInGame(true);
			}
		}
	}
}
