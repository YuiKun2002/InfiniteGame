// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Boss/Boss_LXQS.h"
#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameStart/VS/GameMapInstance.h"
#include "Paper2D/Classes/PaperFlipbookActor.h"
#include <Kismet/KismetMathLibrary.h>

#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>


ABoss_LXQS::ABoss_LXQS()
{
	this->CurMesheComp = CreateDefaultSubobject<UBoxComponent>(TEXT("MesheColision"));
	this->CurMouseColisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("MouseColision"));

	this->CurMesheComp->SetupAttachment(this->GetPointComponent());
	this->CurMouseColisionComp->SetupAttachment(this->CurMesheComp);
}

void ABoss_LXQS::BeginPlay()
{
	Super::BeginPlay();

	UGameSystemFunction::InitMouseMeshe(this->CurMesheComp, this->CurMouseColisionComp, FVector2D(110.f, 20.f));


}

void ABoss_LXQS::OnResourceLoadFinished()
{
	//设置偏移
	this->SetActorLocation(this->GetActorLocation() + FVector(0.f, 600.f, 0.f));
	//初始化网格组件
	this->MesheCtlComp = AGameMapInstance::GetGameMapInstance()->GetMesheControllComponent();
	//初始化Boss
	this->Init();
	this->CloseInWaterTimeLine();
	this->InMapMeshe(ELineType::OnGround);
	//显示boss
	//this->SetActorHiddenInGame(false);
}

class UMesheControllComponent* ABoss_LXQS::GetMesheCtlComp() const
{
	return this->MesheCtlComp;
}

void ABoss_LXQS::InMapMeshe(ELineType NewType)
{
	if (NewType == ELineType::OnWater)
	{
		Super::InMapMeshe(NewType);

	}
	else {
		//this->InWaterAnim->SetHiddenInGame(true);
		/*this->GetRenderComponent()->
			SetScalarParameterValueOnMaterials(FName(TEXT("Range")), 0);*/
		this->M_Proper_Condition.M_CurrentInType = NewType;
	}
}

void ABoss_LXQS::ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime)
{
	if (BuffTag == EGameBuffTag::SlowDown)
	{
		CurBuffTime = 0.f;
	}

	Super::ExecuteBuff(BuffTag, CurBuffTime);
}

void ABoss_LXQS::MouseInit()
{
	Super::MouseInit();

	this->InitBoss(TEXT("龙虾骑士"), TEXT("629"));

	this->OnResourceLoadFinished();
}

void ABoss_LXQS::MouseDeathed()
{
	Super::MouseDeathed();

	//取消状态模式
	if (IsValid(this->CurStateClass))
	{
		this->CurStateClass = nullptr;
	}

	this->DisEnableCo();

	//设置死亡动画
	//this->SetPlayAnimation(UGameSystemFunction::LoadRes(this->LXQSAnim.Death), true);

	this->SetAnimation(0, SpineMouseDeath, true);

}

void ABoss_LXQS::Init()
{
	//this->OnResourceLoadFinished();

	//解除碰撞
	//this->DisEnableCo();

	//绑定动画播放结束
	//this->GetRenderComponent()->OnAnimationPlayEnd.Unbind();
	//this->GetRenderComponent()->OnAnimationPlayEnd.BindUObject(this, &ABoss_LXQS::AnimPlayEnd);

	//初始化第一个状态
	this->CurStateClass = ABoss_LXQS::MakeLXQSState<ULXQS_BeginState>(this);
}

void ABoss_LXQS::EnableCo()
{
	this->CurMesheComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	this->CurMesheComp->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	this->CurMesheComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	this->CurMesheComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);
	this->CurMesheComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel9, ECollisionResponse::ECR_Block);

	this->CurMouseColisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	this->CurMouseColisionComp->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	this->CurMouseColisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	this->CurMouseColisionComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void ABoss_LXQS::DisEnableCo()
{
	this->CurMesheComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->CurMouseColisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABoss_LXQS::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//更新状态
	if (this->CurStateClass)
	{
		this->CurStateClass->Update(DeltaTime);
	}
}

void ABoss_LXQS::AnimPlayEnd()
{
	if (this->CurStateClass)
	{
		this->CurStateClass->AnimPlayEnd();
	}
}

void ULXQS_StateBase::Init(class ABoss_LXQS* OnwerClass) { this->Onwer = OnwerClass; }
void ULXQS_StateBase::Update(const float& tick) {}
void ULXQS_StateBase::AnimPlayEnd() {}
void ULXQS_StateBase::Finished() {}


