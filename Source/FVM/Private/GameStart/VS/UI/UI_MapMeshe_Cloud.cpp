// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/VS/UI/UI_MapMeshe_Cloud.h"
#include "GameStart/VS/MapMeshe_Cloud.h"

void UUI_MapMeshe_Cloud::CreateNewCard(UCardManagerComponent* _CardMComp)
{
	//获取当前选择的卡片
	if (_CardMComp->GetCurrentSelectActor())
	{
		//获取卡片数据
		FItemCard& CardData = _CardMComp->GetCardData(_CardMComp->M_CurrentSelectCardName);

		if (CardData.ItemName.ToString().Equals(TEXT("棉花糖")))
		{
			if (IsValid(_CardMComp->GetCurrentCoolDownManager()))
			{
				//重置冷却
				_CardMComp->GetCurrentCoolDownManager()->SetCoolDownFinish(false);
			}

			//减去费用
			AGameMapInstance::GetGameMapInstance()->M_ResourceManagerComponent->SubtractFlameNum(
				_CardMComp->GetCardFlameNum(_CardMComp->M_CurrentSelectCardName)
			);

			_CardMComp->CancelSelect();

			//播放音效
			UFVMGameInstance::PlayBGM_S_Static("PlayCardToGround", "ItemAudio");

			if (this->M_AMapMeshe_Cloud && !this->M_AMapMeshe_Cloud->IsPendingKillPending())
			{
				//移除云朵图片标记
				this->M_AMapMeshe_Cloud->RemoveCloudTag();
				//销毁云朵Actor对象
				this->M_AMapMeshe_Cloud->Destroy();
			}

		}
	}
}

void UUI_MapMeshe_Cloud::SetMapMesheCloud(AMapMeshe_Cloud* _Meshe)
{
	this->M_AMapMeshe_Cloud = _Meshe;
}
