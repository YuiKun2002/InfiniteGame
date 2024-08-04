// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Boss/Boss_DongJun.h"
#include "GameStart/Flipbook/GameActor/Mouse/Normal/MachineMouse.h"

#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/VS/Components/CardManagerComponent.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameSystem/GameMapStructManager.h"
#include "GameSystem/FVMGameInstance.h"
#include <Kismet/KismetMathLibrary.h>
#include <Components/BoxComponent.h> 
#include <Components/SphereComponent.h> 
#include "GameStart/VS/MapMeshe.h"

ADJChannel::ADJChannel()
{
	this->MBoxMesh = CreateDefaultSubobject<UBoxComponent>(TEXT("Meshe"));
	this->MBody = CreateDefaultSubobject<USphereComponent>(TEXT("Body"));

	this->MBoxMesh->SetupAttachment(this->GetRootComponent());
	this->MBody->SetupAttachment(this->MBoxMesh);
}

void ADJChannel::MouseInit()
{
	Super::MouseInit();

	UGameSystemFunction::InitMouseMeshe(this->MBoxMesh, this->MBody);

	//切换线路
	this->GetMouseManager()->ChangeMouseLineType(
		this, this->GetMouseLine().Row, this->GetMouseLineType(), this->MBoxMesh, this->MBody
	);

	this->Begin();
}

bool ADJChannel::BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType)
{
	bool bRe = true;

	//如果血量已经为0，则直接退出
	if (this->GetCurrentHP() <= 0.f)
	{
		bRe = false;
	}

	//更新血量
	bRe = !this->UpdateHP(_HurtValue);

	if (bRe)
	{
		this->BPBeHit(this->GetCurrentHP(), this->GetTotalHP());
	}

	return bRe;
}

void ADJChannel::MouseTick(const float& DeltaTime)
{
	if (!this->bEn)
	{
		return;
	}

	if (this->bCheckTime > 0.f)
	{
		this->bCheckTime -= DeltaTime;
	}
	else {
		this->bCheckTime = 0.2f;
		//两个检查点
		if (IsValid(this->CurMapMesheIn) && IsValid(this->CurMapMeshe))
		{
			TArray<FHitResult> AllMouse;
			if (UGameSystemFunction::AddSphereTraceMulti(
				this->CurMapMesheIn, AllMouse, FVector::ZeroVector, FVector::ZeroVector, 15.f,
				UGameSystemFunction::GetMouseCollisionTraceType(EMouseCollisionType::MouseActor)
			))
			{
				//传送非boss的老鼠
				for (const auto& Cur : AllMouse)
				{
					AMouseActor* CurMouse = Cast<AMouseActor>(Cur.GetActor());
					if (
						IsValid(CurMouse)
						&&
						CurMouse->GetCurrentHP() > 0.f
						&&
						CurMouse->GetMouseBaseType() == EMouseBaseType::Normal)
					{
						FVector CurLo = this->CurMapMeshe->GetActorLocation();
						CurLo.Z = AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->GetMapMeshLocation(
							this->CurMapMeshe->GetLine().Row, AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->GetMapMeshRowAndCol().Col - 1
						).Z + MOUSE_Z_OFFSET;
						CurMouse->GetMouseManager()->ForceChangeLine(
							UGameSystemFunction::GetObjectName(CurMouse), CurMouse->GetMouseLine().Row, this->CurMapMeshe->GetLine().Row,
							CurMouse->GetMouseLine().Col
						);
						CurMouse->InMapMeshe(this->CurMapMeshe->GetLineType());
						CurMouse->CloseInWaterTimeLine();
						CurMouse->SetRenderLayer(this->CurMapMeshe->GetTranslucency());
						CurMouse->SetActorLocation(CurLo);
					}
				}
			}
		}

	}
}

void ADJChannel::MouseDeathed()
{
	Super::MouseDeathed();

	this->Begin();
}

void ADJChannel::Begin()
{
	this->bEn = false;
	this->ClosedBoxComponent(this->MBoxMesh);
	this->ClosedBoxComponent(this->MBody);
	this->Hidde();

	if (IsValid(this->CurMapMeshe))
	{
		this->CurMapMeshe->SetMesheEnable(true, this);
	}

	if (IsValid(this->CurMapMesheIn))
	{
		this->CurMapMesheIn->SetMesheEnable(true, this);
	}
}

void ADJChannel::End()
{
	this->bEn = true;
	this->EnableBoxComponent(this->MBoxMesh);
	this->EnableBoxComponent(this->MBody);

	if (IsValid(AGameMapInstance::GetGameMapInstance()))
	{
		if (IsValid(AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent))
		{
			//获取当前网格的卡片
			this->CurMapMeshe =
				AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->GetMapMesh(
					ChannelOut.Row, ChannelOut.Col
				);

			//获取当前网格的卡片
			this->CurMapMesheIn =
				AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->GetMapMesh(
					ChannelIn.Row, ChannelIn.Col
				);
		}
	}

	if (IsValid(this->CurMapMeshe))
	{
		this->CurMapMeshe->SetMesheEnable(false, this);

		if (IsValid(this->CurMapMeshe->GetUIMapMeshe()))
		{
			this->CurMapMeshe->GetUIMapMeshe()->KillAllCards();
		}
	}

	if (IsValid(this->CurMapMesheIn))
	{
		this->CurMapMesheIn->SetMesheEnable(false, this);

		if (IsValid(this->CurMapMesheIn->GetUIMapMeshe()))
		{
			this->CurMapMesheIn->GetUIMapMeshe()->KillAllCards();
		}
	}
}

