#include "GameStart/VS/Components/Card/CardFuncCompImplement.h"

#include "SpineSkeletonAnimationComponent.h"

#include "GameStart/Flipbook/GameActor/CardActor/AttackCardActor.h"
#include "GameStart/Flipbook/GameActor/FlameActor.h"
#include "GameStart/Flipbook/GameActor/FlyItemCardFuncImplement.h"


#include "GameStart/VS/Components/Card/CardFunctionComponent.h"
#include "GameStart/VS/Components/Item/ShootLineComponent.h"
#include "GameStart/VS/Components/CardManagerComponent.h"
#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"

#include "GameStart/Flipbook/GameActor/Mouse/Normal/NormalMouse.h"

#include "GameStart/VS/MapMeshe.h"
#include "GameStart/VS/MapBaseType.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/GameMapFogManager.h"

#include "GameStart/VS/UI/UI_Card.h"
#include "GameStart/VS/UI/UI_MapMeshe.h"
#include "GameStart/VS/UI/Meshe/UIMesheWidthAll.h"

#include "GameSystem/Tools/GameSystemFunction.h"

#include  <Kismet/GameplayStatics.h>
#include  <Kismet/KismetSystemLibrary.h>
#include  <Kismet/KismetMathLibrary.h>

//生成对象
UCardFunctionBase* UCardFunctionBase::MakeNewClass() { return NewObject<UCardFunctionBase>(); }
//执行
bool UCardFunctionBase::Execute(class UCardFunctionComponent* CardFuncComp) { return true; }
//重叠
bool UCardFunctionBase::OnOverlapBegin(class UCardFunctionComponent* CardFuncComp, AActor* OverlapObj) { return true; }
//动画播放完毕
bool UCardFunctionBase::OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp) { return true; }
//更新
bool UCardFunctionBase::Update(class UCardFunctionComponent* CardFuncComp, const float& DeltaTime) { return true; }
//卡片死亡
void UCardFunctionBase::OnDeath(class UCardFunctionComponent* CardFuncComp) {};


//卡片生长功能
UCardFunctionBase* UCardFunctionGrow::MakeNewClass() { return NewObject<UCardFunctionGrow>(); }
bool UCardFunctionGrow::Execute(class UCardFunctionComponent* CardFuncComp) {
	UDataTable* CurData = LoadObject<UDataTable>(0,
		TEXT("DataTable'/Game/Resource/BP/Data/CardData/CardFunction/DT_Grow_CardFunc.DT_Grow_CardFunc'")
	);
	if (CurData)
	{
		FCardFunctionGrowImplementTRB* CurFindData = CurData->FindRow<FCardFunctionGrowImplementTRB>(
			FName(FString(CardFuncComp->GetCardActor()->GetFunctionCardData().ItemName)),
			TEXT("CardFuncGrow")
		);
		if (CurFindData)
		{
			this->CardDataTRB = *CurFindData;

			this->bCardGrowFunc = true;

			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("启动卡片生长功能"));
			}

			return true;
		}
		else {
			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("没有找到对应卡片的生长数据，请检查填写数据的Row名称"));
			}
		}
	}
	else {
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("功能卡片生长数据表加载失败，请检查路径"));
		}
	}

	return false;
}

bool UCardFunctionGrow::Update(class UCardFunctionComponent* CardFuncComp, const float& DeltaTime)
{
	if (this->bCardGrowFunc && this->CardDataTRB.GrowTime > 0.f)
	{
		//切换默认生长动画
		if (!this->bCardGrowBegin)
		{
			this->bCardGrowBegin = true;

			//计算生长时间
			float SourceTime = this->CardDataTRB.GrowTime;

			float ResultTime = SourceTime -
				SourceTime * this->CardDataTRB.GrowTimeRate *
				CardFuncComp->GetCardActor()->GetCardGrade(CardFuncComp->GetCardActor()->GetFunctionCardData().ItemName) *
				this->CardDataTRB.GrowTimeRate / SourceTime;

			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__,
					FString(
						TEXT("【") +
						CardFuncComp->GetCardActor()->GetFunctionCardData().ItemName +
						TEXT("】生长所需时间") +
						FString::SanitizeFloat(ResultTime)
					));
			}

			this->fCardGrowTime = ResultTime;
		}

		//开始生长倒计时
		if (this->fCardGrowTime > 0.f)
		{
			this->fCardGrowTime -= DeltaTime;
		}
		else
		{

			//开始播放生长动画
			/*CardFuncComp->GetCardActor()->SetAnimation(
				UGameSystemFunction::LoadRes(this->CardDataTRB.GrowAnim),
				UGameSystemFunction::LoadRes(this->CardDataTRB.GrowFinishAnim)
			);*/

			//当前动画轨道
			UTrackEntry* CurTrack = CardFuncComp->GetCardActor()->AddAnimation(
				0,
				SpineAnimationState_SpawnCard_Grow,
				false,
				0.f
			);

			float GrowTime = 1.f;
			if (IsValid(CurTrack))
			{
				GrowTime = CurTrack->GetTrackTime();
			}

			//轨道融合【生长完毕】
			CardFuncComp->GetCardActor()->AddAnimation(
				0,
				SpineAnimationState_SpawnCard_GrowDef,
				true,
				GrowTime
			);

			//播放BGM
			if (!this->CardDataTRB.GrowAudioName.Equals(""))
			{
				UFVMGameInstance::PlayBGM_S_Static(
					this->CardDataTRB.GrowAudioName,
					TEXT("ItemAudio")
				);
			}

			this->fCardGrowTime = 0.f;
			this->bCardGrowFinish = true;
			this->bCardGrowFunc = false;

			return false;
		}
	}

	return true;
}





UCardFunctionBase* UCardFunctionBombBase::MakeNewClass()
{
	return NewObject<UCardFunctionBombBase>();
}

bool UCardFunctionBombBase::Execute(class UCardFunctionComponent* CardFuncComp)
{
	UDataTable* CurData = LoadObject<UDataTable>(0,
		TEXT("DataTable'/Game/Resource/BP/Data/CardData/CardFunction/DT_Bomb_CardFunc.DT_Bomb_CardFunc'")
	);
	if (CurData)
	{
		FCardFunctionBombImplementTRB* CurFindData = CurData->FindRow<FCardFunctionBombImplementTRB>(
			FName(FString(CardFuncComp->GetCardActor()->GetFunctionCardData().ItemName)),
			TEXT("CardBomb")
		);
		if (CurFindData)
		{
			this->CardDataTRB = *CurFindData;

			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("启动卡片爆炸功能"));
			}

			return true;
		}
		else {
			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("没有找到对应卡片的爆炸数据，请检查填写数据的Row名称"));
			}
		}
	}
	else {
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("功能卡片爆炸数据表加载失败，请检查路径"));
		}
	}

	return false;
}

