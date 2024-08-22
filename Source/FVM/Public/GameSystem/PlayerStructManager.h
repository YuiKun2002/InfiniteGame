#pragma once

// #include "GameSystem/PlayerStructManager.h"

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameFramework/SaveGame.h"
#include "Data/ShopItemPriceStruct.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameSystem/Item/Task/TaskStructBase.h"
#include "GameSystem/Item/PlayerStructBase.h"
#include "GameSystem/Item/PlayerRecord.h"
#include "Data/CardData/CardDataStruct.h"
#include "Data/EquipmentDataSturct.h"
#include "Data/MaterialDataStruct.h"
//数据表数据-技能书(由GameInstance自动检测更新)
#include "Data/CardSkillBookStruct.h"

#include "PlayerStructManager.generated.h"

//货币请求名称
#define PLAYER_NET_COIN_NAME FName(TEXT("Coin"))
//角色背包请求名称
#define PLAYER_NET_PLAYERBAG_NAME FName(TEXT("PlayerBag"))
//签到请求
#define PLAYER_NET_SIGNIN_NAME FName(TEXT("Signin"))
//商城请求名称
#define PLAYER_NET_SHOP_NAME FName(TEXT("Shop"))
//配方的请求名称
#define PLAYER_NET_MATERIALBLUEPRINT_NAME FName(TEXT("MaterialBlueprint"))



 /**
 *角色更新接口
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UPlayerUpdateClassInterface : public UInterface
{
	GENERATED_BODY()
};

class FVM_API IPlayerUpdateClassInterface
{
	GENERATED_BODY()
public:
	//更新接口
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Update(class UPlayerStructManager* PlayerDataIns);
	virtual void Update_Implementation(class UPlayerStructManager* PlayerDataIns);
};

//角色更新类
UCLASS(BlueprintType, Blueprintable)
class FVM_API UPlayerUpdateClass : public UObject, public IPlayerUpdateClassInterface
{
	GENERATED_BODY()
public:
	//更新接口
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Update(class UPlayerStructManager* PlayerDataIns);
	virtual void Update_Implementation(class UPlayerStructManager* PlayerDataIns) override;
};

//角色更新类数据表
USTRUCT(BlueprintType)
struct FPlayerUpdateClassTableRowBase : public FTableRowBase {
	GENERATED_USTRUCT_BODY()
public:
	//更新的类条目
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, TSoftClassPtr<UPlayerUpdateClass>> UpdateClass;
};

/**
 *  角色存储结构
 */
UCLASS()
class FVM_API UPlayerStructManager : public USaveGame
{
	GENERATED_BODY()
	//-----------------------------------------------角色主要处理-----------------------------------------------
public:
	// 0是女生  1是男生
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 M_PlayerSex;
	//角色临时ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_PlayerTempID = FString(TEXT("00000000001"));
	//角色账户
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_PlayerAccount = FString(TEXT("admin"));
	//角色名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString M_PlayerName = FString(TEXT("面包干&芙蓉"));
	//角色等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayerEx M_FPlayerEx;
	//角色货币
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayerCoin M_FPlayerCoin;
	//角色VIP
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayerVIP M_FPlayerVIP;
	//背包界面中的背包容量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayerBagGirdGroup M_PlayerBagGirdGroup;
	//角色着装
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayerSuit M_FPlayerSuit;
	//关卡开启[0新开启，1访问过]
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, int32> M_NewMapState;
	//关卡评级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, FPlayerLevelGrade> M_PlayerLevelScore;
	//道具分解兑换抽奖信息存储
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, FPlayerExchangeAndSplit> M_FPlayerExchangeAndSplitInfor;
public:
	/*-----------------------地图进入记录------------------------*/
	//玩家当前进入的地图
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayerInMap M_PlayerInMap;
	/*-----------------------地图进入记录------------------------*/

	/*-----------------------剧情------------------------*/

	//触发了剧情
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShowDialogue = false;
	//简章剧情
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bJoinDialogue = false;
	//简章数据表
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class UDialogueDataAsset> JoinDialogueDataAsset;
	//简章信息
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDialogueShowInfor JoinDialogueInfor;

