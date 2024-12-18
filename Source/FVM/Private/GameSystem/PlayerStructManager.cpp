﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/PlayerStructManager.h"
#include "VaRestSubsystem.h"
#include "Game/UI/UI_GamePrepare.h"
#include "GameSystem/TaskSubsystem.h"
#include "GameSystem/GameDataSubsystem.h"
#include "GameSystem/GameCacheSubsystem.h"
#include "GameSystem/GameUserInterfaceSubsystem.h"
#include "GameSystem/Tools/GameSystemFunction.h"

void IPlayerUpdateClassInterface::Update_Implementation(class UPlayerStructManager* PlayerDataIns) {
	UE_LOG(LogTemp, Warning, TEXT("默认角色更新类-IPlayerUpdateClassInterface-更新"));
}
void UPlayerUpdateClass::Update_Implementation(class UPlayerStructManager* PlayerDataIns) {
	UE_LOG(LogTemp, Warning, TEXT("默认角色更新类-UPlayerUpdateClass-更新"));
}

void UPlayerStructManager::SetGameCacheSubsystem(class UGameCacheSubsystem* Subsystem, const FString& Token)
{
	Subsystem->SetToken(Token);
	this->GameCacheSubsystem = Subsystem;
}

void UPlayerStructManager::SetPlayerID(const FString& TempID)
{
	this->M_PlayerTempID = TempID;
}

FString UPlayerStructManager::GetPlayerID() const
{
	return this->M_PlayerTempID;
}

void UPlayerStructManager::SetPlayerUserName(const FString& UserName)
{
	this->M_PlayerName = UserName;
}

void UPlayerStructManager::SetPlayerUserAccount(const FString& UserAccount)
{
	this->M_PlayerAccount = UserAccount;
}

int32 UPlayerStructManager::GetBagNum(int32 _Index)
{
	switch (_Index)
	{
	case 0:return this->M_PlayerItems_Equipment.Num();
	case 1:return this->M_PlayerItems_Card.Num();
	case 2:return this->M_PlayerMails.Num();
	}
	return 0;
}

int32 UPlayerStructManager::GetBagMaxCount(int32 _Index)
{
	return this->M_PlayerBagGirdGroup.GetBagCount(_Index);
}

FEquipmentBase* UPlayerStructManager::GetEquipmentByName(const FString& _Name)
{
	return UItemBaseStruct::GetMetaByName<FEquipmentBase>(this->M_PlayerItems_Equipment, _Name);
}

FItemCard* UPlayerStructManager::GetCardByName(const FString& _Name)
{
	return UItemBaseStruct::GetMetaByName<FItemCard>(this->M_PlayerItems_Card, _Name);
}

int32 UPlayerStructManager::GetCardByBagID(const FString& BagID)
{
	int32 i = 0;
	for (const FItemCard& Data : this->M_PlayerItems_Card)
	{
		if (Data.BagID.Equals(BagID))
		{
			return i;
		}

		i++;
	}

	return -1;
}

int32 UPlayerStructManager::GetCardByGrade(const int32& Grade)
{
	int32 i = 0;
	for (const FItemCard& Data : this->M_PlayerItems_Card)
	{
		if (Data.M_CardGrade == Grade)
		{
			return i;
		}

		i++;
	}

	return -1;
}

FMaterialBase* UPlayerStructManager::GetMaterialByName(const FString& _Name)
{
	return UItemBaseStruct::GetMetaByName<FMaterialBase>(this->M_PlayerItems_Material, _Name);
}

int32 UPlayerStructManager::GetMaterialByBagID(const FString& BagID)
{
	int32 i = 0;
	for (const FMaterialBase& Data : this->M_PlayerItems_Material)
	{
		if (Data.BagID.Equals(BagID))
		{
			return i;
		}

		i++;
	}

	return -1;
}

FString UPlayerStructManager::GetGradeImagePath()
{
	FString _Grade_Path =
		TEXT("Texture2D'/Game/Resource/Texture/PlayerGrade/") +
		FString::FromInt(this->M_FPlayerEx.M_PlayerGrade) +
		TEXT(".") +
		FString::FromInt(this->M_FPlayerEx.M_PlayerGrade) +
		TEXT("'");

	return _Grade_Path;
}

bool UPlayerStructManager::AddCoin(int32 _Num, uint8 _Type)
{
	return FPlayerCoinAdd::Sell(this, _Num, _Type);
}

bool UPlayerStructManager::ReduceCoin(int32 _Num, uint8 _Type)
{
	return FPlayerCoinAdd::Buy(this, _Num, _Type);
}

bool UPlayerStructManager::SendCoin(const FString& CoinName, int32 _Num)
{
	const TArray<FItemBase>& Buffer = FPlayerCoinAdd::GetCoinNames();
	int32 i = 0;
	for (const FItemBase& Data : Buffer)
	{
		if (Data.ItemName.ToString().Equals(CoinName))
		{
			return this->AddCoin(_Num, i);
		}
		i++;
	}
	return false;
}