EMouseBaseType ADJChannel::GetMouseBaseType()
{
	return EMouseBaseType::Normal;
}

ABoss_DongJun::ABoss_DongJun()
{
	this->M_BoxMesh = CreateDefaultSubobject<UBoxComponent>(TEXT("Meshe_Box"));
	this->M_BoxBody = CreateDefaultSubobject<UBoxComponent>(TEXT("Body_Box"));

	this->M_BoxMesh->SetupAttachment(this->GetRootComponent());
	this->M_BoxBody->SetupAttachment(this->M_BoxMesh);
}

void ABoss_DongJun::BeginPlay()
{
	Super::BeginPlay();
}

void ABoss_DongJun::MouseInit()
{
	Super::MouseInit();

	this->InitBoss(TEXT("洞君"), TEXT("T_Boss_H_0"));

	this->GetRenderComponent()->OnAnimationPlayEnd.Unbind();
	this->GetRenderComponent()->OnAnimationPlayEnd.BindUObject(this, &ABoss_DongJun::AnimPlayEnd);

	this->M_MouseDef_res = Cast<UPaperFlipbook>(this->M_MouseDef.TryLoad());
	this->M_MouseLowDef_res = Cast<UPaperFlipbook>(this->M_MouseLowDef.TryLoad());

	this->M_MouseOut_res = Cast<UPaperFlipbook>(this->M_MouseOut.TryLoad());
	this->M_MouseLowOut_res = Cast<UPaperFlipbook>(this->M_MouseLowOut.TryLoad());

	this->M_MouseBuild_res = Cast<UPaperFlipbook>(this->M_MouseBuild.TryLoad());
	this->M_MouseLowBuild_res = Cast<UPaperFlipbook>(this->M_MouseLowBuild.TryLoad());

	this->M_MouseIn_res = Cast<UPaperFlipbook>(this->M_MouseIn.TryLoad());
	this->M_MouseLowIn_res = Cast<UPaperFlipbook>(this->M_MouseLowIn.TryLoad());

	this->M_MouseJump_res = Cast<UPaperFlipbook>(this->M_MouseJump.TryLoad());
	this->M_MouseLowJump_res = Cast<UPaperFlipbook>(this->M_MouseLowJump.TryLoad());

	this->CurMeshe = AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent;

	//初始化状态管理器
	this->StateMan = UDongJunStateManager::MakeDongJunStateManager(this);
	this->StateMan->ChangeState(NewObject<UDJ_Out>());
}

void ABoss_DongJun::MouseTick(const float& DeltaTime)
{
	if (this->GetCurrentHP() > 0.f)
	{
		if (IsValid(this->StateMan))
		{
			if (IsValid(this->StateMan->Get()))
			{
				this->StateMan->Get()->MouseTick(DeltaTime);
			}
		}
	}
}

void ABoss_DongJun::MoveingUpdate(float DeltaTime)
{
	if (IsValid(this->StateMan))
	{
		if (IsValid(this->StateMan->Get()))
		{
			Super::MoveingUpdate(DeltaTime);

			this->StateMan->Get()->MoveingUpdate(DeltaTime);
		}
	}
}

bool ABoss_DongJun::BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType)
{
	bool bRe = Super::BeHit(CurHitMouseObj, _HurtValue, AttackType);

	if (bRe)
	{
		if (this->GetCurrentHP() > 0.f)
		{
			if (IsValid(this->StateMan))
			{
				if (IsValid(this->StateMan->Get()))
				{
					this->StateMan->Get()->BeHit(CurHitMouseObj, _HurtValue, AttackType);
				}
			}
		}
	}

	return bRe;
}

void ABoss_DongJun::MouseDeathed()
{
	Super::MouseDeathed();

	this->M_BoxBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->M_BoxMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//设置死亡动画
	this->SetPlayAnimation(LoadObject<UPaperFlipbook>(0,
		TEXT("PaperFlipbook'/Game/Resource/Texture/Sprite/VS/Sprite/Mouse/Boss/0/Anim/F_M_B_0_Death.F_M_B_0_Death'"))
	);
}


void ABoss_DongJun::ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime)
{
	Super::ExecuteBuff(BuffTag, CurBuffTime);
}

void ABoss_DongJun::ExitBuff(EGameBuffTag BuffTag)
{
	Super::ExitBuff(BuffTag);
}


void ABoss_DongJun::AnimPlayEnd()
{
	if (this->GetCurrentHP() > 0.f)
	{
		if (IsValid(this->StateMan))
		{
			if (IsValid(this->StateMan->Get()))
			{
				this->StateMan->Get()->AnimPlayEnd();
			}
		}
	}
}

void ABoss_DongJun::SetCollision(bool bEnable)
{
	if (bEnable)
	{
		this->EnableBoxComponent(this->M_BoxMesh);
		this->EnableBoxComponent(this->M_BoxBody);
	}
	else {
		this->ClosedBoxComponent(this->M_BoxMesh);
		this->ClosedBoxComponent(this->M_BoxBody);
	}
}

UMesheControllComponent* ABoss_DongJun::GetMeshe()
{
	return this->CurMeshe;
}

UDongJunStateManager* ABoss_DongJun::GetMan()
{
	return this->StateMan;
}

