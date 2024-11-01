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
	virtual	UBuffObject* GetNewBuffObject(EGameBuffTag NewTag, float NewBuffTime,const TSubclassOf<UBuffDynamicProperty>& Property) override;
};