void UPlayerStructManager::SetCoin(const FString& CoinName, int32 _Num)
{
	if (_Num <= 0)
	{
		_Num = 0;
	}
	else if (_Num >= 1000000000)
	{
		_Num = 1000000000;
	}

	if (CoinName.Equals(TEXT("金币")) || CoinName.Equals(TEXT("0")))
	{
		if (_Num + this->M_FPlayerCoin.M_Coin_0 <= MAX_int64)
		{
			this->M_FPlayerCoin.M_Coin_0 += _Num;
		}
	}

	if (CoinName.Equals(TEXT("礼券")) || CoinName.Equals(TEXT("1")))
	{
		if (_Num + this->M_FPlayerCoin.M_Coin_1 <= MAX_int64)
		{
			this->M_FPlayerCoin.M_Coin_1 += _Num;
		}
	}

	if (CoinName.Equals(TEXT("点券")) || CoinName.Equals(TEXT("2")))
	{
		if (_Num + this->M_FPlayerCoin.M_Coin_2 <= MAX_int64)
		{
			this->M_FPlayerCoin.M_Coin_2 += _Num;
		}
	}

	this->Save(__FUNCTION__ + FString(TEXT("货币设置操作")));
}

int64 UPlayerStructManager::GetCoinValue(int32 Type)
{
	if (this->GameCacheSubsystem)
	{
		UGameCache* Cache = this->GameCacheSubsystem->GetGameCache_Im(PLAYER_NET_COIN_NAME);
		if (Cache->GetResult())
		{
			UVaRestJsonObject* Json = Cache->GetRequestJsonObject();
			UVaRestJsonValue* Value = Json->GetArrayField(TEXT("data"))[Type];
			UVaRestJsonObject* JsonObj = Value->AsObject();
			return	int64(JsonObj->GetIntegerField((TEXT("num"))));
		}
	}

	return 0;
}

FString UPlayerStructManager::GetCoinName(int32 Type)
{
	if (this->GameCacheSubsystem)
	{
		UGameCache* Cache = this->GameCacheSubsystem->GetGameCache_Im(PLAYER_NET_COIN_NAME);
		if (Cache->GetResult())
		{
			UVaRestJsonObject* Json = Cache->GetRequestJsonObject();
			UVaRestJsonValue* Value = Json->GetArrayField(TEXT("data"))[Type];
			UVaRestJsonObject* JsonObj = Value->AsObject();
			return	JsonObj->GetStringField(TEXT("name"));
		}
	}

	return TEXT("无效货币");
}

