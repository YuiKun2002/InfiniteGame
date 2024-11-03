// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Data/Buff/MouseGameBuff.h"
#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/GameConfigManager.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"

UBuffObject* UMouseGameBuff::GetNewBuffObject(
	EGameBuffTag NewTag,
	float NewBuffTime,
	UBuffDynamicProperty* Property
)
{
	UBuffObject* CurNewBuff = nullptr;
	switch (NewTag)
	{
	case EGameBuffTag::Freeze: CurNewBuff = NewObject<UFreezeBuffMouse>(); break;
	case EGameBuffTag::Burn:CurNewBuff = NewObject<UBurnBuffMouse>(); break;
	case EGameBuffTag::SlowDown:CurNewBuff = NewObject<USlowDownBuffMouse>(); break;
	case EGameBuffTag::Solidification:CurNewBuff = NewObject<USolidificationBuffMouse>(); break;
	case EGameBuffTag::Accelerate:CurNewBuff = NewObject<UAccelerateBuffMouse>(); break;
	case EGameBuffTag::Poisoning:CurNewBuff = NewObject<UPoisoningBuffMouse>(); break;
	default: CurNewBuff = NewObject<UBuffObject>(); break;
	}

	//初始化数据
	CurNewBuff->CurTag = NewTag;
	CurNewBuff->CurTime = NewBuffTime;
	CurNewBuff->CurBuffObject = this;

	//如果有属性
	if (IsValid(Property))
	{
		CurNewBuff->DynamicProperty = Property;
	}
	else {
		CurNewBuff->DynamicProperty = NewObject<UBuffDynamicProperty>();
	}

	return CurNewBuff;
}

AMouseActor* UBuffMouseObject::GetBuffChar()
{
	return Cast<AMouseActor>(this->GetGameBuff()->GetBuffChar());
}

void UBuffMouseObject::UpdateMaterial()
{
	UGameBuff* Cur = this->GetGameBuff();

	//如果遇到凝固
	if (Cur->GetBuffExistByTag(EGameBuffTag::Solidification))
	{
		/*this->GetBuffChar()->SetMaterial(
			LoadObject<UMaterialInstance>(this,
				TEXT("MaterialInstanceConstant'/Game/Resource/BP/Data/Fx/Solidification/MI_SolidificationMaterial_Inst.MI_SolidificationMaterial_Inst'")
			));*/

		this->GetBuffChar()->SetSpineRenderColor(FLinearColor(
			1.f, 0.6f, 0.f, this->GetBuffChar()->GetColorOpacity())
		);

		if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
		{
			if (UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_GameEF.M_bEnable)
			{
				//播放粒子特效
				AObjectPoolClass* CurObj = Cast<AObjectPoolClass>(
					this->GetBuffChar()->GetMouseManager()->GetSolidifBuffFxObjPoolManager()->GetObjectActor()
				);
				CurObj->SetActorLocation(this->GetBuffChar()->GetActorLocation() + FVector(0.f, 0.f, 40.f));
			}
		}
		return;
	}

	//如果遇到冻结
	if (Cur->GetBuffExistByTag(EGameBuffTag::Freeze))
	{
		/*this->GetBuffChar()->SetMaterial(
			LoadObject<UMaterialInstance>(this,
				TEXT("MaterialInstanceConstant'/Game/Resource/BP/Data/Fx/Ice/MI_IceMaterial_Inst.MI_IceMaterial_Inst'")
			));*/

		this->GetBuffChar()->SetSpineRenderColor(FLinearColor(
			0.f, 0.5f, 1.f, this->GetBuffChar()->GetColorOpacity())
		);

		return;
	}

	//遇到减速
	if (Cur->GetBuffExistByTag(EGameBuffTag::SlowDown))
	{
		/*this->GetBuffChar()->SetMaterial(
			LoadObject<UMaterialInstance>(this,
				TEXT("MaterialInstanceConstant'/Game/Resource/BP/Data/Fx/Ice/MI_SlowDownMaterial_Inst.MI_SlowDownMaterial_Inst'")
			));*/
		this->GetBuffChar()->SetSpineRenderColor(FLinearColor(
			0.f, 1.f, 1.f, this->GetBuffChar()->GetColorOpacity())
		);
		return;
	}

	//其他buff
	/*this->GetBuffChar()->SetMaterial(
		LoadObject<UMaterialInstance>(this,
			TEXT("MaterialInstanceConstant'/Game/Resource/BP/Martials/Mouse/MI_MouseRender.MI_MouseRender'")
		));*/

		//灼烧
	if (Cur->GetBuffExistByTag(EGameBuffTag::Burn))
	{
		this->GetBuffChar()->SetSpineRenderColor(FLinearColor(
			1.f, 0.2f, 0.f, this->GetBuffChar()->GetColorOpacity())
		);

		if (IsValid(UFVMGameInstance::GetFVMGameInstance()))
		{
			if (UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_GameEF.M_bEnable)
			{
				//播放粒子特效
				AObjectPoolClass* CurObj = Cast<AObjectPoolClass>(
					this->GetBuffChar()->GetMouseManager()->GetBurnBuffFxObjPoolManager()->GetObjectActor()
				);
				CurObj->SetActorLocation(this->GetBuffChar()->GetActorLocation() + FVector(0.f, 0.f, 40.f));
			}
		}

		return;
	}

	//中毒
	if (Cur->GetBuffExistByTag(EGameBuffTag::Poisoning))
	{
		this->GetBuffChar()->SetSpineRenderColor(FLinearColor(
			0.f, 1.f, 0.f, this->GetBuffChar()->GetColorOpacity())
		);

		return;
	}

	//加速
	if (Cur->GetBuffExistByTag(EGameBuffTag::Accelerate))
	{
		this->GetBuffChar()->SetSpineRenderColor(FLinearColor(
			1.f, 0.f, 0.f, this->GetBuffChar()->GetColorOpacity())
		);

		return;
	}

	//使用默认颜色
	this->GetBuffChar()->SetSpineRenderColor(FLinearColor(
		1.f, 1.f, 1.f, this->GetBuffChar()->GetColorOpacity())
	);
}

