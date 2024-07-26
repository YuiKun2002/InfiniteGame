// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/Flipbook/GameActor/Mouse/Boss/Boss_DaLiShen.h"
//大力神-导弹
#include "GameStart/Flipbook/GameActor/Mouse/Boss/DaLiShen/DaLiShenMissile.h"

ABoss_DaLiShen::ABoss_DaLiShen()
{
	this->M_BoxMesh = CreateDefaultSubobject<UBoxComponent>(TEXT("Meshe_Box"));
	this->M_BoxBody = CreateDefaultSubobject<UBoxComponent>(TEXT("Body_Box"));

	this->M_BoxMesh->SetupAttachment(this->GetRootComponent());
	this->M_BoxBody->SetupAttachment(this->M_BoxMesh);
}

void ABoss_DaLiShen::Def_Tick(float DeltaTime)
{
	if (this->M_EDaLiShenState == EDaLiShenState::Def)
	{
		this->AttakTimeCurrent -= DeltaTime;
		if (this->AttakTimeCurrent <= 0.f)
		{
			this->AttakTimeCurrent = 10.f;

			//目前制作两个技能(1-3)
			switch (UGameSystemFunction::GetRandomRange(0, 2))
			{
			case 2: this->M_EDaLiShenState = EDaLiShenState::Attack_3; break;
			case 1: this->M_EDaLiShenState = EDaLiShenState::Attack_1; break;
			default:this->M_EDaLiShenState = EDaLiShenState::Attack_2; break;
			}

			this->UpdateState();

			this->SetCurrentMoveSpeed(0.f);

			this->MoveStop();
		}
	}
}

void ABoss_DaLiShen::Attack1_Tick(float DeltaTime)
{
	if (M_EDaLiShenState == EDaLiShenState::Attack_1)
	{
		this->Update_Attack_1(DeltaTime);
	}
}

void ABoss_DaLiShen::Attack3_Tick(float DeltaTime)
{
	if (M_EDaLiShenState == EDaLiShenState::Attack_3)
	{
		this->M_EDaLiShenState = EDaLiShenState::Def;

		this->UpdateState();

		//下
		AGameActorFlipbookBase* DaLiShen_Attack_Line_Down = this->GetWorld()->SpawnActor<AGameActorFlipbookBase>(
			LoadClass<AGameActorFlipbookBase>(0,
				TEXT("Blueprint'/Game/Resource/BP/GameStart/Item/Mouse/boss/DaLiShen/DaLiShen_Attack_Line_Down.DaLiShen_Attack_Line_Down_C'")),
			this->GetActorTransform());

		DaLiShen_Attack_Line_Down->InitRotation();
		DaLiShen_Attack_Line_Down->SetFlipbookPitchRotation(90.f);
		DaLiShen_Attack_Line_Down->SetTranslucentSortPriority(8000.f);

		//上
		AGameActorFlipbookBase* DaLiShen_Attack_Line_Up = this->GetWorld()->SpawnActor<AGameActorFlipbookBase>(
			LoadClass<AGameActorFlipbookBase>(0,
				TEXT("Blueprint'/Game/Resource/BP/GameStart/Item/Mouse/boss/DaLiShen/DaLiShen_Attack_Line_Up.DaLiShen_Attack_Line_Up_C'")),
			this->GetActorTransform());

		DaLiShen_Attack_Line_Up->InitRotation();
		DaLiShen_Attack_Line_Up->SetFlipbookPitchRotation(-90.f);
		DaLiShen_Attack_Line_Up->SetTranslucentSortPriority(8000.f);


		this->SetCurrentMoveSpeed(this->GetMoveSpeed());
		this->MoveStart();
	}
}