void UPlayerStructManager::GetBagItems(TArray<FItemCard>& Cards, TArray<FMaterialBase>& Materials)
{
	Cards.Empty();
	Materials.Empty();
	this->M_PlayerItems_Weapon.Empty();

	if (this->GameCacheSubsystem)
	{
		//通过缓存数据
		UGameCache* Cache = this->GameCacheSubsystem->GetGameCache_Im(PLAYER_NET_PLAYERBAG_NAME);
		if (Cache->GetResult())
		{
			//Json
			UVaRestJsonObject* Json = Cache->GetRequestJsonObject();

			//Array
			TArray<UVaRestJsonValue*> Arrays = Json->GetArrayField(TEXT("data"));
			for (UVaRestJsonValue*& JsonValue : Arrays)
			{
				UVaRestJsonObject* JsonObj = JsonValue->AsObject();
				UVaRestJsonValue* Type = JsonObj->GetField((TEXT("Type")));
				int32 TargetType = Type->AsInt32();

				if (TargetType == 1)
				{
					//卡片数据
					FItemCard CardData;
					//图片查询结果
					bool bResult = UItemBaseStruct::GetTextureResource(
						FCString::Atoi(*JsonObj->GetStringField(TEXT("itemId"))),
						CardData.ItemTexturePath
					);
					if (bResult)
					{
						//解析Json数据

						//设置名称
						CardData.ItemName = FText::FromString(JsonObj->GetStringField(TEXT("name")));
						//设置ID
						CardData.M_ItemID = FCString::Atoi(*JsonObj->GetStringField(TEXT("itemId")));
						//设置BagID
						CardData.BagID = JsonObj->GetStringField(TEXT("packageId"));
						//设置等级
						CardData.M_CardGrade = JsonObj->GetIntegerField(TEXT("itemLevel"));
						//设置卡片类型
						UVaRestJsonValue* subType = JsonObj->GetField((TEXT("subType")));
						CardData.M_ECardType = ECardType(uint8(subType->AsInt32()));

						//卡片数量
						int32 Nums = JsonObj->GetIntegerField(TEXT("itemNum"));
						for (int32 i = 0; i < Nums; i++)
						{
							//添加
							Cards.Emplace(CardData);
						}
					}
					/*bool bResult;
					UVaRestJsonValue* SubType = JsonObj->GetField((TEXT("subType")));
					switch (SubType->AsInt32())
					{
					case 2: {
						bResult = UCardBaseStruct::SearchCardFromDataTableByID(
							FCString::Atoi(*JsonObj->GetStringField(TEXT("itemId"))), CardData, true, ECardType::E_ATK
						);
					}; break;
					case 5: {
						bResult = UCardBaseStruct::SearchCardFromDataTableByID(
							FCString::Atoi(*JsonObj->GetStringField(TEXT("itemId"))), CardData, true, ECardType::E_SPAWN
						);
					}; break;
					case 6: {
						bResult = UCardBaseStruct::SearchCardFromDataTableByID(
							FCString::Atoi(*JsonObj->GetStringField(TEXT("itemId"))), CardData, true, ECardType::E_DEFENCE
						);
					}; break;
					case 9: {
						bResult = UCardBaseStruct::SearchCardFromDataTableByID(
							FCString::Atoi(*JsonObj->GetStringField(TEXT("itemId"))), CardData, true, ECardType::E_Function
						);
					}; break;
					default:
						bResult = UCardBaseStruct::SearchCardFromDataTableByID(
							FCString::Atoi(*JsonObj->GetStringField(TEXT("itemId"))), CardData, true, ECardType::E_ATK
						);
						break;
					}

					if (bResult)
					{

					}*/
				}
				else {

					/************************************************************************/
					/*                              材料                                     */
					/************************************************************************/
					switch (TargetType)
					{
					case 2://材料
					case 3://配方
					case 4://圣水
					case 5://陨石
					{
						//材料
						FMaterialBase Mater;
						//图片查询结果
						bool bResult = UItemBaseStruct::GetTextureResource(
							FCString::Atoi(*JsonObj->GetStringField(TEXT("itemId"))),
							Mater.ItemTexturePath
						);
						if (bResult)
						{
							//设置名称
							Mater.ItemName = FText::FromString(JsonObj->GetStringField(TEXT("name")));
							//设置数量
							Mater.M_Count = JsonObj->GetIntegerField(TEXT("itemNum"));
							//设置ID
							Mater.M_ItemID = FCString::Atoi(*JsonObj->GetStringField(TEXT("itemId")));
							//设置BagID
							Mater.BagID = JsonObj->GetStringField(TEXT("packageId"));
							//设置材料类型
							EMaterialType MaterialsType = EMaterialType::E_Blueprint;
							switch (TargetType)
							{
							case 2:MaterialsType = EMaterialType::E_CardSynthesisMaterial; break;
							case 4:MaterialsType = EMaterialType::E_Spices; break;
							case 5:MaterialsType = EMaterialType::E_Clover; break;
							default:
								MaterialsType = EMaterialType::E_Blueprint;
								break;
							}
							//设置材料类型
							Mater.M_MaterialType = MaterialsType;
							//新增材料
							Materials.Emplace(Mater);
						}
					}
					break;


					/************************************************************************/
					/*                              武器                                    */
					/************************************************************************/
					case 0: {
						//武器数据
						FMainWeaponData WeaponData;
						//图片查询结果
						bool bResult = UItemBaseStruct::GetTextureResource(
							FCString::Atoi(*JsonObj->GetStringField(TEXT("itemId"))),
							WeaponData.ItemTexturePath
						);
						if (bResult)
						{
							//设置名称
							WeaponData.ItemName = FText::FromString(JsonObj->GetStringField(TEXT("name")));
							//设置ID
							WeaponData.M_ItemID = FCString::Atoi(*JsonObj->GetStringField(TEXT("itemId")));
							//设置BagID
							WeaponData.BagID = JsonObj->GetStringField(TEXT("packageId"));
							//设置武器等级
							WeaponData.WeaponLevel = JsonObj->GetIntegerField(TEXT("itemLevel"));
							//设置武器类型
							WeaponData.WeaponType = EWeaponType::MainWeapon;


							int32 Count = JsonObj->GetIntegerField(TEXT("itemNum"));
							for (int32 i = 0; i < Count; i++)
							{
								//新增武器
								this->M_PlayerItems_Weapon.Emplace(WeaponData);
							}
						}
					} break;




					default:
						break;
					}
				}
			}
		}
	}
}

