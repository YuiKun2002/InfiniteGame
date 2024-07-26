// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlayerRecord.generated.h"

/**
 * 角色记录
 */
USTRUCT(BlueprintType)
struct FPlayerRecordStruct {
	GENERATED_USTRUCT_BODY()
public:
	//累计击杀的老鼠【老鼠名称，击杀次数】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, int32> KillMouseCount;
	//累计强化的卡片【等级，数量】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<int32, int32> CardUpCount;
	//累计卡片转职成功次数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 CardChangeFinishCount = 0;
	//拥有的转职等级的卡片次数【转职等级，数量】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<int32, int32> CardChangeCount;
	//累计完成的任务数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 TaskFinishCount = 0;
	//累计武器开槽数量【武器，开凿数量】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, int32> WeaponGemCount;
	//累计宝石强化等级【宝石名称，强化的最高等级】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, int32> GemsGradeCount;
	//累计宝石分解次数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32  GemsSplitCount = 0;
	//累计宝石嵌入武器的次数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32  GemSetWeaponCount = 0;
	//累计关卡通关次数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 LevelWinCount = 0;
	//累计关卡评分【关卡名称，最高评分】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, FString> LevelScore;
	//累计武器的数量【武器名称】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSet<FString> WeaponCount;
	//背包数量【背包名称】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSet<FString> BagCount;
	//累计的金币数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int64 CoinCount = 0;
	//累计的礼券数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int64 Coin2Count = 0;
public:
	//每天累计关卡通关次数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 DayLevelWinCount = 0;
	//卡片强化成功次数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<int32, FString> DayCardUpCount;
	//卡片制作次数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, int32> DayCardMakeCount;
	//最新镶嵌过的宝石
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString DayNewGemAttachName;
	//查看情报岛详细信息次数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSet<FString> DaySeeInforCount;
	//使用过的技能书
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSet<FString> DayUseCardSkillBooks;
	//每次卡片使用次数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, int32> DayCardCount;
	//每次击败老鼠次数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, int32> DayKillMouseCount;
};

UCLASS()
class FVM_API UPlayerRecord : public UObject
{
	GENERATED_BODY()
public:
	//获取玩家角色
	UFUNCTION(BlueprintCallable)
		static class UPlayerStructManager* GetPlayer();
	//重新初始化每日累计
	UFUNCTION(BlueprintCallable)
		static void InitDayRecord();
	//添加每日通关
	UFUNCTION(BlueprintCallable)
		static int32 AddDayLevelWinCount();

	//添加每次使用卡片的次数
	UFUNCTION(BlueprintCallable)
		static void AddDayCardCount(const TMap<FString, int32>& InputCards);
	UFUNCTION(BlueprintCallable)
		static void GetDayCardCount(TMap<FString, int32>& OutCards);

	//添加每次消灭老鼠的次数
	UFUNCTION(BlueprintCallable)
		static void AddDayKillMouseCount(const TMap<FString, int32>& InputMouses);
	UFUNCTION(BlueprintCallable)
		static void GetDayKillMouseCount(TMap<FString, int32>& OutMouses);

	//添加卡片技能书的使用
	UFUNCTION(BlueprintCallable)
		static void AddDayCardSkillBook(FString CardSkillName);
	//获取卡片技能书的使用
	UFUNCTION(BlueprintCallable)
		static bool GetDayCardSkillBook(FString CardSkillName);

	//添加每日强化
	UFUNCTION(BlueprintCallable)
		static void AddDayCardUpCount(int32 CurUpGrade, FString UpCardName);
	//获取每日强化
	UFUNCTION(BlueprintCallable)
		static FString GetDayCardUpCount(int32 CurUpGrade);

	//添加每日制作卡片
	UFUNCTION(BlueprintCallable)
		static void AddDayCardMakeCount(FString CardName);
	//获取每日卡片制作
	UFUNCTION(BlueprintCallable)
		static int32 GetDayCardMakeCount(FString CardName);

	//添加每日查询情报岛信息
	UFUNCTION(BlueprintCallable)
		static void AddDaySeeInforCount(FString InforName);
	//获取每日查询情报岛信息
	UFUNCTION(BlueprintCallable)
		static bool GetDaySeeInfor(FString InforName);

	//设置最新镶嵌的宝石名称
	UFUNCTION(BlueprintCallable)
		static void AddDayDayNewGemAttachName(FString Name);
	//获取当前最新镶嵌的宝石
	UFUNCTION(BlueprintCallable)
		static FString GetDayDayNewGemAttachName();
public:
	//【不会被清理】

	//添加总通关次数
	UFUNCTION(BlueprintCallable)
		static int32 AddLevelWinCount();
	//添加强化成功次数
	UFUNCTION(BlueprintCallable)
		static void AddCardUpGradeCount(int32 Grade);
	//添加宝石镶嵌次数
	UFUNCTION(BlueprintCallable)
		static void AddGemAttachCount();
	//添加宝石强化等级
	UFUNCTION(BlueprintCallable)
		static void AddGemUpGrade(FString GemName, int32 GemGrade);
	//添加宝石分解次数
	UFUNCTION(BlueprintCallable)
		static void AddGemSplitCount();
};