void ABoss_DaLiShen::UpdateState()
{
	if (this->GetCurrentHP() > this->GetTotalHP() * 0.5f)
	{
		switch (this->M_EDaLiShenState)
		{
		case EDaLiShenState::Def:this->SetPlayAnimation(this->M_Mouse_Def); break;
		case EDaLiShenState::Attack_2:this->SetPlayAnimation(this->M_Mouse_Attack_2); break;
		case EDaLiShenState::Attack_1:this->SetPlayAnimation(this->M_Mouse_Attack_1); break;
		case EDaLiShenState::Attack_3:this->SetPlayAnimation(this->M_Mouse_Attack_3); break;
		}
	}
	else if (this->GetCurrentHP() <= this->GetTotalHP() * 0.5f && this->GetCurrentHP() > 0.f)
	{
		switch (this->M_EDaLiShenState)
		{
		case EDaLiShenState::Def:this->SetPlayAnimation(this->M_Mouse_DefLow); break;
		case EDaLiShenState::Attack_2:this->SetPlayAnimation(this->M_Mouse_Attack_2Low); break;
		case EDaLiShenState::Attack_1:this->SetPlayAnimation(this->M_Mouse_Attack_1Low); break;
		case EDaLiShenState::Attack_3:this->SetPlayAnimation(this->M_Mouse_Attack_3Low); break;
		}
	}
}

void ABoss_DaLiShen::Update_Attack_1(float DeltaTime)
{
	//导弹数量为0
	if (this->Attack_2_Count < 1)
		return;

	if (this->Attack_1_FristDelay > 0.f)
	{
		this->Attack_1_FristDelay -= DeltaTime;
	}

	// Blueprint'/Game/Resource/BP/GameStart/Item/Mouse/boss/DaLiShen/DaLiShen_Attack_1_state.DaLiShen_Attack_1_state'

	int32 M_RowCount = AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->GetMapMeshRowAndCol().Row;
	int32 M_ColCount = AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->GetMapMeshRowAndCol().Col;

	int32 SRandRow = UGameSystemFunction::GetRandomRange(0, M_RowCount - 1);
	int32 SRandCol = UGameSystemFunction::GetRandomRange(0, M_ColCount - 1);


	if (this->Attack_1_FristDelay <= 0.f)
	{
		if (!this->bAttack_1_Frist)
		{
			this->bAttack_1_Frist = true;

			//创建第一颗导弹
			ADaLiShenMissile* const LocalObj = this->GetWorld()->SpawnActor<ADaLiShenMissile>(LoadClass<ADaLiShenMissile>(0, TEXT("Blueprint'/Game/Resource/BP/GameStart/Item/Mouse/boss/DaLiShen/MyDaLiShenMissile.MyDaLiShenMissile_C'")), this->GetActorTransform());
			LocalObj->InitRotation();
			LocalObj->AddActorLocalOffset(FVector(5.f, 30.f, 0.f));
			LocalObj->SetTarget(AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->GetMapMeshLocation(SRandRow, SRandCol));
			LocalObj->SetTranslucentSortPriority(8000);

			if (this->Attack_2_CurrentCount < 9)
				this->Attack_2_CurrentCount++;

			this->Attack_2_Count--;
		}
		else {
			if (this->Attack_1_Delay > 0.f)
				this->Attack_1_Delay -= DeltaTime;
			else {
				this->Attack_1_Delay = 0.1f;

				float Locations[9] = {
					30.f, 45.f,60.f,
					30.f, 45.f,60.f,
					30.f, 45.f,60.f,
				};

				//创建后面的导弹
				ADaLiShenMissile* const LocalObj = this->GetWorld()->SpawnActor<ADaLiShenMissile>(LoadClass<ADaLiShenMissile>(0, TEXT("Blueprint'/Game/Resource/BP/GameStart/Item/Mouse/boss/DaLiShen/MyDaLiShenMissile.MyDaLiShenMissile_C'")), this->GetActorTransform());
				LocalObj->InitRotation();
				LocalObj->AddActorLocalOffset(FVector(5.f, Locations[Attack_2_CurrentCount], 0.f));
				LocalObj->SetTarget(AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->GetMapMeshLocation(SRandRow, SRandCol));
				LocalObj->SetTranslucentSortPriority(8000);

				if (this->Attack_2_CurrentCount < 9)
					this->Attack_2_CurrentCount++;

				this->Attack_2_Count--;
			}
		}
	}
}