void UPlayerStructManager::InitBag(const TArray<FItemCard>& Cards, const TArray<FMaterialBase>& Materials)
{
	this->M_PlayerItems_Card.Empty();
	this->M_PlayerItems_Material.Empty();
	this->M_PlayerItems_Card = Cards;
	this->M_PlayerItems_Material = Materials;

	for (const FItemCard& Data : this->M_PlayerItems_Card)
	{
		UGameSystemFunction::FVMLog(__FUNCTION__,
			TEXT("解析到：") +
			Data.ItemName.ToString() +
			TEXT("】防御卡") +
			TEXT("ID：") +
			FString::FromInt(Data.M_ItemID) +
			TEXT("  BagID：") +
			Data.BagID
		);
	}

	for (const FMaterialBase& Data : this->M_PlayerItems_Material)
	{
		UGameSystemFunction::FVMLog(__FUNCTION__,
			TEXT("解析到：") +
			Data.ItemName.ToString() +
			TEXT("】材料") +
			TEXT("ID：") +
			FString::FromInt(Data.M_ItemID) +
			TEXT("  BagID：") +
			Data.BagID
		);
	}
}

class UGameCache* UPlayerStructManager::GetShopCache()
{
	if (this->GameCacheSubsystem)
	{
		return this->GameCacheSubsystem->GetGameCache_Im(PLAYER_NET_SHOP_NAME);
	}
	return nullptr;
}

FPlayerCoin UPlayerStructManager::RequestCoin(UVaRestRequestJSON* RequestJson)
{
	//请求对象有效
	if (IsValid(RequestJson))
	{
		if (RequestJson->GetResponseCode() == 0)
		{
			return FPlayerCoin();
		}

		UVaRestJsonObject* JsonObject = RequestJson->GetResponseObject();
		if (IsValid(JsonObject))
		{
			//货币结构
			FPlayerCoin PlayerCoin;
			//获取货币
			FString Coin_0 = JsonObject->GetField(TEXT("data"))->AsObject()->GetStringField(TEXT("gold"));
			//转换货币
			PlayerCoin.M_Coin_0 = FCString::Atoi64(*Coin_0);

			return PlayerCoin;
		}
	}


	return FPlayerCoin();
}

bool UPlayerStructManager::CheckCoin(const FString& CoinName)
{
	const TArray<FItemBase>& Buffer = FPlayerCoinAdd::GetCoinNames();
	for (const FItemBase& Data : Buffer)
	{
		if (Data.ItemName.ToString().Equals(CoinName))
		{
			return true;
		}
	}
	return false;
}

