// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Boss/Boss_FL.h"
#include "GameStart/Flipbook/GameActor/Mouse/Boss/Boss_FL2.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/MagicMasterMouse.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include <Components/SphereComponent.h>
#include <Components/BoxComponent.h>

//调用父类构造
ABoss_FL::ABoss_FL() : ABossSphereBase()
{

}

void ABoss_FL::BeginPlay()
{
	Super::BeginPlay();

	//初始化碰撞位置
	UGameSystemFunction::InitMouseMeshe(this->MMeshe, this->MBody, FVector2D(20.f), FVector2D(0.f));
}

void ABoss_FL::MouseInit()
{
	Super::MouseInit();

	if (this->bBaseState)
	{
		this->InitBoss(TEXT("法老原形"), TEXT("583"));
	}
	else
	{
		this->InitBoss(TEXT("法老真身"), TEXT("583"));
	}

	if (this->bBaseState)
	{
		this->ChangeState(NewObject<UFL_Out>());
	}
	else
	{
		this->ChangeState(NewObject<UFLB_Create>());
	}
}

void ABoss_FL::ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime)
{
	if (IsValid(this->Get()))
	{
		this->Get()->ExecuteBuff(BuffTag, CurBuffTime);
	}
}

void ABoss_FL::MouseDeathed()
{
	Super::MouseDeathed();

	this->ClosedBoxComponent(this->MMeshe);
	this->ClosedBoxComponent(this->MBody);

	if (this->bBaseState)
	{
		/*this->SetPlayAnimation(
			UGameSystemFunction::LoadRes(this->AnimA_ExitLow), true
		);*/

		this->SetAnimation(0, TEXT("SpawnTag"),true);
	}
	else {
		/*this->SetPlayAnimation(
			UGameSystemFunction::LoadRes(this->AnimB_Death), true
		);*/

		this->SetAnimation(0, TEXT("SpawnTag"),true);
	}
}

bool ABoss_FL::RemoveCallBox(FLine Line)
{
	if (this->CurValidBox.Num() == 0)
	{
		return false;
	}

	bool bRe = false;
	TArray<AFL_CallBox*> ValidBox;
	for (AFL_CallBox* CurBox : this->CurValidBox)
	{
		if (IsValid(CurBox) && CurBox->GetCurrentHP() > 0.f)
		{

			UE_LOG(LogTemp, Error, TEXT("%s"), *UGameSystemFunction::GetObjectName(CurBox));

			if (
				CurBox->GetMouseLine().Row == Line.Row
				&&
				CurBox->GetMouseLine().Col == Line.Col
				)
			{
				UE_LOG(LogTemp, Error, TEXT("找到相同的路线对象，%s"), *UGameSystemFunction::GetObjectName(CurBox));
				CurBox->SetMouseDeath(true);
				bRe = true;
			}
			else {
				ValidBox.Emplace(CurBox);
			}
		}
	}

	this->CurValidBox.Empty();
	this->CurValidBox = ValidBox;

	return bRe;
}

ABoss_FL* UFL_BaseState::GetA()
{
	return Cast<ABoss_FL>(this->Get());
}

FLine UFL_BaseState::GetLine()
{
	return this->GetA()->GetMouseLine();
}

AMapMeshe* UFL_BaseState::GetMeshe(int32 Row, int32 Col)
{
	return this->GetA()->GetMesheComp()->GetMapMesh(Row, Col);
}

FVector UFL_BaseState::GetMesheLocation(int32 Row, int32 Col)
{
	return this->GetA()->GetMesheComp()->GetMapMeshLocation(Row, Col);
}

FLine UFL_BaseState::GetRowAndCol()
{
	return this->GetA()->GetMesheComp()->GetMapMeshRowAndCol();
}

int32 UFL_BaseState::GetLevel()
{
	return this->GetA()->GetMouseLevel();
}

void UFL_BaseState::PlayOpacity(bool bStart)
{
	if (IsValid(this->TimeLine))
	{
		if (bStart)
		{
			this->TimeLine->PlayFromStart();
		}
		else {
			this->TimeLine->ReverseFromEnd();
		}
	}
}

