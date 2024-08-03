// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Data/TreviFountainStruct.h"

bool UTreviFountainStruct::LoadTreviFountainDatas(const FString& _TreviFountainDataTableName, TArray<FTreviFountainItemStruct>& _Datas)
{
	TArray<FTreviFountainStructData*> SourceDatas;
	UGameSystemFunction::GetDataTableRows<FTreviFountainStructData>(FString(TEXT("WidgetBlueprint'/Game/Resource/BP/Data/TreviFountainData/")) + _TreviFountainDataTableName + TEXT(".") + _TreviFountainDataTableName + TEXT("'"), SourceDatas);

	if (SourceDatas.Num() == 0)
		return false;

	for (auto& SourceData : SourceDatas)
	{
		_Datas.Emplace(SourceData->M_FTreviFountainStruct);
	}

	return true;
}

FTreviFountainItemStruct UTreviFountainStruct::GetItemFromPool(TArray<FTreviFountainItemStruct>& _Datas, const int32& _RandomNums)
{

	int32 LResult = UGameSystemFunction::GetRandomRange(0, _RandomNums);

	FTreviFountainItemStruct TargetData;

	for (auto& _Data : _Datas)
	{
		if (_Data.M_Random_Min >= LResult && LResult <= _Data.M_Random_Max)
		{
			return _Data;
		}
	}

	return TargetData;
}

void UTreviFountainStruct::SendItemToPlayerBag(FTreviFountainItemStruct _Item)
{
	//货币资源
	FString Coins[] = { FString(TEXT("金币")),FString(TEXT("礼券")),FString(TEXT("点券")),FString(TEXT("威望")) };

	//货币资源判断
	if (_Item.M_ItemType == ETreviFountainItemType::TT_Material)
	{
		for (int32 CoinIndex = 0; CoinIndex < 4; CoinIndex++)
		{
			if (_Item.M_ItemName.Equals(Coins[CoinIndex]))
			{
				if (UFVMGameInstance::GetPlayerStructManager_Static()->AddCoin(_Item.M_ItemCount, CoinIndex))
				{
					UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("发送货币到角色，发送内容：")) + Coins[CoinIndex]);
					return;
				}
				else {
					if (UFVMGameInstance::GetDebug())
					{
						UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("货币资源添加失败!!"));
					}
				}
			}
		}
	}

	switch (_Item.M_ItemType)
	{
	case ETreviFountainItemType::TT_Equipment:
	{
		if (!UGameSystemFunction::SendEquipmentToPlayerBag(_Item.M_ItemName, _Item.M_ItemCount))
		{
			if (UFVMGameInstance::GetDebug())
				UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("装备道具发送失败"));
		}
	} break;
	case ETreviFountainItemType::TT_Card:
	{
		if (!UGameSystemFunction::SendCardToPlayerBag(_Item.M_ItemName, 0))
		{
			if (UFVMGameInstance::GetDebug())
				UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("卡片发送失败"));
		}
	} break;
	case ETreviFountainItemType::TT_Material:
	{
		if (!UGameSystemFunction::SendMaterialToPlayerBag(_Item.M_ItemName, _Item.M_ItemCount))
		{
			if (UFVMGameInstance::GetDebug())
				UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("材料道具发送失败"));
		}
	}break;
	}
}

//奖池结构，想要抽几次，极限随机值是多少，返回抽奖的所有结果
ETreviFountainItemQualityType UTreviFountainStruct::GetRandomItems(FTreviFountainRandomCategoyStruct& _RandomStruct, int32 GetRandomCount, int32 RandomNumber, TArray<FTreviFountainItemStruct>& _ResultItems)
{
	ETreviFountainItemQualityType TargetQuality = ETreviFountainItemQualityType::TQT_Normal;

	for (int32 i = 0; i < GetRandomCount; i++)
	{
		//更新数据
		_RandomStruct.UpdateExcellentRate();
		_RandomStruct.UpdateEpicRate();
		//更新抽奖次数
		_RandomStruct.UpdateRandomCount();

		//获取随机数
		float Rate = UGameSystemFunction::GetRandomRangeFloat(0.000f, RandomNumber);



		//史诗（保底）
		if (_RandomStruct.GetEpicMax())
		{
			TargetQuality = ETreviFountainItemQualityType::TQT_Epic;

			_ResultItems.Emplace(_RandomStruct.GetData(ETreviFountainItemQualityType::TQT_Epic, UGameSystemFunction::GetRandomRange(0, _RandomStruct.GetNum(ETreviFountainItemQualityType::TQT_Epic))));

			//重置优秀抽奖次数
			_RandomStruct.ResetEpicCount();

			UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("-------------------------------------------------------------------------------")));
			UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("史诗保底")));
			UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("-------------------------------------------------------------------------------")));

		}
		else if (_RandomStruct.GetExcellentMax())//优秀（保底）
		{
			if (TargetQuality != ETreviFountainItemQualityType::TQT_Epic)
				TargetQuality = ETreviFountainItemQualityType::TQT_Excellent;

			_ResultItems.Emplace(_RandomStruct.GetData(ETreviFountainItemQualityType::TQT_Excellent, UGameSystemFunction::GetRandomRange(0, _RandomStruct.GetNum(ETreviFountainItemQualityType::TQT_Excellent))));

			//重置优秀抽奖次数
			_RandomStruct.ResetExcellentCount();
			UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("-------------------------------------------------------------------------------")));
			UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("优秀保底")));
			UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("-------------------------------------------------------------------------------")));
		}
		else if (Rate <= _RandomStruct.GetEpicRate()) //抽奖
		{
			TargetQuality = ETreviFountainItemQualityType::TQT_Epic;

			_ResultItems.Emplace(_RandomStruct.GetData(ETreviFountainItemQualityType::TQT_Epic, UGameSystemFunction::GetRandomRange(0, _RandomStruct.GetNum(ETreviFountainItemQualityType::TQT_Epic))));

			//重置优秀抽奖次数
			_RandomStruct.ResetEpicCount();
		}
		else if (Rate > _RandomStruct.GetEpicRate() && Rate <= _RandomStruct.GetExcellentRate() + _RandomStruct.GetEpicRate())
		{
			if (TargetQuality != ETreviFountainItemQualityType::TQT_Epic)
				TargetQuality = ETreviFountainItemQualityType::TQT_Excellent;

			_ResultItems.Emplace(_RandomStruct.GetData(ETreviFountainItemQualityType::TQT_Excellent, UGameSystemFunction::GetRandomRange(0, _RandomStruct.GetNum(ETreviFountainItemQualityType::TQT_Excellent))));

			//重置优秀抽奖次数
			_RandomStruct.ResetExcellentCount();
		}
		else {
			_ResultItems.Emplace(_RandomStruct.GetData(ETreviFountainItemQualityType::TQT_Normal, UGameSystemFunction::GetRandomRange(0, _RandomStruct.GetNum(ETreviFountainItemQualityType::TQT_Normal))));
		}
	}

	return TargetQuality;
}
