// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameStart/VS/Components/Card/CardAttackComponent.h"

#include "GameStart/Flipbook/GameActor/CardActor/AttackCardActor.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"

#include "GameStart/VS/MapMeshe.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/Components/MesheControllComponent.h"

#include <Paper2D/Classes/PaperSprite.h>
#include <Paper2D/Classes/PaperFlipbook.h>
#include <Paper2D/Classes/PaperFlipbookComponent.h>

#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>

#include "GameSystem/GameDataSubsystem.h"
#include "SpineSkeletonAnimationComponent.h"

void UCardAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	//初始化攻击卡片
	this->AttackCardActor = Cast<AAttackCardActor>(this->GetOwner());

	this->bFirst = false;
	this->bInitItemSort = false;

	if (!this->AttackCardActor)
	{
		if (UFVMGameInstance::GetDebug())
		{
			UGameSystemFunction::FVMLog(TEXT("UCardAttackComponent"), TEXT("攻击卡片初始化失败"));
			this->SetComponentTickEnabled(false);
		}
	}
}

void UCardAttackComponent::LoadResource()
{
	Super::LoadResource();

	//初始化条件
	this->InitLaunchProperty(
		this->AttackCardActor->GetCurrentAttackCount(),
		this->AttackCardActor->GetCurrentAttackSpeed(),
		this->AttackCardActor->GetCurrentFristAttackDelay(),
		this->AttackCardActor->GetCurrentSecondAttackDelay()
	);

	//添加子弹的对象池
	this->Pool.Emplace(UObjectPoolManager::MakePoolManager(
		this->GetWorld(),
		this->AttackCardActor->CardActor_BulletClassObj,
		1));

	//初始化攻击动画
	bool bAddResult = this->AddLaunchRadomItem(
		100,
		this->AttackCardActor->CardActor_BulletClassObj,
		this->AttackCardActor->CardActor_Attack
	);
	if (!bAddResult)
	{
		//添加资源[默认子弹，默认动画] 添加默认攻击动作
		this->OtherItems.Emplace(FCardOtherItem(this->Pool.Num() - 1, 100,
			this->AttackCardActor->CardActor_BulletClassObj,
			SpineCardAnimationState_Attack));
	}

	//初始化发呆动画
	UClass* IdleClass = UGameSystemFunction::LoadRes(this->AttackCardActor->CardActor_Idle);
	TSubclassOf<UAssetCategoryName> IdleNameResource(IdleClass);
	if (IsValid(IdleNameResource.GetDefaultObject()))
	{
		this->TargetIdleAnimationName = IdleNameResource.GetDefaultObject()->GetCategoryName().ToString();
	}
	else {
		this->TargetIdleAnimationName = SpineCardAnimationState_Idle;
	}

	//播放发呆动画
	this->AttackCardActor->SetAnimation(0, this->TargetIdleAnimationName, true);
	this->SetTrackEntry(nullptr);
}

bool UCardAttackComponent::AddLaunchRadomItem(int32 RandomValue, TSoftClassPtr<AFlyItemActor> Res, TSoftClassPtr<UAssetCategoryName> AnimName)
{
	if (RandomValue <= 0)
	{
		RandomValue = 1;
	}
	else if (RandomValue > 100)
	{
		RandomValue = 0;
	}

	UClass* NameClass = AnimName.LoadSynchronous();
	TSubclassOf<UAssetCategoryName> NameObjectResource(NameClass);
	if (IsValid(NameObjectResource.GetDefaultObject()))
	{
		//添加新的攻击方式
		this->Pool.Emplace(UObjectPoolManager::MakePoolManager(this->GetWorld(), Res, 1));
		this->OtherItems.Emplace(FCardOtherItem(
			this->Pool.Num() - 1,
			RandomValue,
			Res,
			NameObjectResource.GetDefaultObject()->GetCategoryName().ToString()
		));

		return true;
	}

	return false;
}

void UCardAttackComponent::OnAnimationComplete(class UTrackEntry* Track)
{
	this->OnAnimationPlayEnd();
}

