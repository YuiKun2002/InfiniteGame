// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Classes/Engine/DataTable.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameLogSubsystem.generated.h"

/**
 * 聊天窗口子系统
 */

USTRUCT(BlueprintType)
struct FInstructionsDataTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString InstructionName = TEXT("");
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Des = TEXT("");
};


UCLASS()
class FVM_API UGameLogSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	//初始化
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	//取消初始化
	virtual void Deinitialize() override;
public:
	UFUNCTION(BlueprintCallable)
		void AddCurrent(const FString& Content);
	UFUNCTION(BlueprintCallable)
		void GetCurrent(TArray<FString>& Contents);
	UFUNCTION(BlueprintCallable)
		int32 GetCurrentCount();

	UFUNCTION(BlueprintCallable)
		void AddPrivate(const FString& Content);
	UFUNCTION(BlueprintCallable)
		void GetPrivate(TArray<FString>& Contents);
	UFUNCTION(BlueprintCallable)
		int32 GetPrivateCount();

	//执行指令
	UFUNCTION(BlueprintCallable)
		bool ExeInstructions(const FString& Content, bool bTip = true);
	//打印指令列表
	UFUNCTION(BlueprintCallable)
		FString GetInstructions(const FString& DataTablePath);
	//添加帮助指令
	UFUNCTION(BlueprintCallable)
		bool AddHelpInstruction(const FString& Instruction);

	//获取子系统
	UFUNCTION(BlueprintCallable)
		static UGameLogSubsystem* GetGameLogSubsystemStatic();
	//添加并且打印指令列表
	UFUNCTION(BlueprintCallable)
		static bool AddHelpInstructionAndPrint(const FString& Instruction, const FString& DataTablePath);
	//指令判断
	UFUNCTION(BlueprintCallable)
		static bool InstructionTest(const FString& Instruction, const FString& TestInstruction);
	//添加一个玩家获得装备的信息
	UFUNCTION(BlueprintCallable)
		static void AddPlayerGetEquipmentLog(const FString& ItemName, int32 Count = 1, int32 Grade = 0);
	//添加一个玩家获得卡片的信息
	UFUNCTION(BlueprintCallable)
		static void AddPlayerGetCardLog(const FString& ItemName, int32 Grade = 0);
	//添加一个玩家获得材料的信息
	UFUNCTION(BlueprintCallable)
		static void AddPlayerGetMaterialLog(const FString& ItemName, int32 Count = 0);

private:
	template<class T>
	void GenInstruction(const FString& Head, const FString& TargetHead, UObject*& OutInstruction)
	{
		if (IsValid(OutInstruction))
		{
			return;
		}

		OutInstruction = nullptr;

		if (Head.ToLower().Equals(TargetHead))
		{
			OutInstruction = NewObject<T>();
		}
	}
private:
	//当前窗口的信息内容
	UPROPERTY()
		TArray<FString> CurrentListConetnt;
	//私聊窗口的信息内容
	UPROPERTY()
		TArray<FString> PrivateListConetnt;
};

UINTERFACE(MinimalAPI)
class ULogInstructionsInterface : public UInterface
{
	GENERATED_BODY()
};

class FVM_API ILogInstructionsInterface
{
	GENERATED_BODY()
public:
	virtual	bool ReadInstruction(const TArray<FString>& Instruction) = 0;
};

//日志指令系统基础类
UCLASS()
class FVM_API ULogInstructionSystemBase : public UObject, public ILogInstructionsInterface {

	GENERATED_BODY()
public:
	virtual bool ReadInstruction(const TArray<FString>& Instruction) override;
public:
	//获取日志系统
	UFUNCTION(BlueprintCallable)
		UGameLogSubsystem* GetSystem();
	//设置指令集
	UFUNCTION(BlueprintCallable)
		void SetInstructions(const FString& InstructionsPath);
	//获取指令描述
	UFUNCTION(BlueprintCallable)
		FString GetInstructionDes(const FString& Instruction);
	//打印帮助指令
	UFUNCTION(BlueprintCallable)
		bool PrintHelpInstruction(const FString& HeadIns, const TArray<FString>& Instruction);

	//执行命令头部
	bool ExecuteInsHead(
		const TArray<FString>& Instruction,
		const FString& TargetIns,
		TFunction<bool(const TArray<FString>&)> ExeFunc
	);
private:
	//当前数据表数据
	UPROPERTY()
		TArray<FInstructionsDataTable> CurData;
};

//邮件指令【测试类型的指令】
UCLASS()
class FVM_API UMailInstructions : public UObject, public ILogInstructionsInterface {

	GENERATED_BODY()
public:
	virtual bool ReadInstruction(const TArray<FString>& Instruction) override;
};