void UBuffMouseObject::UpdateTickRate()
{
	UGameBuff* Cur = this->GetGameBuff();

	//如果有控制buff，优先执行
	if (Cur->GetBuffExistByTag(EGameBuffTag::Solidification) || Cur->GetBuffExistByTag(EGameBuffTag::Freeze))
	{
		this->SetTickRate(0.f);
		this->GetBuffChar()->SetAnimationTimeScale(0, 0.f);
		return;
	}

	//无控制Buff
	this->SetTickRate(1.f);

	//如果存在减速Buff，减去速度
	if (Cur->GetBuffExistByTag(EGameBuffTag::SlowDown))
	{
		this->SetTickRate(Cur->GetTickRate() - 0.5f);
	}

	//如果存在加速Buff，加上速度
	if (Cur->GetBuffExistByTag(EGameBuffTag::Accelerate))
	{
		this->SetTickRate(Cur->GetTickRate() + 0.5f);
	}

	//设置最终移动速度
	this->GetBuffChar()->SetAnimationTimeScale(0, Cur->GetTickRate());
}

void UBuffMouseObject::BuffEnd()
{
	this->UpdateTickRate();
	this->UpdateMaterial();
}

void USeckillBuffMouse::BuffInit(float BuffTime)
{
	if (!IsValid(this->GetBuffChar()))
	{
		return;
	}

	if (this->GetBuffChar()->GetMouseIsDeath())
	{
		return;
	}

	if (this->GetBuffChar()->GetMouseBaseType() == EMouseBaseType::Boss)
	{
		this->SetForceCurTime(0.f);
		return;
	}

	if (this->GetBuffChar()->BeHit(this, this->GetBuffChar()->GetTotalHP(), EFlyItemAttackType::Def))
	{
		this->GetBuffChar()->SetbIsHurt(true);
	}
}

void UFreezeBuffMouse::BuffInit(float BuffTime)
{
	Super::BuffInit(BuffTime);

	//是否触发
	bool bTrigger = true;

	//检测是否存在buff
	if (this->GetGameBuff()->GetBuffExist())
	{
		//与灼烧触发反应
		if (UBurnBuffMouse* BurnBuff = Cast<UBurnBuffMouse>(this->GetBuffByTag(EGameBuffTag::Burn)))
		{
			//当灼烧大于冻结时间
			if (BurnBuff->GetCurTime() >= this->GetCurTime())
			{
				this->SetBuffCurTime(EGameBuffTag::Burn, (BurnBuff->GetCurTime() - this->GetCurTime()));
				this->SetBuffCurTime(EGameBuffTag::Freeze, 0);
				bTrigger = false;
			}
			else {
				this->SetBuffCurTime(EGameBuffTag::Freeze, (this->GetCurTime() - BurnBuff->GetCurTime()));
				this->SetBuffCurTime(EGameBuffTag::Burn, 0);
			}
		}
	}

	//触发效果
	if (bTrigger)
	{
		this->UpdateTickRate();
		this->UpdateMaterial();
	}
}