void ULXQS_TimeLineStateBase::Init(class ABoss_LXQS* OnwerClass)
{
	Super::Init(OnwerClass);

	//绑定时间轴和曲线【出场】
	this->OnTimelineFloatDelegate.BindUFunction(this, TEXT("TimeLineUpdate"));
	this->OnTimeLineFinishDelegate.BindUFunction(this, TEXT("TimeLineFinish"));
	this->CurTimeLine.SetTimelineFinishedFunc(this->OnTimeLineFinishDelegate);
}

void ULXQS_TimeLineStateBase::Update(const float& tick)
{
	this->CurTimeLine.TickTimeline(tick);
}

void ULXQS_TimeLineStateBase::AnimPlayEnd()
{

}

void ULXQS_TimeLineStateBase::Finished()
{

}

void ULXQS_TimeLineStateBase::TimeLineUpdate(float Value)
{
	if (this->Onwer->GetCurrentHP() <= 0.f)
	{
		return;
	}

	this->Onwer->SetActorLocation(UKismetMathLibrary::VLerp(this->M_CurrentLocation, this->M_TargetLocation, Value));

	//完成目标移动，更换行//在移动过程中应该需要动态的更新行
	this->Onwer->SetMouseLine(
		FLine(
			UKismetMathLibrary::Lerp(
				this->Onwer->GetMouseLine().Row, this->TargetRow, Value),
			this->Onwer->GetMouseLine().Col
		)
	);


	this->Onwer->InMapMeshe(
		this->Onwer->GetMesheCtlComp()->GetMapMesh(
			this->Onwer->GetMouseLine().Row, this->Onwer->GetMesheCtlComp()->GetMapMeshRowAndCol().Col - 1
		)->GetLineType()
	);

}

void ULXQS_TimeLineStateBase::TimeLineFinish()
{
	//添加当前行
	//this->Onwer->GetMouseManager()->AddMouse(this->Onwer,this->Onwer->GetMouseLine().Row);
}

void ULXQS_TimeLineStateBase::Start()
{
	//移除当前行
	//this->Onwer->GetMouseManager()->RemoveMouse(this->Onwer);

	this->CurTimeLine.PlayFromStart();
}

void ULXQS_TimeLineStateBase::SetCurve(class UCurveFloat* CurveClass, const FString& _CurName)
{
	//添加出场曲线
	this->CurTimeLine.AddInterpFloat(
		this->Onwer->StateMoveDownCurve,
		this->OnTimelineFloatDelegate,
		FName("P" + _CurName),
		FName("N" + _CurName)
	);

	//设置默认曲线
	this->CurTimeLine.SetFloatCurve(CurveClass, FName("N" + _CurName));
}




void ULXQS_BeginState::Init(class ABoss_LXQS* OnwerClass)
{
	Super::Init(OnwerClass);

	//播放行走动画
	//this->Onwer->SetPlayAnimation(UGameSystemFunction::LoadRes(this->Onwer->LXQSAnim.MoveFront));

	this->Onwer->SetAnimation(0, TEXT("SpineTag"), true);

	this->TargetRow = this->Onwer->GetMouseLine().Row;
	//初始化位置
	this->M_CurrentLocation = this->Onwer->GetActorLocation();
	//获取网格坐标
	FVector MesheLocation = this->Onwer->GetMesheCtlComp()->GetMapMeshCurrentRight(
		this->TargetRow
	)->GetActorLocation();
	//将网格坐标对齐boss位置
	MesheLocation.Z = this->Onwer->GetActorLocation().Z;
	//初始化目标位置
	this->M_TargetLocation = MesheLocation;
	//设置曲线
	this->SetCurve(this->Onwer->StateBeginCurve, TEXT("BeginState"));
	//播放时间线动画
	this->Start();
	//关闭碰撞
	//this->Onwer->DisEnableCo();
}

void ULXQS_BeginState::Finished()
{
	//this->Onwer->EnableCo();

	this->Onwer->InMapMeshe(
		this->Onwer->GetMesheCtlComp()->GetMapMesh(
			this->Onwer->GetMouseLine().Row, this->Onwer->GetMesheCtlComp()->GetMapMeshRowAndCol().Col - 1
		)->GetLineType()
	);

	//出场结束，选择路线移动或者开始攻击
	int32 RandomSelect = UKismetMathLibrary::RandomIntegerInRange(0, 2);
	switch (RandomSelect)
	{
	case 1: {
		//上下移动
		this->Onwer->CurStateClass = ABoss_LXQS::MakeLXQSState<ULXQS_MoveDownState>(this->Onwer);
	}return;
	case 2: {
		//发射泡泡
		this->Onwer->CurStateClass = ABoss_LXQS::MakeLXQSState<ULXQS_ShootBubbleState>(this->Onwer);
	}return;
	default:
	{
		//发射导弹
		this->Onwer->CurStateClass = ABoss_LXQS::MakeLXQSState<ULXQS_ShootMissileState>(this->Onwer);
	}return;
	}

}