void UFL_BaseState::Init()
{
	this->TimeLine = UTimeLineClass::MakeTimeLineClass();
	this->TimeLine->AddCurve(
		this->GetA()->MouseOpacityCurve, this->Get(),
		[](UTimeLineClass* time, UObject* obj, float value) {
			//Cast<ABoss_FL>(obj)->GetRenderComponent()->SetScalarParameterValueOnMaterials(FName(TEXT("A")), value);
		},
		[](UTimeLineClass* time, UObject* obj) {

		}
		);
}

void UFL_BaseState::MouseTick(const float& DeltaTime)
{
	if (IsValid(this->TimeLine))
	{
		this->TimeLine->Tick(DeltaTime);
	}
}

void UFL_BaseState::ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime)
{
	if (
		BuffTag == EGameBuffTag::Freeze
		||
		BuffTag == EGameBuffTag::Solidification
		||
		BuffTag == EGameBuffTag::SlowDown
		)
	{
		CurBuffTime = 0.f;
	}
}

void UFL_Out::Init()
{
	Super::Init();

	this->PlayOpacity(true);

	this->PlayAnim(
		UGameSystemFunction::LoadRes(this->GetA()->AnimA_Out),
		UGameSystemFunction::LoadRes(this->GetA()->AnimA_OutLow)
	);

	//随机选择路线
	int32 RanRow = UGameSystemFunction::GetRandomRange(0, this->GetRowAndCol().Row - 1);
	int32 ConstCol = this->GetRowAndCol().Col - 1;

	//切换路线
	this->GetA()->GetMouseManager()->ForceChangeLine(
		UGameSystemFunction::GetObjectName(this->GetA()),
		this->GetLine().Row, RanRow, ConstCol
	);

	//设置老鼠进入网格
	this->GetA()->InMapMeshe(this->GetMeshe(RanRow, ConstCol)->GetLineType());
	this->GetA()->CloseInWaterTimeLine();

	//设置老鼠目标位置
	this->GetA()->SetActorLocation(this->GetMesheLocation(RanRow, ConstCol));
}

void UFL_Out::AnimPlayEnd()
{
	//切换状态【法老真身】【发呆】
	if (this->GetLevel() <= 2)
	{
		this->GetA()->ChangeState(NewObject<UFL_Idle>());
	}
	else {
		if (this->GetA()->GetCurrentHP() < this->GetA()->GetTotalHP() * 0.4f)
		{
			if (UGameSystemFunction::GetRange(80, 100))
			{
				this->GetA()->ChangeState(NewObject<UFL_Idle>());
			}
			else
			{
				//变身法老真身
				this->GetA()->ChangeState(NewObject<UFL_ReverseHP>());
			}
		}
		else {
			this->GetA()->ChangeState(NewObject<UFL_Idle>());
		}
	}
}

void UFL_Exit::Init()
{
	Super::Init();

	this->PlayOpacity(false);

	this->PlayAnim(
		UGameSystemFunction::LoadRes(this->GetA()->AnimA_Idle),
		UGameSystemFunction::LoadRes(this->GetA()->AnimA_IdleLow)
	);
}

void UFL_Exit::AnimPlayEnd()
{
	this->GetA()->ChangeState(NewObject<UFL_Out>());
}

void UFL_Idle::Init()
{
	Super::Init();

	//发呆次数
	this->IdleCount = 5;

	this->PlayAnim(
		UGameSystemFunction::LoadRes(this->GetA()->AnimA_Idle),
		UGameSystemFunction::LoadRes(this->GetA()->AnimA_IdleLow)
	);
}

void UFL_Idle::BeHit(UObject* CurHitMouseObj, float& _HurtValue, EFlyItemAttackType AttackType)
{
	this->PlayAnim(
		UGameSystemFunction::LoadRes(this->GetA()->AnimA_Idle),
		UGameSystemFunction::LoadRes(this->GetA()->AnimA_IdleLow)
	);
}