void UCardFunctionBombBase::CreateBomb(UCardFunctionComponent* CardFuncComp, bool bCollision)
{
	this->bBomb = true;

	this->ResourceLoad(CardFuncComp);

	TArray<FHitResult> Hits;
	for (const EMouseCollisionType& MouseCollisionType : this->CardDataTRB.MouseCollisionType)
	{
		if (UGameSystemFunction::AddSphereTraceMulti(CardFuncComp->GetCardActor(), Hits,
			this->CardDataTRB.BeginPosition,
			this->CardDataTRB.EndPosition,
			this->CardDataTRB.Radius,
			UGameSystemFunction::GetMouseCollisionTraceType(
				MouseCollisionType
			)))
		{
			AMouseActor* MouseAc = nullptr;
			FItem_Buff CurBuff = this->CardDataTRB.Buffs;
			for (auto& Mouse : Hits)
			{
				MouseAc = Cast<AMouseActor>(Mouse.GetActor());

				bool bResult = true;
				if (this->CardDataTRB.bConstLine)
				{
					if (IsValid(MouseAc) && MouseAc->GetMouseLine().Row != CardFuncComp->GetCardActor()->GetLine().Row)
					{
						bResult = false;
					}
				}

				if (bResult)
				{
					bool bKill = false;
					for (const EMouseTag& CurTag : this->CardDataTRB.MouseTag)
					{
						if (MouseAc->GetMouseTag() == CurTag)
						{
							bKill = true;
							break;
						}
					}


					float BombATK = this->CardDataTRB.BombATK;
					float BombATKRate = this->CardDataTRB.BombATKRate;
					float TargetATK =
						BombATK + BombATK * BombATKRate *
						CardFuncComp->GetCardActor()->GetCardGrade(CardFuncComp->GetCardActor()->GetFunctionCardData().ItemName) *
						0.1f;

					if (bKill)
					{
						TargetATK = MouseAc->GetTotalHP();
					}

					if (UGameSystemFunction::HitMouse(
						CardFuncComp->GetCardActor(),
						TargetATK,
						MouseAc,
						CurBuff,
						this->CardDataTRB.AttackType
					)
						)
					{
						if (UFVMGameInstance::GetDebug())
						{
							UGameSystemFunction::FVMLog(__FUNCTION__,
								TEXT("基于此范围所有的老鼠造成[爆炸伤害：") +
								FString::SanitizeFloat(TargetATK) +
								TEXT("]")
							);
						}

						//当前老鼠血量为0并且生产数量大于0
						this->SpawnFlame(MouseAc, CardFuncComp);
					}
				}
			}
		}
	}

	if (!bCollision)
	{
		if (IsValid(CardFuncComp->GetCardActor()->GetOverlapBoxComponent()))
		{
			CardFuncComp->GetCardActor()->GetOverlapBoxComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		if (IsValid(CardFuncComp->GetCardActor()->GetBoxComponent()))
		{
			CardFuncComp->GetCardActor()->GetBoxComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void UCardFunctionBombBase::CreateBombLine(UCardFunctionComponent* CardFuncComp, bool bCollision)
{
	//防止多出执行
	this->bBomb = true;

	this->ResourceLoad(CardFuncComp);

	FLine CurLine = CardFuncComp->GetCardActor()->GetLine();

	for (const EMouseCollisionType& MouseCollisionType : this->CardDataTRB.MouseCollisionType)
	{
		UMouseLineManager* CurLineComp = AGameMapInstance::GetGameMapInstance()->
			M_MouseManagerComponent->GetMouseLineManager(CurLine.Row);

		TMap<FString, AMouseActor*> CurMouse;

		switch (MouseCollisionType)
		{
		case EMouseCollisionType::MouseGround:
			CurMouse.Append(CurLineComp->GetMouseGround());
			break;
		case EMouseCollisionType::MouseSky:
			CurMouse.Append(CurLineComp->GetMouseSky());
			break;
		case EMouseCollisionType::MouseUnder:
			CurMouse.Append(CurLineComp->GetMouseUnderGround());
			break;
			//任何对象都可以
		default:
			CurMouse.Append(CurLineComp->GetMouseAll());
			break;
		}

		for (const auto& CurMouseObj : CurMouse)
		{
			if (IsValid(CurMouseObj.Value) && CurMouseObj.Value->GetCurrentHP() > 0.f)
			{
				bool bKill = false;
				for (const EMouseTag& CurTag : this->CardDataTRB.MouseTag)
				{
					if (CurMouseObj.Value->GetMouseTag() == CurTag)
					{
						bKill = true;
						break;
					}
				}

				FItem_Buff CurBuff = this->CardDataTRB.Buffs;
				float BombATK = this->CardDataTRB.BombATK;
				float BombATKRate = this->CardDataTRB.BombATKRate;
				float TargetATK =
					BombATK + BombATK * BombATKRate *
					CardFuncComp->GetCardActor()->GetCardGrade(CardFuncComp->GetCardActor()->GetFunctionCardData().ItemName) *
					0.1f;

				if (bKill)
				{
					TargetATK = CurMouseObj.Value->GetTotalHP();
				}

				if (UGameSystemFunction::HitMouse(
					CardFuncComp->GetCardActor(),
					TargetATK,
					CurMouseObj.Value,
					CurBuff,
					this->CardDataTRB.AttackType
				)
					)
				{
					if (UFVMGameInstance::GetDebug())
					{
						UGameSystemFunction::FVMLog(__FUNCTION__,
							TEXT("基于此范围所有的老鼠造成[爆炸伤害：") +
							FString::SanitizeFloat(TargetATK) +
							TEXT("]")
						);
					}

					//当前老鼠血量为0并且生产数量大于0
					this->SpawnFlame(CurMouseObj.Value, CardFuncComp);
				}
			}
		}
	}

	if (!bCollision)
	{
		if (IsValid(CardFuncComp->GetCardActor()->GetOverlapBoxComponent()))
		{
			CardFuncComp->GetCardActor()->GetOverlapBoxComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		if (IsValid(CardFuncComp->GetCardActor()->GetBoxComponent()))
		{
			CardFuncComp->GetCardActor()->GetBoxComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

//UMesheControllComponent* CurComp = AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent;

//FVector CurLocation = CurComp->GetMapMeshLocation(CurLine.Row, 0);
//CurLocation = CurLocation + FVector(0.f, -25.f, 0.f);

//FVector TargetLocation = CurComp->GetMapMeshLocation(CurLine.Row,
//	CurComp->GetMapMeshRowAndCol().Col - 1
//);
//TargetLocation = TargetLocation + FVector(0.f, 25.f, 0.f);


//for (const EMouseCollisionType& MouseCollisionType : this->CardDataTRB.MouseCollisionType)
//{
//	TArray<TEnumAsByte<EObjectTypeQuery>>  ObjTypes;
//	ObjTypes.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn)
//	);

//	UGameSystemFunction::AddLineTranceMulti(
//		CardFuncComp->GetCardActor()->GetWorld(),
//		CurLocation, TargetLocation, ObjTypes, CardFuncComp,
//		[&](UObject* Obj, AActor* Actor) {
//			AMouseActor* CurMouse = Cast<AMouseActor>(Actor);
//			if (IsValid(CurMouse) && CurMouse->GetCurrentHP() > 0.f)
//			{

//				ELineType CurLineType = CurMouse->GetMouseLineType();

//				switch (MouseCollisionType)
//				{
//				case EMouseCollisionType::MouseGround:
//					if (CurLineType != ELineType::OnGround)
//					{
//						return;
//					}
//					break;
//				case EMouseCollisionType::MouseSky:
//					if (CurLineType != ELineType::Sky)
//					{
//						return;
//					}
//					break;
//				case EMouseCollisionType::MouseUnder:
//					if (CurLineType != ELineType::OnGround)
//					{
//						return;
//					}
//					break;
//				//任何对象都可以
//				default:break;
//				}

//				if (this->CardDataTRB.bConstLine)
//				{
//					if (CurMouse->GetMouseLine().Row != CardFuncComp->GetCardActor()->GetLine().Row)
//					{
//						return;
//					}
//				}

//				bool bKill = false;
//				for (const EMouseTag& CurTag : this->CardDataTRB.MouseTag)
//				{
//					if (CurMouse->GetMouseTag() == CurTag)
//					{
//						bKill = true;
//						break;
//					}
//				}

//				FItem_Buff CurBuff = this->CardDataTRB.Buffs;
//				float BombATK = this->CardDataTRB.BombATK;
//				float BombATKRate = this->CardDataTRB.BombATKRate;
//				float TargetATK =
//					BombATK + BombATK * BombATKRate *
//					CardFuncComp->GetCardActor()->GetCardGrade(CardFuncComp->GetCardActor()->GetFunctionCardData().ItemName) *
//					0.1f;

//				if (bKill)
//				{
//					TargetATK = CurMouse->GetTotalHP();
//				}

//				if (UGameSystemFunction::HitMouse(
//					CardFuncComp->GetCardActor(),
//					TargetATK,
//					CurMouse,
//					CurBuff,
//					this->CardDataTRB.AttackType
//				)
//					)
//				{
//					if (UFVMGameInstance::GetDebug())
//					{
//						UGameSystemFunction::FVMLog(__FUNCTION__,
//							TEXT("基于此范围所有的老鼠造成[爆炸伤害：") +
//							FString::SanitizeFloat(TargetATK) +
//							TEXT("]")
//						);
//					}

//					//当前老鼠血量为0并且生产数量大于0
//					this->SpawnFlame(CurMouse, CardFuncComp);
//				}
//			}
//		}
//	);
//}

void UCardFunctionBombBase::CreateSingleMouse(UCardFunctionComponent* CardFuncComp)
{

	this->bBomb = true;

	this->ResourceLoad(CardFuncComp);

	if (IsValid(this->CurCheckMouse) && this->CurCheckMouse->GetCurrentHP() > 0.f)
	{
		if (this->CardDataTRB.bConstLine)
		{
			if (this->CurCheckMouse->GetMouseLine().Row != CardFuncComp->GetCardActor()->GetLine().Row)
			{
				return;
			}
		}

		bool bKill = false;
		for (const EMouseTag& CurTag : this->CardDataTRB.MouseTag)
		{
			if (this->CurCheckMouse->GetMouseTag() == CurTag)
			{
				bKill = true;
				break;
			}
		}

		float BombATK = this->CardDataTRB.BombATK;
		float BombATKRate = this->CardDataTRB.BombATKRate;
		float TargetATK =
			BombATK + BombATK * BombATKRate *
			CardFuncComp->GetCardActor()->GetCardGrade(CardFuncComp->GetCardActor()->GetFunctionCardData().ItemName) *
			0.1f;

		if (bKill)
		{
			TargetATK = this->CurCheckMouse->GetTotalHP();
		}

		FItem_Buff CurBuff = this->CardDataTRB.Buffs;
		if (UGameSystemFunction::HitMouse(
			CardFuncComp->GetCardActor(),
			TargetATK,
			this->CurCheckMouse,
			CurBuff,
			this->CardDataTRB.AttackType
		)
			)
		{
			if (UFVMGameInstance::GetDebug())
			{
				if (bKill)
				{
					UGameSystemFunction::FVMLog(__FUNCTION__,
						TEXT("对") + UGameSystemFunction::GetObjectName(this->CurCheckMouse) + TEXT("造成[秒杀伤害]")
					);
				}
				else {
					UGameSystemFunction::FVMLog(__FUNCTION__,
						TEXT("对") + UGameSystemFunction::GetObjectName(this->CurCheckMouse) + TEXT("造成[单次伤害：") +
						FString::SanitizeFloat(TargetATK) +
						TEXT("]")
					);
				}
			}

			//当前老鼠血量为0并且生产数量大于0
			this->SpawnFlame(this->CurCheckMouse, CardFuncComp);
		}
	}
}

bool UCardFunctionBombBase::CheckMouse(UCardFunctionComponent* CardFuncComp)
{
	FHitResult Hit;
	for (const EMouseCollisionType& MouseCollisionType : this->CardDataTRB.MouseCollisionType)
	{
		if (UGameSystemFunction::AddSphereTrace(
			CardFuncComp->GetCardActor(),
			Hit,
			this->CardDataTRB.BeginPosition,
			this->CardDataTRB.EndPosition,
			this->CardDataTRB.Radius,
			UGameSystemFunction::GetMouseCollisionTraceType(
				MouseCollisionType
			)))
		{

			//限制到本行
			if (this->CardDataTRB.bConstLine)
			{
				AMouseActor* MouseAc = Cast<AMouseActor>(Hit.GetActor());
				if (
					IsValid(MouseAc) &&
					MouseAc->GetCurrentHP() > 0.f &&
					MouseAc->GetMouseLine().Row == CardFuncComp->GetCardActor()->GetLine().Row
					)
				{
					this->CurCheckMouse = MouseAc;
					return true;
				}

				return false;
			}

			this->CurCheckMouse = Cast<AMouseActor>(Hit.GetActor());
			return true;
		}
	}
	return false;
}

void UCardFunctionBombBase::SpawnFlame(class AMouseActor* CurMouse, UCardFunctionComponent* CardFuncComp)
{
	//当前老鼠血量为0并且生产数量大于0
	if (CurMouse->GetCurrentHP() <= 0.f && this->CardDataTRB.MouseDeathSpawnFlameCount > 0)
	{
		for (int32 i = 0; i < this->CardDataTRB.FlameConcurrencyCount; i++)
		{
			//获取火苗
			AFlameActor* CurFlame = UResourceManagerComponent::GetFlame();
			if (IsValid(CurFlame))
			{
				CurFlame->SetActorScale3D(FVector(1.f));
				CurFlame->SetActorLocation(CurMouse->GetActorLocation());
				CurFlame->SetFlameValue(25);
				CurFlame->Run();
			}
			else {
				UResourceManagerComponent::GetResourceManagerComponent()->AddFlameNum(25);
			}
		}
		this->CardDataTRB.MouseDeathSpawnFlameCount--;
	}
}

void UCardFunctionBombBase::ResourceLoad(UCardFunctionComponent* CardFuncComp)
{
	//播放爆炸动画
	/*CardFuncComp->GetCardActor()->SetPlayAnimation(
		UGameSystemFunction::LoadRes(CardDataTRB.BombAnimPath)
	);*/

	CardFuncComp->GetCardActor()->SetAnimation(
		0,
		SpineAnimationState_UniformCard_Bomb,
		true
	);

	//actor对象加载
	UClass* Obj = UGameSystemFunction::LoadRes(this->CardDataTRB.OtherShow);
	if (IsValid(Obj))
	{
		//生成对象
		AFunctionActor* CurFunc = CardFuncComp->GetCardActor()->GetWorld()->
			SpawnActor<AFunctionActor>(Obj);
		CurFunc->OnInit(CardFuncComp->GetCardActor());
	}

	//播放爆炸音效
	if (!CardDataTRB.BombAudioName.Equals(""))
	{
		UFVMGameInstance::PlayBGM_S_Static(
			CardDataTRB.BombAudioName,
			TEXT("ItemAudio")
		);
	}
}






UCardFunctionBase* UCardFunctionBomb::MakeNewClass()
{
	return NewObject<UCardFunctionBomb>();
}

bool UCardFunctionBomb::Execute(class UCardFunctionComponent* CardFuncComp)
{
	return Super::Execute(CardFuncComp);
}

bool UCardFunctionBomb::OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp)
{
	if (this->bBomb)
	{
		return false;
	}

	this->CreateBomb(CardFuncComp, false);

	return true;
}


UCardFunctionBase* UCardFunctionBombLine::MakeNewClass()
{
	return NewObject<UCardFunctionBombLine>();
}

bool UCardFunctionBombLine::Execute(class UCardFunctionComponent* CardFuncComp)
{
	return Super::Execute(CardFuncComp);
}

bool UCardFunctionBombLine::OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp)
{
	if (this->bBomb)
	{
		return false;
	}

	this->CreateBombLine(CardFuncComp, false);

	return true;
}



UCardFunctionBase* UCardFunctionOverlapBomb::MakeNewClass()
{
	return NewObject<UCardFunctionOverlapBomb>();
}

bool UCardFunctionOverlapBomb::Execute(class UCardFunctionComponent* CardFuncComp)
{
	if (Super::Execute(CardFuncComp))
	{
		return true;
	}
	return false;
}

bool UCardFunctionOverlapBomb::Update(class UCardFunctionComponent* CardFuncComp, const float& DeltaTime)
{
	if (this->CheckTime > 0.f)
	{
		this->CheckTime -= DeltaTime;
	}
	else {
		this->CheckTime = this->CheckDelay;

		//检测老鼠
		if (this->CheckMouse(CardFuncComp))
		{
			//符合条件，并且结束当前功能
			this->CreateBomb(CardFuncComp, false);

			return false;
		}

	}

	return true;
}

UCardFunctionBase* UCardFunctionOverlapAttack::MakeNewClass()
{
	return NewObject<UCardFunctionOverlapAttack>();
}

bool UCardFunctionOverlapAttack::Execute(class UCardFunctionComponent* CardFuncComp)
{
	if (Super::Execute(CardFuncComp))
	{
		return true;
	}
	return false;
}

bool UCardFunctionOverlapAttack::Update(class UCardFunctionComponent* CardFuncComp, const float& DeltaTime)
{
	if (this->CheckTime > 0.f)
	{
		this->CheckTime -= DeltaTime;
	}
	else {
		this->CheckTime = this->CheckDelay;

		//检测老鼠
		if (this->CheckMouse(CardFuncComp))
		{
			//符合条件，并且结束当前功能
			this->CreateSingleMouse(CardFuncComp);

			return false;
		}

	}

	return true;
}


UCardFunctionBase* UCardFunctionTransfromDayToNight::MakeNewClass()
{
	return NewObject<UCardFunctionTransfromDayToNight>();
}

bool UCardFunctionTransfromDayToNight::OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp)
{
	return Transfrom(CardFuncComp, false);
}

bool UCardFunctionTransfromDayToNight::Transfrom(class UCardFunctionComponent* CardFuncComp, bool bDay)
{
	for (auto& Data : CardFuncComp->GetCardActor()->GetUIMapMesh()->GetCardDatas())
	{
		if (Data.Value->GetCardDay() && !bDay)
		{
			Data.Value->CardDayTransform(false);
		}
		else if (!Data.Value->GetCardDay() && bDay)
		{
			Data.Value->CardDayTransform(true);
		}
	}

	return false;
}




UCardFunctionBase* UCardFunctionTransfromNightToDay::MakeNewClass()
{
	return NewObject<UCardFunctionTransfromNightToDay>();
}

bool UCardFunctionTransfromNightToDay::OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp)
{
	return Transfrom(CardFuncComp, true);
}





UCardFunctionBase* UCardFunctionActive::MakeNewClass()
{
	return NewObject<UCardFunctionActive>();
}

bool UCardFunctionActive::Execute(class UCardFunctionComponent* CardFuncComp)
{
	UDataTable* CurData = LoadObject<UDataTable>(0,
		TEXT("DataTable'/Game/Resource/BP/Data/CardData/CardFunction/DT_Active_CardFunc.DT_Active_CardFunc'")
	);
	if (CurData)
	{
		FCardFunctionActiveTRB* CurFindData = CurData->FindRow<FCardFunctionActiveTRB>(
			FName(FString(CardFuncComp->GetCardActor()->GetFunctionCardData().ItemName)),
			TEXT("CardActive")
		);
		if (CurFindData)
		{
			this->CardDataTRB = *CurFindData;

			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("启动卡片激活功能"));
			}

			return true;
		}
		else {
			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("没有找到对应卡片的激活数据，请检查填写数据的Row名称"));
			}
		}
	}
	else {
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("功能卡片激活数据表加载失败，请检查路径"));
		}
	}

	return false;
}

