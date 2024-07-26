// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameSystem/Item/Equipment/PlayerEquipmentStruct.h"
#include "Templates/SharedPointer.h"
#include "GameSystem/GlobalDatas.h"

//--------------------------------------------------------------------------------------------------------------
// 
//    具体部位的具体数据表位置
// 
//--------------------------------------------------------------------------------------------------------------

//对应的部位的总工厂
class PlayerEquipmentItemPro {
public:
	virtual FString GetPath(const EPlayerEquipmentType& _EPlayerEquipmentType) = 0;
};



//【翅膀】数据表工厂
class PlayerEquipmentTypeFlyItemFactory : public PlayerEquipmentItemPro
{
public:
	FString GetPath(const EPlayerEquipmentType& _EPlayerEquipmentType)
	{
		switch (_EPlayerEquipmentType)
		{
			//女
		case EPlayerEquipmentType::E_Men:
			return TEXT("DataTable'/Game/Resource/BP/Data/PlayerSuitData/Base/Men/FlyItem_PlayerSuit_Men.FlyItem_PlayerSuit_Men'");
			//男
		case EPlayerEquipmentType::E_Man:
			return TEXT("DataTable'/Game/Resource/BP/Data/PlayerSuitData/Base/Man/FlyItem_PlayerSuit_Man.FlyItem_PlayerSuit_Man'");
			//不区分
		case EPlayerEquipmentType::E_Base:
			return TEXT("DataTable'/Game/Resource/BP/Data/PlayerSuitData/Base/Ot/FlyItem/FlyItem_PlayerSuit_Base.FlyItem_PlayerSuit_Base'");
		}

		return TEXT("");
	}
};

//【头发】数据表工厂
class PlayerEquipmentTypeHairFactory : public PlayerEquipmentItemPro
{
public:
	FString GetPath(const EPlayerEquipmentType& _EPlayerEquipmentType)
	{
		switch (_EPlayerEquipmentType)
		{
			//女
		case EPlayerEquipmentType::E_Men:
			return TEXT("DataTable'/Game/Resource/BP/Data/PlayerSuitData/Base/Men/Hair_PlayerSuit_Base_Men.Hair_PlayerSuit_Base_Men'");
			//男
		case EPlayerEquipmentType::E_Man:
			return TEXT("");
			//不区分
		case EPlayerEquipmentType::E_Base:
			return TEXT("");
		}

		return TEXT("");
	}
};

//【服装-着装】数据表工厂
class PlayerEquipmentTypeBodyFactory : public PlayerEquipmentItemPro
{
public:
	FString GetPath(const EPlayerEquipmentType& _EPlayerEquipmentType)
	{
		switch (_EPlayerEquipmentType)
		{
			//女
		case EPlayerEquipmentType::E_Men:
			return TEXT("DataTable'/Game/Resource/BP/Data/PlayerSuitData/Base/Men/Body_PlayerSuit_Base_Men.Body_PlayerSuit_Base_Men'");
			//男
		case EPlayerEquipmentType::E_Man:
			return TEXT("DataTable'/Game/Resource/BP/Data/PlayerSuitData/Base/Man/Body_PlayerSuit_Base_Man.Body_PlayerSuit_Base_Man'");
			//不区分
		case EPlayerEquipmentType::E_Base:
			return TEXT("");
		}

		return TEXT("");
	}
};

//【眼睛】数据表工厂
class PlayerEquipmentTypeEyeFactory : public PlayerEquipmentItemPro
{
public:
	FString GetPath(const EPlayerEquipmentType& _EPlayerEquipmentType)
	{
		switch (_EPlayerEquipmentType)
		{
			//女
		case EPlayerEquipmentType::E_Men:
			return TEXT("DataTable'/Game/Resource/BP/Data/PlayerSuitData/Base/Men/Eye_PlayerSuit_Base_Men.Eye_PlayerSuit_Base_Men'");
			//男
		case EPlayerEquipmentType::E_Man:
			return TEXT("");
			//不区分
		case EPlayerEquipmentType::E_Base:
			return TEXT("DataTable'/Game/Resource/BP/Data/PlayerSuitData/Base/Ot/Eye/Eye_PlayerSuit_Base.Eye_PlayerSuit_Base'");
		}

		return TEXT("");
	}
};

//【脸饰品】数据表工厂
class PlayerEquipmentTypeFaceFactory : public PlayerEquipmentItemPro
{
public:
	FString GetPath(const EPlayerEquipmentType& _EPlayerEquipmentType)
	{
		switch (_EPlayerEquipmentType)
		{
			//女
		case EPlayerEquipmentType::E_Men:
			return TEXT("DataTable'/Game/Resource/BP/Data/PlayerSuitData/Base/Men/Face_PlayerSuit_Base_Men.Face_PlayerSuit_Base_Men'");
			//男
		case EPlayerEquipmentType::E_Man:
			return TEXT("");
			//不区分
		case EPlayerEquipmentType::E_Base:
			return TEXT("");
		}

		return TEXT("");
	}
};

//【眼镜】数据表工厂
class PlayerEquipmentTypeGlassesFactory : public PlayerEquipmentItemPro
{
public:
	FString GetPath(const EPlayerEquipmentType& _EPlayerEquipmentType)
	{
		switch (_EPlayerEquipmentType)
		{
			//女
		case EPlayerEquipmentType::E_Men:
			return TEXT("DataTable'/Game/Resource/BP/Data/PlayerSuitData/Base/Men/Glasses_PlayerSuit_Base_Men.Glasses_PlayerSuit_Base_Men'");
			//男
		case EPlayerEquipmentType::E_Man:
			return TEXT("");
			//不区分
		case EPlayerEquipmentType::E_Base:
			return TEXT("");
		}

		return TEXT("");
	}
};

