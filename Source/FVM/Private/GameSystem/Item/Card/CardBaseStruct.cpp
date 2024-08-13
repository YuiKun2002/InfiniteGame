// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameSystem/Item/Card/CardBaseStruct.h"
#include "Data/CardData/CardDataStruct.h"
#include "GameSystem/PlayerStructManager.h"

//将卡片数据结构表转换成数据组
template<class CardDataStructType, class CardType>
static void SourceCardDataStructTransform(TArray<CardDataStructType>& Datas, TArray<CardType*>& _OutCards)
{
	for (auto& LData : Datas)
	{
		_OutCards.Emplace(&LData.M_FItemCard);
	}
}

//搜索卡片得到结果
void SearchCardResult(const FString& _CardName, FItemCard& OutputData, bool _SelectCardType, ECardType _CardType, int32 _UniformGrade, bool& _LResult, const ECardType& _ConstCardType)
{
	if (!_SelectCardType || _SelectCardType && _CardType == _ConstCardType)
	{
		TArray<FItemCard*> LocalDatas;

		UCardBaseStruct::GetAllCardsByType(_ConstCardType, LocalDatas);

		//查询卡片
		for (auto _Card = LocalDatas.CreateConstIterator(); _Card; ++_Card)
		{
			if ((*_Card)->ItemName.ToString().Equals(_CardName))
			{
				OutputData = *(*_Card);
				_LResult = true;
				break;
			}
		}
	}
}

class SpawnSearchCardManager {
public:
	//卡片名称
	const FString& M_CardName;
	//卡片返回的数据
	FItemCard& M_OutputData;
	//查询结果
	bool& M_LResult;
	//是否选择卡片类型
	bool M_SelectCardType;
	//卡片具体类型
	ECardType M_CardType;
	//卡片等级
	int32 M_UniformGrade;
public:
	SpawnSearchCardManager() = delete;
	SpawnSearchCardManager(const FString& _CardName, FItemCard& _OutputData, bool& _LResult, bool _SelectCardType, ECardType _CardType, int32 _UniformGrade) :M_CardName(_CardName), M_OutputData(_OutputData), M_LResult(_LResult), M_SelectCardType(_SelectCardType), M_CardType(_CardType), M_UniformGrade(_UniformGrade) {}
public:
	void SpawnSearch(const ECardType& _ConstCardType)
	{
		SearchCardResult(this->M_CardName, this->M_OutputData, this->M_SelectCardType, this->M_CardType, this->M_UniformGrade, this->M_LResult, _ConstCardType);
	}
};

//----------------------------------------------------------------------------------------------------------

//根据类型返回数据表路径
FString UCardBaseStruct::GetCardDataTablePath(ECardType _Type)
{
	switch (_Type)
	{
	case ECardType::E_ATK:
		return "DataTable'/Game/Resource/BP/Data/CardData/CardATKData.CardATKData'";
	case ECardType::E_SPAWN:
		return "DataTable'/Game/Resource/BP/Data/CardData/CardSpawnData.CardSpawnData'";
	case ECardType::E_DEFENCE:
		return "DataTable'/Game/Resource/BP/Data/CardData/CardDefenceData.CardDefenceData'";
	case ECardType::E_Function:
		return "DataTable'/Game/Resource/BP/Data/CardData/CardFunctionData.CardFunctionData'";
	case ECardType::E_RangeATK:
		return "DataTable'/Game/Resource/BP/Data/CardData/CardRangeATKData.CardRangeATKData'";
	}
	return "";
}

//----------------------------------------------------------------------------------------------
// 
// 如果有新卡片->首先修改类型->得到全局数据->然后在此添加数据引用
// 
//----------------------------------------------------------------------------------------------
void UCardBaseStruct::GetAllCardsByType(ECardType _Type, TArray<FItemCard*>& _OutCardSourceDatas)
{
	//获取缓存数据
	UCardDataAssetCache* Cache = Cast<UCardDataAssetCache>(
		UGameDataSubsystem::GetGameDataSubsystemStatic()->GetGameDataAssetCache(GLOBALASSET_CARD)
	);

	if (!IsValid(Cache))
	{
		Cache = NewObject<UCardDataAssetCache>();
		UGameDataSubsystem::GetGameDataSubsystemStatic()->AddGameDataAssetCache(
			GLOBALASSET_CARD,
			Cache
		);
	}

	switch (_Type)
	{
	case ECardType::E_ATK:SourceCardDataStructTransform(Cache->GetCardATK(), _OutCardSourceDatas); return;
	case ECardType::E_SPAWN:SourceCardDataStructTransform(Cache->GetCardSpawn(), _OutCardSourceDatas); return;
	case ECardType::E_DEFENCE:SourceCardDataStructTransform(Cache->GetCardDefence(), _OutCardSourceDatas); return;
	case ECardType::E_Function:SourceCardDataStructTransform(Cache->GetCardFunction(), _OutCardSourceDatas); return;
	case ECardType::E_RangeATK:SourceCardDataStructTransform(Cache->GetCardRangeATK(), _OutCardSourceDatas); return;
	default:break;
	}
}
//----------------------------------------------------------------------------------------------

//返回所有卡片的名称
void UCardBaseStruct::GetAllCardName(TArray<FItemBase>& _CardName)
{
	TArray<FItemCard*> LocalAll;

	ECardType LocalType = ECardType::E_ATK;
	for (; (uint8)LocalType <= (uint8)ECardType::E_Max; LocalType = (ECardType)((uint8)LocalType + 1u))
	{
		UCardBaseStruct::GetAllCardsByType(LocalType, LocalAll);
	}

	for (auto Data : LocalAll)
	{
		//不需要转换->它会自动切割->从原属性->切割至【FItemBase】
		_CardName.Emplace((*Data));
	}
}

bool UCardBaseStruct::SearchCardFromDataTable(const FString& _CardName, FItemCard& OutputData, bool _SelectCardType, ECardType _CardType, int32 _UniformGrade)
{
	bool _Result = false;

	SpawnSearchCardManager LocalSpawnSearchCardManager(_CardName, OutputData, _Result, _SelectCardType, _CardType, _UniformGrade);

	for (uint8 LocalType = 0u; LocalType <= ((uint8)ECardType::E_Max); LocalType++)
	{
		if (!_Result)
		{
			LocalSpawnSearchCardManager.SpawnSearch(((const ECardType&)LocalType));
		}
		else
			return true;
	}

	return false;
}

void UCardBaseStruct::GetAllCardsData(TArray<FItemCard>& _CardDatas, int32 _UniformGrade)
{
	TArray<FItemCard*> LocalAll;


	for (uint8 LocalType = 0u; LocalType <= ((uint8)ECardType::E_Max); LocalType++)
	{
		UCardBaseStruct::GetAllCardsByType(((const ECardType&)LocalType), LocalAll);
	}

	for (auto Data : LocalAll)
	{
		//统一等级
		Data->M_CardGrade = _UniformGrade;
		//不需要转换->它会自动切割->从原属性->切割至【FItemCard】
		_CardDatas.Emplace((*Data));
	}
}

//创建所有的卡片到背包
void UCardBaseStruct::CreateAllCardToBag(UPlayerStructManager* _Player, int32 _UniformCardGrade)
{
	if (_Player)
	{
		TArray<FItemCard> AllCards;
		UCardBaseStruct::GetAllCardsData(AllCards, _UniformCardGrade);
		_Player->M_PlayerItems_Card.Append(AllCards);
	}
}