void ULXQS_BeginState::TimeLineFinish()
{
	Super::TimeLineFinish();

	//播放默认状态的动画
	if (this->Onwer->GetCurrentHP() > (this->Onwer->GetTotalHP() * 0.55f))
	{
		/*this->Onwer->SetPlayAnimationOnce(this->Onwer->GetRenderComponent()->GetFlipbook(),
			UGameSystemFunction::LoadRes(this->Onwer->LXQSAnim.Idle));*/

		this->Onwer->SetAnimation(0, TEXT("SpineTag"), true);

		this->Onwer->SetAnimation(0, TEXT("SpineTag"), true);
	}
	else
	{
		/*this->Onwer->SetPlayAnimationOnce(this->Onwer->GetRenderComponent()->GetFlipbook(),
			UGameSystemFunction::LoadRes(this->Onwer->LXQSAnim.IdleLow));*/

		this->Onwer->SetAnimation(0, TEXT("SpineTag"), true);

		this->Onwer->SetAnimation(0, TEXT("SpineTag"), true);
	}


	this->Finished();
}

void ULXQS_MoveDownState::Init(class ABoss_LXQS* OnwerClass)
{
	Super::Init(OnwerClass);

	//this->Onwer->DisEnableCo();

	TArray<int32> CurRows;
	for (int32 i = 0; i < this->Onwer->GetMesheCtlComp()->GetMapMeshRowAndCol().Row; i++)
	{
		if (this->Onwer->GetMouseLine().Row == i)
		{
			continue;
		}
		CurRows.Emplace(i);
	}

	int32 RanIndex = CurRows[UKismetMathLibrary::RandomIntegerInRange(0, CurRows.Num() - 1)];

	//需要移动到x行
	int32 RanLine = RanIndex;

	//播放上下移动的动画
	if (this->Onwer->GetCurrentHP() > (this->Onwer->GetTotalHP() * 0.55f))
	{
		//播放行走动画
		//this->Onwer->SetPlayAnimation(UGameSystemFunction::LoadRes(this->Onwer->LXQSAnim.MoveDown), true);

		this->Onwer->SetAnimation(0, TEXT("SpineTag"), true);
	}
	else
	{
		//播放行走动画
		//this->Onwer->SetPlayAnimation(UGameSystemFunction::LoadRes(this->Onwer->LXQSAnim.MoveDownLow), true);

		this->Onwer->SetAnimation(0, TEXT("SpineTag"), true);
	}


	this->TargetRow = RanLine;
	//初始化位置
	this->M_CurrentLocation = this->Onwer->GetActorLocation();
	//获取网格坐标
	FVector MesheLocation = this->Onwer->GetMesheCtlComp()->GetMapMeshCurrentRight(RanLine)->GetActorLocation();
	//将网格坐标对齐boss位置
	MesheLocation.Z -= 10;
	//切换行
	AGameMapInstance::GetMouseManagerComponent_Static()->ChangeLine(
		UKismetSystemLibrary::GetObjectName(this->Onwer), this->Onwer->GetMouseLine().Row, this->TargetRow
	);
	//初始化目标位置
	this->M_TargetLocation = MesheLocation;
	//设置排序
	this->Onwer->SetRenderLayer(
		this->Onwer->GetMesheCtlComp()->GetMapMeshCurrentRight(RanLine)->GetTranslucency()
	);
	//设置曲线
	this->SetCurve(this->Onwer->StateMoveDownCurve, TEXT("StateMoveDownCurve"));
	//播放时间线动画
	this->Start();
}

void ULXQS_MoveDownState::Update(const float& tick)
{
	if (this->Onwer->GetCurrentHP() <= 0.f)
	{
		return;
	}

	Super::Update(tick);

	if (this->bMoveFinish)
	{
		//this->bMoveFinish = false;
		//this->Finished();

		if (this->FinishDalay > 0.f)
		{
			this->FinishDalay -= tick;
		}
		else {
			this->bMoveFinish = false;
			this->Finished();
		}

	}
}