void UFL_Idle::AnimPlayEnd()
{
	if (this->IdleCount > 0)
	{
		this->IdleCount--;
	}
	else {
		//切换形态【选择技能】
		if (UGameSystemFunction::GetRange(50, 100))
		{
			this->GetA()->ChangeState(NewObject<UFL_CallMouse>());
		}
		else {
			this->GetA()->ChangeState(NewObject<UFL_KillCard>());
		}
	}
}

AFL_CallBox::AFL_CallBox()
{
	this->SetMouseTag(EMouseTag::Boss);
}

bool AFL_CallBox::BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType)
{
	if (Super::BeHit(CurHitMouseObj, _HurtValue, AttackType))
	{
		if (this->GetCurrentHP() > this->GetTotalHP() * 0.5f)
		{
			this->OnDefAnim();
		}
		else {
			this->OnDefLowAnim();
		}

		return true;
	}

	return false;
}

void AFL_CallBox::ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime)
{
	CurBuffTime = 0.f;
}

EMouseBaseType AFL_CallBox::GetMouseBaseType()
{
	return EMouseBaseType::Boss;
}

void AFL_CallBox::MouseDeathed()
{
	Super::MouseDeathed();

	this->OnDeathed();
}

void UFL_CallMouse::Init()
{
	this->bInit = false;
	this->bFinish = false;

	if (this->GetA()->GetCurrentHP() > this->GetA()->GetTotalHP() * 0.5f)
	{
		/*this->GetA()->SetPlayAnimationOnce(
			UGameSystemFunction::LoadRes(this->GetA()->AnimA_InitArm),
			UGameSystemFunction::LoadRes(this->GetA()->AnimA_UseLeftArm)
		);*/

		this->Get()->SetAnimation(0, TEXT("SpawnTag"),true);
		this->Get()->SetAnimation(0, TEXT("SpawnTag"),true);
	}
	else {
		/*this->GetA()->SetPlayAnimationOnce(
			UGameSystemFunction::LoadRes(this->GetA()->AnimA_InitArmLow),
			UGameSystemFunction::LoadRes(this->GetA()->AnimA_UseLeftArmLow)
		);*/

		this->Get()->SetAnimation(0, TEXT("SpawnTag"),true);
		this->Get()->SetAnimation(0, TEXT("SpawnTag"),true);
	}

	this->GetA()->DefSpawnMouse(this->GetA()->GetMouseLevel());
}

AFL_CallMouseActor::AFL_CallMouseActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFL_CallMouseActor::Init(UFL_CallMouse* CurState)
{
	if (IsValid(CurState))
	{
		if (IsValid(CurState->GetA()) && CurState->GetA()->GetCurrentHP() > 0.f)
		{
			this->State = CurState;

			if (CurState->GetLevel() == 1)
			{
				this->CallCount = 2;
			}
			else if (CurState->GetLevel() == 2)
			{
				this->CallCount = 3;
			}
			else {
				this->CallCount = 4;
			}

			return;
		}
	}

	this->Destroy();
}