	//当前游戏进行的剧情ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayerCurrentDialogueID = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDialogueShowInfor CurrentDialogueInfor;

	//上一次游戏进行的剧情ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayerLastDialogueID = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDialogueShowInfor LastDialogueInfor;

	//已经观看的【间章】剧情ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSet<int32> FinishOtherDialogueID;
	//完成的剧情标题
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, FString> PlayerFinishedDialogue;
	/*-----------------------剧情------------------------*/


	/*-----------------------历史累积------------------------*/

	//历史累积
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayerRecordStruct PlayerRecord;
	/*-----------------------历史累积------------------------*/

	/*-----------------------任务------------------------*/
	//已经完成任务【非每日】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTaskIndex> CurFinishTask;
	//完成未领取【非每日】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTaskIndex> CurFinishTaskNotClaimed;
	//每日任务完成【每一天都会被清理】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTaskIndex> CurFinishDayTask;
	//每日任务完成未领取【每一天都会被清理】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTaskIndex> CurFinishDayTaskNotClaimed;
	//每日任务的时间记录
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DayTaskTime = "20-6-2";
	/*-----------------------任务------------------------*/


	/*-----------------------防御卡 + 装备------------------------*/

	//背包 _防御卡
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemCard> M_PlayerItems_Card;
	//背包_装备
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEquipmentBase> M_PlayerItems_Equipment;

	/*-----------------------防御卡 + 装备------------------------*/


	/*-----------------------装备区域的ID存储库------------------------*/
	//背包_装备ID号(可以使用的)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> M_PlayerItems_Equip_ID;
	//背包_装备ID号(被占用的)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> M_PlayerItems_Equip_IDUse;
	/*----------------------------------------------------------------*/


	/*-----------------------材料 + 邮件 + 技能书------------------------*/
	//背包_材料
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMaterialBase> M_PlayerItems_Material;
	//背包邮件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMail> M_PlayerMails;
	//技能书数据(存储信息表)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSkillBookData> M_SkillBookDatas;
	/*-----------------------材料 + 邮件 + 技能书------------------------*/


	/*-----------------------装备区域的武器数据存储库------------------------*/
	//玩家武器数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPlayerWeaponBase> M_FPlayerWeaponDatas;
	/*-----------------------装备区域的武器数据存储库------------------------*/


	/*-----------------------装备区域的宝石据存储库--------------------------*/
	//玩家宝石数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FWeaponGem> M_FPlayerWeaponGemDatas;
	/*-----------------------装备区域的宝石数据存储库------------------------*/

	//版本补偿
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPlayerVersionCompensate> M_GameVersionCompensate;
public:
	//设置全局缓存子系统
	UFUNCTION(BlueprintCallable)
	void SetGameCacheSubsystem(class UGameCacheSubsystem* Subsystem);
	//设置角色临时ID
	UFUNCTION(BlueprintCallable)
	void SetPlayerID(const FString& TempID);
	//获取角色临时ID
	UFUNCTION(BlueprintPure)
	FString GetPlayerID() const;
	//设置角色昵称
	UFUNCTION(BlueprintCallable)
	void SetPlayerUserName(const FString& UserName);
	//设置角色账户
	UFUNCTION(BlueprintCallable)
	void SetPlayerUserAccount(const FString& UserAccount);
public:
	/************************************************************************/
	/*                              货币获取                                 */
	/************************************************************************/
	//获取货币值
	UFUNCTION(BlueprintPure)
	int64 GetCoinValue(int32 Type);
	//获取货币名称
	UFUNCTION(BlueprintPure)
	FString GetCoinName(int32 Type);

	/************************************************************************/
	/*                              道具获获取                               */
	/************************************************************************/


	/************************************************************************/
	/*                              背包道具获取                             */
	/************************************************************************/

	//获取玩家背包道具
	UFUNCTION(BlueprintPure)
	void GetBagItems(TArray<FItemCard>& Cards, TArray<FMaterialBase>& Materials);
	//初始化角色背包
	UFUNCTION(BlueprintCallable)
	void InitBag(const TArray<FItemCard>& Cards, const TArray<FMaterialBase>& Materials);

