// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "GameStart/VS/UI/UI_MapMeshe.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "UIMesheWidthMagma.generated.h"

#define MAGMACARDNAME FString(TEXT("冰片"))

class ACardActor;

//网格更新
USTRUCT(BlueprintType)
struct FMapMeshUpdate {
	GENERATED_USTRUCT_BODY()
public:
	//是否可以更新
	UPROPERTY()
		bool M_IsbUpdate = false;
	//更新时间
	UPROPERTY()
		float M_Time = 0.f;
private:
	//待更新的卡片对象
	UPROPERTY()
		TArray<ACardActor*> M_UpdateCards;
public:
	FMapMeshUpdate() {}
	//添加卡片更新
	void AddCardUpdate(ACardActor* _Card)
	{
		if (IsValid(_Card) && _Card->GetCurrentHP() > 0.f)
		{
			if (this->M_UpdateCards.Find(_Card) == -1)
			{
				this->M_UpdateCards.Emplace(_Card);
			}
		}
	}
	//添加卡片更新
	void AddCardUpdateAt(ACardActor* _Card, int32 _Index)
	{
		if (IsValid(_Card) && _Card->GetCurrentHP() > 0.f)
		{
			if (this->M_UpdateCards.Find(_Card) == -1)
			{
				this->M_UpdateCards.EmplaceAt(_Index, _Card);
			}
			else {
				this->M_UpdateCards.RemoveAt(this->M_UpdateCards.Find(_Card));
				this->M_UpdateCards.EmplaceAt(_Index, _Card);
			}
		}
	}
	//获取卡片更新
	TArray<ACardActor*>& GetUpdateCards()
	{
		TArray<ACardActor*> CurBuffer;
		//清理无效卡片
		for (ACardActor* CardClass : this->M_UpdateCards)
		{
			if (IsValid(CardClass) && CardClass->GetCurrentHP() > 0.f)
			{
				CurBuffer.Emplace(CardClass);
			}
		}

		this->M_UpdateCards.Empty();

		this->M_UpdateCards.Append(CurBuffer);

		return this->M_UpdateCards;
	}
};


/**
 * 岩浆
 */
UCLASS()
class FVM_API UUIMesheWidthMagma : public UUI_MapMeshe
{
	GENERATED_BODY()

public:
	//初始化
	virtual bool Initialize() override;

	virtual bool CreateCard(UUI_MapMeshe* _Meshe, UCardManagerComponent* _CardMComp, UClass* _CardRes, const FString& _CardActorName, int32 _Price, bool ResetCoolDown, bool _bCheck) override;

	//当一个Actor开始与网格发生重叠
	virtual void BoxOverlapBeginByAMapMeshe(AActor* _Actor) override;

	//当一个Actor与网格发生重叠结束
	virtual void BoxOverlapEndByAMapMeshe(AActor* _Actor) override;

	//网格更新
	virtual void TickByAMapMeshe(float DeltaTime) override;

	virtual bool UpdateCardByMesheType(TMap<int32, ACardActor*>& KillCards, int32 Layer, ACardActor* CurCard) override;

	virtual void PlayFinish(ACardActor* NewCard) override;

private:
	//网格更新
	UPROPERTY()
		FMapMeshUpdate M_MapMeshUpdate;
};