void AFL_CallMouseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (this->CallTimeDelay > 0.f)
	{
		this->CallTimeDelay -= DeltaTime;
	}
	else {
		this->CallTimeDelay = 2.f;

		if (!IsValid(this->State))
		{
			return;
		}

		if (!IsValid(this->State->GetA()))
		{
			return;
		}

		if (this->CallCount <= 0)
		{
			this->Destroy();
			return;
		}

		int32 MaxCount = 12;
		if (!this->State->GetA()->bBaseState)
		{
			MaxCount = 20;
		}
		//检查无效的对象
		this->State->GetA()->RemoveCallBox(FLine(-1, -1));
		if (this->State->GetA()->CurValidBox.Num() >= MaxCount)
		{
			this->Destroy();
			return;
		}

		//随机生成行
		int32 RanRow = UGameSystemFunction::GetRandomRange(0, this->State->GetRowAndCol().Row - 1);
		//随机生成列
		int32 RanCol = UGameSystemFunction::GetRandomRange(3, this->State->GetRowAndCol().Col - 2);

		//获取目标网格
		AMapMeshe* CurMeshe = this->State->GetMeshe(RanRow, RanCol);
		if (IsValid(CurMeshe->GetUIMapMeshe()))
		{
			if (CurMeshe->GetUIMapMeshe()->GetPlayer())
			{
				return;
			}
		}

		//移除对象的盒子
		this->State->GetA()->RemoveCallBox(FLine(RanRow, RanCol));

		//生成棺材
		AMouseActor* CurNewMouse = UMouseSpawnManager::MakeNewMouseByClass(
			this->State->GetA()->GetMouseManager(),
			UGameSystemFunction::LoadRes(this->State->GetA()->CallMouseBoxRes),
			this->State->GetMesheLocation(RanRow, RanCol),
			10, 0.f, 0.f, FLine(RanRow, RanCol), ELineType::OnGround, EMouseTag::Boss
		);
		CurNewMouse->SetRenderLayer(CurMeshe->GetTranslucency());
		CurNewMouse->InMapMeshe(CurMeshe->GetLineType());
		CurNewMouse->SetMouseLine(FLine(RanRow, RanCol));
		Cast<AFL_CallBox>(CurNewMouse)->SpawnBox(CurMeshe, this->State->GetMesheLocation(RanRow, RanCol));
		CurMeshe->SetMesheEnable(false, CurNewMouse);
		this->State->GetA()->CurValidBox.Emplace(Cast<AFL_CallBox>(CurNewMouse));

		//消灭当前所有防御卡
		AMapMeshe* CurMapMeshe = this->State->GetA()->GetMesheComp()->GetMapMesh(
			RanRow, RanCol);
		if (IsValid(CurMapMeshe) && IsValid(CurMapMeshe->GetUIMapMeshe()))
		{
			CurMapMeshe->GetUIMapMeshe()->KillAllCards();
		}

		/*TArray<FHitResult> Cards;
		if (UGameSystemFunction::AddSphereTraceMulti(
			CurNewMouse, Cards, FVector::ZeroVector, FVector::ZeroVector, 10.f,
			UGameSystemFunction::GetCardCollisionTraceType(ECardCollisionType::E_CardActor)
		))
		{
			for (const FHitResult& Cur : Cards)
			{
				ACardActor* CurA = Cast<ACardActor>(Cur.GetActor());
				if (IsValid(CurA) && CurA->GetCurrentHP() > 0.f)
				{
					CurA->UpdateCardState(CurA->GetTotalHP(), 0.f);
					CurA->KillCard();
					UResourceManagerComponent::ResourceAddBadCard();
				}
			}
		}*/

		this->CallCount--;
	}
}

void UFL_CallMouse::AnimPlayEnd()
{
	if (!this->bInit)
	{
		this->bInit = true;

		//召唤老鼠
		this->SpawnMouse();

		return;
	}

	if (!this->bFinish)
	{
		this->bFinish = true;

		//切换形态
		this->GetA()->ChangeState(NewObject<UFL_Exit>());
		return;
	}
}

void UFL_CallMouse::SpawnMouse()
{
	AFL_CallMouseActor* New = this->GetA()->GetWorld()->SpawnActor<AFL_CallMouseActor>(
		AFL_CallMouseActor::StaticClass()
		);
	New->Init(this);
}


AFL_KillCardActor::AFL_KillCardActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFL_KillCardActor::Init(UFL_KillCard* CurState)
{
	if (IsValid(CurState))
	{
		if (IsValid(CurState->GetA()) && CurState->GetA()->GetCurrentHP() > 0.f)
		{
			this->State = CurState;

			if (CurState->GetLevel() == 1)
			{
				this->CallCount = 2;
			}
			else if (CurState->GetLevel() == 2)
			{
				this->CallCount = 3;
			}
			else {
				this->CallCount = 4;
			}

			CurState->GetA()->DefSpawnMouse(2);

			return;
		}
	}

	this->Destroy();
}