	/************************************************************************/
	/*                              商城道具获取                             */
	/************************************************************************/
	//获取商城的数据
	UFUNCTION(BlueprintPure)
	class UGameCache* GetShopCache();
public:
	//获取背包道具数量(获取当前背包道具现有的总数量)
	int32 GetBagNum(int32 _Index);
	//获取背包最大个数(150+背包拓展容量，并非道具总数量)
	int32 GetBagMaxCount(int32 _Index);
	//通过名称获取装备
	FEquipmentBase* GetEquipmentByName(const FString& _Name);
	//通过名称获取卡片
	FItemCard* GetCardByName(const FString& _Name);
	//通过卡片ID获取背包位置
	int32 GetCardByBagID(const FString& BagID);
	//通过卡片等级获取背包位置
	int32 GetCardByGrade(const int32& Grade);
	//通过名称获取材料
	FMaterialBase* GetMaterialByName(const FString& _Name);
	//通过材料ID获取背包位置
	int32 GetMaterialByBagID(const FString& BagID);
public:
	//获取当前玩家的等级图片路径
	UFUNCTION(BlueprintCallable)
	FString GetGradeImagePath();
	//获取最后一个新出的地图
	UFUNCTION(BlueprintCallable)
	static FString GetLastNewLevelName();
public:
	//更新【进入游戏后自动调用】
	UFUNCTION(BlueprintCallable)
	void Update();
	//增加硬币
	UFUNCTION(BlueprintCallable)
	bool AddCoin(int32 _Num, uint8 _Type = 0);
	//减少硬币
	UFUNCTION(BlueprintCallable)
	bool ReduceCoin(int32 _Num, uint8 _Type = 0);
	//发送货币
	UFUNCTION(BlueprintCallable)
	bool SendCoin(const FString& CoinName, int32 _Num);
	//设置货币
	UFUNCTION(BlueprintCallable)
	void SetCoin(const FString& CoinName, int32 _Num);
	//请求货币
	UFUNCTION(BlueprintPure)
	FPlayerCoin RequestCoin(class UVaRestRequestJSON* RequestJson);
	//检查当前资源是否是货币
	UFUNCTION(BlueprintCallable)
	bool CheckCoin(const FString& CoinName);
	//为当前角色添加经验
	UFUNCTION(BlueprintCallable)
	static void AddPlayerEx(float _ExValue);
	//检查角色等级是否满足条件
	UFUNCTION(BlueprintCallable)
	static bool CheckPlayerGrade(int32 CurGrade);
	//添加关卡评级
	UFUNCTION(BlueprintCallable)
	static void AddPlayerLevelScore(const FString& _LevelName, const FPlayerLevelGrade& _Grade);
	//添加关卡
	UFUNCTION(BlueprintCallable)
	static bool AddNewPlayerLevel(const FString& LevelName, int32 State = 0);
public:
	/*---------------------------------索引查询---------------------------------*/
	//通过名称查询卡片
	UFUNCTION()
	TMap<int32, FItemCard> FindCardByName(const FString& CardName);
	//通过名称查询装备
	UFUNCTION()
	int32 FindEquipmentByName(const FString& EquipName);
	//通过名称查询材料
	UFUNCTION()
	int32 FindMaterialByName(const FString& MateriName);
	//通过ID查询材料
	int32 FindMaterialByID(const int32& MateriID);
	/*---------------------------------索引查询---------------------------------*/

	/*---------------------------------道具操作---------------------------------*/
	UFUNCTION()
	bool UseMaterial(const int32& Index, const FString& MateriName, const int32& ItemCount, bool bSave);
	/*---------------------------------道具操作---------------------------------*/


	/*---------------------------------ID库操作开始---------------------------------*/
	//返回一个可使用的ID
	int32 GetEquipID();
	//销毁一个ID
	void DestroyEquipID(int32 _ID);
	//刷新ID库->是否扩容（如果容量大于ID库-扩容ID）
	void UpdateEquipID(int32 _BagCount);
	//使用装备->通过ID使用->将对应绑定的装备进行装备使用
	void UseEquipByID(int32 _ID);
	//取消使用装备->通过ID号对改ID对应的装备进行装备取消
	void CancelEquipByID(int32 _ID);
	//添加装备到角色背包【只会注册ID号和存储库】
	bool AddEquipmentToPlayerBag(FEquipmentBase& _WeaponData);
	//将当前装备从背包移除【只会销毁ID和存储库】
	void RemoveEquipmentFromPlayerBag(FEquipmentBase& _WeaponData);
	/*---------------------------------ID库操作结束---------------------------------*/