bool UCardFunctionActive::Update(class UCardFunctionComponent* CardFuncComp, const float& DeltaTime)
{
	//如果选择关闭碰撞
	if (!this->CardDataTRB.CollisionEnable)
	{
		for (auto& Comp : CardFuncComp->GetCardActor()->GetComponents())
		{
			if (Cast<UBoxComponent>(Comp))
			{
				Cast<UBoxComponent>(Comp)->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
	}

	//动画
	/*UPaperFlipbook* Anim = UGameSystemFunction::LoadRes(this->CardDataTRB.ActiveAnimPath);
	if (!Anim)
	{
		Anim = this->CardDataTRB.ActiveAnimPath.LoadSynchronous();
	}

	if (Anim)
	{
		CardFuncComp->GetCardActor()->SetPlayAnimation(Anim);
	}*/

	CardFuncComp->GetCardActor()->SetAnimation(
		0,
		SpineAnimationState_ActiveCard_Active,
		true
	);

	return false;
}





UCardFunctionBase* UCardFunctionCoodDownFinish::MakeNewClass()
{
	return NewObject<UCardFunctionCoodDownFinish>();
}

bool UCardFunctionCoodDownFinish::OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp)
{
	TArray<int32> ID = { 0,1 ,-1,3 };

	for (auto id : ID)
	{
		if (UUI_Card* UICard = CardFuncComp->GetCardMeshe()->GetCardUIByID(id))
		{
			if (UFVMGameInstance::GetDebug())
			{
				UE_LOG(LogTemp, Error,
					TEXT("[UCardFunctionComponent::CardFunc_CurrentMesheCoodDownFinish]当前加速冷却的卡片：%s"),
					*UICard->GetCardName());
			}

			UICard->SetCoolDownFinish();

			return false;
		}
	}

	return false;
}

UCardFunctionBase* UCardFunctionAllCoodDownFinish::MakeNewClass()
{
	return NewObject<UCardFunctionAllCoodDownFinish>();
}

bool UCardFunctionAllCoodDownFinish::OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp)
{
	for (const auto& Item : AGameMapInstance::GetCardManagerComponent_Static()->GetUICardArrays())
	{
		if (Item->bEnableCopy)
		{
			Item->SetCoolDownFinish();
		}
	}

	return false;
}




UCardFunctionBase* UCardFunctionCopyNextCard::MakeNewClass()
{
	return NewObject<UCardFunctionCopyNextCard>();
}