void ULXQS_MoveDownState::Finished()
{
	this->Onwer->InMapMeshe(
		this->Onwer->GetMesheCtlComp()->GetMapMesh(
			this->Onwer->GetMouseLine().Row, this->Onwer->GetMesheCtlComp()->GetMapMeshRowAndCol().Col - 1
		)->GetLineType()
	);

	//上下移动结束一定是技能的使用
	//出场结束，选择路线移动或者开始攻击
	int32 RandomSelect = UKismetMathLibrary::RandomIntegerInRange(0, 2);
	switch (RandomSelect)
	{
	case 1: {
		//翻滚移动
		this->Onwer->CurStateClass = ABoss_LXQS::MakeLXQSState<ULXQS_RotationMoveState>(this->Onwer);
	}return;
	case 2: {
		//发射泡泡
		this->Onwer->CurStateClass = ABoss_LXQS::MakeLXQSState<ULXQS_ShootBubbleState>(this->Onwer);
	}return;
	default:
	{
		//发射导弹
		this->Onwer->CurStateClass = ABoss_LXQS::MakeLXQSState<ULXQS_ShootMissileState>(this->Onwer);
	}return;
	}
}

void ULXQS_MoveDownState::TimeLineFinish()
{
	Super::TimeLineFinish();

	//播放默认状态的动画
	if (this->Onwer->GetCurrentHP() > (this->Onwer->GetTotalHP() * 0.55f))
	{
		/*this->Onwer->SetPlayAnimationOnce(this->Onwer->GetRenderComponent()->GetFlipbook(),
			UGameSystemFunction::LoadRes(this->Onwer->LXQSAnim.Idle));*/

		this->Onwer->SetAnimation(0, TEXT("SpineTag"), true);
		this->Onwer->SetAnimation(0, TEXT("SpineTag"), true);
	}
	else
	{
		/*this->Onwer->SetPlayAnimationOnce(this->Onwer->GetRenderComponent()->GetFlipbook(),
			UGameSystemFunction::LoadRes(this->Onwer->LXQSAnim.IdleLow));*/

		this->Onwer->SetAnimation(0, TEXT("SpineTag"), true);
		this->Onwer->SetAnimation(0, TEXT("SpineTag"), true);
	}

	this->bMoveFinish = true;
	//this->Onwer->EnableCo();
}






void ULXQS_ShootMissileState::Init(class ABoss_LXQS* OnwerClass)
{
	Super::Init(OnwerClass);

	//this->Onwer->EnableCo();

	//初始化设置位置
	//初始化瞄准的行位置
	this->ShootTarget.Row = UKismetMathLibrary::RandomIntegerInRange(0, OnwerClass->GetMesheCtlComp()->GetMapMeshRowAndCol().Row - 1);
	this->ShootTarget.Col = OnwerClass->GetMesheCtlComp()->GetMapMeshRowAndCol().Col / 2;

	//变换位置
	FTransform Trans;
	Trans.SetLocation(OnwerClass->GetMesheCtlComp()->GetMapMeshLocation(this->ShootTarget.Row, this->ShootTarget.Col));
	Trans.SetRotation(FQuat::MakeFromEuler(FVector(0.f, 0.f, 90.f)));

	//创建瞄准图标
	OnwerClass->GetWorld()->SpawnActor<APaperFlipbookActor>(
		UGameSystemFunction::LoadRes(OnwerClass->MouseAimLog),
		Trans
	);

	//播放上下移动的动画
	if (this->Onwer->GetCurrentHP() > (this->Onwer->GetTotalHP() * 0.55f))
	{
		//播放行走动画
		/*this->Onwer->SetPlayAnimationOnce(UGameSystemFunction::LoadRes(this->Onwer->LXQSAnim.Shoot_1),
			UGameSystemFunction::LoadRes(this->Onwer->LXQSAnim.Idle));*/
		this->Onwer->SetAnimation(0, TEXT("SpineTag"), true);
		this->Onwer->SetAnimation(0, TEXT("SpineTag"), true);
	}
	else
	{
		//播放行走动画
		/*this->Onwer->SetPlayAnimationOnce(UGameSystemFunction::LoadRes(this->Onwer->LXQSAnim.Shoot_1Low),
			UGameSystemFunction::LoadRes(this->Onwer->LXQSAnim.IdleLow));*/
		this->Onwer->SetAnimation(0, TEXT("SpineTag"), true);
		this->Onwer->SetAnimation(0, TEXT("SpineTag"), true);
	}
}

