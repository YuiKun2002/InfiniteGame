// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/Interface.h"
#include "Engine/Classes/Engine/DataTable.h"
#include "GameBuff.generated.h"

/**
 * 游戏buff
 */

UENUM(BlueprintType)
enum class EGameBuffTag : uint8
{
	//加速
	Accelerate UMETA(DisplayName = "Accelerate"),
	//减速
	SlowDown UMETA(DisplayName = "SlowDown"),
	//冻结
	Freeze UMETA(DisplayName = "Freeze"),
	//灼烧
	Burn UMETA(DisplayName = "Burn"),
	//凝固
	Solidification UMETA(DisplayName = "Solidification")
};

//对象buff，针对谁的buff
UENUM(BlueprintType)
enum class EGameBuffCharTag : uint8
{
	//防御卡
	Card UMETA(DisplayName = "Card"),
	//老鼠
	Mouse UMETA(DisplayName = "Mouse")
};

//buff的信息结构
USTRUCT(BlueprintType)
struct FGameBuffInfor {
	GENERATED_USTRUCT_BODY()
public:
	//当前所有的buff
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<EGameBuffTag, float> CurBuffs;
};

UINTERFACE(MinimalAPI)
class UGameBuffInterface : public UInterface
{
	GENERATED_BODY()
};

class FVM_API IGameBuffInterface
{
	GENERATED_BODY()
protected:
	//初始化
	virtual void BuffInit(float BuffTime) = 0;
	//buff结束
	virtual void BuffEnd() = 0;
	//是否是负面状态的buff
	virtual bool GetDebuff() = 0;
	//是否是限制类型的buff[冻结，凝固]
	virtual bool GetConstbuff() = 0;
};

//buff基本类型
UCLASS()
class FVM_API UBuffObject : public UObject, public IGameBuffInterface
{
	GENERATED_BODY()

		friend class UGameBuff;
	friend class UMouseGameBuff;

protected:
	virtual void BuffInit(float BuffTime) override;
	virtual void BuffEnd() override;
	virtual bool GetDebuff() override;
	virtual bool GetConstbuff() override;
protected:
	//获取游戏buff对象
	class UGameBuff* GetGameBuff();
	//获取buff
	UBuffObject* GetBuffByTag(EGameBuffTag NewTag);
	//设置更新率
	void SetTickRate(float NewRate);
	//设置buff的时间
	void SetBuffCurTime(const EGameBuffTag& NewTag, const float& NewTime);
public:
	//获取buff时间
	float GetCurTime() const;
	//获取buff标记
	const EGameBuffTag& GetCurTag() const;
private:
	//当前持续的时间
	UPROPERTY()
		float CurTime = 0.f;
	//当前buff的标签
	UPROPERTY()
		EGameBuffTag CurTag = EGameBuffTag::Burn;
	//当前buff对象
	UPROPERTY()
		class UGameBuff* CurBuffObject = nullptr;
};

//执行buff
DECLARE_DELEGATE_TwoParams(ExecuteBuffDelegate, EGameBuffTag CurTag, float& CurBuffTime);
//退出buff
DECLARE_DELEGATE_OneParam(ExitBuffDelegate, EGameBuffTag CurTag);
//Buff全部执行完毕
DECLARE_DELEGATE_OneParam(GameBuffFinishedDelegate, class UGameBuff* CurBuffObject);



//Buff管理器基类
UCLASS()
class FVM_API UGameBuff : public UObject
{
	GENERATED_BODY()

		friend class UBuffObject;

public:
	//当执行buff时
	ExecuteBuffDelegate OnExecuteBuffDelegate;
	//当退出buff时
	ExitBuffDelegate OnExitBuffDelegate;
	//当前buff全部执行完毕
	GameBuffFinishedDelegate OnBuffExecuteFinishedDelegate;
public:
	//生成buff对象
	UFUNCTION(BlueprintCallable)
		static UGameBuff* MakeGameBuff(UObject* NewBuffChar, EGameBuffCharTag NewBuffTag);
	//添加buff
	UFUNCTION(BlueprintCallable)
		void AddBuff(EGameBuffTag NewTag, float NewBuffTime);
	//添加buff集合
	UFUNCTION(BlueprintCallable)
		void AddBuffInfor(FGameBuffInfor NewBuff);
	//直接清除所有buff
	UFUNCTION(BlueprintCallable)
		void ClearBuffs();
	//直接结束所有Buff
	UFUNCTION(BlueprintCallable)
		void EndBuffs();
	//打印
	UFUNCTION(BlueprintCallable)
		void DebugLog(const FString& ActorName,EGameBuffTag NewTag, bool Tirgger);
	//立刻执行所有buff
	UFUNCTION(BlueprintCallable)
		void ExecuteBuffs();
	//更新buff
	UFUNCTION(BlueprintCallable)
		void UpdateBuff(const float& DeltaTime);
	//移除buff
	UFUNCTION(BlueprintCallable)
		void RemoveBuff(EGameBuffTag NewTag);
	//获取是否存在buff
	UFUNCTION(BlueprintCallable)
		bool GetBuffExist() const;
	//获取某个buff是否存在
	UFUNCTION(BlueprintCallable)
		bool GetBuffExistByTag(EGameBuffTag NewTag);
	//获取是否存在限制buff
	UFUNCTION(BlueprintCallable)
		bool GetConstBuff() const;
	//获取tick的更新率
	UFUNCTION(BlueprintCallable)
		float GetTickRate() const;
	//获取buff的对象
	UFUNCTION(BlueprintCallable)
		UObject* GetBuffChar();
protected:
	//生成一个新的buff对象
	virtual	UBuffObject* GetNewBuffObject(EGameBuffTag NewTag, float NewBuffTime);
	//设置buff的时间
	void SetBuffCurTime(const EGameBuffTag& CurTag, const float& NewTime);
private:
	//buff集合
	UPROPERTY()
		TMap<EGameBuffTag, UBuffObject*> CurBuffs;
	//角色buff类型
	UPROPERTY()
		EGameBuffCharTag BuffTag = EGameBuffCharTag::Mouse;
	//buff对象
	UPROPERTY()
		UObject* BuffChar = nullptr;
	//是否存在限制类型的buff
	UPROPERTY()
		bool bConstBuff = false;
	//时间更新率
	UPROPERTY()
		float UpdateTickRate = 1.f;
};