bool UCardFunctionCopyNextCard::OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp)
{
	//判断资产是否有效
	if (IsValid(CardFuncComp->GetLasrCardResourceClass()))
	{
		//创建卡片
		CardFuncComp->GetCardActor()->GetUIMapMesh()->CreateCard(
			CardFuncComp->GetCardActor()->GetUIMapMesh(),
			AGameMapInstance::GetGameMapInstance()->M_CardManagerComponent,
			CardFuncComp->GetLasrCardResourceClass(),
			CardFuncComp->GetLastCardName(),
			0,
			false,
			false);
	}
	else {
		if (UFVMGameInstance::GetDebug())
		{
			UE_LOG(LogTemp, Error, TEXT("[UCardFunctionComponent::CardFunc_CopyNextCard][%s]资产无效，无法创建卡片")
				, *CardFuncComp->GetLastCardName());
		}
	}

	return false;
}

UCardFunctionBase* UCardFunctionRandomPlayCard::MakeNewClass()
{
	return NewObject<UCardFunctionRandomPlayCard>();
}

bool UCardFunctionRandomPlayCard::OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp)
{
	int32 CurIndex = 0;
	TArray<int32> RandomSet;
	for (const auto& Item : AGameMapInstance::GetCardManagerComponent_Static()->GetUICardArrays())
	{
		//如果允许复制卡片对象
		if (Item->bEnableCopy)
		{
			RandomSet.Emplace(CurIndex);
		}
		CurIndex++;
	}

	if (RandomSet.Num())
	{
		//随机一张卡片
		int32 CurRandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, RandomSet.Num() - 1);
		//获取对应的卡片UI
		UUI_Card* CurCard = AGameMapInstance::GetCardManagerComponent_Static()->GetUICardArraysByIndex(RandomSet[CurRandomIndex]);
		//获取对应卡片的实例资产指针
		TSoftClassPtr<ACardActor> CardResourceClassPtr(CurCard->CardActorSource);


		//如果资产是有效的
		if (IsValid(UGameSystemFunction::LoadRes(CardResourceClassPtr)))
		{

			if (UFVMGameInstance::GetDebug())
			{
				UE_LOG(LogTemp, Error, TEXT("[UCardFunctionRandomPlayCard::OnAnimPlayEnd] 创建卡片[%s]")
					, *CurCard->M_Card_ActorName);
			}

			//创建卡片
			CardFuncComp->GetCardActor()->GetUIMapMesh()->CreateCard(
				CardFuncComp->GetCardActor()->GetUIMapMesh(),
				AGameMapInstance::GetGameMapInstance()->M_CardManagerComponent,
				UGameSystemFunction::LoadRes(CardResourceClassPtr),
				CurCard->M_Card_ActorName,
				0,
				false,
				false);
		}

	}

	return false;
}



UCardFunctionBase* UCardFunctionProjectileATK::MakeNewClass()
{
	return NewObject<UCardFunctionProjectileATK>();
}

bool UCardFunctionProjectileATK::Execute(class UCardFunctionComponent* CardFuncComp)
{
	UDataTable* CurData = LoadObject<UDataTable>(0,
		TEXT("DataTable'/Game/Resource/BP/Data/CardData/CardFunction/DT_ProjectileATK_CardFunc.DT_ProjectileATK_CardFunc'")
	);
	if (CurData)
	{
		FCardFunctionProjectileATKImplementTRB* CurFindData = CurData->FindRow<FCardFunctionProjectileATKImplementTRB>(
			FName(FString(CardFuncComp->GetCardActor()->GetFunctionCardData().ItemName)),
			TEXT("CardProjectileATK")
		);
		if (CurFindData)
		{
			this->CardDataTRB = *CurFindData;

			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__,
					TEXT("启动卡片投掷物卡片攻击力增强功能"));
			}

			return true;
		}
		else {
			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__,
					TEXT("没有找到对应卡片的投掷物卡片攻击力数据，请检查填写数据的Row名称"));
			}
		}
	}
	else {
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__,
				TEXT("功能卡片投掷物卡片攻击力数据表加载失败，请检查路径"));
		}
	}

	return false;
}

bool UCardFunctionProjectileATK::OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp)
{
	this->ClearATK(CardFuncComp);
	this->AddtionATK(CardFuncComp);
	return true;
}

void UCardFunctionProjectileATK::OnDeath(class UCardFunctionComponent* CardFuncComp)
{
	this->ClearATK(CardFuncComp);
}

void UCardFunctionProjectileATK::ClearATK(class UCardFunctionComponent* CardFuncComp)
{
	//获取网格组件
	UMesheControllComponent* MesheComp = AGameMapInstance::GetGameMapInstance()->GetMesheControllComponent();

	//还原之前行的卡片
	for (AAttackCardActor* CurCardActor : this->LastProjectileCards)
	{
		if (IsValid(Cast<AAttackCardActor>(CurCardActor)))
		{
			Cast<AAttackCardActor>(CurCardActor)->SetCurrentATK(
				Cast<AAttackCardActor>(CurCardActor)->GetSourceATK()
			);
		}
	}

	//遍历本行的所有网格组件
	for (int32 BeginCol = 0; BeginCol < MesheComp->GetMapMeshRowAndCol().Col; BeginCol++)
	{
		AMapMeshe* CurMeshe = MesheComp->GetMesh(CardFuncComp->GetCardActor()->GetLine().Row, BeginCol);

		if (CurMeshe->GetUIMapMeshe())
		{
			ACardActor* CurCard = CurMeshe->GetUIMapMeshe()->GetCardByID(0);
			//投掷物卡片
			if (IsValid(CurCard) && CurCard->GetCardData().CardCategoryTag == ECardCategoryTag::Projectile)
			{
				//类型转换
				AAttackCardActor* CurATKInstance = Cast<AAttackCardActor>(CurCard);
				if (IsValid(CurATKInstance))
				{
					//还原攻击力
					CurATKInstance->SetCurrentATK(CurATKInstance->GetSourceATK());
				}
			}
		}
	}

	this->LastProjectileCards.Empty();
}

void UCardFunctionProjectileATK::AddtionATK(class UCardFunctionComponent* CardFuncComp)
{
	//获取网格组件
	UMesheControllComponent* MesheComp = AGameMapInstance::GetGameMapInstance()->GetMesheControllComponent();
	//遍历本行的所有网格组件
	for (int32 BeginCol = 0; BeginCol < MesheComp->GetMapMeshRowAndCol().Col; BeginCol++)
	{
		AMapMeshe* CurMeshe = MesheComp->GetMesh(CardFuncComp->GetCardActor()->GetLine().Row, BeginCol);

		if (!CurMeshe->GetUIMapMeshe())
		{
			continue;
		}

		ACardActor* CurCard = CurMeshe->GetUIMapMeshe()->GetCardByID(0);

		//投掷物卡片
		if (IsValid(CurCard) && CurCard->GetCardData().CardCategoryTag == ECardCategoryTag::Projectile)
		{
			//类型转换
			AAttackCardActor* CurATKInstance = Cast<AAttackCardActor>(CurCard);
			if (IsValid(CurATKInstance))
			{
				//攻击力增强
				CurATKInstance->SetCurrentATK(
					CurATKInstance->GetSourceATK() *
					(
						CardDataTRB.ATKAddtion +
						CardDataTRB.BombATKRate *
						CardFuncComp->GetCardActor()->GetCardGrade(
							CardFuncComp->GetCardActor()->GetFunctionCardData().ItemName
						)
						* 0.1f
						)
				);

				if (UFVMGameInstance::GetDebug())
				{
					UGameSystemFunction::FVMLog(__FUNCTION__,
						FString(
							TEXT("投掷物卡片攻击力增强：") +
							FString::SanitizeFloat(CurATKInstance->GetCurrentATK())
						));
				}

				//添加卡片
				this->LastProjectileCards.Emplace(CurATKInstance);
			}
		}
	}
}







UCardFunctionBase* UCardFunctionTrigger::MakeNewClass()
{
	return NewObject<UCardFunctionTrigger>();
}

bool UCardFunctionTrigger::Execute(class UCardFunctionComponent* CardFuncComp)
{
	UDataTable* CurData = LoadObject<UDataTable>(0,
		TEXT("DataTable'/Game/Resource/BP/Data/CardData/CardFunction/DT_Trigger_CardFunc.DT_Trigger_CardFunc'")
	);
	if (CurData)
	{
		FCardFunctionTriggerImplementTRB* CurFindData = CurData->FindRow<FCardFunctionTriggerImplementTRB>(
			FName(FString(CardFuncComp->GetCardActor()->GetFunctionCardData().ItemName)),
			TEXT("CardTrigger")
		);
		if (CurFindData)
		{
			this->CardDataTRB = *CurFindData;

			//绑定触发函数
			//组件绑定
			if (IsValid(CardFuncComp->GetCardActor()->GetOverlapBoxComponent()))
			{
				CardFuncComp->GetCardActor()->GetOverlapBoxComponent()->
					OnComponentBeginOverlap.AddDynamic(CardFuncComp, &UCardFunctionComponent::EventTrigger);
			}

			//初始化类
			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__,
					FString(TEXT("遇到飞行物功能卡片函数执行：") + CardFuncComp->GetCardActor()->GetFunctionCardData().ItemName)
				);
			}

			//解析当前卡片预设的功能
			UFlyItemCardFunc* LastFuncAcross = nullptr;
			for (const TSoftClassPtr<UFlyItemCardFunc>& CurFunClassSource : this->CardDataTRB.FlyItemFunctionClass)
			{
				UClass* CurClass = CurFunClassSource.LoadSynchronous();
				if (CurClass)
				{
					TSubclassOf<UFlyItemCardFunc> CurFunClass(CurClass);
					if (CurFunClass.GetDefaultObject())
					{
						//穿越功能置底【永远是最后一个执行】
						if (Cast<UFlyItemAcrossCardFunc>(CurFunClass.GetDefaultObject()))
						{
							LastFuncAcross = CurFunClass.GetDefaultObject();
							continue;
						}
						else {
							this->CardInfluenceFlyItemClass.Emplace(CurFunClass.GetDefaultObject()->MakeNewClass());
						}
					}
				}
				else {
					UGameSystemFunction::FVMLog(__FUNCTION__,
						FString(TEXT("功能地址出错，请检查数据表：") + CurFunClassSource.ToString())
					);
				}
			}

			//执行置底功能
			if (LastFuncAcross)
			{
				this->CardInfluenceFlyItemClass.Emplace(LastFuncAcross->MakeNewClass());
			}

			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("启动卡片触发器功能"));
			}

			return true;
		}
		else {
			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("没有找到对应卡片的触发器数据，请检查填写数据的Row名称"));
			}
		}
	}
	else {
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("功能卡片触发器数据表加载失败，请检查路径"));
		}
	}
	return false;
}