void UFreezeBuffMouse::BuffEnd()
{
	//触发减速buff
	this->GetGameBuff()->AddBuff(EGameBuffTag::SlowDown, 10.f, nullptr);

	Super::BuffEnd();
}

bool UFreezeBuffMouse::GetConstbuff()
{
	return true;
}

void UBurnBuffMouse::BuffInit(float BuffTime)
{
	Super::BuffInit(BuffTime);

	this->bTrigger = true;

	//检测是否存在buff
	if (this->GetGameBuff()->GetBuffExist())
	{
		//与冻结触发反应
		if (UFreezeBuffMouse* LocalBuffF = Cast<UFreezeBuffMouse>(this->GetBuffByTag(EGameBuffTag::Freeze)))
		{
			//当冻结大于灼烧时间
			if (LocalBuffF->GetCurTime() >= this->GetCurTime())
			{
				this->SetBuffCurTime(EGameBuffTag::Freeze, (LocalBuffF->GetCurTime() - this->GetCurTime()));
				this->SetBuffCurTime(EGameBuffTag::Burn, 0);
				bTrigger = false;
			}
			else {
				this->SetBuffCurTime(EGameBuffTag::Burn, (this->GetCurTime() - LocalBuffF->GetCurTime()));
				this->SetBuffCurTime(EGameBuffTag::Freeze, 0);
			}
		}
		else if (USlowDownBuffMouse* LocalBuffS = Cast<USlowDownBuffMouse>(this->GetBuffByTag(EGameBuffTag::SlowDown)))
		{
			//当减速大于灼烧时间
			if (LocalBuffS->GetCurTime() >= this->GetCurTime())
			{
				this->SetBuffCurTime(EGameBuffTag::SlowDown, (LocalBuffS->GetCurTime() - this->GetCurTime()));
				this->SetBuffCurTime(EGameBuffTag::Burn, 0);
				bTrigger = false;
			}
			else {
				this->SetBuffCurTime(EGameBuffTag::Burn, (this->GetCurTime() - LocalBuffS->GetCurTime()));
				this->SetBuffCurTime(EGameBuffTag::SlowDown, 0);
			}
		}
	}

	//触发效果
	if (bTrigger)
	{
		//更新
		this->UpdateTickRate();
		this->UpdateMaterial();
	}
}

void UBurnBuffMouse::BuffUpdate()
{
	if (bTrigger)
	{
		//初始化持续灼烧时间
		this->GetDynamicProperty()->GetFloatProperty(TEXT("CurBuffTime"), this->CurBuffTime);

		//初始化触发灼烧时间
		this->GetDynamicProperty()->GetFloatProperty(TEXT("BuffDelay"), this->BuffDelay);

		//初始化持续伤害
		UObject* DefObj = nullptr;
		this->GetDynamicProperty()->GetDefObject(DefObj);
		if (AFlyItemActor* FlyItem = Cast<AFlyItemActor>(DefObj))
		{
			//初始化伤害倍率
			float TempRate = 0.1f;
			this->GetDynamicProperty()->GetFloatProperty(TEXT("ATKRate"), TempRate);
			this->ATK = FlyItem->GetATK() * TempRate;
		}

		//防止刷新时，刷新间隔导致无法触发
		if (!this->bEnable)
		{
			this->BuffDelayTime = this->BuffDelay;

			if (this->ATK > 0.f)
			{
				this->bEnable = true;
			}
		}
	}
}

void UBurnBuffMouse::Tick(float BuffTime)
{
	if (this->bEnable)
	{
		if (this->CurBuffTime > 0.f)
		{
			this->CurBuffTime -= BuffTime;
			this->BuffDelayTime -= BuffTime;

			if (this->BuffDelayTime <= 0.f) {

				this->BuffDelayTime = this->BuffDelay;

				//触发伤害
				if (IsValid(this->GetBuffChar()) && !this->GetBuffChar()->GetMouseIsDeath())
				{
					if (this->GetBuffChar()->BeHit(this, this->ATK, EFlyItemAttackType::Def))
					{
						this->GetBuffChar()->SetbIsHurt(true);

						UE_LOG(LogTemp, Error, TEXT(""));

						UGameSystemFunction::FVMLog(__FUNCTION__,
							TEXT("灼烧buff：【") +
							UGameSystemFunction::GetObjectName(this->GetBuffChar()) +
							TEXT("】灼烧伤害：") +
							FString::SanitizeFloat(this->ATK)
						);
					}
				}
			}
		}
		else {
			this->bEnable = false;
		}
	}
}