void ABoss_DaLiShen::BeginPlay()
{
	this->M_Mouse_Def = LoadObject<UPaperFlipbook>(0,
		*this->GetPath(FString(TEXT("PaperFlipbook'/Game/Resource/Texture/Sprite/VS/Sprite/Mouse/Boss/1/Anim/")),
			FString(TEXT("Def"))));
	this->M_Mouse_DefLow = LoadObject<UPaperFlipbook>(0,
		*this->GetPath(FString(TEXT("PaperFlipbook'/Game/Resource/Texture/Sprite/VS/Sprite/Mouse/Boss/1/Anim/")),
			FString(TEXT("Def_Low"))));

	this->M_Mouse_Attack_1 = LoadObject<UPaperFlipbook>(0,
		*this->GetPath(FString(TEXT("PaperFlipbook'/Game/Resource/Texture/Sprite/VS/Sprite/Mouse/Boss/1/Anim/")),
			FString(TEXT("Attack_1_Def"))));
	this->M_Mouse_Attack_1Low = LoadObject<UPaperFlipbook>(0,
		*this->GetPath(FString(TEXT("PaperFlipbook'/Game/Resource/Texture/Sprite/VS/Sprite/Mouse/Boss/1/Anim/")),
			FString(TEXT("Attack_1_Low"))));

	this->M_Mouse_Attack_2 = LoadObject<UPaperFlipbook>(0,
		*this->GetPath(FString(TEXT("PaperFlipbook'/Game/Resource/Texture/Sprite/VS/Sprite/Mouse/Boss/1/Anim/")),
			FString(TEXT("Attack_2_Def"))));
	this->M_Mouse_Attack_2Low = LoadObject<UPaperFlipbook>(0,
		*this->GetPath(FString(TEXT("PaperFlipbook'/Game/Resource/Texture/Sprite/VS/Sprite/Mouse/Boss/1/Anim/")),
			FString(TEXT("Attack_2_Low"))));

	this->M_Mouse_Attack_3 = LoadObject<UPaperFlipbook>(0,
		*this->GetPath(FString(TEXT("PaperFlipbook'/Game/Resource/Texture/Sprite/VS/Sprite/Mouse/Boss/1/Anim/")),
			FString(TEXT("Attack_3_Def"))));
	this->M_Mouse_Attack_3Low = LoadObject<UPaperFlipbook>(0,
		*this->GetPath(FString(TEXT("PaperFlipbook'/Game/Resource/Texture/Sprite/VS/Sprite/Mouse/Boss/1/Anim/")),
			FString(TEXT("Attack_3_Low"))));

	this->M_Mouse_Death = LoadObject<UPaperFlipbook>(0,
		*this->GetPath(FString(TEXT("PaperFlipbook'/Game/Resource/Texture/Sprite/VS/Sprite/Mouse/Boss/1/Anim/")),
			FString(TEXT("Death"))));

	this->M_BoxMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->M_BoxBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	Super::BeginPlay();
}

void ABoss_DaLiShen::MouseInit()
{
	Super::MouseInit();

	this->InitBoss(TEXT("大力神"), TEXT("605"));

	UGameSystemFunction::InitMouseMeshe(this->M_BoxMesh, this->M_BoxBody, FVector2D(115.f, 20.f), FVector2D(0.f, 17.f), true);
	UGameSystemFunction::InintMouseCollisonByBox(this->M_BoxBody,FVector2D(115.f,55.f),true);

	this->GetRenderComponent()->OnAnimationPlayEnd.Unbind();
	this->GetRenderComponent()->OnAnimationPlayEnd.BindUObject(this, &ABoss_DaLiShen::OnAnimationPlayEnd);

	this->M_BoxMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	this->M_BoxBody->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ABoss_DaLiShen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//默认
	this->Def_Tick(DeltaTime);
	//攻击1
	this->Attack1_Tick(DeltaTime);
	//攻击3
	this->Attack3_Tick(DeltaTime);
}

