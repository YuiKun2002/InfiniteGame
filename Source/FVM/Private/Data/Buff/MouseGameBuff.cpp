// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Data/Buff/MouseGameBuff.h"
#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/GameConfigManager.h"==
#include "GameStart/Flipbook/GameActor/MouseActor.h"

UBuffObject* UMouseGameBuff::GetNewBuffObject(EGameBuffTag NewTag, float NewBuffTime)
{
	UBuffObject* CurNewBuff = nullptr;
	switch (NewTag)
	{
	case EGameBuffTag::Freeze: CurNewBuff = NewObject<UFreezeBuffMouse>(); break;
	case EGameBuffTag::Burn:CurNewBuff = NewObject<UBurnBuffMouse>(); break;
	case EGameBuffTag::SlowDown:CurNewBuff = NewObject<USlowDownBuffMouse>(); break;
	case EGameBuffTag::Solidification:CurNewBuff = NewObject<USolidificationBuffMouse>(); break;
	case EGameBuffTag::Accelerate:CurNewBuff = NewObject<UAccelerateBuffMouse>(); break;
	default: CurNewBuff = NewObject<UBuffObject>(); break;
	}

	//初始化数据
	CurNewBuff->CurTag = NewTag;
	CurNewBuff->CurBuffObject = this;
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
		this->SetTickRate(Cur->GetTickRate() - 0.35f);
	}

	//如果存在加速Buff，加上速度
	if (Cur->GetBuffExistByTag(EGameBuffTag::Accelerate))
	{
		this->SetTickRate(Cur->GetTickRate() + 0.35f);
	}

	//设置最终移动速度
	this->GetBuffChar()->SetAnimationTimeScale(0, Cur->GetTickRate());
}

void UBuffMouseObject::BuffEnd()
{
	this->UpdateTickRate();
	this->UpdateMaterial();
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
	this->GetGameBuff()->AddBuff(EGameBuffTag::SlowDown, 10.f);

	Super::BuffEnd();
}

bool UFreezeBuffMouse::GetConstbuff()
{
	return true;
}

void UBurnBuffMouse::BuffInit(float BuffTime)
{
	Super::BuffInit(BuffTime);

	//是否触发
	bool bTrigger = true;

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
		this->UpdateTickRate();
		this->UpdateMaterial();
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