void USlowDownBuffMouse::BuffInit(float BuffTime)
{
	Super::BuffInit(BuffTime);

	//是否触发
	bool bTrigger = true;

	//检测是否存在buff
	if (this->GetGameBuff()->GetBuffExist())
	{
		//与灼烧触发反应
		if (UBurnBuffMouse* BurnBuff = Cast<UBurnBuffMouse>(this->GetBuffByTag(EGameBuffTag::Burn)))
		{
			//当灼烧大于冻结时间
			if (BurnBuff->GetCurTime() >= this->GetCurTime())
			{
				this->SetBuffCurTime(EGameBuffTag::Burn, (BurnBuff->GetCurTime() - this->GetCurTime()));
				this->SetBuffCurTime(EGameBuffTag::SlowDown, 0);
				bTrigger = false;
			}
			else {
				this->SetBuffCurTime(EGameBuffTag::SlowDown, (this->GetCurTime() - BurnBuff->GetCurTime()));
				this->SetBuffCurTime(EGameBuffTag::Burn, 0);
			}
		}
	}

	//触发效果
	if (bTrigger)
	{
		this->UpdateTickRate();
		this->UpdateMaterial();
	}
}

void USolidificationBuffMouse::BuffInit(float BuffTime)
{
	Super::BuffInit(BuffTime);

	this->UpdateTickRate();
	this->UpdateMaterial();
}

bool USolidificationBuffMouse::GetConstbuff()
{
	return true;
}

void UAccelerateBuffMouse::BuffInit(float BuffTime)
{
	Super::BuffInit(BuffTime);

	this->UpdateTickRate();
	this->UpdateMaterial();
}

bool UAccelerateBuffMouse::GetDebuff()
{
	return false;
}

void UPoisoningBuffMouse::BuffInit(float BuffTime)
{
	Super::BuffInit(BuffTime);

	//更新
	this->UpdateTickRate();
	this->UpdateMaterial();
}

void UPoisoningBuffMouse::BuffUpdate()
{
	//初始化持续中毒时间
	this->GetDynamicProperty()->GetFloatProperty(TEXT("CurBuffTime"), this->CurBuffTime);

	//初始化触发中毒时间
	this->GetDynamicProperty()->GetFloatProperty(TEXT("BuffDelay"), this->BuffDelay);

	//初始化伤害
	int32 TempATK = 0;
	if (this->GetDynamicProperty()->GetIntProperty(TEXT("ATK"), TempATK))
	{
		this->ATK = TempATK;
	}

	//如果固定伤害=0
	if (TempATK == 0)
	{
		//初始化比例伤害
		UObject* DefObj = nullptr;
		this->GetDynamicProperty()->GetDefObject(DefObj);
		if (AFlyItemActor* FlyItem = Cast<AFlyItemActor>(DefObj))
		{
			//初始化伤害倍率
			float TempRate = 0.1f;
			this->GetDynamicProperty()->GetFloatProperty(TEXT("ATKRate"), TempRate);
			this->ATK = FlyItem->GetATK() * TempRate;
		}
	}

	//防止刷新时，刷新间隔导致无法触发
	if (!this->bEnable)
	{
		this->BuffDelayTime = this->BuffDelay;

		if (this->ATK > 0.f)
		{
			this->bEnable = true;
		}
	}
}

void UPoisoningBuffMouse::Tick(float BuffTime)
{
	if (this->bEnable)
	{
		if (this->CurBuffTime > 0.f)
		{
			this->CurBuffTime -= BuffTime;
			this->BuffDelayTime -= BuffTime;

			if (this->BuffDelayTime <= 0.f) {

				this->BuffDelayTime = this->BuffDelay;

				//触发伤害
				if (IsValid(this->GetBuffChar()) && !this->GetBuffChar()->GetMouseIsDeath())
				{
					if (this->GetBuffChar()->BeHit(this, this->ATK, EFlyItemAttackType::Def))
					{
						this->GetBuffChar()->SetbIsHurt(true);

						UE_LOG(LogTemp, Error, TEXT(""));

						UGameSystemFunction::FVMLog(__FUNCTION__,
							TEXT("中毒buff：【") +
							UGameSystemFunction::GetObjectName(this->GetBuffChar()) +
							TEXT("】中毒伤害：") +
							FString::SanitizeFloat(this->ATK)
						);
					}
				}
			}
		}
		else {
			this->bEnable = false;
		}
	}
}