void AFL_KillCardActor::Tick(float DeltaTime)
{
	if (this->CallTimeDelay > 0.f)
	{
		this->CallTimeDelay -= DeltaTime;
	}
	else {
		this->CallTimeDelay = 2.f;

		if (!IsValid(this->State))
		{
			return;
		}

		if (!IsValid(this->State->GetA()))
		{
			return;
		}

		if (this->CallCount <= 0)
		{
			this->Destroy();
			return;
		}

		int32 MaxCount = 12;
		if (!this->State->GetA()->bBaseState)
		{
			MaxCount = 20;
		}
		//检查无效的对象
		this->State->GetA()->RemoveCallBox(FLine(-1, -1));
		if (this->State->GetA()->CurValidBox.Num() >= MaxCount)
		{
			this->Destroy();
			return;
		}

		//随机生成行
		int32 RanRow = UGameSystemFunction::GetRandomRange(0, this->State->GetRowAndCol().Row - 1);
		//随机生成列
		int32 RanCol = UGameSystemFunction::GetRandomRange(3, this->State->GetRowAndCol().Col - 2);
		//获取目标网格
		AMapMeshe* CurMeshe = this->State->GetMeshe(RanRow, RanCol);
		if (IsValid(CurMeshe->GetUIMapMeshe()))
		{
			if (CurMeshe->GetUIMapMeshe()->GetPlayer())
			{
				return;
			}
		}
		//移除对象的盒子
		this->State->GetA()->RemoveCallBox(FLine(RanRow, RanCol));

		//生成绷带
		AMouseActor* CurNewMouse = UMouseSpawnManager::MakeNewMouseByClass(
			this->State->GetA()->GetMouseManager(),
			UGameSystemFunction::LoadRes(this->State->GetA()->KillCardBoxRes),
			this->State->GetMesheLocation(RanRow, RanCol),
			10, 0.f, 0.f, FLine(RanRow, RanCol), ELineType::OnGround, EMouseTag::Boss
		);
		CurNewMouse->SetRenderLayer(CurMeshe->GetTranslucency());
		CurNewMouse->InMapMeshe(CurMeshe->GetLineType());
		CurNewMouse->SetMouseLine(FLine(RanRow, RanCol));
		//禁用网格
		CurMeshe->SetMesheEnable(false, CurNewMouse);
		this->State->GetA()->CurValidBox.Emplace(Cast<AFL_CallBox>(CurNewMouse));

		//消灭当前所有防御卡
		AMapMeshe* CurMapMeshe = this->State->GetA()->GetMesheComp()->GetMapMesh(
			RanRow, RanCol);
		if (IsValid(CurMapMeshe) && IsValid(CurMapMeshe->GetUIMapMeshe()))
		{
			CurMapMeshe->GetUIMapMeshe()->KillAllCards();
		}

		/*TArray<FHitResult> Cards;
		if (UGameSystemFunction::AddSphereTraceMulti(
			CurNewMouse, Cards, FVector::ZeroVector, FVector::ZeroVector, 10.f,
			UGameSystemFunction::GetCardCollisionTraceType(ECardCollisionType::E_CardActor)
		))
		{
			for (const FHitResult& Cur : Cards)
			{
				ACardActor* CurA = Cast<ACardActor>(Cur.GetActor());
				if (IsValid(CurA) && CurA->GetCurrentHP() > 0.f)
				{
					CurA->UpdateCardState(CurA->GetTotalHP(), 0.f);
					CurA->KillCard();
					UResourceManagerComponent::ResourceAddBadCard();
				}
			}
		}*/

		this->CallCount--;
	}
}