UDongJunStateManager* UDongJunStateManager::MakeDongJunStateManager(ABoss_DongJun* NewMouseActor)
{
	UDongJunStateManager* NewObj = NewObject<UDongJunStateManager>();
	NewObj->MouseActor = NewMouseActor;
	return NewObj;
}
void UDongJunStateManager::ChangeState(class UDongJunStateBase* NewState)
{
	if (IsValid(NewState))
	{
		//设置新的状态
		this->State = NewState;
		this->State->InitState(this);
		this->State->Init();
	}
}
class UDongJunStateBase* UDongJunStateManager::Get()
{
	return this->State;
}

//--------------------------------------------------------------------------------
void UDongJunStateBase::Init()
{
}
void UDongJunStateBase::MouseTick(const float& DeltaTime)
{
}
void UDongJunStateBase::MoveingUpdate(const float& DeltaTime)
{
}
void UDongJunStateBase::BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType)
{
}
void UDongJunStateBase::AnimPlayEnd()
{
}

void UDongJunStateBase::InitState(UDongJunStateManager* NewMan)
{
	if (IsValid(NewMan))
	{
		this->CurMan = NewMan;
	}
}

class ABoss_DongJun* UDongJunStateBase::Get()
{
	return this->CurMan->MouseActor;
}

void UDongJunStateBase::PlayAnim(UPaperFlipbook* Anim1, UPaperFlipbook* Anim2Low)
{
	if (this->Get()->GetCurrentHP() > this->Get()->GetTotalHP() * 0.5f)
	{
		this->Get()->GetRenderComponent()->SetPlayAnimation(Anim1);
	}
	else {
		this->Get()->GetRenderComponent()->SetPlayAnimation(Anim2Low);
	}
}

//--------------------------------------------------------------------------------

void UDJ_Out::Init()
{
	this->PlayAnim(this->Get()->M_MouseOut_res, this->Get()->M_MouseLowOut_res);

	this->CurTime = UTimeLineClass::MakeTimeLineClass();
	this->CurTime->AddCurve(this->Get()->MouseCurveJumpInChannel, this,
		[](UTimeLineClass* Time, UObject* Obj, float time) {
			float A = UKismetMathLibrary::Lerp(0.f, 1.f, time);
			Cast<UDJ_Out>(Obj)->Get()->GetRenderComponent()->SetScalarParameterValueOnMaterials(FName(TEXT("A")), A);
		},
		[](UTimeLineClass* Time, UObject* Obj) {
			Cast<UDJ_Out>(Obj)->Get()->GetRenderComponent()->SetScalarParameterValueOnMaterials(FName(TEXT("A")), 1);
		}
		);
	this->CurTime->PlayFromStart();

	int32 Row = UGameSystemFunction::GetRandomRange(0,
		this->Get()->GetMeshe()->GetMapMeshRowAndCol().Row - 1
	);

	//切换线路
	bool bRe = this->Get()->GetMouseManager()->ForceChangeLine(
		UGameSystemFunction::GetObjectName(this->Get()),
		this->Get()->GetMouseLine().Row,
		Row, this->Get()->GetMouseLine().Col
	);

	if (bRe)
	{
		//设置到陆地
		this->Get()->InMapMeshe(ELineType::OnGround);
		this->Get()->CloseInWaterTimeLine();

		this->Get()->SetActorHiddenInGame(false);
		this->Get()->SetCollision(true);

		//设置位置
		FVector NewLocation = this->Get()->GetMeshe()->GetMapMeshLocation(
			this->Get()->GetMouseLine().Row,
			this->Get()->GetMeshe()->GetMapMeshRowAndCol().Col - 1);
		NewLocation.Y = 430.f;
		this->Get()->SetActorLocation(NewLocation);
		this->Get()->SetRenderLayer(
			this->Get()->GetMeshe()->GetMapMesh(this->Get()->GetMouseLine().Row,
				this->Get()->GetMeshe()->GetMapMeshRowAndCol().Col - 1
			)->GetTranslucency());
	}

	this->bEnd = false;
}

void UDJ_Out::MouseTick(const float& DeltaTime)
{
	if (IsValid(this->CurTime))
	{
		this->CurTime->Tick(DeltaTime);
	}
}

void UDJ_Out::AnimPlayEnd()
{
	if (!this->bEnd)
	{
		this->bEnd = true;
	}
	else {
		return;
	}

	this->Get()->GetMan()->ChangeState(NewObject<UDJ_Idle>());
}



void UDJ_Idle::Init()
{
	this->PlayAnim(this->Get()->M_MouseDef_res, this->Get()->M_MouseLowDef_res);

	if (!IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		return;
	}

	//根据当前地图等级，决定使用的模式【更新老鼠等级】
	int32 CurLevel = UFVMGameInstance::GetFVMGameInstance()->GetGameMapStructManager()->LevelConfig.MouseLevel;
	this->Get()->MouseLevel = CurLevel;

	if (CurLevel == 1)
	{
		//打洞或者跳跃
		static bool BuildState = false;
		if (BuildState)
		{
			BuildState = false;
		}
		else {
			BuildState = true;
		}

		this->bAction = BuildState;
	}
	else if (CurLevel == 2)
	{
		this->Get()->DefSpawnMouse(1);
		// 50%打洞，50%跳跃
		this->bAction = UGameSystemFunction::GetRange(50, 100);
	}
	else {
		this->Get()->DefSpawnMouse(2);
		// 40%打洞，60%跳跃
		this->bAction = UGameSystemFunction::GetRange(60, 100);
	}

	//反应速度
	if (this->Get()->MouseLevel == 1)
	{
		this->Count = this->Get()->IdleCount;
	}
	else if (this->Get()->MouseLevel == 2)
	{
		this->Count = this->Get()->IdleCount - (this->Get()->IdleCount - this->Get()->IdleCount * 0.8f);
	}
	else if (this->Get()->MouseLevel == 3)
	{
		this->Count = this->Get()->IdleCount - (this->Get()->IdleCount - this->Get()->IdleCount * 0.5f);
	}
	else {
		this->Count = 0;
	}
}