void UPlayerStructManager::AddPlayerEx(float _ExValue)
{
	UPlayerStructManager* PlayerData = UFVMGameInstance::GetPlayerStructManager_Static();

	//如果角色60级则不在增加经验
	if (PlayerData->M_FPlayerEx.M_PlayerGrade >= 60)
	{
		PlayerData->M_FPlayerEx.M_CurrentEx = PlayerData->M_FPlayerEx.M_CurrentExTop;
		return;
	}

	//叠加当前经验值
	PlayerData->M_FPlayerEx.M_CurrentEx += _ExValue;

	bool bRe = false;
	bool bUp = false;

	//将溢出的经验值保留，并且等级提升一级
	if (PlayerData->M_FPlayerEx.M_CurrentEx >= PlayerData->M_FPlayerEx.M_CurrentExTop)
	{
		//最高等级限制 60
		if (PlayerData->M_FPlayerEx.M_PlayerGrade >= 60)
		{
			PlayerData->M_FPlayerEx.M_CurrentEx = PlayerData->M_FPlayerEx.M_CurrentExTop;
			return;
		}

		//减去经验
		PlayerData->M_FPlayerEx.M_CurrentEx -= PlayerData->M_FPlayerEx.M_CurrentExTop;

		//提升等级
		PlayerData->M_FPlayerEx.M_PlayerGrade += 1;
		UTaskSubsystem::GetTaskSubsystemStatic()->ExecuteTasks(PlayerData);

		bUp = true;

		//下一次所需经验值提升  100 + 10 * 0.4
		PlayerData->M_FPlayerEx.M_CurrentExTop +=
			(PlayerData->M_FPlayerEx.M_CurrentExTop + PlayerData->M_FPlayerEx.M_ExUp) *
			PlayerData->M_FPlayerEx.M_ExUpRate;

		//经验成长值[最高限制]
		if (PlayerData->M_FPlayerEx.M_ExUp >= 8000)
		{
			PlayerData->M_FPlayerEx.M_ExUp = 8000;
		}
		else {
			PlayerData->M_FPlayerEx.M_ExUp += PlayerData->M_FPlayerEx.M_CurrentExTop * 0.50f;
		}

		if (PlayerData->M_FPlayerEx.M_CurrentEx >= PlayerData->M_FPlayerEx.M_CurrentExTop)
		{
			bRe = true;
		}

		int32 Cur = PlayerData->M_FPlayerEx.M_PlayerGrade;
		if (Cur == 3)
		{
			UGameSystemFunction::AddGameinstructions(
				UFVMGameInstance::GetFVMGameInstance(), TEXT("/mail 新人礼盒"),
				false);
		}
		else if (Cur == 5)
		{
			UGameSystemFunction::AddGameinstructions(
				UFVMGameInstance::GetFVMGameInstance(), TEXT("/mail 5级礼盒"),
				false);
		}
		else if (Cur == 10)
		{
			UGameSystemFunction::AddGameinstructions(
				UFVMGameInstance::GetFVMGameInstance(), TEXT("/mail 10级礼盒"),
				false);
		}
		else if (Cur == 15)
		{
			UGameSystemFunction::AddGameinstructions(
				UFVMGameInstance::GetFVMGameInstance(), TEXT("/mail 15级礼盒"),
				false);
		}
		else if (Cur == 20)
		{
			UGameSystemFunction::AddGameinstructions(
				UFVMGameInstance::GetFVMGameInstance(), TEXT("/mail 20级礼盒"),
				false);
		}
		else if (Cur == 30)
		{
			UGameSystemFunction::AddGameinstructions(
				UFVMGameInstance::GetFVMGameInstance(), TEXT("/mail 30级礼盒"),
				false);
		}
		else if (Cur == 40)
		{
			UGameSystemFunction::AddGameinstructions(
				UFVMGameInstance::GetFVMGameInstance(), TEXT("/mail 40级礼盒"),
				false);
		}

		if (PlayerData->M_FPlayerEx.M_PlayerGrade <= 15)
		{
			if (PlayerData->M_FPlayerEx.M_CurrentExTop > 1000)
			{
				PlayerData->M_FPlayerEx.M_CurrentExTop = 1000;
			}
		}
		else if (PlayerData->M_FPlayerEx.M_PlayerGrade > 15 && PlayerData->M_FPlayerEx.M_PlayerGrade <= 30)
		{
			if (PlayerData->M_FPlayerEx.M_CurrentExTop > 2500)
			{
				PlayerData->M_FPlayerEx.M_CurrentExTop = 2500;
			}
		}
		else if (PlayerData->M_FPlayerEx.M_PlayerGrade > 30 && PlayerData->M_FPlayerEx.M_PlayerGrade <= 45)
		{
			if (PlayerData->M_FPlayerEx.M_CurrentExTop > 4000)
			{
				PlayerData->M_FPlayerEx.M_CurrentExTop = 4000;
			}
		}
		else {
			if (PlayerData->M_FPlayerEx.M_CurrentExTop > 8000)
			{
				PlayerData->M_FPlayerEx.M_CurrentExTop = 8000;
			}
		}
	}

	//如果当前经验值依旧大于所需经验值-则递归
	if (bRe)
	{
		UPlayerStructManager::AddPlayerEx(0);
	}
	else {
		if (bUp)
		{
			CreateWidget<UWidgetBase>(
				UFVMGameInstance::GetFVMGameInstance(),
				LoadClass<UWidgetBase>(0
					, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/UI_Tip/BP_PlayerGradeUp.BP_PlayerGradeUp_C'"))
			)->AddToViewport(100);

			if (UGameUserInterfaceSubsystem::GetGameTaskUIInViewportSub())
			{
				UGameUserInterfaceSubsystem::RemoveGameTaskUIViewportSub();

				UGameSystemFunction::LoadLastMap();
			}
		}
	}
}

bool UPlayerStructManager::CheckPlayerGrade(int32 CurGrade)
{

	UPlayerStructManager* PlayerData = UFVMGameInstance::GetPlayerStructManager_Static();
	if (IsValid(PlayerData))
	{
		if (PlayerData->M_FPlayerEx.M_PlayerGrade >= CurGrade)
		{
			return true;
		}
	}

	return false;
}

void UPlayerStructManager::AddPlayerLevelScore(const FString& _LevelName, const FPlayerLevelGrade& _Grade)
{
	UPlayerStructManager* PlayerData = UFVMGameInstance::GetPlayerStructManager_Static();

	const FPlayerLevelGrade* CurLevel = PlayerData->M_PlayerLevelScore.Find(_LevelName);
	if (CurLevel)
	{
		FPlayerLevelGrade CurGrade = *CurLevel;
		CurGrade.Init(_Grade.Time, _Grade.Score, _Grade.BadCardCount);
		CurGrade.InitScoreLevel(_Grade.BadCardCount);
		PlayerData->M_PlayerLevelScore.Emplace(_LevelName, CurGrade);
	}
	else {
		PlayerData->M_PlayerLevelScore.Emplace(_LevelName, _Grade);
	}
}

bool UPlayerStructManager::AddNewPlayerLevel(const FString& LevelName, int32 State)
{
	UPlayerStructManager* PlayerData = UFVMGameInstance::GetPlayerStructManager_Static();

	int32* Cur = PlayerData->M_NewMapState.Find(LevelName);
	bool bState = true;

	if (Cur)
	{
		bState = false;
		if (*Cur != 0)
		{
			return bState;
		}
	}

	PlayerData->M_NewMapState.Emplace(LevelName, State);

	if (bState)
	{
		UPlayerStructManager::Save(__FUNCTION__ + FString(TEXT("添加新开启的关卡操作")));
	}

	return bState;
}

FString UPlayerStructManager::GetLastNewLevelName()
{
	UPlayerStructManager* PlayerData = UFVMGameInstance::GetPlayerStructManager_Static();

	if (PlayerData->M_NewMapState.Num())
	{
		TArray<FString> OutKeys;
		PlayerData->M_NewMapState.GetKeys(OutKeys);

		return OutKeys[OutKeys.Num() - 1];
	}

	return FString();
}

void UPlayerStructManager::Update()
{
	if (!IsValid(UGameDataSubsystem::GetGameDataSubsystemStatic()))
	{
		return;
	}

	UDataTable* Table = UGameDataSubsystem::GetGameDataSubsystemStatic()->GetAsset()->
		GetDataByName(GLOBALASSET_PLAYER)->GetDataByName(TEXT("Update"));
	DataTableAssetData<FPlayerUpdateClassTableRowBase> UpdataClassData(Table);

	//执行更新
	for (const auto& CurUpdateClass : UpdataClassData.GetDatas())
	{
		for (const auto& ClassPtr : CurUpdateClass.Value.UpdateClass)
		{
			UPlayerUpdateClass* CurNewPlayerUpdateClass = nullptr;
			UClass* CurClass = ClassPtr.Value.LoadSynchronous();
			if (IsValid(CurClass))
			{
				TSubclassOf<UPlayerUpdateClass> CurFunClass(CurClass);
				if (CurFunClass.GetDefaultObject())
				{
					CurNewPlayerUpdateClass = CurFunClass.GetDefaultObject();
					if (IsValid(CurNewPlayerUpdateClass))
					{
						IPlayerUpdateClassInterface* UpdateInterface = nullptr;
						UpdateInterface = Cast<IPlayerUpdateClassInterface>(CurNewPlayerUpdateClass);
						if (UpdateInterface)
						{
							UpdateInterface->Execute_Update(CurNewPlayerUpdateClass, this);
						}
					}
				}
			}
		}
	}

	UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("角色存档核心数据更新")));
}