void UFL_KillCard::Init()
{
	this->bInit = false;
	this->bFinish = false;

	this->GetA()->DefSpawnMouse(this->GetA()->GetMouseLevel());

	if (this->GetA()->GetCurrentHP() > this->GetA()->GetTotalHP() * 0.5f)
	{
		/*this->GetA()->SetPlayAnimationOnce(
			UGameSystemFunction::LoadRes(this->GetA()->AnimA_InitArm),
			UGameSystemFunction::LoadRes(this->GetA()->AnimA_UseRightArm)
		);*/

		this->GetA()->SetAnimation(0, TEXT("SpawnTag"),true);
		this->GetA()->SetAnimation(0, TEXT("SpawnTag"),true);
	}
	else {
		/*this->GetA()->SetPlayAnimationOnce(
			UGameSystemFunction::LoadRes(this->GetA()->AnimA_InitArmLow),
			UGameSystemFunction::LoadRes(this->GetA()->AnimA_UseRightArmLow)
		);*/

		this->Get()->SetAnimation(0, TEXT("SpawnTag"),true);
		this->Get()->SetAnimation(0, TEXT("SpawnTag"),true);
	}
}

void UFL_KillCard::AnimPlayEnd()
{
	if (!this->bInit)
	{
		this->bInit = true;

		//消灭卡片
		this->KillCard();

		return;
	}

	if (!this->bFinish)
	{
		this->bFinish = true;

		//切换形态
		this->GetA()->ChangeState(NewObject<UFL_Exit>());
		return;
	}
}

void UFL_KillCard::KillCard()
{
	AFL_KillCardActor* New = this->GetA()->GetWorld()->SpawnActor<AFL_KillCardActor>(AFL_KillCardActor::StaticClass());
	New->Init(this);
}

void UFL_ReverseHP::Init()
{
	/*this->GetA()->SetPlayAnimationOnce(
		UGameSystemFunction::LoadRes(this->GetA()->AnimB_Create),
		UGameSystemFunction::LoadRes(this->GetA()->AnimB_Use1)
	);*/

	this->Get()->SetAnimation(0, TEXT("SpawnTag"),true);
	this->Get()->SetAnimation(0, TEXT("SpawnTag"),true);

	UGameSystemFunction::AddGameLog(this->GetA(),
		FALD(TEXT("请使用【")) + FAL(TEXT("冰桶炸弹"), TEXT("c")) + FALD(TEXT("】来")) +
		FAL(TEXT("解除"), TEXT("g")) + FALD(TEXT("法老的血量回复！"))
	);

	this->UseCount = UGameSystemFunction::GetRandomRange(5, 10);
}

void UFL_ReverseHP::AnimPlayEnd()
{
	if (this->bInit)
	{
		if (this->UseCount > 0)
		{
			this->UseCount--;

			if (this->GetA()->BeHit(nullptr, this->GetA()->GetTotalHP() * 0.02f * -1.f, EFlyItemAttackType::Def))
			{
				this->GetA()->SetbIsHurt(true);
			}

			//生成加血动画
			AMagicMasterHpAddtionBuff* CurAnimObj = this->GetA()->GetWorld()->SpawnActor<AMagicMasterHpAddtionBuff>();
			CurAnimObj->Init(this->GetA(), this->GetA()->AnimA_HP);

			if (this->UseCount <= 0)
			{
				this->GetA()->ChangeState(NewObject<UFL_Exit>());
			}
		}
		else {
			this->GetA()->ChangeState(NewObject<UFL_Exit>());
		}
	}
	else {
		this->bInit = true;
	}
}

void UFL_ReverseHP::ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime)
{
	if (
		BuffTag == EGameBuffTag::Solidification
		||
		BuffTag == EGameBuffTag::SlowDown
		)
	{
		CurBuffTime = 0.f;
		return;
	}

	if (BuffTag == EGameBuffTag::Freeze)
	{
		this->UseCount = 0;
	}
}

void UFL_CallBug::Init()
{
	this->PlayAnim(
		UGameSystemFunction::LoadRes(this->GetA()->AnimB_Use1),
		UGameSystemFunction::LoadRes(this->GetA()->AnimB_Use1Low)
	);

	UGameSystemFunction::AddGameLog(this->GetA(),
		FALD(TEXT("请使用【")) + FAL(TEXT("木塞子"), TEXT("c")) + FALD(TEXT("】来")) +
		FAL(TEXT("解除"), TEXT("g")) + FALD(TEXT("甲虫洞的甲虫生成！"))
	);
}