void UDJ_Idle::BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType)
{
	this->PlayAnim(this->Get()->M_MouseDef_res, this->Get()->M_MouseLowDef_res);
}

void UDJ_Idle::SpawnMouse(int32 Row)
{
	//生成老鼠
	if (this->Get()->SpawnMouseNames.Num())
	{
		int32 Index = UGameSystemFunction::GetRandomRange(0, this->Get()->SpawnMouseNames.Num() - 1);
		//int32 Row = UGameSystemFunction::GetRandomRange(0, this->Get()->GetMeshe()->GetMapMeshRowAndCol().Row - 1);
		UMouseSpawnManager::MakeNewMouseByName(
			UGameSystemFunction::LoadRes(this->Get()->MouseDataTable), this->Get()->SpawnMouseNames[Index], FLine(Row, 0)
		);
	}
}

void UDJ_Idle::AnimPlayEnd()
{
	// game playm 洞君,2

	if (this->Count > 0)
	{
		this->Count--;
		return;
	}

	//打洞或者跳跃
	if (this->bAction)
	{
		this->Get()->GetMan()->ChangeState(NewObject<UDJ_Jump>());
	}
	else
	{
		this->CurRow = this->Get()->GetMouseLine().Row;

		AMapMeshe* CurMapMeshe = this->Get()->GetMeshe()->GetMapMesh(this->CurRow,
			this->Get()->GetMeshe()->GetMapMeshRowAndCol().Col - 1
		);

		//如果当前位置有角色，则直接触发跳跃
		if (IsValid(CurMapMeshe->GetUIMapMeshe()))
		{
			if (CurMapMeshe->GetUIMapMeshe()->GetPlayer())
			{
				this->Get()->GetMan()->ChangeState(NewObject<UDJ_Jump>());
				return;
			}
		}

		if (this->Get()->Channels.Contains(this->CurRow))
		{
			ADJChannel** Cur = this->Get()->Channels.Find(this->CurRow);
			if (IsValid(*Cur) && (*Cur)->GetCurrentHP() > 0.f)
			{
				//当前管道有效
				UDJ_RebuildChannel* NewState = NewObject<UDJ_RebuildChannel>();
				this->Get()->GetMan()->ChangeState(NewState);
				NewState->Rebuild(*Cur);
				return;
			}
		}
		//当前管道无效
		UDJ_Create* NewState = NewObject<UDJ_Create>();
		this->Get()->GetMan()->ChangeState(NewState);
		NewState->GenChannel(this->CurRow);

		int32 Level = this->Get()->MouseLevel;
		if (Level == 1)
		{
			int32 LRow = UGameSystemFunction::GetRandomRange(0, this->Get()->GetMeshe()->GetMapMeshRowAndCol().Row - 1);
			this->SpawnMouse(LRow);
		}
		else if (Level == 2)
		{
			this->Get()->DefSpawnMouseTemp(2, Cast<ABoss_DongJun>(this->Get())->SpawnMouseNames);
		}
		else {
			if (UGameSystemFunction::GetRange(10, 100))
			{
				this->Get()->DefSpawnMouseTemp(3, Cast<ABoss_DongJun>(this->Get())->SpawnMouseNames);
			}
			else {
				this->Get()->DefSpawnMouseTemp(2, Cast<ABoss_DongJun>(this->Get())->SpawnMouseNames);
			}
		}
	}
}


void UDJ_Create::Init()
{
	this->PlayAnim(this->Get()->M_MouseBuild_res, this->Get()->M_MouseLowBuild_res);
}

void UDJ_Create::AnimPlayEnd()
{
	//初始化洞口
	if (IsValid(this->CurNewChannel))
	{
		this->CurNewChannel->InitChannelIn(this->CurNewChannel->ChannelIn);
	}

	//执行跳入管道的状态
	UDJ_JumpInChannel* NewState = NewObject<UDJ_JumpInChannel>();
	NewState->InitChannel(this->CurNewChannel);
	this->Get()->GetMan()->ChangeState(NewState);
}