TMap<int32, FItemCard> UPlayerStructManager::FindCardByName(const FString& CardName)
{
	TMap<int32, FItemCard> Temp;
	int32 CurNum = this->GetBagNum(1);
	int32 CurMax = this->GetBagMaxCount(1);
	int32 TargetNum = CurNum > CurMax ? CurMax : CurNum;

	for (int32 i = 0; i < TargetNum; i++)
	{
		if (this->M_PlayerItems_Card[i].ItemName.ToString().Equals(CardName))
		{
			Temp.Emplace(i, this->M_PlayerItems_Card[i]);
		}
	}

	return Temp;
}

int32 UPlayerStructManager::FindEquipmentByName(const FString& EquipName)
{
	int32 CurNum = this->GetBagNum(0);
	for (int32 i = 0; i < CurNum; i++)
	{
		if (this->M_PlayerItems_Equipment[i].M_ItemID != -1)
		{
			if (this->M_PlayerItems_Equipment[i].ItemName.ToString().Equals(EquipName))
			{
				return i;
			}
		}
	}
	return -1;
}

int32 UPlayerStructManager::FindMaterialByName(const FString& MateriName)
{
	for (int32 i = 0; i < this->M_PlayerItems_Material.Num(); i++)
	{
		if (this->M_PlayerItems_Material[i].ItemName.ToString().Equals(MateriName))
		{
			//如果是标记对象，直接为-1
			if (this->M_PlayerItems_Material[i].bWaitRemove)
			{
				return -1;
			}

			return i;
		}
	}
	return -1;
}

int32 UPlayerStructManager::FindMaterialByID(const int32& MateriID)
{
	for (int32 i = 0; i < this->M_PlayerItems_Material.Num(); i++)
	{
		if (this->M_PlayerItems_Material[i].M_ItemID == MateriID)
		{
			return i;
		}
	}
	return -1;
}

bool UPlayerStructManager::UseMaterial(const int32& Index, const FString& MateriName, const int32& ItemCount, bool bSave)
{
	bool Re = false;
	int32 CurIndex = Index;
	if (this->M_PlayerItems_Material[CurIndex].ItemName.ToString().Equals(MateriName))
	{
		Re = true;
	}
	else if (CurIndex == -1)
	{
		int32 i = 0;
		for (FMaterialBase& CurMateriData : this->M_PlayerItems_Material)
		{
			if (CurMateriData.ItemName.ToString().Equals(MateriName))
			{
				Re = true;
				CurIndex = i;
				break;
			}
			i++;
		}
	}

	if (Re)
	{
		this->M_PlayerItems_Material[CurIndex].M_Count -= ItemCount;
		if (this->M_PlayerItems_Material[CurIndex].M_Count <= 0)
		{
			this->M_PlayerItems_Material[CurIndex].bWaitRemove = true;
		}
	}

	if (bSave)
	{
		this->Remove_Item(this->M_PlayerItems_Material);
		UGameSystemFunction::SaveCurrentPlayerData(__FUNCTION__ + FString(TEXT("清理待移除的-材料道具")));
	}

	return Re;
}