	/*------------------------------玩家武器数据功能开始 ->针对主武器-副武器-超级武器------------------------------*/
	//ID查询-武器库
	bool SearchPlayerWeaponDataByID(int32 _WeaponID);
	//通过ID查询武器库对应位置(传入ID查询，返回武器在库中的位置) 返回ID值 如果不等于-1 表示ID有效
	int32 SearchPlayerWeaponDataByID_A(int32 _WeaponID, int32& _WeaponIndex);
	//添加武器库-数据
	void AddPlayerWeaponData(FEquipmentBase& _WeaponData);
	/*------------------------------玩家武器数据功能结束------------------------------*/

	/*------------------------------玩家武器数据功能开始------------------------------*/
	//添加宝石库-数据
	void AddPlayerWepaonGemData(FEquipmentBase& _WeaponData);
	/*------------------------------玩家武器数据功能结束------------------------------*/


	/*------------------------------玩家特殊的装备道具查询【模板】------------------------------*/

	//查询特殊数据存储库的对应ID号
	template<class TEquipType>
	static int32 SerachEquipDataByID(int32 _ID, const TArray<TEquipType>& _Datas)
	{
		int32 LIndex = 0;
		for (const auto& Data : _Datas)
		{
			if (Data.M_ItemID == _ID)
			{

				return LIndex;
			}

			LIndex++;
		}
		return -1;
	}

	//保存数据
	static void Save(const FString& Msg);

	//移除道具->ID不会移除
	template <class ItemStruct>
	static void Remove_Item(TArray<ItemStruct>& _Arrays)
	{
		//获取没有被标记的道具
		TArray<ItemStruct> Type;

		for (TIndexedContainerIterator<const TArray<ItemStruct>, const ItemStruct, int32> PP = _Arrays.CreateConstIterator(); PP; PP++)
		{
			if (!(*PP).bWaitRemove)
			{
				Type.Emplace((*PP));
			}
		}

		_Arrays.Empty();

		//复制新的卡片数据
		_Arrays = Type;
	}
private:
	//设置全局缓存子系统
	UPROPERTY()
	class UGameCacheSubsystem* GameCacheSubsystem = nullptr;
};

//货币计算
USTRUCT(BlueprintType)
struct FPlayerCoinAdd
{
	GENERATED_USTRUCT_BODY()
public:
	static bool _Buy(int64& _Coin, const int64& _Num)
	{
		if (_Coin < 0)
			_Coin = 0;

		if (_Coin < _Num)
			return false;

		_Coin -= _Num;
		return true;
	}
	static bool _Sell(int64& _Coin, const int64& _Num)
	{
		if (_Coin < 0)
			_Coin = 0;

		_Coin += _Num;

		if (_Coin > 1000000000)
			_Coin = 1000000000;

		return true;
	}
public:
	//获取货币名称
	static TArray<FItemBase> GetCoinNames()
	{
		return {
		FItemBase(FText::FromString(TEXT("金币"))),
		FItemBase(FText::FromString(TEXT("礼券"))),
		FItemBase(FText::FromString(TEXT("点券"))),
		FItemBase(FText::FromString(TEXT("威望")))
		};
	}
public:
	FPlayerCoinAdd() {}

	static bool Buy(UPlayerStructManager* _Player, const int64& _Num, const uint8& _CoinGrade) {
		if (!_Player)
			return false;

		return _Buy(_Player->M_FPlayerCoin.GetCoinRef(_CoinGrade), _Num);
	}

	static bool Sell(UPlayerStructManager* _Player, const int64& _Num, const uint8& _CoinGrade) {
		if (!_Player)
			return false;

		return _Sell(_Player->M_FPlayerCoin.GetCoinRef(_CoinGrade), _Num);
	}
};