bool UCardFunctionTrigger::OnOverlapBegin(class UCardFunctionComponent* CardFuncComp, AActor* OverlapObj)
{
	//子弹转换失败
	if (!Cast<AFlyItemActor>(OverlapObj))
	{
		return true;
	}

	if (!Cast<AFlyItemActor>(OverlapObj)->GetFlyInfluence())
	{
		return true;
	}

	if (UFVMGameInstance::GetDebug())
	{
		UGameSystemFunction::FVMLog(__FUNCTION__,
			FString(TEXT("遇到飞行物功能卡片函数执行：") + CardFuncComp->GetCardActor()->GetFunctionCardData().ItemName)
		);
	}

	//解析当前卡片预设的功能
	for (UFlyItemCardFunc* CurFunc : this->CardInfluenceFlyItemClass)
	{
		if (UFVMGameInstance::GetDebug())
		{
			UE_LOG(LogTemp, Error,
				TEXT("UCardFunctionComponent::Trigger_FlyItem:执行功能，地址：%p"),
				__FUNCTION__,
				CurFunc);
		}

		CurFunc->Execute(CardFuncComp, CardDataTRB, Cast<AFlyItemActor>(OverlapObj));
	}

	return true;
}

UCardFunctionBase* UCardFunctionCatBox::MakeNewClass()
{
	return NewObject<UCardFunctionCatBox>();
}

bool UCardFunctionCatBox::Execute(class UCardFunctionComponent* CardFuncComp)
{
	UDataTable* CurData = LoadObject<UDataTable>(0,
		TEXT("DataTable'/Game/Resource/BP/Data/CardData/CardFunction/DT_CatBox_CardFunc.DT_CatBox_CardFunc'")
	);
	if (CurData)
	{
		FCardFunctionCatBoxImplementTRB* CurFindData = CurData->FindRow<FCardFunctionCatBoxImplementTRB>(
			FName(FString(CardFuncComp->GetCardActor()->GetFunctionCardData().ItemName)),
			TEXT("CardCatBox")
		);
		if (CurFindData)
		{
			this->CardDataTRB = *CurFindData;

			int32 CardGrade = CardFuncComp->GetCardActor()->GetCardGrade(CardFuncComp->GetCardActor()->GetCardData().ItemName);
			//初始化血量提升
			CardFuncComp->GetCardActor()->SetCardHP(
				CardFuncComp->GetCardActor()->GetTotalHP() + CardGrade * 0.25f * CardFuncComp->GetCardActor()->GetTotalHP(), 0
			);

			//初始化血量
			this->fCurHp = CardFuncComp->GetCardActor()->GetCurrentHP();

			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("启动猫猫盒功能"));
			}

			return true;
		}
		else {
			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("没有找到对应卡片的猫猫盒数据，请检查填写数据的Row名称"));
			}
		}
	}
	else {
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("功能卡片激活数据表加载失败，请检查路径"));
		}
	}

	return false;
}

bool UCardFunctionCatBox::Update(class UCardFunctionComponent* CardFuncComp, const float& DeltaTime)
{
	if (this->fUpdateDelay > 0.f)
	{
		this->fUpdateDelay -= DeltaTime;
	}
	else {
		this->fUpdateDelay = 0.2f;

		if (this->JudgeState(CardFuncComp))
		{
			UFVMGameInstance::PlayBGM_S_Static(TEXT("CatBox"), TEXT("ItemAudio"));
		}
	}

	return true;
}

bool UCardFunctionCatBox::OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp)
{

	float CurHP = CardFuncComp->GetCardActor()->GetCurrentHP();
	float HP = CardFuncComp->GetCardActor()->GetTotalHP();

	if (CurHP > HP * 0.6)
	{
		//CardFuncComp->GetCardActor()->SetPlayAnimation(UGameSystemFunction::LoadRes(this->CardDataTRB.State1));
		CardFuncComp->GetCardActor()->SetAnimation(
			0,
			SpineAnimationState_CatBoxCard_FullReady,
			true
		);
	}
	else if (CurHP > HP * 0.3)
	{
		CardFuncComp->GetCardActor()->SetAnimation(
			0,
			SpineAnimationState_CatBoxCard_DamageReady,
			true
		);
	}
	else
	{
		CardFuncComp->GetCardActor()->SetAnimation(
			0,
			SpineAnimationState_CatBoxCard_LowReady,
			true
		);
	}

	this->JudgeState(CardFuncComp);

	return true;
}

bool UCardFunctionCatBox::JudgeState(class UCardFunctionComponent* CardFuncComp)
{
	//判断血量是否有变化
	if (CardFuncComp->GetCardActor()->GetCurrentHP() != this->fCurHp && CardFuncComp->GetCardActor()->GetCurrentHP() > 0.f)
	{
		//出现设置血量
		this->fCurHp = CardFuncComp->GetCardActor()->GetCurrentHP();

		//获取当前攻击自身的老鼠
		AMouseActor* CurMouse = CardFuncComp->GetCardActor()->GetCurrentAttackSelfMouse();
		if (IsValid(CurMouse) && CurMouse->GetCurrentHP() > 0.f)
		{
			if (CurMouse->GetMouseBaseType() != EMouseBaseType::Boss)
			{
				//播放攻击动画
				float CurHP = CardFuncComp->GetCardActor()->GetCurrentHP();
				float HP = CardFuncComp->GetCardActor()->GetTotalHP();

				if (CurHP > HP * 0.6)
				{
					CardFuncComp->GetCardActor()->SetAnimation(
						0,
						SpineAnimationState_CatBoxCard_FullAttack,
						true
					);
					//CardFuncComp->GetCardActor()->SetPlayAnimation(UGameSystemFunction::LoadRes(this->CardDataTRB.StateATK1));
				}
				else if (CurHP > HP * 0.3)
				{
					CardFuncComp->GetCardActor()->SetAnimation(
						0,
						SpineAnimationState_CatBoxCard_DamageAttack,
						true
					);
					//CardFuncComp->GetCardActor()->SetPlayAnimation(UGameSystemFunction::LoadRes(this->CardDataTRB.StateATK2));
				}
				else
				{
					CardFuncComp->GetCardActor()->SetAnimation(
						0,
						SpineAnimationState_CatBoxCard_LowAttack,
						true
					);
					//CardFuncComp->GetCardActor()->SetPlayAnimation(UGameSystemFunction::LoadRes(this->CardDataTRB.StateATK3));
				}

				const FItemCardFunction& CurCardData = CardFuncComp->GetCardActor()->GetFunctionCardData();

				//给予伤害，并且进行移位换行
				float CurATK = UGameSystemFunction::CalculateCardATK(
					this->CardDataTRB.CatBoxAtkBase, CardDataTRB.CatBoxAtkRate,
					CardFuncComp->GetCardActor()->GetCardGrade(CurCardData.ItemName), CurCardData.M_M_ECardUpGradeUpRate);

				//设置命中老鼠
				CurMouse->SetbIsHurt(true);
				CurMouse->BeHit(CardFuncComp->GetCardActor(), CurATK, EFlyItemAttackType::Def);

				//如果老鼠受到攻击并没有死亡，才会执行移动
				if (CurMouse->GetCurrentHP() > 0.f)
				{
					//计算移动线路
					FLine NewLine = UGameSystemFunction::CalculateMouseMoveLine(
						AGameMapInstance::GetGameMapInstance()->GetMesheControllComponent()->GetMapMeshRowAndCol().Row,
						CardFuncComp->GetCardActor()->GetLine()
					);

					//获取当前卡片的列
					NewLine.Col = CardFuncComp->GetCardActor()->GetLine().Col;

					//设置移动老鼠
					CurMouse->SetMouseMoveOtherLine(NewLine);

					return true;
				}
			}
		}
	}

	return false;
}

UCardFunctionBase* UCardFunctionBurger::MakeNewClass()
{
	return NewObject<UCardFunctionBurger>();
}

bool UCardFunctionBurger::Execute(class UCardFunctionComponent* CardFuncComp)
{
	UDataTable* CurData = LoadObject<UDataTable>(0,
		TEXT("DataTable'/Game/Resource/BP/Data/CardData/CardFunction/DT_Burger_CardFunc.DT_Burger_CardFunc'")
	);
	if (CurData)
	{
		FCardFunctionBurgerImplementTRB* CurFindData = CurData->FindRow<FCardFunctionBurgerImplementTRB>(
			FName(FString(CardFuncComp->GetCardActor()->GetFunctionCardData().ItemName)),
			TEXT("CardCatBox")
		);
		if (CurFindData)
		{
			this->CardDataTRB = *CurFindData;

			//初始化更新时间
			this->fTime = this->fConstCheckTime;
			//初始化攻击延迟
			this->fAttackTime = this->fConstAttackTime;

			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("启动汉堡包功能"));
			}

			return true;
		}
		else {
			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("没有找到对应卡片的汉堡包数据，请检查填写数据的Row名称"));
			}
		}
	}
	else {
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("功能卡片激活数据表加载失败，请检查路径"));
		}
	}

	return false;
}

