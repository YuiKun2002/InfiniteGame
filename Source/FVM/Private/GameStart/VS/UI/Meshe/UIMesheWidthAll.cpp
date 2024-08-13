// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/VS/UI/Meshe/UIMesheWidthAll.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameStart/Flipbook/GameActor/GamePlayer.h"

#include "GameStart/VS/Components/CardManagerComponent.h"
#include "GameStart/VS/Components/VSManagerComponent.h"
#include "GameStart/VS/Components/Card/CardDataComponent.h"
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


bool UUIMesheWidthAll::Initialize()
{
	return Super::Initialize();
}

bool UUIMesheWidthAll::NewCard(UUI_MapMeshe* _Meshe, UCardManagerComponent* _CardMComp, UClass* _CardRes, const FString& _CardActorName, int32 _Price, bool ResetCoolDown)
{
	//获取卡片数据
	FItemCard CardData = _CardMComp->GetCardData(_CardActorName);

	//如果当前卡片有效
	if (_CardMComp->GetCurrentSelectActor())
	{
		//判断当前格子是否有角色
		ACardActor** _Reult = _Meshe->M_Card_Data.Find(0);
		if (_Reult && (*_Reult) == nullptr && CardData.M_CardLayer == 0)
		{
			//查询到角色
			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("卡片放置失败，当前ID已经被角色占用")));
			}
				

			AGameMapInstance::GetVSManagerComponent_Static()->ShowText(TEXT("当前位置被主角占用，你将无法放置"));

			return false;
		}
	}

	//如果名称是空的则返回false
	if (CardData.ItemName.ToString().Equals(TEXT("Null")))
	{
		//全卡片搜索，重新赋值卡片数据
		CardData = UCardDataComponent::GetCardBaseData(_CardActorName);
		CardData.M_CardOverlap = false;

		UGameSystemFunction::FVMLog(__FUNCTION__,
			FString(TEXT("卡片：") + CardData.ItemName.ToString() + TEXT("Layer:") + FString::FromInt(CardData.M_CardLayer))
		);

		if (CardData.ItemName.ToString().Equals(TEXT("Null")))
		{
			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("卡片数据出错：") + _CardActorName));
			}
			AGameMapInstance::GetVSManagerComponent_Static()->ShowText(
				FString(TEXT("[UI_MapMeshe.cpp 349]卡片数据出错：") + _CardActorName)
			);
			return false;
		}
	}

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("准备创建新卡片：") + CardData.ItemName.ToString()));
	}

	//先判断当前卡片能否自身替换
	if (CardData.M_ReplaceSelf)
	{
		//查询卡片
		for (auto Card = _Meshe->M_Card_Data.CreateIterator(); Card; ++Card)
		{
			if (IsValid(Card.Value()))
			{
				if ((Card.Value())->GetCardData().ItemName.EqualTo(CardData.ItemName))
				{
					//卡片查询成功(销毁卡片)
					Card.Value()->KillCard();

					if (UFVMGameInstance::GetDebug())
						UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("卡片替换")));

					//返回
					break;
				}
			}
		}
	}


	//如果允许则直接放置
	if (CardData.M_ELineType == ELineType::All || _Meshe->M_ELineType == ELineType::All)
	{
		if (_Meshe->TestID(CardData.M_CardLayer))
		{
			_Meshe->PlayCard(_CardMComp, _CardRes, CardData, _Price, ResetCoolDown);
		}

		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("当前卡片可以直接放置：") + CardData.ItemName.ToString()));
		}

		return false;
	}


	return true;
}

bool UUIMesheWidthAll::CreateCard(UUI_MapMeshe* _Meshe, UCardManagerComponent* _CardMComp, UClass* _CardRes, const FString& _CardActorName, int32 _Price, bool ResetCoolDown, bool _bCheck)
{
	if (Super::CreateCard(_Meshe, _CardMComp, _CardRes, _CardActorName, _Price, ResetCoolDown, _bCheck))
	{
		//创建新卡片
		UUIMesheWidthAll::NewCard(_Meshe, _CardMComp, _CardRes, _CardActorName, _Price, ResetCoolDown);

		//获取卡片数据
		FItemCard CardData = _CardMComp->GetCardData(_CardActorName);
		if (CardData.ItemName.ToString().Equals(TEXT("Null")))
		{
			//全卡片搜索，重新赋值卡片数据
			CardData = UCardDataComponent::GetCardBaseData(_CardActorName);
			CardData.M_CardOverlap = false;
		}

		if (CardData.M_ELineType == ELineType::All || this->M_ELineType == ELineType::All)
		{
			return true;
		}
	}

	return false;
}

bool UUIMesheWidthAll::CreateCard(UUI_MapMeshe* _Meshe, UCardManagerComponent* _CardMComp, UClass* _CardRes, const FString& _CardActorName, int32 _Price, bool ResetCoolDown, TFunctionRef<bool(UCardManagerComponent*)> _Func)
{
	//判断ALL类型或者卡片是否有效
	if (UUIMesheWidthAll::NewCard(_Meshe, _CardMComp, _CardRes, _CardActorName, _Price, ResetCoolDown))
	{
		return _Func(_CardMComp);
	}

	return false;
}
