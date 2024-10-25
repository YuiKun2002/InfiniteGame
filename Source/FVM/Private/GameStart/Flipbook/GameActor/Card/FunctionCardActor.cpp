// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Card/FunctionCardActor.h"
#include "GameStart/VS/Components/Card/CardFunctionComponent.h"
#include <Components/BoxComponent.h>

const FItemCardFunction& AFunctionCardActor::GetFunctionCardData()
{
	return this->ItemCardFunction;
}

UBoxComponent* AFunctionCardActor::GetOverlapBoxComponent()
{
	return this->OverlapBoxComponent;
}

void AFunctionCardActor::SetOverlapBoxComponent(UBoxComponent* Box)
{
	this->OverlapBoxComponent = Box;

	if (IsValid(this->OverlapBoxComponent))
	{
		//设置卡片碰撞类型
		this->OverlapBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		this->OverlapBoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		this->OverlapBoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	}
}

void AFunctionCardActor::SetFunctionComponent(UCardFunctionComponent* CardFuncComp)
{
	this->CardFuncComponent = CardFuncComp;
}

void AFunctionCardActor::ExecuteCardFuncClassByCardFunction(UCardFunctionComponent* CardComp)
{
	if (this->CardFunctionClassInstanceQueue.Num())
	{
		if (!this->CardFunctionClassInstanceQueue[0]->Execute(CardComp))
		{
			this->CardFunctionClassInstanceQueue.RemoveAt(0);

			if (this->CardFunctionClassInstanceQueue.Num())
			{
				//递归调用
				this->ExecuteCardFuncClassByCardFunction(CardComp);
			}
		}
	}
}

void AFunctionCardActor::ExecuteCardFuncClassByCardFunctionOnOverlapBegin(UCardFunctionComponent* CardFuncComp, AActor* OverlapObj)
{
	if (this->CardFunctionClassInstanceQueue.Num())
	{
		if (!this->CardFunctionClassInstanceQueue[0]->OnOverlapBegin(CardFuncComp, OverlapObj))
		{
			this->CardFunctionClassInstanceQueue.RemoveAt(0);

			//调用下一个功能
			this->ExecuteCardFuncClassByCardFunction(CardFuncComp);
			return;
		}
	}
}

void AFunctionCardActor::ExecuteCardFuncClassByCardFunctionOnAnimPlayEnd(UCardFunctionComponent* CardFuncComp)
{
	//执行功能
	if (this->CardFunctionClassInstanceQueue.Num())
	{
		if (!this->CardFunctionClassInstanceQueue[0]->OnAnimPlayEnd(CardFuncComp))
		{
			this->CardFunctionClassInstanceQueue.RemoveAt(0);

			//调用下一个功能
			this->ExecuteCardFuncClassByCardFunction(CardFuncComp);

			if (!this->CardFunctionClassInstanceQueue.Num())
			{
				//销毁卡片
				CardFuncComp->GetCardActor()->KillCard();
			}

			return;
		}
	}
	else {
		//销毁卡片
		CardFuncComp->GetCardActor()->KillCard();
	}
}

void AFunctionCardActor::ExecuteCardFuncClassByCardFunctionUpdate(UCardFunctionComponent* CardFuncComp, const float& DeltaTime)
{
	//执行功能
	if (this->CardFunctionClassInstanceQueue.Num())
	{
		if (!this->CardFunctionClassInstanceQueue[0]->Update(CardFuncComp, DeltaTime))
		{
			this->CardFunctionClassInstanceQueue.RemoveAt(0);

			//调用下一个功能
			this->ExecuteCardFuncClassByCardFunction(CardFuncComp);
			return;
		}
	}
}

void AFunctionCardActor::ExecuteCardFuncDeath()
{
	for (UCardFunctionBase* CurFunc : this->CardFunctionClassInstanceOnDeathQueue)
	{
		CurFunc->OnDeath(this->CardFuncComponent);
	}

	this->CardFunctionClassInstanceOnDeathQueue.Empty();
}

UCardFunctionBase* AFunctionCardActor::GetCurrentExecuteCardFuncClass()
{
	if (this->CardFunctionClassInstanceQueue.Num() > 0)
	{
		return this->CardFunctionClassInstanceQueue[0];
	}

	return nullptr;
}

FString AFunctionCardActor::GetIdleAnimName()
{
	return FunCardIdleAnimName.GetDefaultObject()->GetCategoryName().ToString();
}

void AFunctionCardActor::BeginPlay()
{
	this->ItemCardFunction = this->GetCardDataComponent()->GetCardFunction(this->CardActor_Name.ToString());

	//强化提升生命值
	if (this->ItemCardFunction.M_ECardUpGradeType == ECardUpGradeType::E_Defence_Up)
	{
		//强化提升生命值
		float HP = this->ItemCardFunction.M_CardHP;

		HP = this->ItemCardFunction.M_CardHP +
			this->ItemCardFunction.M_CardHP * this->ItemCardFunction.M_M_ECardUpGradeUpRate *
			this->GetCardGrade(this->ItemCardFunction.ItemName.ToString());

		//写入血量数据
		this->ItemCardFunction.M_CardHP = HP;

		//重新设置卡片生命值
		this->SetCardHP(this->ItemCardFunction.M_CardHP, 0.f);
	}

	//当前卡片的功能
	this->CardFunctionClass = this->ItemCardFunction.Functions;
	//初始化功能
	if (!this->CardFunctionClassInstanceQueue.Num())
	{
		//执行功能
		for (const TSoftClassPtr<UCardFunctionBase>& SourceClass : this->CardFunctionClass)
		{
			UClass* CurClass = SourceClass.LoadSynchronous();
			if (CurClass)
			{
				TSubclassOf<UCardFunctionBase> CurFunClass(CurClass);
				if (CurFunClass.GetDefaultObject())
				{
					UCardFunctionBase* CurFunc = CurFunClass.GetDefaultObject()->MakeNewClass();
					this->CardFunctionClassInstanceQueue.Emplace(CurFunc);
					this->CardFunctionClassInstanceOnDeathQueue.Emplace(CurFunc);
				}
			}
			else {
				UGameSystemFunction::FVMLog(__FUNCTION__,
					FString(TEXT("功能地址出错，请检查数据表：") + SourceClass.ToString())
				);
			}
		}
	}

	//兼容老框架
	this->SetCardActor(this->ItemCardFunction);
	//绑定卡片死亡函数
	this->OnCardDeathDelegate.Bind(this, TEXT("ExecuteCardFuncDeath"));

	Super::BeginPlay();
}

void AFunctionCardActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}
