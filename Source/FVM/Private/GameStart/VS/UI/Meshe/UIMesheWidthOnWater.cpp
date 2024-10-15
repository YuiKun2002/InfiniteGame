// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/VS/UI/Meshe/UIMesheWidthOnWater.h"
#include "GameStart/VS/UI/Meshe/UIMesheWidthAll.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
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

bool UUIMesheWidthOnWater::Initialize()
{
	return Super::Initialize();
}

bool UUIMesheWidthOnWater::CreateCard(UUI_MapMeshe* _Meshe, UCardManagerComponent* _CardMComp, UClass* _CardRes, const FString& _CardActorName, int32 _Price, bool ResetCoolDown, bool _bCheck)
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
			[&](UCardManagerComponent* _CardMComp)->bool {

				//如果地形被禁止
				if (!this->GetUIMesheEnabled())
				{
					return false;
				}

				//获取卡片数据
				FItemCard& CardData = _CardMComp->GetCardData(_CardActorName);

				//卡片属于水上类型
				if (CardData.M_ELineType == ELineType::OnWater)
				{
					//查询卡片 ->层级为-1 （查询承载类型卡片）
					ACardActor** _Reult = this->M_Card_Data.Find(-1);
					//如果查询成功并且这个卡片类型和当前网格类型匹配
					if (_Reult && IsValid(*_Reult))
					{
						AGameMapInstance::GetVSManagerComponent_Static()->ShowText(
							TEXT("当前卡片无法放置，已经存在一张水上类型的卡片！！！")
						);

						return false;
					}
					else {
						//反向查询
						for (const auto& CurCard : this->GetCardDatas())
						{
							if (IsValid(CurCard.Value) && CurCard.Value->GetLineType() == ELineType::OnWater)
							{
								AGameMapInstance::GetVSManagerComponent_Static()->ShowText(
									TEXT("当前卡片无法放置，已经存在一张水上类型的卡片！！！")
								);
								return false;
							}
						}
					}

					return this->BeginPlayCard(CardData, _CardMComp, _CardRes, CardData.M_CardPrice, ResetCoolDown);
				}
				else {
					//卡片属于非水上类型
					if (this->GroundCardToOnWater(CardData, _CardMComp))
					{
						return this->BeginPlayCard(CardData, _CardMComp, _CardRes, CardData.M_CardPrice, ResetCoolDown);
					}
				}

				this->CardTypeDebug(CardData.ItemName.ToString(), CardData.M_ELineType);

				return false;
			}
		);
	}

	return false;
}

bool UUIMesheWidthOnWater::UpdateCardByMesheType(TMap<int32, ACardActor*>& KillCards, int32 Layer, ACardActor* CurCard)
{
	switch (CurCard->GetLineType())
	{
	case ELineType::OnGround:
	{
		//创建落水动画
		AActor* CurAnim = this->GetMapMeshe()->GetWorld()->SpawnActor(
			LoadClass<AActor>(
				0,
				TEXT("Blueprint'/Game/Resource/Texture/Sprite/VS/Map/0/WaterAnim/BP_落水动画.BP_落水动画_C'")
				)
		);

		CurAnim->SetActorLocation(CurCard->GetUIMapMesh()->GetMapMeshe()->GetActorLocation());
	}
	break;
	case ELineType::SkyAndGround:
	case ELineType::Sky:
		return false;
	}

	KillCards.Emplace(Layer, CurCard);

	return true;
}

void UUIMesheWidthOnWater::UpdateCardsLocation(ACardActor* SingleCard /*= nullptr*/)
{
	if (IsValid(SingleCard))
	{
		int32 CurLayer = SingleCard->GetCardData().M_CardLayer;
		SingleCard->SetActorLocation(this->GetMapMeshe()->GetActorLocation());
		if (CurLayer == -1)
		{
			SingleCard->AddActorLocalOffset(FVector(0.f, 0.f, MesheWidthOnWaterCardOffset));
		}
		else {
			SingleCard->AddActorLocalOffset(FVector(0.f, 0.f, MesheWidthOnWaterOtherCardOffset));
		}
	}
	else {
		for (const auto& Cur : this->M_Card_Data)
		{
			if (IsValid(Cur.Value))
			{
				int32 CurLayer = Cur.Value->GetCardData().M_CardLayer;
				Cur.Value->SetActorLocation(this->GetMapMeshe()->GetActorLocation());
				if (CurLayer == -1)
				{
					Cur.Value->AddActorLocalOffset(FVector(0.f, 0.f,MesheWidthOnWaterCardOffset));
				}
				else {
					Cur.Value->AddActorLocalOffset(FVector(0.f, 0.f, MesheWidthOnWaterOtherCardOffset));
				}
			}
		}
	}
}

void UUIMesheWidthOnWater::PlayFinish(ACardActor* NewCard)
{
	Super::PlayFinish(NewCard);

	if (this->GetMapMeshe())
	{
		this->GetMapMeshe()->UpdateCardsLocation(NewCard);
	}
}

bool UUIMesheWidthOnWater::GroundCardToOnWater(FItemCard& _Card, UCardManagerComponent* _CardMComp)
{
	//如果卡片是陆地类型
	if (
		_Card.M_ELineType == ELineType::OnGround
		||
		_Card.M_ELineType == ELineType::UnderwaterAndGround
		)
	{
		//查询卡片 ->层级为-1 （查询承载类型卡片）
		ACardActor** _Reult = this->M_Card_Data.Find(-1);

		//如果查询成功并且这个卡片类型和当前网格类型匹配
		if (_Reult && IsValid(*_Reult) && (*_Reult)->GetLineType() == this->M_ELineType)
		{
			return true;
		}
	}
	else {
		if (
			_Card.M_ELineType == ELineType::SkyAndGround
			||
			_Card.M_ELineType == ELineType::Sky
			)
		{
			return true;
		}
	}

	this->CardTypeDebug(_Card.ItemName.ToString(), _Card.M_ELineType);

	return false;
}
