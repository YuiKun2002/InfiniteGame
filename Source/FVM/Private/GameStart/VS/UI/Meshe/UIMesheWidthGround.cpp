// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/VS/UI/Meshe/UIMesheWidthGround.h"
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

bool UUIMesheWidthGround::Initialize()
{
	return Super::Initialize();
}

bool UUIMesheWidthGround::CreateCard(UUI_MapMeshe* _Meshe, UCardManagerComponent* _CardMComp, UClass* _CardRes, const FString& _CardActorName, int32 _Price, bool ResetCoolDown, bool _bCheck)
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

				//如果地形被禁止
				if (!this->GetUIMesheEnabled())
				{
					return false;
				}

				//获取卡片数据，判断卡片是否属于陆地类型
				FItemCard& CardData = _CardMComp->GetCardData(_CardActorName);

				if (
					CardData.M_ELineType == ELineType::OnGround ||
					CardData.M_ELineType == ELineType::UnderwaterAndGround ||
					CardData.M_ELineType == ELineType::SkyAndGround
					)
				{
					return this->BeginPlayCard(CardData, _CardMComp, _CardRes, _Price, ResetCoolDown);
				}

				//打印Log
				this->CardTypeDebug(CardData.ItemName, CardData.M_ELineType);

				return false;
			});
	}

	return false;
}

bool UUIMesheWidthGround::UpdateCardByMesheType(TMap<int32, ACardActor*>& KillCards, int32 Layer, ACardActor* CurCard)
{
	switch (CurCard->GetLineType())
	{
	case ELineType::UnderwaterAndGround:
	case ELineType::SkyAndGround:
		return false;
	}

	KillCards.Emplace(Layer, CurCard);

	return true;
}