void UCardAttackComponent::Spawn()
{
	Super::Spawn();

	//当前老鼠无效
	if (!IsValid(this->AttackCardActor->GetCurrentMouse()))
	{
		//手动置空
		this->AttackCardActor->SetCurrentMouse(nullptr);
		return;
	}

	if (this->bFirst)
	{
		//设置随机数
		this->SetRandom();
		//随机子弹
		this->LauncherItem(this->OtherItems, this->CurFinishItems, this->TargetCardOtherItem);
	}
	else {
		//首次发射完毕，下一次发射将进行初始化随机数
		this->bFirst = true;
	}

	if (IsValid(this->TargetCardOtherItem.GetRes()))
	{
		if (!IsValid(this->Pool[this->TargetCardOtherItem.GetIndex()]) || this->Pool.Num() < 1)
		{
			return;
		}

		//获取对象的变换位置
		const FTransform& _trans = this->AttackCardActor->GetActorTransform();
		FTransform NewTrans;
		NewTrans.SetLocation(_trans.GetLocation());


		//获取对象池的对象
		//生成子弹
		AFlyItemActor* _TargetActor = Cast<AFlyItemActor>(
			this->Pool[this->TargetCardOtherItem.GetIndex()]->GetObjectActor()
		);

		//新生成的对象设置自定义拥有者(CardActor)
		_TargetActor->SetLine(this->AttackCardActor->GetLine().Row);
		_TargetActor->SetActorTransform(NewTrans);
		_TargetActor->SetMouseActorLocation(this->AttackCardActor->GetCurrentMouse());
		_TargetActor->SetATK(this->AttackCardActor->GetCurrentATK());
		_TargetActor->SetSecondATK(
			this->AttackCardActor->GetCurrentSecondATK(
				this->AttackCardActor->GetATKCardData().M_SputteringATKRate)
		);
		_TargetActor->SetFloatModeEnable(this->AttackCardActor->GetFloatMode());
		_TargetActor->Init();
		_TargetActor->OnInit();

		AGameMapInstance::GetGameMapInstance()->M_ResourceManagerComponent;
	}
}

void UCardAttackComponent::PlayAttackAnimation()
{
	Super::PlayAttackAnimation();

	//初始化首次发射
	this->bFirst = false;
	//初始化
	this->InitLaunch(this->bInitItemSort, this->OtherItems, this->CurFinishItems);
	//设置随机数
	this->SetRandom();
	//初始化数据
	this->LauncherItem(this->OtherItems, this->CurFinishItems, this->TargetCardOtherItem);

	//播放动画
	UTrackEntry* Track = this->AttackCardActor->SetAnimation(0, this->TargetCardOtherItem.GetAnim(), true);
	//绑定动画事件
	Track->AnimationComplete.AddDynamic(
		this, &UCardAttackComponent::OnAnimationComplete);
	this->SetTrackEntry(Track);
}

void UCardAttackComponent::PlayIdleAnimation()
{
	Super::PlayIdleAnimation();

	this->AttackCardActor->SetAnimation(0, this->TargetIdleAnimationName, true);
	this->SetTrackEntry(nullptr);
}

void UCardAttackComponent::BeginDestroy()
{
	Super::BeginDestroy();

	for (auto& Cur : this->Pool)
	{
		if (IsValid(Cur))
		{
			Cur->ClearAllObjectActor();
			Cur = nullptr;
		}
	}

	this->Pool.Empty();
}

void UCardAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (time < 0.3f)
	{
		this->time += DeltaTime;
	}
	else {
		//老鼠检测
		if (FVM_VS_GamePause())
		{
			return;
		}

		//卡片休息
		if (!this->AttackCardActor->GetCardDay() && this->AttackCardActor->GetMapDay())
		{
			return;
		}

		this->time = 0;

		//计算浮动地板的检测线条
		if (this->AttackCardActor->GetFloatMode())
		{
			this->AttackCardActor->CalculateLineTracePosition(this->AttackCardActor->GetLineTracePosition());
		}

		//当前检测到的老鼠线路
		ELineTraceType CurCheckLineType;
		//当前老鼠
		AActor* CurMouse = UGameSystemFunction::CreateCheckMouseLineTrace(
			this->GetWorld(),
			this->AttackCardActor->GetLineTraceSetting(), CurCheckLineType
		);

		//老鼠有效则设置攻击模式
		if (Cast<AMouseActor>(CurMouse) && IsValid(CurMouse))
		{
			this->AttackCardActor->AnalysisActor(CurMouse);
			this->SetAttackModEnabled(true);
		}
		else
		{
			this->AttackCardActor->SetCurrentMouse(nullptr);
			this->SetAttackModEnabled(false);
		}
	}
}