bool UCardFunctionBurger::Update(class UCardFunctionComponent* CardFuncComp, const float& DeltaTime)
{
	if (this->fTime > 0.f)
	{
		this->fTime -= DeltaTime;
	}
	else
	{
		this->fTime = this->fConstCheckTime;

		if (this->CurState == 0u)
		{
			//是否启动吸附功能【决定老鼠检测方式】
			if (this->CardDataTRB.bCatchFunc)
			{
				this->EnableRangeATK(CardFuncComp);
			}
			else {
				this->DisEnableRangeATK(CardFuncComp);
			}
		}
	}

	if (this->CurState == 1u)
	{
		if (this->fAttackTime > 0.f)
		{
			this->fAttackTime -= DeltaTime;
		}
		else {
			this->fAttackTime = this->fConstAttackTime;
			//攻击所有老鼠
			this->HitAllMouse(CardFuncComp);
		}
	}

	return true;
}

bool UCardFunctionBurger::OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp)
{
	//如果处于攻击状态
	if (this->CurState == 1u)
	{
		//设置动画播放
		//CardFuncComp->GetCardActor()->SetPlayAnimation(UGameSystemFunction::LoadRes(this->CardDataTRB.StateEat));
		CardFuncComp->GetCardActor()->SetAnimation(
			0,
			SpineAnimationState_BurgerCard_Eatting,
			true
		);
		if (this->bHit)
		{
			//设置咀嚼次数
			int32 CardGrade = CardFuncComp->GetCardActor()->GetCardGrade(CardFuncComp->GetCardActor()->GetCardData().ItemName);
			int32 CurEttingCount = this->CardDataTRB.EattingCount - CardGrade * 0.5f;
			//咀嚼次数
			this->iEattingCount = CurEttingCount <= 0 ? 1 : CurEttingCount;
		}
		else {
			this->iEattingCount = 1;
		}

		//设置状态
		this->CurState = 2u;
	}
	else if (this->CurState == 2u)//处于咀嚼状态
	{
		if (this->iEattingCount > 0)
		{
			this->iEattingCount -= 1;
		}
		else {
			//咀嚼结束
			this->iEattingCount = 0;
			this->CurState = 4u;

			CardFuncComp->GetCardActor()->SetAnimation(
				0,
				SpineAnimationState_BurgerCard_EattingCompelet,
				true
			);

			//播放动画
			//CardFuncComp->GetCardActor()->SetPlayAnimation(UGameSystemFunction::LoadRes(this->CardDataTRB.StateFinish));
		}
	}
	else if (this->CurState == 3u)//吸附状态
	{
		//将目前所有的老鼠进行拉扯至前方位置
		//获取当前网格前方的位置，如果当前以及是最后一格就直接吸附当前位置
		UMesheControllComponent* CurComp = AGameMapInstance::GetGameMapInstance()->GetMesheControllComponent();
		//是否是最后一格
		bool bLastGird = CardFuncComp->GetCardActor()->GetLine().Col == CurComp->GetMapMeshRowAndCol().Col - 1;
		//目标网格位置
		FVector TargetLocation = FVector::ZeroVector;
		//老鼠偏移位置
		const FVector MouseOffsetLocation = FVector(0.f, 0.f, -15.f);
		if (bLastGird)
		{
			TargetLocation = CurComp->GetMapMesh(CardFuncComp->GetCardActor()->GetLine().Row, CardFuncComp->GetCardActor()->GetLine().Col)->GetActorLocation();
		}
		else {
			TargetLocation = CurComp->GetMapMesh(CardFuncComp->GetCardActor()->GetLine().Row, CardFuncComp->GetCardActor()->GetLine().Col + 1)->GetActorLocation();
		}
		//加上最终偏移量
		TargetLocation += MouseOffsetLocation;

		//将所有的老鼠拉扯至当前位置
		for (AMouseActor* CurMouse : this->MouseBuffer)
		{
			if (IsValid(CurMouse))
			{
				//切换路线
				CurMouse->GetMouseManager()->ChangeLine(UKismetSystemLibrary::GetObjectName(CurMouse), CurMouse->GetMouseLine().Row, CardFuncComp->GetCardActor()->GetLine().Row);
				//切换线路
				CurMouse->SetMouseLine(CardFuncComp->GetCardActor()->GetLine());
				//设置老鼠位置
				CurMouse->SetActorLocation(TargetLocation);
			}
		}

		//设置攻击状态
		//CardFuncComp->GetCardActor()->SetPlayAnimation(UGameSystemFunction::LoadRes(this->CardDataTRB.StateATK));
		CardFuncComp->GetCardActor()->SetAnimation(
			0,
			SpineAnimationState_BurgerCard_Attack,
			true
		);
		//攻击状态
		this->CurState = 1u;
	}
	else {
		//完成所有动作
		if (this->CurState == 4u)
		{
			//如果全部的动作以及完成，直接退出当前功能
			if (this->CardDataTRB.EattingFinishEndCurFunction)
			{
				return false;
			}

			//重置攻击延迟
			this->fAttackTime = this->fConstAttackTime;
			//恢复默认状态
			this->CurState = 0u;
		}

		CardFuncComp->GetCardActor()->SetAnimation(
			0,
			SpineAnimationState_BurgerCard_Idle,
			true
		);

		//CardFuncComp->GetCardActor()->SetPlayAnimation(UGameSystemFunction::LoadRes(this->CardDataTRB.StateDef));
	}

	return true;
}

void UCardFunctionBurger::EnableRangeATK(class UCardFunctionComponent* CardFuncComp)
{
	//吸附老鼠
	this->MouseBuffer.Empty();
	for (const ELineTraceType& CurMouseLineType : this->CardDataTRB.LineTraceType)
	{
		TArray<FHitResult> AllResult;
		if (UGameSystemFunction::AddSphereTraceMulti(CardFuncComp->GetCardActor(), AllResult,
			FVector(0.f, 60.f, 0.f), FVector(0.f, 60.f, 0.f), this->CardDataTRB.fCatchRange,
			UGameSystemFunction::GetMouseCollisonTraceTypeByLineType(CurMouseLineType)))
		{
			for (const FHitResult& CurHit : AllResult) {
				AMouseActor* CurMouse = Cast<AMouseActor>(CurHit.GetActor());
				if (IsValid(CurMouse) && CurMouse->GetCurrentHP() > 0.f && CurMouse->GetMouseBaseType() != EMouseBaseType::Boss)
				{
					for (const EMouseTag& CurTag : this->CardDataTRB.CanCatchMouse)
					{
						if (CurMouse->GetMouseTag() == CurTag)
						{
							this->MouseBuffer.Emplace(CurMouse);

							break;
						}
					}
				}
			}

		}
	}

	if (this->MouseBuffer.Num() > 0.f)
	{
		//播放动画
		//CardFuncComp->GetCardActor()->SetPlayAnimation(UGameSystemFunction::LoadRes(this->CardDataTRB.StateCatch));

		CardFuncComp->GetCardActor()->SetAnimation(
			0,
			SpineAnimationState_BurgerCard_Catch,
			true
		);
		//切换吸附状态
		this->CurState = 3u;
	}
}

void UCardFunctionBurger::DisEnableRangeATK(class UCardFunctionComponent* CardFuncComp)
{
	TArray<FLineTraceSetting> CurLine;
	AFunctionCardActor* CurCard = CardFuncComp->GetCardActor();
	for (const FLineTraceSetting& CurLineSetting : CurCard->GetFunctionCardData().M_LineTraceSettings)
	{
		UGameSystemFunction::CalculateLineTracePosition(CurCard->GetActorLocation(),
			CurCard->GetLine(), CurCard->GetUIMapMesh(), CurLineSetting, CurLine);
	}

	//将计算好的线路使用，进行老鼠检测
	ELineTraceType CurTraceType;
	AActor* CurMouseActor = UGameSystemFunction::CreateCheckMouseLineTrace(
		CardFuncComp->GetCardActor()->GetWorld(), CurLine, CurTraceType);
	if (IsValid(CurMouseActor))
	{
		AMouseActor* CurMouse = Cast<AMouseActor>(CurMouseActor);
		if (IsValid(CurMouse) && CurMouse->GetCurrentHP() > 0.f && CurMouse->GetMouseBaseType() != EMouseBaseType::Boss)
		{
			this->MouseBuffer.Empty();
			this->MouseBuffer.Emplace(CurMouse);

			//播放动画
			//CardFuncComp->GetCardActor()->SetPlayAnimation(UGameSystemFunction::LoadRes(this->CardDataTRB.StateATK));
			CardFuncComp->GetCardActor()->SetAnimation(
				0,
				SpineAnimationState_BurgerCard_Attack,
				true
			);
			//切换状态
			this->CurState = 1u;
		}
	}

	//UGameSystemFunction::CreateCheckMouseLineTrace()
}

void UCardFunctionBurger::SingleAttack(class UCardFunctionComponent* CardFuncComp)
{

}

void UCardFunctionBurger::HitAllMouse(class UCardFunctionComponent* CardFuncComp)
{
	this->bHit = false;

	for (AMouseActor* CurMouse : this->MouseBuffer)
	{
		if (!IsValid(CurMouse))
		{
			continue;
		}

		bool bDeathNormalMouse = false;
		for (const EMouseTag& CurTag : this->CardDataTRB.CatchMouse)
		{
			if (CurMouse->GetMouseTag() == CurTag)
			{
				bDeathNormalMouse = true;
				CurMouse->SetMouseDeath(true);
				CurMouse->SetPlayAnimation(nullptr);
				this->bHit = true;
				break;
			}
		}

		//表示里面的老鼠并不匹配，直接进行攻击
		if (!bDeathNormalMouse)
		{
			const FItemCardFunction& CurCardData = CardFuncComp->GetCardActor()->GetFunctionCardData();

			float CurATK = UGameSystemFunction::CalculateCardATK(
				this->CardDataTRB.AtkBase, CardDataTRB.AtkRate,
				CardFuncComp->GetCardActor()->GetCardGrade(CurCardData.ItemName), CurCardData.M_M_ECardUpGradeUpRate);

			if (CurMouse->BeHit(CardFuncComp->GetCardActor(), CurATK, EFlyItemAttackType::Def))
			{
				CurMouse->SetbIsHurt(true);
			}

			if (CurMouse->GetCurrentHP() <= 0.f)
			{
				CurMouse->SetPlayAnimation(nullptr);
				this->bHit = true;
			}
		}
	}

	//如果bHit == false  设置咀嚼次数为1
	if (!bHit)
	{
		this->iEattingCount = 1;
	}

	//清空老鼠缓存
	this->MouseBuffer.Empty();
}

