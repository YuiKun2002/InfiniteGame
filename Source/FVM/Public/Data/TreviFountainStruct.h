// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Classes/Engine/DataTable.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "TreviFountainStruct.generated.h"


//奖励对应的类型
UENUM(BlueprintType)
enum class ETreviFountainItemType : uint8
{
	//装备
	TT_Equipment UMETA(DisplayName = "Equipment"),
	//防御卡
	TT_Card UMETA(DisplayName = "Card"),
	//材料
	TT_Material UMETA(DisplayName = "Material")
};

//奖励对应的类型
UENUM(BlueprintType)
enum class ETreviFountainItemQualityType : uint8 {
	//普通
	TQT_Normal UMETA(DisplayName = "Normal"),
	//优秀
	TQT_Excellent UMETA(DisplayName = "Excellent"),
	//史诗
	TQT_Epic UMETA(DisplayName = "Epic")
};


//许愿池-单个奖励物品
USTRUCT(BlueprintType)
struct FTreviFountainItemStruct
{
	GENERATED_USTRUCT_BODY()
public:
	//物品名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText M_ItemName;
	//物品的头像
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath M_ItemHeadPath;
	//物品类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ETreviFountainItemType M_ItemType = ETreviFountainItemType::TT_Equipment;
	//物品品质
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ETreviFountainItemQualityType M_ItemQuality = ETreviFountainItemQualityType::TQT_Normal;
	//物品个数(决定抽到的奖励发送到背包的个数)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_ItemCount = 1;
	//物品价格(许愿池一般用不到，只要在券兑换商城时才会使用)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_TransPrice = 0;
	//随机数范围-最小值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_Random_Min = 0;
	//随机数范围-最大值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_Random_Max = 10;
};


//许愿池-单个奖励数据表数据
USTRUCT(BlueprintType)
struct FTreviFountainStructData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	//许愿池单个奖励的数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTreviFountainItemStruct M_FTreviFountainStruct;
};

//许愿池-抽奖结构
USTRUCT(BlueprintType)
struct FTreviFountainRandomCategoyStruct
{
	GENERATED_USTRUCT_BODY()
private:
	//奖励等级（普通）
	TArray<FTreviFountainItemStruct> M_Normal;
	//优秀
	TArray<FTreviFountainItemStruct> M_Excellent;
	//史诗
	TArray<FTreviFountainItemStruct> M_Epic;
private:
	bool M_bNormal = false;
	bool M_bExcellent = false;
	bool M_bEpic = false;
private:
	//源概率
	float M_fSourceEpicRate = 0.5f;
	float M_fSourceExcellentRate = 10.0f;
	//概率
	float M_fEpicRate = 0.5f;
	float M_fExcellentRate = 10.0f;
	//保底次数
	int32 M_iEpicMaxCount = 90;
	int32 M_iExcellentMaxCount = 10;
	//当前抽奖次数
	int32 CurrentCount = 0;
	//当前已抽优秀次数
	int32 CurrentExcellentCount = 0;
	//当前已抽史诗次数
	int32 CurrentEpicCount = 0;
public:
	FTreviFountainRandomCategoyStruct() {};
	//获取抽奖中普通道具的数据
	const TArray<FTreviFountainItemStruct>& GetNormalPool()
	{
		return this->M_Normal;
	}
	//获取抽奖中优秀道具的数据
	const TArray<FTreviFountainItemStruct>& GetExcellentPool()
	{
		return this->M_Excellent;
	}
	//获取抽奖中史诗道具的数据
	const TArray<FTreviFountainItemStruct>& GetEpicPool()
	{
		return this->M_Epic;
	}
public:
	//重置优秀抽奖次数
	void ResetExcellentCount()
	{
		this->CurrentExcellentCount = 0;
		this->M_fExcellentRate = this->M_fSourceExcellentRate;
	}
	//重置史诗抽奖次数
	void ResetEpicCount()
	{
		this->CurrentEpicCount = 0;
		this->M_fEpicRate = this->M_fSourceEpicRate;
	}
public:
	//更新优秀概率
	void UpdateExcellentRate()
	{
		this->M_fExcellentRate += M_fExcellentRate * 0.1f * this->CurrentExcellentCount;
	}
	//更新史诗概率
	void UpdateEpicRate()
	{
		this->M_fEpicRate += M_fEpicRate * 0.002f * this->CurrentEpicCount;
	}
	//更新抽奖次数
	void UpdateRandomCount()
	{
		this->CurrentCount++;

		this->CurrentExcellentCount++;
		this->CurrentEpicCount++;
	}
	//设置概率（通过抽奖次数）->一般用于恢复上一次的进度(消耗巨大)
	void SetRandomRateByRandomCount(int32 Count)
	{
		int32 _ExcellentCount = Count % 10;
		//恢复    优秀
		for (int32 i = 0; i < _ExcellentCount; i++)
		{
			this->M_fExcellentRate += M_fExcellentRate * 0.1f * (i + 1);
		}
		//恢复 史诗
		for (int32 i = 0; i < Count; i++)
		{
			this->M_fEpicRate += M_fEpicRate * 0.002f * (i + 1);
		}
	}
public:
	//设置抽奖次数
	inline void SetRandomCount(int32 _Count)
	{
		this->CurrentCount = _Count;
	}
public:
	//获取史诗保底
	inline bool GetEpicMax()
	{
		return 	this->CurrentEpicCount >= this->M_iEpicMaxCount ? true : false;
	}
	//获取优秀保底
	inline bool GetExcellentMax()
	{
		return 	this->CurrentExcellentCount >= this->M_iExcellentMaxCount ? true : false;
	}
	//获取史诗概率
	inline float GetEpicRate()
	{
		return this->M_fEpicRate;
	}
	//获取优秀概率
	inline float GetExcellentRate()
	{
		return this->M_fExcellentRate;
	}
	//获取抽奖次数
	inline int32 GetRandomCount()
	{
		return this->CurrentCount;
	}
	//获取优秀抽奖次数
	inline int32 GetExcellentRandomCount()
	{
		return this->CurrentExcellentCount;
	}
	//获取史诗抽奖次数
	inline int32 GetEpicRandomCount()
	{
		return this->CurrentEpicCount;
	}
public:
	//设置概率
	void SetRadom(float _EpicRate, int32 _EpicMaxCount, float _ExcellentRate, int32 _ExcellentMaxCount)
	{
		this->M_fEpicRate = _EpicRate;
		this->M_fExcellentRate = _ExcellentRate;

		this->M_fSourceEpicRate = _EpicRate;
		this->M_fSourceExcellentRate = _ExcellentRate;

		this->M_iEpicMaxCount = _EpicMaxCount;
		this->M_iExcellentMaxCount = _ExcellentMaxCount;
	}