void ULXQS_ShootMissileState::Update(const float& tick)
{
	//当导弹发射完毕后，3-6秒延迟之后，在做其他决定
	if (!this->bShootFinish && this->bShoot)
	{
		if (this->ShootDelay > 0.f)
		{
			this->ShootDelay -= tick;
		}
		else {
			if (this->ColNum != 0)
			{
				this->ShootDelay = 0.1f;
				//创建导弹
				//变换位置
				FTransform Trans;
				Trans.SetLocation(this->Onwer->GetMesheCtlComp()->GetMapMeshLocation(this->ShootTarget.Row, this->ColNum - 1));
				//创建导弹
				this->Onwer->GetWorld()->SpawnActor<AGameActorFlipbookBase>(
					UGameSystemFunction::LoadRes(this->Onwer->MouseMissileClass),
					Trans
				);

				this->ColNum--;
			}
			else {
				//导弹射击完成
				this->bShootFinish = true;
				this->Finished();
				return;
			}
		}
	}
}

void ULXQS_ShootMissileState::AnimPlayEnd()
{
	//如果没有发射导弹，则发射导弹
	if (!this->bShoot)
	{
		//初始化射击条件
		this->bShoot = true;
		this->bShootFinish = false;
		//初始化列数量
		this->ColNum = this->Onwer->GetMesheCtlComp()->GetMapMeshRowAndCol().Col;
	}
}

void ULXQS_ShootMissileState::Finished()
{
	this->Onwer->InMapMeshe(
		this->Onwer->GetMesheCtlComp()->GetMapMesh(
			this->Onwer->GetMouseLine().Row, this->Onwer->GetMesheCtlComp()->GetMapMeshRowAndCol().Col - 1
		)->GetLineType()
	);

	if (this->Onwer->GetCurrentHP() > (this->Onwer->GetTotalHP() * 0.55f))
	{
		//开始上下移动
		this->Onwer->CurStateClass = ABoss_LXQS::MakeLXQSState<ULXQS_MoveDownState>(this->Onwer);
	}
	else {
		int32 RandomSelect = UKismetMathLibrary::RandomIntegerInRange(0, 2);
		switch (RandomSelect)
		{
		case 1: {
			this->Onwer->CurStateClass = ABoss_LXQS::MakeLXQSState<ULXQS_MoveDownState>(this->Onwer);
		}return;
		case 2: {
			//发射泡泡
			this->Onwer->CurStateClass = ABoss_LXQS::MakeLXQSState<ULXQS_ShootBubbleState>(this->Onwer);
		}return;
		default:
		{
			//发射导弹
			this->Onwer->CurStateClass = ABoss_LXQS::MakeLXQSState<ULXQS_ShootMissileState>(this->Onwer);
		}return;
		}
	}
}

void ULXQS_ShootBubbleState::Init(class ABoss_LXQS* OnwerClass)
{
	Super::Init(OnwerClass);

	//this->Onwer->EnableCo();

	//播放上下移动的动画
	if (this->Onwer->GetCurrentHP() > (this->Onwer->GetTotalHP() * 0.55f))
	{
		//播放行走动画
		/*this->Onwer->SetPlayAnimationOnce(UGameSystemFunction::LoadRes(this->Onwer->LXQSAnim.Shoot_2),
			UGameSystemFunction::LoadRes(this->Onwer->LXQSAnim.Idle));*/

		this->Onwer->SetAnimation(0, TEXT("SpineTag"), true);
		this->Onwer->SetAnimation(0, TEXT("SpineTag"), true);
	}
	else
	{
		//播放行走动画
		/*this->Onwer->SetPlayAnimationOnce(UGameSystemFunction::LoadRes(this->Onwer->LXQSAnim.Shoot_2Low),
			UGameSystemFunction::LoadRes(this->Onwer->LXQSAnim.IdleLow));*/

		this->Onwer->SetAnimation(0, TEXT("SpineTag"), true);
		this->Onwer->SetAnimation(0, TEXT("SpineTag"), true);
	}

	this->bPlay = true;
	this->CardMesheBuffer.Empty();

	for (auto Meshe : this->Onwer->GetMesheCtlComp()->GetAllMapMeshes())
	{
		AMapMeshe* CardMeshe = Cast<AMapMeshe>(Meshe);

		if (!IsValid(CardMeshe))
		{
			continue;
		}

		if (!IsValid(CardMeshe->GetUIMapMeshe()))
		{
			continue;
		}

		//测试带有苏打气泡的网格
		if (!CardMeshe->GetUIMapMeshe()->TestID(3))
		{
			this->CardMesheBuffer.Emplace(CardMeshe);
			continue;
		}
	}

	if (this->CardMesheBuffer.Num() == 0)
	{
		this->bPlay = false;
		this->bPlayEnd = true;
		this->ShootCount = 0;

		return;
	}

	//初始化射击位置
	for (int32 i = 0; i < this->ShootCount; i++)
	{

		int32 RandomSelect = UKismetMathLibrary::RandomIntegerInRange(0, this->CardMesheBuffer.Num() - 1);

		//变换位置
		FTransform Trans;
		Trans.SetLocation(this->Onwer->GetMesheCtlComp()->GetMapMeshLocation(
			this->CardMesheBuffer[RandomSelect]->GetLine().Row,
			this->CardMesheBuffer[RandomSelect]->GetLine().Col
		));
		Trans.SetRotation(FQuat::MakeFromEuler(FVector(0.f, 0.f, 90.f)));
		//创建瞄准图标
		this->Onwer->GetWorld()->SpawnActor<APaperFlipbookActor>(
			UGameSystemFunction::LoadRes(this->Onwer->MouseAimLog),
			Trans
		);

		//加入射击目标
		this->TargetCardMeshes.Emplace(this->CardMesheBuffer[RandomSelect]);

		//移动当前移除网格
		this->CardMesheBuffer.RemoveAt(RandomSelect);

		if (this->CardMesheBuffer.Num() == 0)
		{
			this->ShootCount = this->TargetCardMeshes.Num();
			break;
		}
	}
}