void UFL_CallBug::AnimPlayEnd()
{
	AFL_CallBugActor* New = this->GetA()->GetWorld()->SpawnActor<AFL_CallBugActor>();
	New->Init(this);

	this->GetA()->ChangeState(NewObject<UFL_Exit>());
}

void AFL_CallBugActor::Init(UFL_CallBug* CurState)
{
	if (IsValid(CurState))
	{
		if (IsValid(CurState->GetA()) && CurState->GetA()->GetCurrentHP() > 0.f)
		{

			int32 MaxCount = 12;
			if (!CurState->GetA()->bBaseState)
			{
				MaxCount = 20;
			}
			//检查无效的对象
			CurState->GetA()->RemoveCallBox(FLine(-1, -1));
			if (CurState->GetA()->CurValidBox.Num() >= MaxCount)
			{
				this->Destroy();
				return;
			}

			//生成行
			int32 RanRow = CurState->GetA()->GetMouseLine().Row;
			//生成列
			int32 RanCol = CurState->GetRowAndCol().Col - 1;
			//获取目标网格
			AMapMeshe* CurMeshe = CurState->GetMeshe(RanRow, RanCol);
			if (IsValid(CurMeshe->GetUIMapMeshe()))
			{
				if (CurMeshe->GetUIMapMeshe()->GetPlayer())
				{
					return;
				}
			}

			//移除对象的盒子
			CurState->GetA()->RemoveCallBox(FLine(RanRow, RanCol));

			//生成甲虫洞口
			AMouseActor* CurNewMouse = UMouseSpawnManager::MakeNewMouseByClass(
				CurState->GetA()->GetMouseManager(),
				UGameSystemFunction::LoadRes(CurState->GetA()->BugBoxRes),
				CurState->GetMesheLocation(RanRow, RanCol),
				10, 0.f, 0.f, FLine(RanRow, RanCol), ELineType::OnGround, EMouseTag::Boss
			);

			CurNewMouse->SetRenderLayer(CurMeshe->GetTranslucency());
			CurNewMouse->InMapMeshe(CurMeshe->GetLineType());
			CurNewMouse->SetMouseLine(FLine(RanRow, RanCol));
			//禁用网格
			CurMeshe->SetMesheEnable(false, CurNewMouse);
			CurState->GetA()->CurValidBox.Emplace(Cast<AFL_CallBox>(CurNewMouse));
			CurNewMouse->SetRenderLayer(
				TranslucencyConst::GetLayer(TranslucencyConst::ETranslucency::EBgBlock) + 50
			);

			//消灭当前所有防御卡
			AMapMeshe* CurMapMeshe = CurState->GetA()->GetMesheComp()->GetMapMesh(
				RanRow, RanCol);
			if (IsValid(CurMapMeshe) && IsValid(CurMapMeshe->GetUIMapMeshe()))
			{
				CurMapMeshe->GetUIMapMeshe()->KillAllCards();
			}

			/*TArray<FHitResult> Cards;
			if (UGameSystemFunction::AddSphereTraceMulti(
				CurNewMouse, Cards, FVector::ZeroVector, FVector::ZeroVector, 10.f,
				UGameSystemFunction::GetCardCollisionTraceType(ECardCollisionType::E_CardActor)
			))
			{
				for (const FHitResult& Cur : Cards)
				{
					ACardActor* CurA = Cast<ACardActor>(Cur.GetActor());
					if (IsValid(CurA) && CurA->GetCurrentHP() > 0.f)
					{
						CurA->UpdateCardState(CurA->GetTotalHP(), 0.f);
						CurA->KillCard();
						UResourceManagerComponent::ResourceAddBadCard();
					}
				}
			}*/
		}
	}

	this->Destroy();
}
