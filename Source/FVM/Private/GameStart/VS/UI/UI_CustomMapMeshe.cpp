// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/VS/UI/UI_CustomMapMeshe.h"
#include "GameStart/VS/Components/VSManagerComponent.h"
#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/CustomMapMeshe.h"

void UUI_CustomMapMeshe::CreateNewCard(UCardManagerComponent* _CardMComp)
{
	this->BPCreateNewCard(_CardMComp);
}

void UUI_CustomMapMeshe::BPCreateNewCard_Implementation(UCardManagerComponent* _CardMComp) {}

ACardActor* UUI_CustomMapMeshe::CreateCard(UCardManagerComponent* _CardMComp)
{
	//获取当前选择的卡片
	if (IsValid(_CardMComp->GetCurrentSelectActor()) && IsValid(this->CurMeshe))
	{
		//获取卡片数据
		const FItemCard& CardData = _CardMComp->GetCardData(_CardMComp->M_CurrentSelectCardName);

		if (IsValid(_CardMComp->GetCurrentCoolDownManager()))
		{
			//重置冷却
			_CardMComp->GetCurrentCoolDownManager()->SetCoolDownFinish(false);
		}

		//减去费用
		AGameMapInstance::GetGameMapInstance()->M_ResourceManagerComponent->SubtractFlameNum(
			_CardMComp->GetCardFlameNum(_CardMComp->M_CurrentSelectCardName)
		);

		//网格组件
		UMesheControllComponent* Comp = AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent;
		FLine CurLine;
		if (!CurMeshe->GetLine(CurLine))
		{

			this->ShowText(TEXT("CustomMapMeshe并没有设置线路！创建卡片失败！"));
			return nullptr;
		}

		AMapMeshe* CurMap = Comp->GetMapMesh(CurLine.Row, CurLine.Col);

		FTransform Trans;
		Trans.SetLocation(Comp->GetMapMeshLocation(CurLine.Row, CurLine.Col));
		ACardActor* Card = Cast <ACardActor>(
			UGameplayStatics::BeginDeferredActorSpawnFromClass(
				_CardMComp->GetWorld(),
				UGameSystemFunction::LoadRes(
					TSoftClassPtr<ACardActor>(CardData.CardActorResource)
				),
				Trans)
			);

		//卡片无效
		if (!IsValid(Card))
		{
			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("当前卡片资源无效"));
			}

			return nullptr;
		}

		//设置线路
		Card->SetLine(CurLine);

		//设置UIMapMesh
		Card->SetUIMapMesh(CurMap->GetUIMapMeshe());

		//设置UICard
		Card->SetCardUI(_CardMComp->GetUICard(CardData.ItemName));

		//设置层
		Card->SetTranslucentSortPriority(CurMap->GetTranslucency() + CardData.M_CardLayer);

		//产生阶段结束
		UGameplayStatics::FinishSpawningActor(Card, Trans);

		_CardMComp->CancelSelect();

		return Card;
	}

	return nullptr;
}

void UUI_CustomMapMeshe::ShowText(FString NewContent)
{
	AGameMapInstance::GetVSManagerComponent_Static()->ShowText(NewContent);
}

ACustomMapMeshe* UUI_CustomMapMeshe::GetCustomMapMeshe()
{
	return this->CurMeshe;
}