void ULXQS_ShootBubbleState::Update(const float& tick)
{
	if (this->bPlay)
	{
		//前摇
		if (this->FirstDelay > 0.f)
		{
			this->FirstDelay -= tick;
		}
		else
		{
			//射击延迟
			if (this->ShootDelay > 0.f)
			{
				this->ShootDelay -= tick;
			}
			else {

				this->ShootDelay = 0.5f;

				if (this->ShootCount > 0)
				{
					this->ShootCount--;

					//射击
					AMapMeshe* CurMeshe = this->TargetCardMeshes[this->ShootCount];

					//创建泡泡
					//变换位置
					FTransform Trans;
					Trans.SetLocation(this->Onwer->GetMesheCtlComp()->GetMapMeshLocation(
						CurMeshe->GetLine().Row,
						CurMeshe->GetLine().Col
					));

					//创建泡泡
					ALXQS_BubbleBomb* BubbleBomb = this->Onwer->GetWorld()->SpawnActor<ALXQS_BubbleBomb>(
						UGameSystemFunction::LoadRes(this->Onwer->MouseBubbleClass),
						Trans
					);
					//给泡泡初始化网格
					BubbleBomb->CurMehshe = CurMeshe;
				}
				else {
					//播放上下移动的动画
					if (this->Onwer->GetCurrentHP() > (this->Onwer->GetTotalHP() * 0.55f))
					{
						//播放行走动画
						//this->Onwer->SetPlayAnimation(UGameSystemFunction::LoadRes(this->Onwer->LXQSAnim.Idle));

						this->Onwer->SetAnimation(0, TEXT("SpineTag"), true);
					}
					else
					{
						//播放行走动画
						//this->Onwer->SetPlayAnimation(UGameSystemFunction::LoadRes(this->Onwer->LXQSAnim.IdleLow));

						this->Onwer->SetAnimation(0, TEXT("SpineTag"), true);
					}

					this->bPlay = false;
					this->bPlayEnd = true;
				}
			}
		}
	}
}

//泡泡
ACardActor* ALXQS_BubbleBomb::GetCard()
{
	//发现苏打气泡
	if (!this->CurMehshe->GetUIMapMeshe()->TestID(3))
	{
		return this->CurMehshe->GetUIMapMeshe()->GetCardByID(3);
	}
	return nullptr;
}


void ULXQS_ShootBubbleState::AnimPlayEnd() {
	if (!this->bPlay && this->bPlayEnd)
	{
		this->bPlayEnd = false;

		this->Finished();
	}
}

void ULXQS_ShootBubbleState::Finished()
{
	this->Onwer->InMapMeshe(
		this->Onwer->GetMesheCtlComp()->GetMapMesh(
			this->Onwer->GetMouseLine().Row, this->Onwer->GetMesheCtlComp()->GetMapMeshRowAndCol().Col - 1
		)->GetLineType()
	);

	//上下移动
	this->Onwer->CurStateClass = ABoss_LXQS::MakeLXQSState<ULXQS_MoveDownState>(this->Onwer);
}