void UDJ_Create::GenChannel(int32 Row)
{
	// Row 管道洞口的行位置

	//管道创建的行
	TArray<int32> AllRows;
	//当前存在的管道
	TMap<int32, ADJChannel*> CurAll = this->Get()->Channels;

	//初始化行，校验当前行是否可用
	for (int32 i = 0; i < this->Get()->GetMeshe()->GetMapMeshRowAndCol().Row; i++)
	{
		int32 Count = 0;
		//查询当前行是否有可用的格子【排除最后一列】
		for (int32 j = 0; j < this->Get()->GetMeshe()->GetMapMeshRowAndCol().Col - 1; j++)
		{
			//已经存在的管道
			for (const auto& ValidChannel : CurAll)
			{
				//当前管道出口的行与当前行匹配
				if (IsValid(ValidChannel.Value) && ValidChannel.Value->ChannelOut.Row == i)
				{
					//当前列是否被占用
					if (ValidChannel.Value->ChannelOut.Col == j)
					{
						//更新占用个数
						Count++;
						CurAll.Remove(ValidChannel.Value->ChannelIn.Row);
						break;
					}
				}
			}
		}

		//表示当前行还有空位
		if (Count < this->Get()->GetMeshe()->GetMapMeshRowAndCol().Col - 1)
		{
			//表示当前行通过，继续测试下一行
			AllRows.Emplace(i);
		}
	}

	//如果没有任何行可用！！！，直接切换状态
	if (AllRows.Num() == 0)
	{
		//直接切换状态
		this->Get()->GetMan()->ChangeState(NewObject<UDJ_Jump>());
		return;
	}

	//随机初始化管道的行
	int32 RowIndex = UGameSystemFunction::GetRandomRange(0, AllRows.Num() - 1);
	//随机的目标行
	int32 TargetRow = AllRows[RowIndex];

	//管道创建的列
	TArray<int32> AllCols;
	//重新获取所有管道
	CurAll = this->Get()->Channels;
	//生成列【默认排除最后一列】
	for (int32 i = 2; i < this->Get()->GetMeshe()->GetMapMeshRowAndCol().Col - 1; i++)
	{
		bool bMatch = false;
		//已经存在的管道
		for (const auto& ValidChannel : CurAll)
		{
			//当前管道出口的行与当前行匹配
			if (IsValid(ValidChannel.Value) && ValidChannel.Value->ChannelOut.Col == i)
			{
				CurAll.Remove(ValidChannel.Value->ChannelIn.Row);
				bMatch = true;
				break;
			}
		}

		if (!bMatch)
		{
			AllCols.Emplace(i);
		}
	}

	//如果出现不可用列
	if (AllCols.Num() == 0)
	{
		//直接切换状态
		this->Get()->GetMan()->ChangeState(NewObject<UDJ_Jump>());
		return;
	}

	//随机初始化管道的列
	int32 ColIndex = UGameSystemFunction::GetRandomRange(0, AllCols.Num() - 1);
	//随机的目标列
	int32 TargetCol = AllCols[ColIndex];

	//生成目标的线路
	FLine NewTargetLine(TargetRow, TargetCol);

	//生成一个新的管道
	AMouseActor* NewC =
		UMouseSpawnManager::MakeNewMouseByClass(
			this->Get()->GetMouseManager(),
			this->Get()->MouseChannelRes,
			FVector::ZeroVector, 50.f, 0.f, 0.f, NewTargetLine,
			ELineType::OnGround, EMouseTag::Boss
		);

	if (IsValid(NewC))
	{
		ADJChannel* CurChannel = Cast<ADJChannel>(NewC);
		//隐藏
		CurChannel->SetPlayAnimation(nullptr);
		//初始化准备
		CurChannel->Begin();
		//设置管道的位置
		FVector Loca = this->Get()->GetMeshe()->GetMapMesh(
			NewTargetLine.Row,
			NewTargetLine.Col
		)->GetActorLocation();

		CurChannel->SetActorLocation(Loca);
		//初始化管道出口
		CurChannel->ChannelOut = NewTargetLine;
		//初始化管道入口
		CurChannel->ChannelIn = FLine(Row, this->Get()->GetMeshe()->GetMapMeshRowAndCol().Col - 1);
		//更新网格
		CurChannel->InMapMeshe(this->Get()->GetMeshe()->GetMapMesh(NewTargetLine.Row, NewTargetLine.Col)->GetLineType());
		CurChannel->CloseInWaterTimeLine();
		//切换线路
		CurChannel->GetMouseManager()->ForceChangeLine(
			UGameSystemFunction::GetObjectName(CurChannel),
			CurChannel->GetMouseLine().Row, NewTargetLine.Row, NewTargetLine.Col
		);
		CurChannel->SetActorLocation(
			this->Get()->GetMeshe()->GetMapMesh(NewTargetLine.Row, NewTargetLine.Col)->GetActorLocation()
		);

		ADJChannel** Cur = this->Get()->Channels.Find(Row);
		if (Cur && IsValid(*Cur))
		{
			(*Cur)->SetMouseDeath(true);
		}

		//添加到列表
		this->Get()->Channels.Emplace(
			Row, CurChannel
		);

		this->CurNewChannel = CurChannel;
	}
}


void UDJ_RebuildChannel::Init()
{
	this->PlayAnim(this->Get()->M_MouseBuild_res, this->Get()->M_MouseLowBuild_res);
}

void UDJ_RebuildChannel::AnimPlayEnd()
{
	if (IsValid(this->CurNewChannel) && this->CurNewChannel->GetCurrentHP() > 0.f)
	{
		//执行跳入管道的状态
		UDJ_JumpInChannel* NewState = NewObject<UDJ_JumpInChannel>();
		NewState->InitChannel(this->CurNewChannel);
		this->Get()->GetMan()->ChangeState(NewState);
	}
	else {
		//当前管道无效
		UDJ_Create* NewState = NewObject<UDJ_Create>();
		this->Get()->GetMan()->ChangeState(NewState);
		NewState->GenChannel(this->Get()->GetMouseLine().Row);
	}
}