int32 UPlayerStructManager::GetEquipID()
{
	if (this->M_PlayerItems_Equip_ID.Num() > 0)
	{
		//获取当前可以使用的ID
		int32 LocalID = this->M_PlayerItems_Equip_ID[0];

		//移出当前ID
		this->M_PlayerItems_Equip_ID.RemoveAt(0);

		//将当前ID占用
		this->M_PlayerItems_Equip_IDUse.Emplace(LocalID);

		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("ID被标记为使用，被标记的ID:") + FString::FromInt(LocalID)));
		}

		return LocalID;
	}

	return -1;
}

void UPlayerStructManager::DestroyEquipID(int32 _ID)
{
	int32 Index = 0;
	bool bResult = false;
	for (auto& Id : this->M_PlayerItems_Equip_IDUse)
	{
		if (Id == _ID)
		{
			bResult = true;
			break;
		}
		Index++;
	}

	if (bResult)
	{
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("开始销毁ID，当前已经被销毁的ID：") +
				FString::FromInt(this->M_PlayerItems_Equip_IDUse[Index])));
		}

		//将当前占用的ID 取消还原
		this->M_PlayerItems_Equip_ID.Emplace(this->M_PlayerItems_Equip_IDUse[Index]);
		//移出原来被占用的ID
		this->M_PlayerItems_Equip_IDUse.RemoveAt(Index);
	}
}

void UPlayerStructManager::UpdateEquipID(int32 _BagCount)
{
	//获取当前ID库总量
	int32 Total = this->M_PlayerItems_Equip_ID.Num() + this->M_PlayerItems_Equip_IDUse.Num();

	//需要添加多个ID库
	if (Total < _BagCount)
	{
		int32 NeedTotal = _BagCount - Total;
		while (NeedTotal > 0)
		{
			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, FString(TEXT("ID库更新，当前加入的新ID：") + FString::FromInt(this->M_PlayerItems_Equip_ID.Num() + this->M_PlayerItems_Equip_IDUse.Num())));
			}

			//添加ID
			this->M_PlayerItems_Equip_ID.Emplace(this->M_PlayerItems_Equip_ID.Num() + this->M_PlayerItems_Equip_IDUse.Num());

			NeedTotal--;
		}
	}
}

void UPlayerStructManager::UseEquipByID(int32 _ID)
{
	for (auto& Equip : this->M_PlayerItems_Equipment)
	{
		if (Equip.M_ItemID == _ID)
		{
			Equip.M_Used = true;

			break;
		}
	}
}

void UPlayerStructManager::CancelEquipByID(int32 _ID)
{
	for (auto& Equip : this->M_PlayerItems_Equipment)
	{
		if (Equip.M_ItemID == _ID)
		{
			Equip.M_Used = false;

			break;
		}
	}
}

bool UPlayerStructManager::AddEquipmentToPlayerBag(FEquipmentBase& _WeaponData)
{
	if (_WeaponData.M_ItemID != -1)
	{
		return false;
	}

	//申请ID
	int32 ID = this->GetEquipID();

	if (ID != -1)//ID申请成功
	{
		_WeaponData.M_ItemID = ID;
	}
	else
	{
		return false;//ID 申请失败直接退出
	}

	switch (_WeaponData.M_EquipmentType)
	{
		//武器
	case EEquipment::E_PlayerWeaponFirst:
	case EEquipment::E_PlayerWeaponSecond:
	case EEquipment::E_PlayerWeaponSuper: { this->AddPlayerWeaponData(_WeaponData); }break;
		//宝石
	case EEquipment::E_WeaponGem:this->AddPlayerWepaonGemData(_WeaponData); break;

	default:
		return true;
	}

	return true;
}