void ULXQS_RotationMoveState::Init(class ABoss_LXQS* OnwerClass)
{
	Super::Init(OnwerClass);

	//设置曲线
	this->SetCurve(this->Onwer->StateRotationMoveCurve, TEXT("StateRotationMoveCurve"));

	this->bRotationMove = true;
	this->bBack = false;

	if (this->Onwer->GetCurrentHP() > (this->Onwer->GetTotalHP() * 0.55f))
	{
		//播放滚动的动画
		/*this->Onwer->SetPlayAnimationOnce(UGameSystemFunction::LoadRes(this->Onwer->LXQSAnim.RotationMoveBegin),
			UGameSystemFunction::LoadRes(this->Onwer->LXQSAnim.RotationMove));*/

		this->Onwer->SetAnimation(0, TEXT("SpineTag"), true);
		this->Onwer->SetAnimation(0, TEXT("SpineTag"), true);
	}
	else
	{
		//播放滚动的动画
		/*this->Onwer->SetPlayAnimationOnce(UGameSystemFunction::LoadRes(this->Onwer->LXQSAnim.RotationMoveBeginLow),
			UGameSystemFunction::LoadRes(this->Onwer->LXQSAnim.RotationMove));*/

		this->Onwer->SetAnimation(0, TEXT("SpineTag"), true);
		this->Onwer->SetAnimation(0, TEXT("SpineTag"), true);
	}
}

void ULXQS_RotationMoveState::Update(const float& tick)
{
	Super::Update(tick);

	if (!this->bBackEnd && this->bBack && !this->bRotationMove)
	{
		if (this->bBackRotationDelay > 0.f)
		{
			this->bBackRotationDelay -= tick;
		}
		else {
			this->bRotationMove = true;
		}
	}

	//正在翻滚，创建压痕
	if (this->bRotation)
	{
		if (this->CreateItemDelay > 0.f)
		{
			this->CreateItemDelay -= tick;
		}
		else {
			this->CreateItemDelay = 0.5f;

			FTransform Trans;
			Trans.SetLocation(this->Onwer->GetActorLocation());
			//创建压痕
			this->Onwer->GetWorld()->SpawnActor<AGameActorFlipbookBase>(
				UGameSystemFunction::LoadRes(this->Onwer->MouseRotationMoveItem),
				Trans
			);
		}
	}
}

void ULXQS_RotationMoveState::AnimPlayEnd()
{
	if (this->bRotationMove)
	{
		this->bRotationMove = false;

		if (this->bBack)//往返
		{
			this->bBackEnd = true;

			if (this->Onwer->GetCurrentHP() > (this->Onwer->GetTotalHP() * 0.55f))
			{
				//播放滚动的动画
				/*this->Onwer->SetPlayAnimationOnce(
					UGameSystemFunction::LoadRes(this->Onwer->LXQSAnim.RotationMoveBegin),
					UGameSystemFunction::LoadRes(this->Onwer->LXQSAnim.RotationMove)
				);*/
				this->Onwer->SetAnimation(0, TEXT("SpineTag"), true);
				this->Onwer->SetAnimation(0, TEXT("SpineTag"), true);
			}
			else
			{
				//播放滚动的动画
				/*this->Onwer->SetPlayAnimationOnce(
					UGameSystemFunction::LoadRes(this->Onwer->LXQSAnim.RotationMoveBeginLow),
					UGameSystemFunction::LoadRes(this->Onwer->LXQSAnim.RotationMove));*/

				this->Onwer->SetAnimation(0, TEXT("SpineTag"), true);
				this->Onwer->SetAnimation(0, TEXT("SpineTag"), true);
			}

			//寻找移动目标
			int32 RanRow = UKismetMathLibrary::RandomIntegerInRange(0, this->Onwer->GetMesheCtlComp()->GetMapMeshRowAndCol().Row - 1);
			this->TargetRow = RanRow;
			//初始化位置
			this->M_CurrentLocation = this->Onwer->GetActorLocation();
			//获取网格坐标
			FVector MesheLocation = this->Onwer->GetMesheCtlComp()->GetMapMesh(RanRow,
				this->Onwer->GetMesheCtlComp()->GetMapMeshRowAndCol().Col - 1)->GetActorLocation();
			//将网格坐标对齐boss位置
			MesheLocation.Z -= 10;
			//切换行
			AGameMapInstance::GetMouseManagerComponent_Static()->ChangeLine(
				UKismetSystemLibrary::GetObjectName(this->Onwer), this->Onwer->GetMouseLine().Row, this->TargetRow
			);
			//初始化目标位置
			this->M_TargetLocation = MesheLocation;
			//设置排序
			this->Onwer->SetRenderLayer(
				this->Onwer->GetMesheCtlComp()->GetMapMesh(RanRow,
					this->Onwer->GetMesheCtlComp()->GetMapMeshRowAndCol().Col - 1)->GetTranslucency());
			//播放时间线动画
			this->Start();
		}
		else {
			//寻找移动目标
			int32 RanRow = UKismetMathLibrary::RandomIntegerInRange(0, this->Onwer->GetMesheCtlComp()->GetMapMeshRowAndCol().Row - 1);
			this->TargetRow = RanRow;
			//初始化位置
			this->M_CurrentLocation = this->Onwer->GetActorLocation();
			//获取网格坐标
			FVector MesheLocation = this->Onwer->GetMesheCtlComp()->GetMapMesh(RanRow, 1)->GetActorLocation();
			//将网格坐标对齐boss位置
			MesheLocation.Z -= 10;
			//切换行
			AGameMapInstance::GetMouseManagerComponent_Static()->ChangeLine(
				UKismetSystemLibrary::GetObjectName(this->Onwer), this->Onwer->GetMouseLine().Row, this->TargetRow
			);
			//初始化目标位置
			this->M_TargetLocation = MesheLocation;
			//设置排序
			this->Onwer->SetRenderLayer(
				this->Onwer->GetMesheCtlComp()->GetMapMesh(RanRow, 1)->GetTranslucency());
			//播放时间线动画
			this->Start();
		}

		//this->Onwer->DisEnableCo();
	}
}