UCardFunctionBase* UCardFunctionFlourbag::MakeNewClass()
{
	return NewObject<UCardFunctionFlourbag>();
}

bool UCardFunctionFlourbag::Execute(class UCardFunctionComponent* CardFuncComp)
{
	UDataTable* CurData = LoadObject<UDataTable>(0,
		TEXT("DataTable'/Game/Resource/BP/Data/CardData/CardFunction/DT_Flourbag_CardFunc.DT_Flourbag_CardFunc'")
	);
	if (CurData)
	{
		FCardFunctionFlourbagImplementTRB* CurFindData = CurData->FindRow<FCardFunctionFlourbagImplementTRB>(
			FName(FString(CardFuncComp->GetCardActor()->GetFunctionCardData().ItemName)),
			TEXT("Flourbag")
		);
		if (CurFindData)
		{
			this->CardDataTRB = *CurFindData;

			//初始化
			this->Time = UTimeLineClass::MakeTimeLineClass();
			this->Init(CardFuncComp);

			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("启动面粉袋功能"));
			}

			return true;
		}
		else {
			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("没有找到对应卡片的面粉袋数据，请检查填写数据的Row名称"));
			}
		}
	}
	else {
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("功能卡片激活数据表加载失败，请检查路径"));
		}
	}

	return false;
}

bool UCardFunctionFlourbag::Update(class UCardFunctionComponent* CardFuncComp, const float& DeltaTime)
{
	if (this->CardDataTRB.UseCount <= 0)
	{
		return false;
	}

	//更新攻击
	if (this->bCheckPlay)
	{
		if (IsValid(this->Time))
		{
			this->Time->Tick(DeltaTime);
		}
		return true;
	}

	//老鼠检测
	if (this->fCheckTime > 0.f)
	{
		this->fCheckTime -= DeltaTime;
	}
	else {
		this->fCheckTime = 0.2f;
		this->bCheckPlay = false;

		//老鼠无效触发检测
		if (!IsValid(this->CurMouse))
		{
			if (IsValid(this->Gen(CardFuncComp, true)))
			{
				this->bCheckPlay = true;
				/*CardFuncComp->GetCardActor()->SetPlayAnimation(
					UGameSystemFunction::LoadRes(this->CardDataTRB.StateLeft)
				);*/

				CardFuncComp->GetCardActor()->SetAnimation(
					0,
					SpineAnimationState_FlourbagCard_Left,
					true
				);

				//关闭碰撞
				CardFuncComp->GetCardActor()->SetCollisionEnable(false);
				//初始化位置
				this->CurLocation = CardFuncComp->GetCardActor()->GetActorLocation();
				this->TargetLocation = this->CurMouse->GetActorLocation();
				this->TargetLocation.Z = this->CurLocation.Z;
				this->Time->PlayFromStart();
				return true;
			}

			if (IsValid(this->Gen(CardFuncComp, false)))
			{
				this->bCheckPlay = true;
				/*CardFuncComp->GetCardActor()->SetPlayAnimation(
					UGameSystemFunction::LoadRes(this->CardDataTRB.StateRight)
				);*/

				CardFuncComp->GetCardActor()->SetAnimation(
					0,
					SpineAnimationState_FlourbagCard_Right,
					true
				);

				//关闭碰撞
				CardFuncComp->GetCardActor()->SetCollisionEnable(false);
				//初始化位置
				this->CurLocation = CardFuncComp->GetCardActor()->GetActorLocation();
				this->TargetLocation = this->CurMouse->GetActorLocation();
				this->TargetLocation.Z = this->CurLocation.Z;
				this->Time->PlayFromStart();
				return true;
			}
		}
	}

	return true;
}

bool UCardFunctionFlourbag::OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp)
{
	if (this->CardDataTRB.UseCount <= 0)
	{
		return false;
	}

	if (this->bEnd)
	{
		this->bEnd = false;

		/*CardFuncComp->GetCardActor()->SetPlayAnimation(
			UGameSystemFunction::LoadRes(this->CardDataTRB.State1)
		);*/

		CardFuncComp->GetCardActor()->SetAnimation(
			0,
			SpineAnimationState_FlourbagCard_Idle,
			true
		);

		FLine Cur = CardFuncComp->GetCardActor()->GetLine();

		//回归原位置
		CardFuncComp->GetCardActor()->SetActorLocation(
			AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->
			GetMapMesh(Cur.Row, Cur.Col)->GetActorLocation()
		);

		//开启碰撞
		CardFuncComp->GetCardActor()->SetCollisionEnable(true);
	}

	return true;
}

AMouseActor* UCardFunctionFlourbag::Gen(UCardFunctionComponent* CardFuncComp, bool bLeft)
{
	float bCheckLen = this->CardDataTRB.CheckLength;
	this->bCurStateLeft = bLeft;
	if (bLeft)
	{
		bCheckLen = bCheckLen * -1.f;
	}

	for (const auto& CurCo : this->CardDataTRB.MouseLineType)
	{
		if (UGameSystemFunction::AddLineTrance(
			CardFuncComp->GetCardActor()->GetWorld(),
			CardFuncComp->GetCardActor()->GetActorLocation(),
			FVector(0.f, 0.f, 0.f), FVector(0.f, bCheckLen, 0.f),
			UGameSystemFunction::GetMouseCollisionBoxTypeByELineType(CurCo),
			this,
			[](UObject* Obj, AActor* Ac) {

				AMouseActor* Cur = Cast<AMouseActor>(Ac);
				if (IsValid(Cur) && Cur->GetCurrentHP() > 0.f)
				{
					UCardFunctionFlourbag* CurBag = Cast<UCardFunctionFlourbag>(Obj);
					CurBag->CurMouse = Cur;
				}
			}
		))
		{
			return this->CurMouse;
		}
	}

	return this->CurMouse;
}

void UCardFunctionFlourbag::Init(UCardFunctionComponent* CardFuncComp)
{
	this->CurCardFuncComp = CardFuncComp;

	this->Time->AddCurve(
		this->CardDataTRB.MoveCurve, this,
		[](UTimeLineClass* Ltime, UObject* Obj, float value) {
			UCardFunctionFlourbag* CurObj = Cast<UCardFunctionFlourbag>(Obj);
			CurObj->CurCardFuncComp->GetCardActor()->SetActorLocation(
				UKismetMathLibrary::VLerp(
					CurObj->CurLocation,
					CurObj->TargetLocation,
					value
				)
			);
		},
		[](UTimeLineClass* Ltime, UObject* Obj) {

			UCardFunctionFlourbag* CurObj = Cast<UCardFunctionFlourbag>(Obj);

			//使用次数减少
			CurObj->CardDataTRB.UseCount--;

			const FItemCardFunction& CurCardData = CurObj->CurCardFuncComp->GetCardActor()->GetFunctionCardData();
			//设置攻击力
			float CurATK = UGameSystemFunction::CalculateCardATK(
				CurObj->CardDataTRB.FlourbagAtkBase, CurObj->CardDataTRB.FlourbagAtkRate,
				CurObj->CurCardFuncComp->GetCardActor()->GetCardGrade(CurCardData.ItemName),
				CurCardData.M_M_ECardUpGradeUpRate);

			//生成攻击球体检测
			for (const auto& CurOffset : CurObj->CardDataTRB.HitLocationOffset)
			{
				//遍历碰撞
				for (const auto& CurCollision : CurObj->CardDataTRB.MouseLineType)
				{
					//生成范围攻击
					TArray<FHitResult> Hits;
					if (UGameSystemFunction::AddSphereTraceMulti(
						CurObj->CurCardFuncComp->GetCardActor(), Hits,
						FVector(0.f, CurOffset.X, CurOffset.Y), FVector(0.f, CurOffset.X, CurOffset.Y),
						CurObj->CardDataTRB.RangeRadius,
						UGameSystemFunction::GetMouseCollisionTraceTypeByELineType(CurCollision)
					))
					{
						for (const FHitResult& CurHit : Hits)
						{
							AMouseActor* CurMouseClass = Cast<AMouseActor>(CurHit.GetActor());
							if (IsValid(CurMouseClass))
							{
								if (CurMouseClass->BeHit(CurObj->CurCardFuncComp->GetCardActor(),
									CurATK, EFlyItemAttackType::Def))
								{
									CurMouseClass->SetbIsHurt(true);
								}
							}
						}
					}
				}
			}

			UFVMGameInstance::PlayBGM_S_Static(TEXT("Flourbag"), TEXT("ItemAudio"));

			CurObj->bEnd = true;
			CurObj->bCheckPlay = false;
		}
	);
}

UCardFunctionBase* UCardFunctionHiddFogBlock::MakeNewClass()
{

	UFVMGameInstance::PlayBGM_S_Static(TEXT("Fog"), TEXT("ItemAudio"));

	AGameMapInstance::GetGameMapFogManager_Static()->FullTimeHiddBlock(15.f);

	return NewObject<UCardFunctionHiddFogBlock>();
}

bool UCardFunctionHiddFogBlock::OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp)
{
	return false;
}

UCardFunctionBase* UCardFunctionFogBlock_3x5::MakeNewClass()
{
	return NewObject<UCardFunctionFogBlock_3x5>();
}

