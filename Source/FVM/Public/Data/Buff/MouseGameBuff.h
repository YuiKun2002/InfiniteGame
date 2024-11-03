// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "Data/GameBuff.h"
#include "MouseGameBuff.generated.h"

/**
 * 老鼠专用Buff系统
 */

class AMouseActor;

//老鼠buff对象
UCLASS()
class FVM_API UBuffMouseObject : public UBuffObject
{
	GENERATED_BODY()
public:
	UFUNCTION()
	AMouseActor* GetBuffChar();
	UFUNCTION()
	void UpdateMaterial();
	UFUNCTION()
	void UpdateTickRate();
protected:
	virtual void BuffEnd() override;
};

//秒杀buff
UCLASS()
class FVM_API USeckillBuffMouse : public UBuffMouseObject
{
	GENERATED_BODY()
protected:
	virtual void BuffInit(float BuffTime) override;
};

//中毒buff
UCLASS()
class FVM_API UPoisoningBuffMouse : public UBuffMouseObject
{
	GENERATED_BODY()
protected:
	virtual void BuffInit(float BuffTime) override;
	virtual void BuffUpdate() override;
	virtual void Tick(float BuffTime) override;
private:
	//延迟多少触发
	UPROPERTY()
	float BuffDelay = 0.2f;
	//延迟计时器
	UPROPERTY()
	float BuffDelayTime = 0.f;
	//持续时间
	UPROPERTY()
	float CurBuffTime = 0.f;
	//伤害
	UPROPERTY()
	float ATK = 0.f;
	//触发
	UPROPERTY()
	bool bEnable = false;
};

//冻结buff
UCLASS()
class FVM_API UFreezeBuffMouse : public UBuffMouseObject
{
	GENERATED_BODY()
protected:
	virtual void BuffInit(float BuffTime) override;
	virtual void BuffEnd() override;
	virtual bool GetConstbuff() override;
};

//凝固buff
UCLASS()
class FVM_API USolidificationBuffMouse : public UBuffMouseObject
{
	GENERATED_BODY()
protected:
	virtual void BuffInit(float BuffTime) override;
	virtual bool GetConstbuff() override;
};

//灼烧buff
UCLASS()
class FVM_API UBurnBuffMouse : public UBuffMouseObject
{
	GENERATED_BODY()
protected:
	virtual void BuffInit(float BuffTime) override;
	virtual void BuffUpdate() override;
	virtual void Tick(float BuffTime) override;
private:
	UPROPERTY()
	bool bTrigger = true;
	//延迟多少触发
	UPROPERTY()
	float BuffDelay = 0.2f;
	//延迟计时器
	UPROPERTY()
	float BuffDelayTime = 0.f;
	//持续时间
	UPROPERTY()
	float CurBuffTime = 0.f;
	//伤害
	UPROPERTY()
	float ATK = 0.f;
	//触发
	UPROPERTY()
	bool bEnable = false;
};

//减速buff
UCLASS()
class FVM_API USlowDownBuffMouse : public UBuffMouseObject
{
	GENERATED_BODY()
protected:
	virtual void BuffInit(float BuffTime) override;
};

//加速buff
UCLASS()
class FVM_API UAccelerateBuffMouse : public UBuffMouseObject
{
	GENERATED_BODY()
protected:
	virtual void BuffInit(float BuffTime) override;
	virtual bool GetDebuff() override;
};

UCLASS()
class FVM_API UMouseGameBuff : public UGameBuff
{
	GENERATED_BODY()
	friend class UBuffObject;

protected:
	//生成一个新的buff对象
	virtual	UBuffObject* GetNewBuffObject(EGameBuffTag NewTag, float NewBuffTime, UBuffDynamicProperty* Property) override;
};