void UDJ_RebuildChannel::Rebuild(ADJChannel* NewChannel)
{
	this->CurNewChannel = NewChannel;

	if (IsValid(this->CurNewChannel))
	{
		this->CurNewChannel->SetHP(this->CurNewChannel->GetTotalHP());
		this->CurNewChannel->Begin();
		this->CurNewChannel->InitChannelIn(this->CurNewChannel->ChannelIn);
	}
}

void UDJ_JumpInChannel::Init()
{
	this->PlayAnim(this->Get()->M_MouseJump_res, this->Get()->M_MouseLowJump_res);
	if (IsValid(this->CurChannelObj))
	{
		//初始化位置
		this->CurLocation = this->Get()->GetActorLocation();

		this->TargetLocation =
			this->Get()->GetMeshe()->GetMapMesh(
				this->CurChannelObj->ChannelIn.Row, this->CurChannelObj->ChannelIn.Col
			)->GetActorLocation();

		//初始化老鼠跳跃时间线
		this->JumpTime = UTimeLineClass::MakeTimeLineClass();
		UCurveFloat* NewCur = UGameSystemFunction::LoadRes(this->Get()->MouseCurveJumpInChannel);
		this->JumpTime->AddCurve(
			this->Get()->MouseCurveJumpInChannel, this,
			[](UTimeLineClass* TimeLine, UObject* Obj, float time) {

				UDJ_JumpInChannel* Cur = Cast<UDJ_JumpInChannel>(Obj);

				Cur->Get()->SetActorLocation(UKismetMathLibrary::VLerp(Cur->CurLocation, Cur->TargetLocation, time));

				Cur->Get()->GetRenderComponent()->SetScalarParameterValueOnMaterials(FName(TEXT("A")), (1.f - time));

			}, [](UTimeLineClass* TimeLine, UObject* Obj) {
				UDJ_JumpInChannel* Cur = Cast<UDJ_JumpInChannel>(Obj);
				Cur->JumpTime = nullptr;
			}
			);
		this->JumpTime->PlayFromStart();
		this->bInit = false;

		//从洞里钻出的时间
		int32 Level = this->Get()->MouseLevel;
		if (Level == 1)
		{
			this->CurTime = this->ConstTime;
		}
		else if (Level == 2)
		{
			this->CurTime = this->ConstTime * 0.8f;
		}
		else if (Level == 3)
		{
			this->CurTime = this->ConstTime * 0.5f;
		}
		else {
			this->CurTime = this->ConstTime * 0.2f;
		}
	}
}

void UDJ_JumpInChannel::MouseTick(const float& DeltaTime)
{
	if (!this->bInit)
	{
		if (IsValid(this->JumpTime))
		{
			this->JumpTime->Tick(DeltaTime);
		}
	}

	if (this->bInit)
	{
		if (this->CurTime > 0.f)
		{
			this->CurTime -= DeltaTime;
		}
		else {
			//切换
			this->Get()->GetMan()->ChangeState(NewObject<UDJ_Out>());
		}
	}
}

void UDJ_JumpInChannel::AnimPlayEnd()
{
	if (!this->bInit)
	{
		this->bInit = true;
	}
	else {
		return;
	}

	//初始化管道完成
	if (IsValid(this->CurChannelObj))
	{
		this->CurChannelObj->InitChannelOut(this->CurChannelObj->ChannelOut);
	}

	//设置老鼠消失
	this->Get()->SetActorHiddenInGame(true);
	//关闭碰撞
	this->Get()->SetCollision(false);
	//设置老鼠位置到最后
	this->Get()->AddActorLocalOffset(FVector(0.f, 2000.f, 0.f));
}

void UDJ_JumpInChannel::InitChannel(class ADJChannel* CurChannel)
{
	this->CurChannelObj = CurChannel;
}


void UDJ_Jump::Init()
{
	this->PlayAnim(this->Get()->M_MouseJump_res, this->Get()->M_MouseLowJump_res);

	this->bIdle = false;

	if (this->Get()->MouseLevel == 1)
	{
		this->NewDefJump();
	}
	else if (this->Get()->MouseLevel == 2)
	{
		if (UGameSystemFunction::GetRange(90, 100)) {
			this->NewDefJump();
		}
		else {
			this->NewSecondJump();
		}
	}
	else if (this->Get()->MouseLevel > 2)
	{
		if (UGameSystemFunction::GetRange(90, 100)) {
			if (UGameSystemFunction::GetRange(80, 100)) {
				this->NewDefJump();
			}
			else {
				this->NewSecondJump();
			}
		}
		else {
			this->NewLastJump();
		}
	}

}

void UDJ_Jump::MouseTick(const float& DeltaTime)
{
	if (IsValid(this->JumpTimeLine))
	{
		this->JumpTimeLine->Tick(DeltaTime);
	}
}

void UDJ_Jump::AnimPlayEnd()
{
	if (this->bIdle)
	{
		this->bIdle = false;
		this->PlayAnim(this->Get()->M_MouseJump_res, this->Get()->M_MouseLowJump_res);
		this->JumpTimeLine->PlayFromStart();
	}
}

void UDJ_Jump::DefJump(UTimeLineClass* time, UObject* Obj, float Value)
{
	UDJ_Jump* Cur = Cast<UDJ_Jump>(Obj);
	Cur->Get()->SetActorLocation(UKismetMathLibrary::VLerp(Cur->CurLocation, Cur->TarLocation, Value));
}