void UCardAttackComponent::InitLaunch(bool& InitItemSort, const TArray<FCardOtherItem>& Items, TMap<int32, FCardOtherItemInitFinish>& Out)
{
	if (!InitItemSort)
	{
		InitItemSort = true;

		/*
		将所有的攻击动作按照概率分类
		【45%】多少个攻击动作
		【100%】多少个攻击动作
		*/
		//初始化数据
		TMap<int32, TArray<const FCardOtherItem*>> CurMap;
		for (const FCardOtherItem& Cur : Items)
		{
			//集合存在元素
			if (CurMap.Num())
			{
				//遍历集合，添加相同概率的飞行物
				bool bAdd = false;
				for (auto& LMap : CurMap)
				{
					//判断有没有相同概率的对象
					if (LMap.Key == Cur.GetValue())
					{
						LMap.Value.Emplace(&Cur);
						bAdd = true;
					}
				}

				//不相同的概率，单独添加集合
				if (!bAdd)
				{
					TArray<const FCardOtherItem*> CurU;
					CurU.Emplace(&Cur);
					CurMap.Emplace(Cur.GetValue(), CurU);
				}
			}
			else {
				//添加集合
				TArray<const FCardOtherItem*> CurU;
				CurU.Emplace(&Cur);
				CurMap.Emplace(Cur.GetValue(), CurU);
			}
		}

		//排序
		CurMap.KeySort([](const int32& A, const int32& B) {
			return (A < B);
			});

		//初始化元素【完成分类】
		for (const auto& LMap : CurMap)
		{
			TArray<FCardOtherItem> Temp;
			for (const auto& LItem : LMap.Value)
			{
				Temp.Emplace(*LItem);
			}
			Out.Emplace(LMap.Key, FCardOtherItemInitFinish(Temp));
		}
	}
}

void UCardAttackComponent::LauncherItem(
	const TArray<FCardOtherItem>& Items,
	const TMap<int32, FCardOtherItemInitFinish>& OtherItemInitFinish,
	FCardOtherItem& Out
)
{
	if (!Items.Num())
	{
		return;
	}

	//获取随机数
	int32 CurRandom = this->GetRandom();


	/*
		根据数值概率随机选择一种攻击动作
	*/
	int32 CurSelectIndexKey = 100;
	//选择资源
	for (const auto& LMap : OtherItemInitFinish)
	{
		if (LMap.Key >= CurRandom)
		{
			CurSelectIndexKey = LMap.Key;
			break;
		}
	}

	//选择当前段
	int32 CurSplitIndex = UGameSystemFunction::GetRandomRange(0,
		(*(OtherItemInitFinish.Find(CurSelectIndexKey))).GetCount() - 1);

	//获取当前需要发射的物品【初始化当前需要使用的攻击动作】
	Out = (*(OtherItemInitFinish.Find(CurSelectIndexKey))).GetItems()[CurSplitIndex];
}

void UCardAttackComponent::SetRandom()
{
	this->RandomNumber = UGameSystemFunction::GetRandomRange(1, 100);
}

int32 UCardAttackComponent::GetRandom()
{
	return this->RandomNumber;
}

FCardOtherItem::FCardOtherItem(
	int32 PoolIndex,
	int32 RValue,
	TSoftClassPtr<AFlyItemActor> Res,
	FString AnimName) : CurValue(RValue), CurPoolIndex(PoolIndex)
{
	this->CurRes = UGameSystemFunction::LoadRes(Res);
	this->ActionAnim = AnimName;
}

int32 FCardOtherItem::GetValue() const
{
	return this->CurValue;
}

int32 FCardOtherItem::GetIndex() const
{
	return this->CurPoolIndex;
}

UClass* FCardOtherItem::GetRes() const
{
	return this->CurRes;
}

FString FCardOtherItem::GetAnim() const
{
	return this->ActionAnim;
}