void ULXQS_RotationMoveState::Finished()
{
	this->Onwer->InMapMeshe(
		this->Onwer->GetMesheCtlComp()->GetMapMesh(
			this->Onwer->GetMouseLine().Row, this->Onwer->GetMesheCtlComp()->GetMapMeshRowAndCol().Col - 1
		)->GetLineType()
	);

	//上下移动结束一定是技能的使用
	//出场结束，选择路线移动或者开始攻击
	int32 RandomSelect = UKismetMathLibrary::RandomIntegerInRange(0, 2);
	switch (RandomSelect)
	{
	case 1: {
		if (this->Onwer->GetCurrentHP() > (this->Onwer->GetTotalHP() * 0.55f))
		{
			//上下移动
			this->Onwer->CurStateClass = ABoss_LXQS::MakeLXQSState<ULXQS_MoveDownState>(this->Onwer);
		}
		else {
			//翻滚移动
			this->Onwer->CurStateClass = ABoss_LXQS::MakeLXQSState<ULXQS_RotationMoveState>(this->Onwer);
		}
	}return;
	case 2: {
		//发射泡泡
		this->Onwer->CurStateClass = ABoss_LXQS::MakeLXQSState<ULXQS_ShootBubbleState>(this->Onwer);
	}return;
	default:
	{
		//发射导弹
		this->Onwer->CurStateClass = ABoss_LXQS::MakeLXQSState<ULXQS_ShootMissileState>(this->Onwer);
	}return;
	}
}

void ULXQS_RotationMoveState::TimeLineUpdate(float Value)
{
	Super::TimeLineUpdate(Value);

	this->bRotation = true;
}

void ULXQS_RotationMoveState::TimeLineFinish()
{
	Super::TimeLineFinish();

	//this->Onwer->EnableCo();

	this->bRotation = false;

	//反转显示
	this->Onwer->SetActorScale3D(
		FVector(
			this->Onwer->GetActorScale3D().X,
			this->Onwer->GetActorScale3D().Y * -1.f,
			this->Onwer->GetActorScale3D().Z
		)
	);

	if (this->Onwer->GetCurrentHP() > (this->Onwer->GetTotalHP() * 0.55f))
	{
		//播放滚动的动画
		/*this->Onwer->SetPlayAnimationOnce(
			UGameSystemFunction::LoadRes(this->Onwer->LXQSAnim.RotationMoveEnd),
			UGameSystemFunction::LoadRes(this->Onwer->LXQSAnim.Idle)
		);*/

		this->Onwer->SetAnimation(0,TEXT("SpineTag"),true);
		this->Onwer->SetAnimation(0,TEXT("SpineTag"),true);
	}
	else
	{
		//播放滚动的动画
		/*this->Onwer->SetPlayAnimationOnce(
			UGameSystemFunction::LoadRes(this->Onwer->LXQSAnim.RotationMoveEndLow),
			UGameSystemFunction::LoadRes(this->Onwer->LXQSAnim.IdleLow)
		);*/

		this->Onwer->SetAnimation(0,TEXT("SpineTag"),true);
		this->Onwer->SetAnimation(0,TEXT("SpineTag"),true);
	}

	if (!this->bBack)
	{
		this->bBack = true;
	}
	else {
		this->bBack = false;
		this->Finished();
	}
}