//【帽子】数据表工厂
class PlayerEquipmentTypeCapFactory : public PlayerEquipmentItemPro
{
public:
	FString GetPath(const EPlayerEquipmentType& _EPlayerEquipmentType)
	{
		switch (_EPlayerEquipmentType)
		{
			//女
		case EPlayerEquipmentType::E_Men:
			return TEXT("DataTable'/Game/Resource/BP/Data/PlayerSuitData/Base/Men/Cap_PlayerSuit_Base_Men.Cap_PlayerSuit_Base_Men'");
			//男
		case EPlayerEquipmentType::E_Man:
			return TEXT("");
			//不区分
		case EPlayerEquipmentType::E_Base:
			return TEXT("DataTable'/Game/Resource/BP/Data/PlayerSuitData/Base/Ot/Cap/Cap_PlayerSuit_Base.Cap_PlayerSuit_Base'");
		}

		return TEXT("");
	}
};


//【套装】数据表工厂
class PlayerEquipmentTypeSuitFactory : public PlayerEquipmentItemPro
{
public:
	FString GetPath(const EPlayerEquipmentType& _EPlayerEquipmentType)
	{
		switch (_EPlayerEquipmentType)
		{
			//女
		case EPlayerEquipmentType::E_Men:
			return TEXT("DataTable'/Game/Resource/BP/Data/PlayerSuitData/Suit/PlayerSuit_Men.PlayerSuit_Men'");
			//男
		case EPlayerEquipmentType::E_Man:
			return TEXT("DataTable'/Game/Resource/BP/Data/PlayerSuitData/Suit/PlayerSuit_Man.PlayerSuit_Man'");
			//不区分
		case EPlayerEquipmentType::E_Base:
			return TEXT("");
		}

		return TEXT("");
	}
};

//--------------------------------------------------------------------------------------------------------------
// 
//    数据表路径返回
// 
//--------------------------------------------------------------------------------------------------------------

//所有部位的集合总工厂
class PlayerEquipmentTypeMainFactory
{
public:
	//获取路径
	FString GetDataTablePath(const EPlayerEquipmentSlotPosition& _EPlayerEquipmentSlotPosition, const EPlayerEquipmentType& _EPlayerEquipmentType)
	{
		switch (_EPlayerEquipmentSlotPosition)
		{
		case EPlayerEquipmentSlotPosition::E_Player_FlyItem:return PlayerEquipmentTypeFlyItemFactory().GetPath(_EPlayerEquipmentType);
		case EPlayerEquipmentSlotPosition::E_Player_Hair:return PlayerEquipmentTypeHairFactory().GetPath(_EPlayerEquipmentType);
		case EPlayerEquipmentSlotPosition::E_Player_Body:return PlayerEquipmentTypeBodyFactory().GetPath(_EPlayerEquipmentType);
		case EPlayerEquipmentSlotPosition::E_Player_Head:return TEXT("");
		case EPlayerEquipmentSlotPosition::E_Player_Eye:return PlayerEquipmentTypeEyeFactory().GetPath(_EPlayerEquipmentType);
		case EPlayerEquipmentSlotPosition::E_Player_Face:return PlayerEquipmentTypeFaceFactory().GetPath(_EPlayerEquipmentType);
		case EPlayerEquipmentSlotPosition::E_Player_Glasses:return PlayerEquipmentTypeGlassesFactory().GetPath(_EPlayerEquipmentType);
		case EPlayerEquipmentSlotPosition::E_Player_Cap:return PlayerEquipmentTypeCapFactory().GetPath(_EPlayerEquipmentType);
		case EPlayerEquipmentSlotPosition::E_Player_Suit:return PlayerEquipmentTypeSuitFactory().GetPath(_EPlayerEquipmentType);
		}

		return TEXT("");
	}
};

//--------------------------------------------------------------------------------------------------------------



bool UPlayerEquipmentStruct::SearchPlayerSuitByEquipment(FString& _ItemName, EPlayerEquipmentType _EPlayerEquipmentType, FPlayerEquipment& OutData)
{
	for (const auto& Datas : UGlobalDatas::Global_SourceEquipmentData_PlayerEquipment)
	{
		UE_LOG(LogTemp, Error, TEXT("UPlayerStructManager::SearchPlayerSuitByEquipment: 当前物品名称：%s 目标名称：%s"), *Datas.M_FEquipment.ItemName, *_ItemName);

		if (Datas.M_FEquipment.ItemName.Equals(_ItemName) && Datas.M_FEquipment.M_PlayerEquipmentType == _EPlayerEquipmentType)
		{
			OutData = Datas.M_FEquipment;
			return true;
		}
	}

	return false;
}

FString UPlayerEquipmentStruct::GetDataTablePath(const EPlayerEquipmentSlotPosition& _EPlayerEquipmentSlotPosition, const EPlayerEquipmentType& _EPlayerEquipmentType)
{
	return PlayerEquipmentTypeMainFactory().GetDataTablePath(_EPlayerEquipmentSlotPosition, _EPlayerEquipmentType);
}
