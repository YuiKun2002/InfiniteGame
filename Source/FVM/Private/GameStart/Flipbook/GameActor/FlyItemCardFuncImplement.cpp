#include "GameStart/Flipbook/GameActor/FlyItemCardFuncImplement.h"
#include "GameStart/Flipbook/GameActor/Card/FunctionCardActor.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include "GameStart/VS/Components/Card/CardFunctionComponent.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include "GameStart/VS/Components/Item/ShootLineComponent.h"

//生成对象
UFlyItemCardFunc* UFlyItemCardFunc::MakeNewClass() { return NewObject<UFlyItemCardFunc>(); }
//空类的实现
void UFlyItemCardFunc::Execute(UCardFunctionComponent* CardFuncComp, FCardFunctionTriggerImplementTRB CardData, AFlyItemActor* CurFlyItem) {}

UFlyItemCardFunc* UFlyItemReboundCardFunc::MakeNewClass()
{
	return NewObject<UFlyItemReboundCardFunc>();
}

void UFlyItemReboundCardFunc::Execute(UCardFunctionComponent* CardFuncComp, FCardFunctionTriggerImplementTRB CardData, AFlyItemActor* CurFlyItem)
{
	//被标记之后直接退出
	if (!IsValid(CurFlyItem))
	{
		return;
	}

	UShootLineComponent* _Comp = Cast<UShootLineComponent>(
		CurFlyItem->GetComponentByClass(UShootLineComponent::StaticClass())
	);

	if (_Comp)
	{
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__,
				FString(TEXT("遇到管线类的子弹：执行反弹"))
			);
		}

		//修改方向
		switch (_Comp->GetMoveDirection())
		{
		case EShootDirection::EDown:
		{
			_Comp->SetMoveDirection(EShootDirection::EUp);
			CurFlyItem->SetRotation(90.f);
			return;
		}
		case EShootDirection::ELeft:
		{
			_Comp->SetMoveDirection(EShootDirection::ERight);
			CurFlyItem->SetRotation(-180);
			//CurFlyItem->GetMyActor()->SetRelativeLocation(CurFlyItem->GetMyActor()->GetRelativeLocation() * -1.f);
			return;
		}
		case EShootDirection::ERight:
		{
			_Comp->SetMoveDirection(EShootDirection::ELeft);
			CurFlyItem->SetRotation(180);
			//CurFlyItem->GetMyActor()->SetRelativeLocation(CurFlyItem->GetMyActor()->GetRelativeLocation() * -1.f);
			return;
		}
		case EShootDirection::EUp:
		{
			_Comp->SetMoveDirection(EShootDirection::EDown);
			CurFlyItem->SetRotation(-90.f);
			return;
		}
		case EShootDirection::ERotation:
		{
			//设置位移方向
			CurFlyItem->SetActorRelativeRotation(
				FRotator(CurFlyItem->GetActorRotation().Pitch,
					CurFlyItem->GetActorRotation().Yaw,
					CurFlyItem->GetActorRotation().Roll + 180.f)
			);
			return;
		}
		}
	}
}

UFlyItemCardFunc* UFlyItemBuffCardFunc::MakeNewClass()
{
	return NewObject<UFlyItemBuffCardFunc>();
}

void UFlyItemBuffCardFunc::Execute(UCardFunctionComponent* CardFuncComp, FCardFunctionTriggerImplementTRB CardData, AFlyItemActor* CurFlyItem)
{
	UShootLineComponent* _Comp = Cast<UShootLineComponent>(
		CurFlyItem->GetComponentByClass(UShootLineComponent::StaticClass())
	);

	if (_Comp)
	{
		//添加buff
		if (IsValid(CurFlyItem))
		{
			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__,
					FString(TEXT("遇到管线类的子弹：执行Buff添加"))
				);
			}

			TMap<Buff_Infor, float> Buffs = CardData.BuffSet;
			CurFlyItem->AddBuff(Buffs);
		}
	}
}

UFlyItemCardFunc* UFlyItemAcrossCardFunc::MakeNewClass()
{
	UFlyItemAcrossCardFunc* NewClass = NewObject<UFlyItemAcrossCardFunc>();
	return NewClass;
}