void UPlayerStructManager::RemoveEquipmentFromPlayerBag(FEquipmentBase& _WeaponData)
{
	_WeaponData.bWaitRemove = true;

	//不管是什么道具  -1 是永远不会出现在任何存储库里面的[可以直接删除]
	if (_WeaponData.M_ItemID == -1)
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("当前道具ID为-1：") + _WeaponData.ItemName.ToString());

		return;
	}

	//销毁道具库存档的数据
	switch (_WeaponData.M_EquipmentType)
	{
		//武器
	case EEquipment::E_PlayerWeaponFirst:
	case EEquipment::E_PlayerWeaponSecond:
	case EEquipment::E_PlayerWeaponSuper: {

		int32 LID = -1;
		if (UFVMGameInstance::GetPlayerStructManager_Static()->SearchPlayerWeaponDataByID_A(_WeaponData.M_ItemID, LID) != -1)
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("从武器库销毁一个道具：") + _WeaponData.ItemName.ToString());

			UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_FPlayerWeaponDatas[LID].bWaitRemove = true;

			UPlayerStructManager::Remove_Item<FPlayerWeaponBase>(
				UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_FPlayerWeaponDatas
			);
		}
		else {
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("从武器库销毁道具失败，不存在的道具：") + _WeaponData.ItemName.ToString());
		}

	}break;
										//宝石
	case EEquipment::E_WeaponGem: {

		//映射ID->匹配宝石库
		//查询宝石数据
		int32 LID = UPlayerStructManager::SerachEquipDataByID(
			_WeaponData.M_ItemID, UFVMGameInstance::GetPlayerStructManager_Static()->M_FPlayerWeaponGemDatas
		);
		if (LID != -1)
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("从宝石库销毁一个道具：") + _WeaponData.ItemName.ToString());

			UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_FPlayerWeaponGemDatas[LID].bWaitRemove = true;

			UPlayerStructManager::Remove_Item<FWeaponGem>(
				UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_FPlayerWeaponGemDatas
			);
		}
		else {
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("从宝石库销毁道具失败，不存在的道具：") + _WeaponData.ItemName.ToString());
		}

	} break;

	default:break;
	}

	//销毁ID号
	this->DestroyEquipID(_WeaponData.M_ItemID);

	//销毁道具
	//UGameSystemFunction::ClearWaitingItemsForEquip(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager_Static()->M_PlayerItems_Equipment);
}

bool UPlayerStructManager::SearchPlayerWeaponDataByID(int32 _WeaponID)
{
	return UPlayerStructManager::SerachEquipDataByID<FPlayerWeaponBase>(_WeaponID, this->M_FPlayerWeaponDatas) != -1 ? true : false;
}

int32 UPlayerStructManager::SearchPlayerWeaponDataByID_A(int32 _WeaponID, int32& _WeaponIndex)
{
	_WeaponIndex = SerachEquipDataByID(_WeaponID, this->M_FPlayerWeaponDatas);

	return _WeaponIndex != -1 ? _WeaponID : -1;
}

void UPlayerStructManager::AddPlayerWeaponData(FEquipmentBase& _WeaponData)
{
	//没有ID则无法入库
	if (_WeaponData.M_ItemID == -1)
		return;

	//类型添加(根据类型设置具体属性)
	FPlayerWeaponBase PlayerWeaponBaseData;
	FEquipmentBase* LData = nullptr;
	if (UEquipmentBaseStruct::SearchSourceEquipmentFromDataTable(_WeaponData.ItemName.ToString(), LData, true, _WeaponData.M_EquipmentType))
	{
		PlayerWeaponBaseData = *(FPlayerWeaponBase*)(LData);

		PlayerWeaponBaseData.M_ItemID = _WeaponData.M_ItemID;

		this->M_FPlayerWeaponDatas.Emplace(PlayerWeaponBaseData);

		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("将武器:") + PlayerWeaponBaseData.ItemName.ToString() + TEXT("添加到武器存储库中，ID：") + FString::FromInt(PlayerWeaponBaseData.M_ItemID));
		}
	}
}

void UPlayerStructManager::AddPlayerWepaonGemData(FEquipmentBase& _WeaponData)
{
	//没有ID则无法入库
	if (_WeaponData.M_ItemID == -1)
		return;

	TArray<FEquipmentBase*> Gems;
	UEquipmentBaseStruct::GetAllEquipmentsSourceData(Gems, true, { EEquipment::E_WeaponGem });
	//初始化宝石数据
	FWeaponGem LFWeaponGem;
	for (auto& SourceData : Gems)
	{
		if ((FWeaponGem*)SourceData->ItemName.EqualTo(_WeaponData.ItemName))
		{
			LFWeaponGem = *(FWeaponGem*)SourceData;
			break;
		}
	}

	//ID绑定
	LFWeaponGem.M_ItemID = _WeaponData.M_ItemID;

	//设置等级
	if (LFWeaponGem.M_GemGradeMax >= _WeaponData.M_EquipmentGrade)
	{
		LFWeaponGem.M_GemGrade = _WeaponData.M_EquipmentGrade;
	}
	else
	{
		LFWeaponGem.M_GemGrade = LFWeaponGem.M_GemGradeMax;
	}

	this->M_FPlayerWeaponGemDatas.Emplace(LFWeaponGem);

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("将道具:") +
			LFWeaponGem.ItemName.ToString() + TEXT("添加到宝石存储库中，ID：") +
			FString::FromInt(_WeaponData.M_ItemID)
		);
	}
}

void UPlayerStructManager::Save(const FString& Msg)
{
	UGameSystemFunction::SaveCurrentPlayerData(Msg);
}