void UDJ_Jump::DefFinish(UTimeLineClass* time, UObject* Obj)
{
	UDJ_Jump* Cur = Cast<UDJ_Jump>(Obj);
	Cur->Col -= 2;
	int32 LastCount = 2;
	if (Cur->Get()->MouseLevel >= 2)
	{
		LastCount = 0;
	}
	if (Cur->Col >= LastCount)
	{
		Cur->CurLocation = Cur->Get()->GetActorLocation();
		Cur->TarLocation = Cur->Get()->GetMeshe()->GetMapMeshLocation(Cur->Get()->GetMouseLine().Row, Cur->Col);
		Cur->PlayAnim(Cur->Get()->M_MouseDef_res, Cur->Get()->M_MouseLowDef_res);
		Cur->bIdle = true;

		//消灭当前所有防御卡
		AMapMeshe* CurMapMeshe = Cur->Get()->GetMeshe()->GetMapMesh(
			Cur->Get()->GetMouseLine().Row, Cur->Col + 2
		);
		if (IsValid(CurMapMeshe) && IsValid(CurMapMeshe->GetUIMapMeshe()))
		{
			CurMapMeshe->GetUIMapMeshe()->KillAllCards();
		}

		if (Cur->Get()->M_Proper_Condition.M_CurrentInType == ELineType::OnWater)
		{
			UFVMGameInstance::PlayBGM_S_Static(TEXT("MouseInWater"), TEXT("ItemAudio"));
		}
	}
	else {
		//消灭当前所有防御卡
		AMapMeshe* CurMapMeshe = Cur->Get()->GetMeshe()->GetMapMesh(
			Cur->Get()->GetMouseLine().Row, LastCount
		);
		if (IsValid(CurMapMeshe) && IsValid(CurMapMeshe->GetUIMapMeshe()))
		{
			CurMapMeshe->GetUIMapMeshe()->KillAllCards();
		}

		if (Cur->Get()->M_Proper_Condition.M_CurrentInType == ELineType::OnWater)
		{
			UFVMGameInstance::PlayBGM_S_Static(TEXT("MouseInWater"), TEXT("ItemAudio"));
		}

		//结束
		Cur->Get()->GetMan()->ChangeState(NewObject<UDJ_JumpEnd>());
	}
}

void UDJ_Jump::SecondFinish(class UTimeLineClass* time, UObject* Obj)
{
	UDJ_Jump* Cur = Cast<UDJ_Jump>(Obj);
	Cur->Col -= 1;
	int32 LastCount = 2;
	if (Cur->Get()->MouseLevel >= 2)
	{
		LastCount = 0;
	}
	if (Cur->Col >= 0)
	{
		Cur->CurLocation = Cur->Get()->GetActorLocation();
		Cur->TarLocation = Cur->Get()->GetMeshe()->GetMapMeshLocation(
			Cur->Get()->GetMouseLine().Row, Cur->Col
		);
		Cur->PlayAnim(Cur->Get()->M_MouseDef_res, Cur->Get()->M_MouseLowDef_res);
		Cur->bIdle = true;
		//消灭当前所有防御卡
		AMapMeshe* CurMapMeshe = Cur->Get()->GetMeshe()->GetMapMesh(
			Cur->Get()->GetMouseLine().Row, Cur->Col + 1
		);
		if (IsValid(CurMapMeshe) && IsValid(CurMapMeshe->GetUIMapMeshe()))
		{
			CurMapMeshe->GetUIMapMeshe()->KillAllCards();
		}

		if (Cur->Get()->M_Proper_Condition.M_CurrentInType == ELineType::OnWater)
		{
			UFVMGameInstance::PlayBGM_S_Static(TEXT("MouseInWater"), TEXT("ItemAudio"));
		}
	}
	else {

		//消灭当前所有防御卡
		AMapMeshe* CurMapMeshe = Cur->Get()->GetMeshe()->GetMapMesh(
			Cur->Get()->GetMouseLine().Row, LastCount
		);
		if (IsValid(CurMapMeshe) && IsValid(CurMapMeshe->GetUIMapMeshe()))
		{
			CurMapMeshe->GetUIMapMeshe()->KillAllCards();
		}

		if (Cur->Get()->M_Proper_Condition.M_CurrentInType == ELineType::OnWater)
		{
			UFVMGameInstance::PlayBGM_S_Static(TEXT("MouseInWater"), TEXT("ItemAudio"));
		}
		//结束
		Cur->Get()->GetMan()->ChangeState(NewObject<UDJ_JumpEnd>());
	}
}