void UFlyItemAcrossCardFunc::Execute(
	UCardFunctionComponent* CardFuncComp,
	FCardFunctionTriggerImplementTRB CardData,
	AFlyItemActor* CurFlyItem
)
{
	if (CurFlyItem->GetActor_CardFunction_Component() == Cast<UActorComponent>(CardFuncComp))
	{
		return;
	}

	//获取射击组件（原来的子弹）
	UShootLineComponent* _Comp = Cast<UShootLineComponent>(
		CurFlyItem->GetComponentByClass(UShootLineComponent::StaticClass())
	);

	//没有该组件
	if (!_Comp)
	{
		return;
	}

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__,
			FString(TEXT("遇到管线类的子弹：穿越"))
		);
	}

	//获取资源组件
	UResourceManagerComponent* CurComp = AGameMapInstance::GetGameMapInstance()->M_ResourceManagerComponent;

	//飞行物子弹对象
	//UClass* FlyActor_Class = nullptr;
	//FlyActor_Class = UGameSystemFunction::LoadRes(CardData.AcrossBulletClass);

	bool bRe = false;
	//如果遇到当前飞行物携带减速buff
	for (auto Buff : CurFlyItem->GetBuff().M_Buffers)
	{
		if (Buff.Key == Buff_Infor::E_SlowDown)
		{
			//回溯攻击力【万一出现加伤害的情况】
			CurFlyItem->SetCurATK(CurFlyItem->GetATK());

			bRe = true;

			break;
		}
	}

	UObjectPoolManager* First = nullptr;
	if (bRe)
	{
		First = CurComp->GetStaticPool(CardData.DefBulletClass.GetAssetName() + TEXT("FC"));
		if (!IsValid(First))
		{
			First = CurComp->GetStaticPoolOrAdd(
				CardData.DefBulletClass.GetAssetName() + TEXT("FC"),
				UObjectPoolManager::MakePoolManager(CardFuncComp->GetWorld(), CardData.DefBulletClass, 1)
			);
		}
	}
	else {
		First = CurComp->GetStaticPool(CardData.AcrossBulletClass.GetAssetName() + TEXT("FC"));
		if (!IsValid(First))
		{
			First = CurComp->GetStaticPoolOrAdd(
				CardData.AcrossBulletClass.GetAssetName() + TEXT("FC"),
				UObjectPoolManager::MakePoolManager(CardFuncComp->GetWorld(), CardData.AcrossBulletClass, 1)
			);
		}
	}

	//预创建飞行物
	AActor* CurActor = First->GetObjectActor();
	AFlyItemActor* _FlyActor = Cast<AFlyItemActor>(CurActor);
	//设置功能组件数据(防止某些功能重复使用)
	_FlyActor->SetActor_CardFunction_Component(Cast<UActorComponent>(CardFuncComp));
	//获取数据
	CurFlyItem->FlyItemActorSwap(_FlyActor);
	_FlyActor->Init();
	_FlyActor->OnInit();

	_FlyActor->SetActorTransform(CurFlyItem->GetActorTransform());
	CurFlyItem->SetActorLocation(FVector::ZeroVector);
	CurFlyItem->ReturnPool();
	////完成生成
	//AFlyItemActor* Finish_Class = Cast<AFlyItemActor>(
	//	UGameplayStatics::FinishSpawningActor(_FlyActor,
	//		CurFlyItem->GetActorTransform())
	//	);

	//获取射击组件（新的子弹）
	UShootLineComponent* CurSComp = Cast<UShootLineComponent>(
		_FlyActor->GetComponentByClass(UShootLineComponent::StaticClass())
	);

	if (IsValid(CurSComp))
	{
		switch (CurSComp->GetMoveDirection())
		{
			//如果是上下或者带有角度的对象  统一右边发射并且统一 位置
		case EShootDirection::EDown:
		case EShootDirection::EUp:
		{
			//设置旋转
			_FlyActor->SetRotation(0.f);
			_FlyActor->SetActorLocation(CardFuncComp->GetCardActor()->GetActorLocation());
			CurSComp->SetMoveDirection(EShootDirection::ERight);
		}
		break;
		case EShootDirection::ELeft:
		{
			//FVector LocalPos = _FlyActor->GetMyActor()->GetRelativeLocation();
			//LocalPos.Y *= -1.f;

			//_FlyActor->GetMyActor()->SetRelativeLocation(LocalPos);
			//_FlyActor->GetMyActor()->SetRelativeScale3D(FVector(-1.f, 1.f, 1.f));

			CurSComp->SetMoveDirection(EShootDirection::ELeft);
		}
		break;
		//其他则排除按照原方向
		default:CurSComp->SetMoveDirection(_Comp->GetMoveDirection()); break;
		}

		Cast<UShootLineComponent>(CurSComp)->M_MoveNode.M_Speed = _Comp->M_MoveNode.M_Speed;
	}
	else {
		_FlyActor->ReturnPool();
	}

}

UFlyItemCardFunc* UFlyItemATKCardFunc::MakeNewClass()
{
	return NewObject<UFlyItemATKCardFunc>();
}

void UFlyItemATKCardFunc::Execute(UCardFunctionComponent* CardFuncComp, FCardFunctionTriggerImplementTRB CardData, AFlyItemActor* CurFlyItem)
{
	if (IsValid(CurFlyItem))
	{
		//获取射击组件（原来的子弹）
		UShootLineComponent* _Comp = Cast<UShootLineComponent>(
			CurFlyItem->GetComponentByClass(UShootLineComponent::StaticClass())
		);

		//没有该组件
		if (!_Comp)
		{
			return;
		}

		//当前伤害加成数值
		float CurRate = CardData.ATK +
			CardFuncComp->GetCardActor()->GetCardGrade(
				CardFuncComp->GetCardActor()->GetFunctionCardData().ItemName.ToString()
			) *
			CardData.ATKRate * 0.1f;

		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__,
				FString(
					FString::FromInt(CardFuncComp->GetCardActor()->GetCardGrade(
						CardFuncComp->GetCardActor()->GetFunctionCardData().ItemName.ToString())
					) +
					TEXT("级【") + CardFuncComp->GetCardActor()->GetFunctionCardData().ItemName.ToString() +
					TEXT("】遇到管线类的子弹：子弹攻击力增强：") +
					FString::SanitizeFloat(CurFlyItem->GetATK()) +
					TEXT(" * ") +
					FString::SanitizeFloat(CurRate))
			);
		}

		//提升攻击力
		CurFlyItem->SetCurATK(CurRate * CurFlyItem->GetATK());
	}
}