	//获取索引
	int32 GetNum(ETreviFountainItemQualityType _ItemType)
	{
		switch (_ItemType)
		{
		case ETreviFountainItemQualityType::TQT_Normal:
			return this->M_Normal.Num();
		case ETreviFountainItemQualityType::TQT_Excellent:
			return this->M_Excellent.Num();
		case ETreviFountainItemQualityType::TQT_Epic:
			return this->M_Epic.Num();
		}
		return -1;
	}

	//获取物品
	FTreviFountainItemStruct GetData(ETreviFountainItemQualityType _ItemType, int32 _Index)
	{
		if (_Index == -1)
			UE_LOG(LogTemp, Error, TEXT("[FTreviFountainRandomCategoyStruct::GetData]当前索引有错"));

		switch (_ItemType)
		{
		case ETreviFountainItemQualityType::TQT_Normal:
		{
			if (_Index < 0 || _Index >= M_Normal.Num())
				_Index = 0;

			return this->M_Normal[_Index];
		}
		case ETreviFountainItemQualityType::TQT_Excellent:
		{
			if (_Index < 0 || _Index >= M_Excellent.Num())
				_Index = 0;

			return this->M_Excellent[_Index];
		}
		case ETreviFountainItemQualityType::TQT_Epic:
		{
			if (_Index < 0 || _Index >= M_Epic.Num())
				_Index = 0;

			return this->M_Epic[_Index];
		}
		}

		if (this->M_Normal.Num() > 0)
			return this->M_Normal[0];

		FTreviFountainItemStruct T;
		return T;
	}

	//设置分类
	void SetRandomItems(TArray<FTreviFountainItemStruct>& _Datas)
	{
		this->M_Normal.Empty();
		this->M_Excellent.Empty();
		this->M_Epic.Empty();

		this->M_bNormal = false;
		this->M_bExcellent = false;
		this->M_bEpic = false;

		for (auto& Item : _Datas)
		{
			switch (Item.M_ItemQuality)
			{
			case ETreviFountainItemQualityType::TQT_Normal:
			{
				this->M_Normal.Emplace(Item);
				this->M_bNormal = true;
			}
			break;
			case ETreviFountainItemQualityType::TQT_Excellent:
			{
				this->M_Excellent.Emplace(Item);
				this->M_bExcellent = true;
			}
			break;
			case ETreviFountainItemQualityType::TQT_Epic:
			{
				this->M_Epic.Emplace(Item);
				this->M_bEpic = true;
			}
			break;
			}
		}
	}
};



UCLASS()
class FVM_API UTreviFountainStruct : public UObject
{
	GENERATED_BODY()
public:
	//加载数据表(许愿池数据表)->数据表名称+数组
	static bool LoadTreviFountainDatas(const FString& _TreviFountainDataTableName, TArray<FTreviFountainItemStruct>& _Datas);
	//从中获取一个道具
	static FTreviFountainItemStruct GetItemFromPool(TArray<FTreviFountainItemStruct>& _Datas, const int32& _RandomNums);
	//将道具发送到角色背包
	static void SendItemToPlayerBag(FTreviFountainItemStruct _Item);
	//抽奖(获取道具->最终返回一个品质)
	static ETreviFountainItemQualityType GetRandomItems(FTreviFountainRandomCategoyStruct& _RandomStruct, int32 GetRandomCount, int32 RandomNumber, TArray<FTreviFountainItemStruct>& _ResultItems);
};