void UDJ_Jump::LastFinish(class UTimeLineClass* time, UObject* Obj)
{
	UDJ_Jump* Cur = Cast<UDJ_Jump>(Obj);
	Cur->Col -= 1;
	if (Cur->Col >= 0)
	{
		Cur->CurLocation = Cur->Get()->GetActorLocation();
		Cur->TarLocation = Cur->Get()->GetMeshe()->GetMapMeshLocation(
			Cur->Get()->GetMouseLine().Row, Cur->Col
		);
		Cur->PlayAnim(Cur->Get()->M_MouseDef_res, Cur->Get()->M_MouseLowDef_res);
		Cur->bIdle = true;

		//消灭当前所有防御卡
		AMapMeshe* CurMapMeshe = Cur->Get()->GetMeshe()->GetMapMesh(
			Cur->Get()->GetMouseLine().Row, Cur->Col + 1
		);

		if (IsValid(CurMapMeshe) && IsValid(CurMapMeshe->GetUIMapMeshe()))
		{
			CurMapMeshe->GetUIMapMeshe()->KillAllCards();
		}

		if (UGameSystemFunction::GetRange(10, 100))
		{
			//生成爆炸资源
			AMachineBombAnim* CurBomb = Cur->Get()->GetWorld()->SpawnActor<AMachineBombAnim>(
				UGameSystemFunction::LoadRes(Cur->Get()->MouseBombRes)
				);
			CurBomb->SetActorLocation(Cur->Get()->GetActorLocation());
			CurBomb->OnInit();
		}
	}
	else {
		//消灭当前所有防御卡
		AMapMeshe* CurMapMeshe = Cur->Get()->GetMeshe()->GetMapMesh(
			Cur->Get()->GetMouseLine().Row,
			0
		);
		if (IsValid(CurMapMeshe) && IsValid(CurMapMeshe->GetUIMapMeshe()))
		{
			CurMapMeshe->GetUIMapMeshe()->KillAllCards();
		}

		if (UGameSystemFunction::GetRange(10, 100))
		{
			//生成爆炸资源
			AMachineBombAnim* CurBomb = Cur->Get()->GetWorld()->SpawnActor<AMachineBombAnim>(
				UGameSystemFunction::LoadRes(Cur->Get()->MouseBombRes)
				);
			CurBomb->SetActorLocation(Cur->Get()->GetActorLocation());
			CurBomb->OnInit();
		}

		if (Cur->Get()->M_Proper_Condition.M_CurrentInType == ELineType::OnWater)
		{
			UFVMGameInstance::PlayBGM_S_Static(TEXT("MouseInWater"), TEXT("ItemAudio"));
		}
		//结束
		Cur->Get()->GetMan()->ChangeState(NewObject<UDJ_JumpEnd>());
	}
}

void UDJ_Jump::NewDefJump()
{
	this->Col = this->Get()->GetMeshe()->GetMapMeshRowAndCol().Col - 1;
	this->CurLocation = this->Get()->GetActorLocation();
	this->TarLocation = this->Get()->GetMeshe()->GetMapMeshLocation(this->Get()->GetMouseLine().Row, this->Col);

	this->JumpTimeLine = UTimeLineClass::MakeTimeLineClass();
	this->JumpTimeLine->AddCurve(this->Get()->MouseCurveJumpInChannel, this,
		(&UDJ_Jump::DefJump), (&UDJ_Jump::DefFinish));
	this->JumpTimeLine->PlayFromStart();
}

void UDJ_Jump::NewSecondJump()
{
	this->Col = this->Get()->GetMeshe()->GetMapMeshRowAndCol().Col - 1;
	this->CurLocation = this->Get()->GetActorLocation();
	this->TarLocation = this->Get()->GetMeshe()->GetMapMeshLocation(this->Get()->GetMouseLine().Row, this->Col);

	this->JumpTimeLine = UTimeLineClass::MakeTimeLineClass();
	this->JumpTimeLine->AddCurve(this->Get()->MouseCurveJumpInChannel, this,
		(&UDJ_Jump::DefJump), (&UDJ_Jump::SecondFinish));
	this->JumpTimeLine->PlayFromStart();
}

void UDJ_Jump::NewLastJump()
{
	this->Col = this->Get()->GetMeshe()->GetMapMeshRowAndCol().Col - 1;
	this->CurLocation = this->Get()->GetActorLocation();
	this->TarLocation = this->Get()->GetMeshe()->GetMapMeshLocation(this->Get()->GetMouseLine().Row, this->Col);

	this->JumpTimeLine = UTimeLineClass::MakeTimeLineClass();
	this->JumpTimeLine->AddCurve(this->Get()->MouseCurveJumpInChannel, this,
		(&UDJ_Jump::DefJump), (&UDJ_Jump::LastFinish));
	this->JumpTimeLine->PlayFromStart();
}

void UDJ_JumpEnd::Init()
{
	this->PlayAnim(this->Get()->M_MouseIn_res, this->Get()->M_MouseLowIn_res);

	this->JumpTimeLine = UTimeLineClass::MakeTimeLineClass();
	this->JumpTimeLine->AddCurve(this->Get()->MouseCurveJumpInChannel, this,
		[](UTimeLineClass* time, UObject* Obj, float Value)
		{
			UDJ_JumpEnd* Cur = Cast<UDJ_JumpEnd>(Obj);
			Cur->Get()->GetRenderComponent()->SetScalarParameterValueOnMaterials(TEXT("A"), (1.f - Value));
		},
		[](UTimeLineClass* time, UObject* Obj)
		{
			UDJ_JumpEnd* Cur = Cast<UDJ_JumpEnd>(Obj);
			Cur->Get()->AddActorLocalOffset(FVector(0.f, 2000.f, 0.f));
			Cur->Get()->GetMan()->ChangeState(NewObject<UDJ_Out>());
		}
		);
	this->JumpTimeLine->PlayFromStart();
}

void UDJ_JumpEnd::MouseTick(const float& DeltaTime)
{
	if (IsValid(this->JumpTimeLine))
	{
		this->JumpTimeLine->Tick(DeltaTime * 0.55f);
	}
}