bool UCardFunctionFogBlock_3x5::Execute(class UCardFunctionComponent* CardFuncComp)
{
	FLine CurCardLine = CardFuncComp->GetCardActor()->GetLine();
	FLine RowRange(CurCardLine.Row - 1, CurCardLine.Row + 1);
	FLine ColRange(CurCardLine.Col - 2, CurCardLine.Col + 2);

	AGameMapInstance::GetGameMapFogManager_Static()->RangeBlock(
		CardFuncComp->GetCardActor(),
		false,
		RowRange,
		ColRange
	);
	return true;
}

void UCardFunctionFogBlock_3x5::OnDeath(class UCardFunctionComponent* CardFuncComp)
{
	FLine CurCardLine = CardFuncComp->GetCardActor()->GetLine();
	FLine RowRange(CurCardLine.Row - 1, CurCardLine.Row + 1);
	FLine ColRange(CurCardLine.Col - 2, CurCardLine.Col + 2);

	AGameMapInstance::GetGameMapFogManager_Static()->RangeBlock(
		CardFuncComp->GetCardActor(),
		true,
		RowRange,
		ColRange
	);
}

UCardFunctionBase* UCardFunctionFogBlock_Full::MakeNewClass()
{
	return NewObject<UCardFunctionFogBlock_Full>();
}

bool UCardFunctionFogBlock_Full::Execute(class UCardFunctionComponent* CardFuncComp)
{
	AGameMapInstance::GetGameMapFogManager_Static()->FullFogBlock(CardFuncComp->GetCardActor(), false);

	return true;
}

void UCardFunctionFogBlock_Full::OnDeath(class UCardFunctionComponent* CardFuncComp)
{
	AGameMapInstance::GetGameMapFogManager_Static()->FullFogBlock(CardFuncComp->GetCardActor(), true);
}

UCardFunctionBase* UCardFunctionElectric::MakeNewClass()
{
	return NewObject<UCardFunctionElectric>();
}

bool UCardFunctionElectric::Execute(class UCardFunctionComponent* CardFuncComp)
{
	UDataTable* CurData = LoadObject<UDataTable>(0,
		TEXT("DataTable'/Game/Resource/BP/Data/CardData/CardFunction/DT_Electric_CardFunc.DT_Electric_CardFunc'")
	);
	if (CurData)
	{
		FCardFunctionElectricImplementTRB* CurFindData = CurData->FindRow<FCardFunctionElectricImplementTRB>(
			FName(FString(CardFuncComp->GetCardActor()->GetFunctionCardData().ItemName)),
			TEXT("Electric")
		);
		if (CurFindData)
		{
			this->CardDataTRB = *CurFindData;

			this->fDelay = this->CardDataTRB.Time;
			this->fCurDelay = this->fDelay;
			this->CurActor = CardFuncComp->GetCardActor();

			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("启动雷电面包功能"));
			}

			return true;
		}
		else {
			if (UFVMGameInstance::GetDebug())
			{
				UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("没有找到对应卡片的雷电面包数据，请检查填写数据的Row名称"));
			}
		}
	}
	else {
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(__FUNCTION__, TEXT("功能卡片激活数据表加载失败，请检查路径"));
		}
	}

	return false;
}

bool UCardFunctionElectric::Update(class UCardFunctionComponent* CardFuncComp, const float& DeltaTime)
{
	if (this->fCurDelay > 0.f)
	{
		//如果没有攻击，才会更新时间
		if (!this->bATK)
		{
			this->fCurDelay -= DeltaTime;
		}
	}
	else {
		if (!this->bATK)
		{
			this->fCurDelay = this->fDelay;

			if (this->CheckState(CardFuncComp))
			{
				if (
					IsValid(this->Target)
					&&
					IsValid(this->Target->CurActor)
					&&
					this->Target->CurActor->GetLine().Col == CardFuncComp->GetCardActor()->GetLine().Col
					)
				{
					//攻击逻辑
					this->SetATK();
					this->Target->SetATK();

					//创建碰撞
					int32 CurMin = CardFuncComp->GetCardActor()->GetLine().Row;
					int32 CurMax = this->Target->CurActor->GetLine().Row;
					if (CurMin > CurMax)
					{
						//交换
						Swap(CurMin, CurMax);
					}

					UMesheControllComponent* CurComp = AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent;
					for (int32 i = CurMin; i <= CurMax; i++)
					{
						FVector Loca = CurComp->GetMapMeshLocation(
							i,
							CardFuncComp->GetCardActor()->GetLine().Row
						);

						Loca.Y = CardFuncComp->GetCardActor()->GetActorLocation().Y;

						if (i == CurMin || i == CurMax)
						{
							this->Create(CardFuncComp, Loca, false);
						}
						else {
							this->Create(CardFuncComp, Loca, true);
						}
					}
				}
			}
		}
	}

	return true;
}

bool UCardFunctionElectric::OnAnimPlayEnd(class UCardFunctionComponent* CardFuncComp)
{
	if (this->bATK)
	{
		//切换动画
		/*CardFuncComp->GetCardActor()->SetPlayAnimation(
			UGameSystemFunction::LoadRes(this->CardDataTRB.State1)
		);*/

		CardFuncComp->GetCardActor()->SetAnimation(
			0,
			SpineAnimationState_ElectricBreadCard_Idle,
			true
		);

		this->bATK = false;
		this->Target = nullptr;
	}
	return true;
}

bool UCardFunctionElectric::CheckState(class UCardFunctionComponent* CardFuncComp)
{
	int32 CurRow = CardFuncComp->GetCardActor()->GetLine().Row;
	int32 CurCol = CardFuncComp->GetCardActor()->GetLine().Col;


	UMesheControllComponent* CurComp = AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent;
	//向上查询
	for (int32 i = CurComp->GetMapMeshRowAndCol().Row - 1; i >= 0; i--)
	{
		if (i != CurRow)
		{
			AMapMeshe* CurMeshe = CurComp->GetMapMeshByLine(i, CurCol);
			if (!IsValid(CurMeshe))
			{
				continue;
			}

			//查询模板
			UUI_MapMeshe* CurUI = CurMeshe->GetUIMapMeshe();
			if (IsValid(CurUI))
			{
				ACardActor* CurCard = CurUI->GetCardByID(0);
				if (IsValid(CurCard))
				{
					AFunctionCardActor* Cur = Cast<AFunctionCardActor>(CurCard);
					if (IsValid(Cur))
					{
						//获取功能
						UCardFunctionElectric* CurFunc = Cast<UCardFunctionElectric>(Cur->GetCurrentExecuteCardFuncClass());
						if (IsValid(CurFunc))
						{
							if (!IsValid(CurFunc->Target))
							{
								this->Target = CurFunc;
								return true;
							}
						}
					}
				}
			}
		}
	}

	//向下查询
	for (int32 i = 0; i < CurComp->GetMapMeshRowAndCol().Row; i++)
	{
		if (i != CurRow)
		{
			//查询模板
			UUI_MapMeshe* CurUI = CurComp->GetMapMesh(i, CurCol)->GetUIMapMeshe();
			if (IsValid(CurUI))
			{
				ACardActor* CurCard = CurUI->GetCardByID(0);
				if (IsValid(CurCard))
				{
					AFunctionCardActor* Cur = Cast<AFunctionCardActor>(CurCard);
					if (IsValid(Cur))
					{
						//获取功能
						UCardFunctionElectric* CurFunc = Cast<UCardFunctionElectric>(Cur->GetCurrentExecuteCardFuncClass());
						if (IsValid(CurFunc))
						{
							if (!IsValid(CurFunc->Target))
							{
								this->Target = CurFunc;
								return true;
							}
						}
					}
				}
			}
		}
	}

	return false;
}

void UCardFunctionElectric::SetATK()
{
	if (IsValid(this->CurActor))
	{
	/*	this->CurActor->SetPlayAnimation(
			UGameSystemFunction::LoadRes(this->CardDataTRB.State2)
		);*/

		this->CurActor->SetAnimation(
			0,
			SpineAnimationState_ElectricBreadCard_Attack,
			true
		);

		this->bATK = true;
	}
}

void UCardFunctionElectric::Create(UCardFunctionComponent* CardFuncComp, FVector Location, bool bShow)
{
	const FItemCardFunction& CurCardData = CardFuncComp->GetCardActor()->GetFunctionCardData();

	//设置攻击力
	float CurATK = UGameSystemFunction::CalculateCardATK(
		this->CardDataTRB.AtkBase, this->CardDataTRB.AtkRate,
		CardFuncComp->GetCardActor()->GetCardGrade(CurCardData.ItemName),
		CurCardData.M_M_ECardUpGradeUpRate);

	for (const auto& CurType : this->CardDataTRB.MouseType)
	{
		TArray<FHitResult> Hits;
		TArray<AActor*> Actors;
		if (UKismetSystemLibrary::SphereTraceMulti(
			CardFuncComp->GetWorld(),
			Location,
			Location,
			15.f,
			UGameSystemFunction::GetMouseCollisionTraceType(CurType)
			, false, Actors, EDrawDebugTrace::ForDuration, Hits, false
		))
		{

			for (const auto& CurHit : Hits)
			{
				AMouseActor* CurMouseClass = Cast<AMouseActor>(CurHit.GetActor());
				if (IsValid(CurMouseClass))
				{
					if (CurMouseClass->BeHit(CardFuncComp->GetCardActor(),
						CurATK, EFlyItemAttackType::Bomb))
					{
						CurMouseClass->SetbIsHurt(true);
					}
				}
			}
		}
	}

	if (bShow)
	{
		AActor* CurActorAnim = CardFuncComp->GetCardActor()->GetWorld()->SpawnActor<AActor>(
			UGameSystemFunction::LoadRes(this->CardDataTRB.ATKAnim)
		);
		//设置坐标
		CurActorAnim->SetActorLocation(Location);
	}

}