void ABoss_DaLiShen::OnAnimationPlayEnd()
{
	//攻击1
	if (this->M_EDaLiShenState == EDaLiShenState::Attack_1)
	{
		this->M_EDaLiShenState = EDaLiShenState::Def;

		this->UpdateState();
		this->SetCurrentMoveSpeed(this->GetMoveSpeed());
		this->MoveStart();

		//攻击时间(间隔)->一开始是6秒，后面统一15秒
		this->AttakTimeCurrent = 15.f;
		//首次投射延迟
		this->Attack_1_FristDelay = 0.5f;
		//首次投射是否完成
		this->bAttack_1_Frist = false;
		//投射间隔
		this->Attack_1_Delay = 0.1f;
		//导弹数量
		this->Attack_2_Count = 9;
		//当前创建个数
		this->Attack_2_CurrentCount = 0;
	}
	//攻击2
	if (this->M_EDaLiShenState == EDaLiShenState::Attack_2)
	{
		this->M_EDaLiShenState = EDaLiShenState::Def;

		this->UpdateState();

		AGameActorFlipbookBase* DaLiShen_Attack_Line_Left = this->GetWorld()->SpawnActor<AGameActorFlipbookBase>(
			LoadClass<AGameActorFlipbookBase>(0,
				TEXT("Blueprint'/Game/Resource/BP/GameStart/Item/Mouse/boss/DaLiShen/DaLiShen_Attack_Line_Left.DaLiShen_Attack_Line_Left_C'")),
			this->GetActorTransform());

		DaLiShen_Attack_Line_Left->InitRotation();
		DaLiShen_Attack_Line_Left->SetTranslucentSortPriority(8000.f);

		AGameActorFlipbookBase* DaLiShen_Attack_Line_Right = this->GetWorld()->SpawnActor<AGameActorFlipbookBase>(
			LoadClass<AGameActorFlipbookBase>(0,
				TEXT("Blueprint'/Game/Resource/BP/GameStart/Item/Mouse/boss/DaLiShen/DaLiShen_Attack_Line_Right.DaLiShen_Attack_Line_Right_C'")),
			this->GetActorTransform());

		DaLiShen_Attack_Line_Right->InitRotation();
		DaLiShen_Attack_Line_Right->SetTranslucentSortPriority(8000.f);


		this->SetCurrentMoveSpeed(this->GetMoveSpeed());
		this->MoveStart();
	}
}

void ABoss_DaLiShen::AttackedBegin()
{
	Super::AttackedBegin();
}

void ABoss_DaLiShen::AttackedEnd()
{
	Super::AttackedEnd();
}

void ABoss_DaLiShen::BeAttakedBegin()
{
	Super::BeAttakedBegin();

}

bool ABoss_DaLiShen::BeHit(UObject* CurHitMouseObj, float _HurtValue, EFlyItemAttackType AttackType)
{
	if (!Super::BeHit(CurHitMouseObj, _HurtValue, AttackType))
	{
		return false;
	}

	this->UpdateHP(_HurtValue);

	this->UpdateState();

	return true;
}

void ABoss_DaLiShen::MouseDeathed()
{
	this->M_BoxMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->M_BoxBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	this->SetPlayAnimation(this->M_Mouse_Death);

	Super::MouseDeathed();
}

void ABoss_DaLiShen::MoveingUpdate(float DeltaTime)
{
	Super::MoveingUpdate(DeltaTime);

	this->AddActorLocalOffset(FVector(0.f, DeltaTime * -this->M_Proper_State.M_CurrentMoveSpeed, 0.f));
}

void ABoss_DaLiShen::ExecuteBuff(EGameBuffTag BuffTag, float& CurBuffTime)
{
	if (BuffTag == EGameBuffTag::SlowDown)
	{
		CurBuffTime = 0;
	}

	Super::ExecuteBuff(BuffTag, CurBuffTime);
}