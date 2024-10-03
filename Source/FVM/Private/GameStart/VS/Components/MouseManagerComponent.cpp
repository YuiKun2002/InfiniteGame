// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/MouseManagerComponent.h"
#include "GameStart/VS/GameMapInstance.h"

#include "GameStart/Flipbook/GameActor/MouseActor.h"

#include "GameSystem/FVMGameInstance.h"

#include "GameStart/VS/PlayerView.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/MapMeshe.h"

#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/VS/Components/VSManagerComponent.h"

#include "GameStart/VS/UI/UI_MouseRoundUpTip.h"
#include "GameStart/VS/UI/UI_MouseRound.h"
#include "GameStart/VS/UI/UI_MouseBoss_Slider.h"

#include "GameSystem/GameConfigManager.h"

#include "Data/CardData/MouseDataStruct.h"
#include "Engine/DataTable.h"

#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include <Math/RandomStream.h>
#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>
#include <Kismet/KismetMathLibrary.h>


void UMouseLineManager::AddMouse(const FString& MouseName, AMouseActor* _CurMouse)
{
	if (IsValid(_CurMouse))
	{
		switch (_CurMouse->GetMouseLineType())
		{
		case ELineType::Sky://添加到空中
			this->CurMouseSky.Emplace(MouseName, _CurMouse);
			break;
		default://添加到陆地【默认老鼠行】
			this->CurMouseGround.Emplace(MouseName, _CurMouse);
			break;
		}
	}
}

bool UMouseLineManager::RemoveMouse(AMouseActor* _CurMouse)
{
	if (IsValid(_CurMouse))
	{
		switch (_CurMouse->GetMouseLineType())
		{
		case ELineType::Sky://从空中移除
		{
			if (this->CurMouseSky.Contains(UKismetSystemLibrary::GetObjectName(_CurMouse)))
			{
				this->CurMouseSky.Remove(UKismetSystemLibrary::GetObjectName(_CurMouse));

				return true;
			}
		}
		break;
		default://从陆地移除【默认老鼠行】
		{
			if (this->CurMouseGround.Contains(UKismetSystemLibrary::GetObjectName(_CurMouse)))
			{
				this->CurMouseGround.Remove(UKismetSystemLibrary::GetObjectName(_CurMouse));

				return true;
			}
		}
		break;
		}
	}

	if (UFVMGameInstance::GetDebug())
	{
		UE_LOG(LogTemp, Error, TEXT("【%s】老鼠移除失败，不知本行"), *UKismetSystemLibrary::GetObjectName(_CurMouse));
	}

	return false;
}

bool UMouseLineManager::RemoveMouseByName(const FString& MouseName)
{
	if (this->CurMouseGround.Contains(MouseName))
	{
		this->CurMouseGround.Remove(MouseName);

		return true;
	}
	if (this->CurMouseUnderGround.Contains(MouseName))
	{
		this->CurMouseUnderGround.Remove(MouseName);

		return true;
	}
	if (this->CurMouseSky.Contains(MouseName))
	{
		this->CurMouseSky.Remove(MouseName);

		return true;
	}

	return false;
}

AMouseActor* UMouseLineManager::FindMouse(const FString& MouseName)
{
	AMouseActor** CurMouse = this->CurMouseGround.Find(MouseName);
	if (CurMouse)
	{
		return *CurMouse;
	}

	CurMouse = this->CurMouseSky.Find(MouseName);
	if (CurMouse)
	{
		return *CurMouse;
	}

	return nullptr;
}

void UMouseLineManager::SortMouseByTick(const float& _Tick)
{
	if (CurTime < this->UpdateTickTime)
	{
		CurTime += _Tick;
	}
	else {
		CurTime = 0;
		//排序老鼠位置
		this->CurMouseGroundTop = this->SortMouseTopLocation(this->CurMouseGround);
		this->CurMouseUnderGroundTop = this->SortMouseTopLocation(this->CurMouseUnderGround);
		this->CurMouseSkyTop = this->SortMouseTopLocation(this->CurMouseSky);
	}
}

AMouseActor* UMouseLineManager::GetMouseTopByGorund() const
{
	return this->CurMouseGroundTop;
}

AMouseActor* UMouseLineManager::GetMouseTopByUnderGorund() const
{
	return this->CurMouseUnderGroundTop;
}

AMouseActor* UMouseLineManager::GetMouseTopBySky() const
{
	return this->CurMouseSkyTop;
}

AMouseActor* UMouseLineManager::GetMouseTopByAllType() const
{
	//得到所有类型最前【忽略地下，因为目前没有任何卡片可以直接追踪地下，妨碍大部分卡片，追踪打不到的地下】

	if (!IsValid(this->CurMouseSkyTop) && !IsValid(this->CurMouseGroundTop))
	{
		return nullptr;
	}

	//如果陆地有效
	if (IsValid(this->CurMouseGroundTop) && !IsValid(this->CurMouseSkyTop))
	{
		return this->CurMouseGroundTop;
	}

	//如果空中有效
	if (IsValid(this->CurMouseSkyTop) && !IsValid(this->CurMouseGroundTop))
	{
		return this->CurMouseSkyTop;
	}

	//判断空中和陆地谁最前【相同则陆地优先】
	if (IsValid(this->CurMouseSkyTop) && IsValid(this->CurMouseGroundTop))
	{
		if (this->CurMouseSkyTop->GetActorLocation().Y < this->CurMouseGroundTop->GetActorLocation().Y)
		{
			return this->CurMouseSkyTop;
		}
		else {
			return this->CurMouseGroundTop;
		}
	}

	return nullptr;
}

const TMap<FString, AMouseActor*>& UMouseLineManager::GetMouseGround()
{
	return this->CurMouseGround;
}

const TMap<FString, AMouseActor*>& UMouseLineManager::GetMouseUnderGround()
{
	return this->CurMouseUnderGround;
}

const TMap<FString, AMouseActor*>& UMouseLineManager::GetMouseSky()
{
	return this->CurMouseSky;
}

TMap<FString, AMouseActor*> UMouseLineManager::GetMouseAll()
{
	TMap<FString, AMouseActor*> Temp;
	Temp.Append(this->GetMouseGround());
	Temp.Append(this->GetMouseUnderGround());
	Temp.Append(this->GetMouseSky());
	return Temp;
}

bool UMouseLineManager::GetMouseExist()
{
	//当全部数组为0 则表示当前线路没有任何老鼠
	if (
		this->CurMouseGround.Num() == 0
		&&
		this->CurMouseUnderGround.Num() == 0
		&&
		this->CurMouseSky.Num() == 0
		)
	{
		return false;
	}

	return true;
}

void UMouseLineManager::KillAllMouse()
{
	TMap<FString, AMouseActor*> CurMouses;
	CurMouses.Append(this->CurMouseGround);
	CurMouses.Append(this->CurMouseUnderGround);
	CurMouses.Append(this->CurMouseSky);

	for (auto const CurMouse : CurMouses)
	{
		if (IsValid(CurMouse.Value))
		{
			if (!CurMouse.Value->IsActorTickEnabled())
			{
				CurMouse.Value->SetActorTickEnabled(true);
			}

			CurMouse.Value->SetMouseDeath(true);
		}
	}

	this->CurMouseGround.Empty();
	this->CurMouseUnderGround.Empty();
	this->CurMouseSky.Empty();
}

AMouseActor* UMouseLineManager::SortMouseTopLocation(TMap<FString, AMouseActor*>& _Mouses)
{
	//当前最前面的老鼠
	AMouseActor* CurTop = nullptr;

	bool bRe = false;
	TArray<FString> NotValidName;

	for (auto CurMouse : _Mouses)
	{
		if (IsValid(CurMouse.Value) && CurMouse.Value->GetCurrentHP() > 0.f)
		{

			if (this->bMouseTimePause)
			{
				if (CurMouse.Value->IsActorTickEnabled())
				{
					CurMouse.Value->SetActorTickEnabled(false);
				}
			}
			else {
				if (!CurMouse.Value->IsActorTickEnabled())
				{
					CurMouse.Value->SetActorTickEnabled(true);
				}
			}

			//如果当前老鼠无效，则直接为最前老鼠
			if (!IsValid(CurTop))
			{
				CurTop = CurMouse.Value;
				continue;
			}

			//排序
			if (
				CurMouse.Value->GetActorLocation().Y < CurTop->GetActorLocation().Y//当前老鼠坐标小于当前最小坐标
				)
			{
				CurTop = CurMouse.Value;
			}
		}
		else {
			if (IsValid(CurMouse.Value))
			{
				CurMouse.Value->SetActorTickEnabled(true);
			}
			bRe = true;
			//记录无效老鼠的名称
			NotValidName.Emplace(CurMouse.Key);
		}
	}

	if (bRe)
	{
		for (const FString& CurName : NotValidName)
		{
			_Mouses.Remove(CurName);
		}
	}

	return CurTop;
}
//-----------------------------------------------------------------------------------------------------------------------------


void UMouseTimeManager::Reset()
{
	this->bEnable = false;
	this->Minute = 0;
	this->Second = 0;
	this->CurTime = 0.f;
	this->AllTime = 0.f;
	this->TimeNodeIndex = 0;
	this->TimeNodeRoundIndex = 0;
	this->RoundIndex = 0;
	this->CurRoundIndex = 0;
}



void UMouseTimeManager::UpdateTime(const float& _tick, UMouseManagerComponent* const MouseManagerComponent)
{
	this->AllTime += _tick;

	if (!IsValid(MouseManagerComponent))
	{
		return;
	}

	if (!IsValid(MouseManagerComponent->M_UMouseStructManager))
	{
		return;
	}

	//当所有老鼠全部生成完毕并且进入最后一个回合的末尾
	if (this->bCurAllMouseSpawnEnd)
	{
		return;
	}

	//检测当前回合是否还有老鼠存在【开启条件都是在当前回合最后子节点末尾】
	if (this->bEnableCheckMouseExist)
	{
		//当还有老鼠存在，则什么都不做
		if (MouseManagerComponent->IsMouseExist())
		{
			return;
		}

		//当没有任何老鼠存在时，进入下个回合
		this->bEnableCheckMouseExist = false;

		//获取当前回合的配置
		FRondWidthMouseConfig RoundConfig;
		if (MouseManagerComponent->M_UMouseStructManager->GetMouseRound(this->GetTimeRound(), RoundConfig))
		{
			//检测是否配置钥匙或者徽章
			if (RoundConfig.RoundKey.Enable)
			{
				//创建徽章或者钥匙交换UI

				//暂停游戏
				AGameMapInstance::GetVSManagerComponent_Static()->SetGamePause();
				//创建UI
				UUI_MouseRoundUpTip* LUI_MouseRoundUpTip =
					CreateWidget<UUI_MouseRoundUpTip>(MouseManagerComponent->GetWorld(),
						LoadClass<UUI_MouseRoundUpTip>(0,
							TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_Player/Tips/UI_MouseRoundUpTips.UI_MouseRoundUpTips_C'")
						));

				bool bRe = false;
				if (RoundConfig.RoundKey.KeyName.Equals(TEXT("")))
				{
					LUI_MouseRoundUpTip->SetKeyName(TEXT("精英徽章AB"));
					int32 A = UFVMGameInstance::GetPlayerStructManager_Static()->FindMaterialByName(
						RoundConfig.RoundKey.BadgeNameA);
					int32 B = UFVMGameInstance::GetPlayerStructManager_Static()->FindMaterialByName(
						RoundConfig.RoundKey.BadgeNameB);
					if (A != -1 && B != -1)
					{
						LUI_MouseRoundUpTip->SetKeyImage(
							UFVMGameInstance::GetPlayerStructManager_Static()->
							M_PlayerItems_Material[A].ItemTexturePath.ToString());
						LUI_MouseRoundUpTip->SetData(
							false, RoundConfig.RoundKey.BadgeNameA, RoundConfig.RoundKey.BadgeNameB);
						bRe = true;
					}
				}
				else {
					LUI_MouseRoundUpTip->SetKeyName(RoundConfig.RoundKey.KeyName);
					int32 A = UFVMGameInstance::GetPlayerStructManager_Static()->FindMaterialByName(RoundConfig.RoundKey.KeyName);
					if (A != -1)
					{
						LUI_MouseRoundUpTip->SetKeyImage(
							UFVMGameInstance::GetPlayerStructManager_Static()->
							M_PlayerItems_Material[A].ItemTexturePath.ToString());
						LUI_MouseRoundUpTip->SetData(true, RoundConfig.RoundKey.KeyName, TEXT(""));
						bRe = true;
					}
				}

				if (bRe)
				{
					LUI_MouseRoundUpTip->AddToViewport();
				}
				else {
					UVSManagerComponent::GameWin();
				}

				return;
			}
			else {
				//进入下个回合
				this->InNextRound(RoundConfig.InNextRoundTime);
			}
		}

		return;
	}

	//开启进入下一个回合的计时
	if (this->bEnableNextRound)
	{
		//检测当前回合是否是最后一个回合
		if (this->bEnableCheckNextRoundIsEnd)
		{
			//判断当前回合是否是最后一个回合
			if (this->GetTimeRound() + 1 == MouseManagerComponent->M_UMouseStructManager->GetRoundTotal())
			{
				this->bCurRoundIsEnd = true;
			}
			else {
				this->bCurRoundIsEnd = false;
			}
		}

		//如果当前是最后一个回合，并且要求进入下一个回合，将不会在更新老鼠生成器，表示需要生成的老鼠已经全部生成
		if (this->bCurRoundIsEnd)
		{
			this->bCurAllMouseSpawnEnd = true;
			return;
		}

		//更新进入下一个会的时间
		if (this->CurNextRoundTime < this->CurNextRoundTotalTime)
		{
			this->CurNextRoundTime += _tick;
		}
		else {
			//关闭进入下一个回合的计时
			this->bEnableNextRound = false;
			//清除当前计时
			this->CurNextRoundTime = 0.f;
			//重置最小子节点
			this->TimeNodeRoundIndex = 0;
			//重置当前回合子节点
			this->TimeNodeIndex = 0;
			//更新回合索引
			this->RoundIndex++;
			this->CurRoundIndex++;
			//继续进行节点更新
			this->bUpdateCurNode = true;
			//当节点发生变换时，触发
			this->OnRoundNodeChanged.ExecuteIfBound();
			//当进入下一个回合时，触发
			this->OnInNextRound.ExecuteIfBound();
			//触发新的回合
			AGameMapInstance::GetGameMapInstance()->OnUpdateRoundDelegate.Broadcast(this->RoundIndex);

			return;
		}
	}
	else {

		//不会更新当前回合的节点
		if (!this->bUpdateCurNode)
			return;

		if (this->CurTime < 1.f)
		{
			this->CurTime += _tick;
		}
		else {
			this->CurTime = 0;

			//得到当前回合的子节点数量
			int32 CurRoundNode = MouseManagerComponent->M_UMouseStructManager->GetRoundNodeCountByRoundIndex(this->RoundIndex);
			if (CurRoundNode != 0)
			{
				//更新最小子节点【必须跑满19 即20秒】
				this->TimeNodeIndex++;
				if (this->TimeNodeIndex > 19)
				{
					this->TimeNodeIndex = 0;


					//更新最小大节点【每20秒必须进1】，必须跑满当前回合数的子节点最大数量
					this->TimeNodeRoundIndex++;
					if (this->TimeNodeRoundIndex == CurRoundNode)
					{
						//不再继续更新当前节点
						this->bUpdateCurNode = false;
						//回合末尾
						this->OnInCurRoundEnd.ExecuteIfBound();
					}
					else {
						//当节点发生变换时，触发[>20 Round++ != CurRoundNode]
						this->OnRoundNodeChanged.ExecuteIfBound();
					}
				}
				else {
					//当节点发生变换时，触发[<= 20]
					this->OnRoundNodeChanged.ExecuteIfBound();
				}
			}
			else {
				//不再继续更新当前节点
				this->bUpdateCurNode = false;
				//回合末尾
				this->OnInCurRoundEnd.ExecuteIfBound();
			}
		}
	}

}

void UMouseTimeManager::InNextRound(const float& _DelayTime)
{
	//开始最后一个的回合检测
	this->bEnableCheckNextRoundIsEnd = true;
	//开启进入下一个回合
	this->bEnableNextRound = true;
	//重置进入下一个会的当前时间
	this->CurNextRoundTime = 0.f;
	//不再更新子节点
	this->bUpdateCurNode = false;
	//设置进入下一个回合的延迟
	this->CurNextRoundTotalTime = _DelayTime;
}

void UMouseTimeManager::InNextRound()
{
	//检测老鼠是否存在
	this->bEnableCheckMouseExist = true;
}

bool UMouseTimeManager::GetCurRoundEnd()
{
	return this->bUpdateCurNode ? false : true;
}

bool UMouseTimeManager::GetAllMouseSpawnEnd()
{
	return this->bCurAllMouseSpawnEnd;
}

void UMouseTimeManager::Enable()
{
	if (this->bEnable)
		return;

	this->bEnable = true;

	//当启动时间时，表示准备时间已经结束
	this->OnRoundNodeChanged.ExecuteIfBound();
}

int32 UMouseTimeManager::GetTimeNode()
{
	return this->TimeNodeIndex;
}

int32 UMouseTimeManager::GetTimeNodeRound()
{
	return this->TimeNodeRoundIndex;
}

int32 UMouseTimeManager::GetTimeRound()
{
	return this->RoundIndex;
}

int32 UMouseTimeManager::GetCurrentTimeRound()
{
	return this->CurRoundIndex;
}

void UMouseTimeManager::SetTimeRound(const int32& Index)
{
	this->RoundIndex = Index;
}

void UMouseTimeManager::SetRoundEnd()
{
	this->bEnableCheckMouseExist = false;
	this->bCurAllMouseSpawnEnd = true;
}

int32 UMouseTimeManager::GetTime()
{
	return (int32)(this->AllTime);
}

//-----------------------------------------------------------------------------------------------------------------------------

AMouseActor* UMouseSpawnManager::SpawnMouse(
	class UMouseManagerComponent* Comp,
	const FString& _MousePath,
	const FLine& Line,
	const int32& RowMax,
	bool DeathFroceGameWin,
	const TArray<FString>& Items,
	const FMouseBase& MouseData
)
{
	TSoftClassPtr<AMouseActor> Res(_MousePath);
	UClass* NewClass = UGameSystemFunction::LoadRes(Res);

	if (!IsValid(NewClass))
	{
		return nullptr;
	}

	FTransform Trans;
	Trans.SetLocation(FVector(2000.f, 0.f, 0.f));
	AMouseActor* CurNewMouse = Comp->GetOwner()->GetWorld()->SpawnActor<AMouseActor>(NewClass, Trans);
	if (IsValid(CurNewMouse))
	{
		//设置老鼠标记
		CurNewMouse->SetMouseTag(MouseData.M_MouseTag);
		//设置老鼠死亡时，是否直接结束游戏
		CurNewMouse->SetCurMouseDeathForceGameWin(DeathFroceGameWin);
		//设置老鼠专属掉落物
		CurNewMouse->SetMouseLevelItems(Items);
		//设置老鼠基础速度
		CurNewMouse->SetMoveSpeed(MouseData.M_MouseBaseSpeed);
		//设置老鼠基础攻击力
		CurNewMouse->SetMouseATK(MouseData.M_MouseBaseATK);
		//设置老鼠基础经验值
		CurNewMouse->SetEx(MouseData.M_MouseEx);
		//设置线路类型
		CurNewMouse->SetMouseLineType(MouseData.LineType);
		//设置坐标
		CurNewMouse->SetActorLocation(
			AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->M_LineLastLocation[Line.Row]
			+ FVector(0.f, 90.f, -15.f)
		);
		//设置旋转
		CurNewMouse->InitSpineShow();

		//获取排序
		int32 CurSort = AGameMapInstance::GetGameMapInstance()->
			M_MesheControllComponent->
			GetMapMeshCurrentRight(Line.Row)->GetTranslucency();
		//设置排序
		CurNewMouse->SetRenderLayer(CurSort);
		//设置线路
		CurNewMouse->SetMouseLine(Line);
		//设置血量
		int32 CurLevel = AGameMapInstance::GetGameMapInstance()->GetGameMapStructManager()->LevelConfig.MouseLevel <= 0
			?
			1 : AGameMapInstance::GetGameMapInstance()->GetGameMapStructManager()->LevelConfig.MouseLevel;
		//当前总生命 + 每一个等级提升 * 当前总生命的50%
		CurNewMouse->SetHP(MouseData.M_MouseBaseHP + (CurLevel - 1) * (MouseData.M_MouseBaseHP / 2.f));
	}

	return CurNewMouse;
}

AMouseActor* UMouseSpawnManager::MakeNewMouseByClass(
	UMouseManagerComponent* Comp,
	const TSoftClassPtr<AMouseActor>& MouseClass,
	const FVector& Location, float HP, float ATK, float Speed, FLine MouseLine,
	const ELineType& MouseLineType,
	EMouseTag NewTag,
	bool bAddManager
)
{
	if (!UGameSystemFunction::LoadRes(MouseClass))
	{
		return nullptr;
	}

	FTransform Trans;
	Trans.SetLocation(FVector(2000.f, 0.f, 0.f));
	AMouseActor* CurNewMouse = Comp->GetOwner()->GetWorld()->SpawnActor<AMouseActor>(
		UGameSystemFunction::LoadRes(MouseClass), Trans
	);

	if (IsValid(CurNewMouse))
	{
		//设置老鼠标记
		CurNewMouse->SetMouseTag(NewTag);
		//设置老鼠基础速度
		CurNewMouse->SetMoveSpeed(Speed);
		//设置老鼠基础攻击力
		CurNewMouse->SetMouseATK(ATK);
		//设置线路类型
		CurNewMouse->SetMouseLineType(MouseLineType);
		//设置坐标
		CurNewMouse->SetActorLocation(Location);
		//设置旋转
		CurNewMouse->InitSpineShow();
		//检测网格是否有效
		if (!AGameMapInstance::GetGameMapInstance()->
			M_MesheControllComponent->CheckMesheIsValid(MouseLine.Row,
				AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->GetMapMeshRowAndCol().Col - 1))
		{
			return nullptr;
		}
		//获取排序
		int32 CurSort = AGameMapInstance::GetGameMapInstance()->
			M_MesheControllComponent->
			GetMapMeshCurrentRight(MouseLine.Row)->GetTranslucency();
		//设置排序
		CurNewMouse->SetRenderLayer(CurSort);
		//设置血量
		int32 CurLevel = AGameMapInstance::GetGameMapInstance()->GetGameMapStructManager()->LevelConfig.MouseLevel <= 0
			?
			1 : AGameMapInstance::GetGameMapInstance()->GetGameMapStructManager()->LevelConfig.MouseLevel;
		//当前总生命
		CurNewMouse->SetHP(HP + (CurLevel - 1) * (HP / 2.f));

		if (UMouseManagerComponent::TestMouseSpawnDeath())
		{
			CurNewMouse->MouseInit();
			CurNewMouse->SetMouseDeath(true);
			return nullptr;
		}

		//设置线路
		CurNewMouse->SetMouseLine(MouseLine);

		if (bAddManager)
		{
			//添加到管理器
			UMouseManagerComponent::AddMouse(CurNewMouse, MouseLine.Row);
		}
		else {
			CurNewMouse->MouseInit();
		}

		//发射射线
		UGameSystemFunction::AddLineTrance(
			CurNewMouse->GetWorld(), CurNewMouse->GetActorLocation(),
			FVector(-9999.f, 0.f, 0.f), FVector(9999.f, 0.f, 0.f), ECollisionChannel::ECC_GameTraceChannel6,
			CurNewMouse,
			[](UObject* Obj, AActor* Ac) {
				AMouseActor* Cur = Cast<AMouseActor>(Obj);
				AMapMeshe* CurMeshe = Cast<AMapMeshe>(Ac);
				if (IsValid(Cur) && IsValid(CurMeshe))
				{
					Cur->InMapMeshe(CurMeshe->GetLineType());
				}
			}
		);

		//触发生成老鼠的委托
		AGameMapInstance::GetGameMapInstance()->OnSpawnMouseDelegate.Broadcast(
			CurNewMouse->MouseObjName,
			CurNewMouse->GetMouseBaseType() == EMouseBaseType::Boss
		);

		return CurNewMouse;
	}

	return nullptr;
}


bool UMouseSpawnManager::MakeNewMouseByName(UDataTable* MouseDataTable, FString MouseName, FLine Line, FVector Location)
{
	//当数据表有效时
	if (MouseDataTable && AGameMapInstance::GetGameMapInstance())
	{
		if (Line.Row >= AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->M_LineLastLocation.Num())
		{
			return false;
		}

		FMouse_Data* CurRow = MouseDataTable->FindRow<FMouse_Data>(FName(MouseName), TEXT("MouseData"));
		if (CurRow)
		{
			const FMouseBase& CurData = CurRow->M_Mouse;

			FTransform Trans;
			Trans.SetLocation(FVector(2000.f, 0.f, 0.f));

			AMouseActor* CurNewMouse = AGameMapInstance::GetGameMapInstance()->GetWorld()->SpawnActor<AMouseActor>(
				Cast<UClass>(CurData.ItemTarget_ActorFilePath.TryLoad()), Trans
			);

			if (IsValid(CurNewMouse))
			{
				//设置老鼠标记
				CurNewMouse->SetMouseTag(CurData.M_MouseTag);
				//设置老鼠基础速度
				CurNewMouse->SetMoveSpeed(CurData.M_MouseBaseSpeed);
				//设置老鼠基础攻击力
				CurNewMouse->SetMouseATK(CurData.M_MouseBaseATK);
				//设置线路类型
				CurNewMouse->SetMouseLineType(CurData.LineType);

				//位置判断
				if (Location == FVector::ZeroVector)
				{
					//根据线路来决定位置
					//设置坐标
					CurNewMouse->SetActorLocation(
						AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->M_LineLastLocation[Line.Row]
						+ FVector(0.f, 90.f, MOUSE_Z_OFFSET)
					);
				}
				else {
					//设置坐标
					CurNewMouse->SetActorLocation(Location);
				}

				//设置旋转
				CurNewMouse->InitSpineShow();
				//获取排序
				int32 CurSort = AGameMapInstance::GetGameMapInstance()->
					M_MesheControllComponent->
					GetMapMeshCurrentRight(Line.Row)->GetTranslucency();
				//设置排序
				CurNewMouse->SetRenderLayer(CurSort);

				//设置血量
				int32 CurLevel =
					AGameMapInstance::GetGameMapInstance()->GetGameMapStructManager()->LevelConfig.MouseLevel <= 0
					?
					1 : AGameMapInstance::GetGameMapInstance()->GetGameMapStructManager()->LevelConfig.MouseLevel;
				//当前总生命
				CurNewMouse->SetHP(CurData.M_MouseBaseHP + (CurLevel - 1) * (CurData.M_MouseBaseHP / 2.f));

				if (UMouseManagerComponent::TestMouseSpawnDeath())
				{
					CurNewMouse->MouseInit();
					CurNewMouse->SetMouseDeath(true);
					return false;
				}

				//设置线路
				CurNewMouse->SetMouseLine(Line);
				//添加到管理器
				UMouseManagerComponent::AddMouse(CurNewMouse, Line.Row);

				//发射射线
				UGameSystemFunction::AddLineTrance(
					CurNewMouse->GetWorld(), CurNewMouse->GetActorLocation(),
					FVector(-9999.f, 0.f, 0.f), FVector(9999.f, 0.f, 0.f), ECollisionChannel::ECC_GameTraceChannel6,
					CurNewMouse,
					[](UObject* Obj, AActor* Ac) {
						AMouseActor* Cur = Cast<AMouseActor>(Obj);
						AMapMeshe* CurMeshe = Cast<AMapMeshe>(Ac);
						if (IsValid(Cur) && IsValid(CurMeshe))
						{
							Cur->InMapMeshe(CurMeshe->GetLineType());
						}
					}
				);

				//触发生成老鼠的委托
				AGameMapInstance::GetGameMapInstance()->OnSpawnMouseDelegate.Broadcast(
					CurNewMouse->MouseObjName,
					CurNewMouse->GetMouseBaseType() == EMouseBaseType::Boss
				);

				return true;
			}
		}
	}

	return false;
}

//-----------------------------------------------------------------------------------------------------------------------------

//静态变量初始化
UMouseManagerComponent* UMouseManagerComponent::M_S_CurrentClass = nullptr;

// Sets default values for this component's properties
UMouseManagerComponent::UMouseManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	UMouseManagerComponent::M_S_CurrentClass = this;
}


// Called when the game starts
void UMouseManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	UMouseManagerComponent::M_S_CurrentClass = this;
}

void UMouseManagerComponent::InitMouseStructManager()
{
	UFVMGameInstance* L_UFVMGameInstance = UFVMGameInstance::GetFVMGameInstance();

	if (IsValid(L_UFVMGameInstance))
	{
		//获取地图存档
		UGameMapStructManager* _L_UGameMapStructManager = L_UFVMGameInstance->GetGameMapStructManager();

		if (!_L_UGameMapStructManager && !IsValid(_L_UGameMapStructManager))
		{
			if (UFVMGameInstance::GetDebug())
			{
				UE_LOG(LogTemp, Error, TEXT("[UMouseManagerComponent::InitMouseStructManager]:地图存档获取失败!初始化失败!"));
			}

			return;
		}

		if (IsValid(L_UFVMGameInstance->GetMouseStructManager()))
		{
			//初始化本老鼠管理器
			this->M_UMouseStructManager = L_UFVMGameInstance->GetMouseStructManager();
		}
		else {
			if (UFVMGameInstance::GetDebug())
			{
				UE_LOG(LogTemp, Error, TEXT("[UMouseManagerComponent::InitMouseStructManager]:老鼠配置加载失败!"));
			}
		}
	}
}


void UMouseManagerComponent::InitMouseLineManager()
{
	//从老鼠配置管理器中拿到当前地图的总行，生成对应的线路
	for (int32 i = 0; i < this->M_UMouseStructManager->GetRowCount(); i++)
	{
		this->MouseLineManager.Emplace(NewObject<UMouseLineManager>());

		if (UFVMGameInstance::GetDebug())
		{
			UE_LOG(LogTemp, Error, TEXT("[UMouseManagerComponent::InitMouseLineManager]:生成线路管理器![%d]"), i);
		}
	}
}

void UMouseManagerComponent::InitTimer()
{
	this->MouseTimeManager = NewObject<UMouseTimeManager>();
	this->CurGameReadyTime = 0.f;

	//绑定事件【更新一个子节点时】
	this->MouseTimeManager->OnRoundNodeChanged.BindUObject(this, &UMouseManagerComponent::OnRoundNodeChangedCallback);
	//进入当前回合末尾时
	this->MouseTimeManager->OnInCurRoundEnd.BindUObject(this, &UMouseManagerComponent::OnCurRoundEndCallback);
	//进入下一个回合时
	this->MouseTimeManager->OnInNextRound.BindUObject(this, &UMouseManagerComponent::InitNextRoundCallBack);
}

void UMouseManagerComponent::InitMouseResource()
{
	if (!IsValid(UFVMGameInstance::GetFVMGameInstance()))
	{
		return;
	}

	if (UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_GameEF.M_bEnable)
	{
		//初始化特效
		//初始化对象池管理器
		this->BurnBuffFxObjPoolManager = UObjectPoolManager::MakePoolManager(
			AGameMapInstance::GetGameMapInstance()->GetWorld(),
			TSoftClassPtr<AActor>(FSoftClassPath(FString(
				TEXT("Blueprint'/Game/Resource/BP/Data/Fx/Burn/BP_Fxctrl.BP_Fxctrl_C'"))))
			, 1
		);

		this->SolidiBuffFxObjPoolManager = UObjectPoolManager::MakePoolManager(
			AGameMapInstance::GetGameMapInstance()->GetWorld(),
			TSoftClassPtr<AActor>(FSoftClassPath(FString(
				TEXT("Blueprint'/Game/Resource/BP/Data/Fx/Solidification/BP_SolidFxctrl.BP_SolidFxctrl_C'"))))
			, 1
		);
	}

	//初始化老鼠数据
	UGameSystemFunction::GetDataTableRows(
		TEXT("DataTable'/Game/Resource/BP/Data/MouseData/MouseData.MouseData'"),
		this->AllMouseData);
}

void UMouseManagerComponent::InitMouseSpawnManager()
{
	this->MouseSpawnManager = NewObject<UMouseSpawnManager>();
}

const UMouseTimeManager* const UMouseManagerComponent::GetMouseTimeManager()
{
	return this->MouseTimeManager;
}

bool UMouseManagerComponent::GetMousePathByName(const FString& MouseName, FString& OutMousePath, FMouseBase& OutData)
{
	const FMouseConfig& CurConfig = this->M_UMouseStructManager->GetMouseConfig();
	
	FString CurName = *CurConfig.AllMouseKeyListMap.Find(FCString::Atoi(*MouseName));

	for (const auto& Cur : this->AllMouseData)
	{
		if (Cur.M_Mouse.M_MouseName.Equals(CurName))
		{
			OutMousePath = Cur.M_Mouse.ItemTarget_ActorFilePath.ToString();

			OutData = Cur.M_Mouse;

			return true;
		}
	}

	return false;
}

void UMouseManagerComponent::OnRoundNodeChangedCallback()
{
	//得到当前回合
	FRondWidthMouseConfig RoundConfig;
	if (this->M_UMouseStructManager->GetMouseRound(this->MouseTimeManager->GetTimeRound(), RoundConfig))
	{
		//得到当前回合子节点
		FTimeNodeWidthRound RoundTimeNode;
		if (this->M_UMouseStructManager->GetMouseTimeNodeByRound(
			RoundConfig,
			this->MouseTimeManager->GetTimeNodeRound(),
			RoundTimeNode))
		{
			//从子节点中找到分段最小子节点，并且生成老鼠
			FTimeNode TimeNode;
			if (this->M_UMouseStructManager->GetMouseTimeNodeByTimeNode(
				RoundTimeNode, this->MouseTimeManager->GetTimeNode(), TimeNode)
				)
			{
				//地图行
				TArray<int32> CurRows;
				for (int32 i = 0; i < this->MouseLineManager.Num(); i++)
				{
					CurRows.Emplace(i);
				}

				//生成老鼠
				for (const auto& Cur : TimeNode.CurMouseNode)
				{
					//老鼠的资产路径
					FString CurMousePath;
					FMouseBase MouseData;
					//拿到路径并且生成老鼠
					if (this->GetMousePathByName(Cur.CurMouseName, CurMousePath, MouseData))
					{
						//当前最新的老鼠
						AMouseActor* CurMouse = nullptr;

						//临时的全行
						TArray<int32> LocalRows = CurRows;
						//排除行
						for (const int32& CurRow : Cur.IgnoreRows)
						{
							int32 CurLocalRowIndex = LocalRows.Find(CurRow);
							if (CurLocalRowIndex != -1)
							{
								LocalRows.RemoveAt(CurLocalRowIndex);
							}
						}

						//当前线路
						FLine CurMouseLine = Cur.CurMouseLine;

						//判断设置的当前线路是否被忽略,被忽略则随机线路
						if (LocalRows.Find(CurMouseLine.Row) == -1 || CurMouseLine.Row == -1)
						{
							int32 CurRowIndex = UKismetMathLibrary::RandomIntegerInRange(0, LocalRows.Num() - 1);
							CurMouseLine.Row = LocalRows[CurRowIndex];
						}

						//生成老鼠
						CurMouse = this->MouseSpawnManager->SpawnMouse(
							this,
							CurMousePath,
							CurMouseLine,
							this->MouseLineManager.Num() - 1,
							Cur.CurMouseDeathForceGameWin,
							Cur.LevelItems, MouseData
						);

						//添加到线路管理器中,实时追踪老鼠
						if (IsValid(CurMouse))
						{
							if (this->bKillMouse)
							{
								CurMouse->MouseInit();
								CurMouse->SetMouseDeath(true);
							}
							else {
								if (this->MouseLineManager.Num() > CurMouse->GetMouseLine().Row)
								{
									//添加老鼠
									UMouseManagerComponent::AddMouse(CurMouse, CurMouse->GetMouseLine().Row);

									//触发生成老鼠的委托
									AGameMapInstance::GetGameMapInstance()->OnSpawnMouseDelegate.Broadcast(
										CurMouse->MouseObjName,
										CurMouse->GetMouseBaseType() == EMouseBaseType::Boss
									);
								}
								else {
									CurMouse->MouseInit();
									CurMouse->SetMouseDeath(true);
								}
							}
						}
					}
				}
			}
		}
	}

	//更新进度值
	this->UpdateRoundProgress();
}

void UMouseManagerComponent::OnCurRoundEndCallback()
{
	//获取当前回合的配置
	FRondWidthMouseConfig CurRoundConfig;
	if (this->M_UMouseStructManager->GetMouseRound(this->MouseTimeManager->GetTimeRound(), CurRoundConfig))
	{
		//如果开启了，当老鼠全部生成完毕自动进入下一个回合，直接进入下一个回合
		//并且没有配置钥匙
		if (CurRoundConfig.AllMouseSpawnFinishAutoNext && !CurRoundConfig.RoundKey.Enable)
		{
			this->MouseTimeManager->InNextRound(CurRoundConfig.InNextRoundTime);
		}
		else {
			//开启进入下一个回合的检测
			this->MouseTimeManager->InNextRound();
		}
	}
}

void UMouseManagerComponent::InitNextRoundCallBack()
{

	UFVMGameInstance::PlayBGM_S_Static(TEXT("MouseComing"), TEXT("ItemAudio"));

	//创建UI显示
	UUserWidget* MouseRoundTip = CreateWidget<UUserWidget>(
		this->GetWorld(),
		LoadClass<UUserWidget>(0,
			TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_Player/Tips/UI_MouseRoundTips.UI_MouseRoundTips_C'")
		));
	MouseRoundTip->AddToViewport();

	//更新进度值
	this->UpdateRoundProgress();

	if (this->IsEndRound())
	{
		//最后一波
		AGameMapInstance::GetGameMapInstance()->OnUpdateRoundLastDelegate.Broadcast();
	}
}

// Called every frame
void UMouseManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(this->M_UMouseStructManager))
	{
		return;
	}


	//如果游戏暂停则不会继续
	if (AGameMapInstance::GetGameMapInstance()->GetGamePause())
	{
		return;
	}


	//准备时间
	this->CurGameReadyTime += DeltaTime;
	if (this->CurGameReadyTime >= AGameMapInstance::GetGameMapInstance()->GetGameMapStructManager()->LevelConfig.GameReadyTime)
	{
		//开启计时
		if (IsValid(this->MouseTimeManager))
		{
			this->MouseTimeManager->Enable();
			this->MouseTimeManager->UpdateTime(DeltaTime, this);
		}

		//更新路线管理器
		for (const auto& CurLine : this->MouseLineManager)
		{
			if (IsValid(CurLine))
			{
				CurLine->SortMouseByTick(DeltaTime);
			}
		}

		this->CurGameReadyTime = AGameMapInstance::GetGameMapInstance()->GetGameMapStructManager()->LevelConfig.GameReadyTime;
	}
}

void UMouseManagerComponent::ShowMouseUI(int32 Round)
{
	if (!IsValid(this->M_UUI_MouseRound))
	{
		this->M_UUI_MouseRound = CreateWidget<UUI_MouseRound>(
			this->GetWorld(),
			LoadClass<UUI_MouseRound>(0,
				TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_Player/UIMouseRound.UIMouseRound_C'")
			));

		this->M_UUI_MouseRound->AddToViewport();
	}

	this->CurRoundTotal = Round;

	//设置属性
	this->M_UUI_MouseRound->InitFlag(this->CurRoundTotal);
}

void UMouseManagerComponent::RemoveMouseUI()
{
	if (IsValid(this->M_UUI_MouseRound) && this->M_UUI_MouseRound->IsInViewport())
	{
		this->M_UUI_MouseRound->RemoveFromParent();
	}
}

void UMouseManagerComponent::UpdateRoundProgress()
{
	//处于正确的回合内
	if (this->MouseTimeManager->GetTimeRound() <= this->CurRoundTotal)
	{
		//得到当前回合的回合总个数【节点个数】
		int32 CurRoundNodeCount = this->M_UMouseStructManager->GetRoundNodeCountByRoundIndex(
			this->MouseTimeManager->GetTimeRound()
		);

		float CurTimeNodeRound =
			(this->MouseTimeManager->GetTimeRound() == this->CurRoundTotal) ?
			this->MouseTimeManager->GetTimeNodeRound() : this->MouseTimeManager->GetTimeNodeRound() + 1.f;

		//计算当前回合与上一个回合的0-1进度，直接当作当前回合与总回合的进度
		float CurRoundClip = (CurRoundNodeCount != 0) ?
			(CurTimeNodeRound / CurRoundNodeCount * 1.f) : 1.f;

		//得到当前回合与下一个回合的进度
		float CurRound = this->MouseTimeManager->GetTimeRound() + CurRoundClip;

		//更新回合进度
		this->M_UUI_MouseRound->UpdateMouseRoundSlider(CurRound, this->CurRoundTotal);
		this->M_UUI_MouseRound->UpdateMouseRoundText(this->MouseTimeManager->GetTimeRound() + 1);
	}
}

void UMouseManagerComponent::ForceNextRound()
{
	//开启进入下一个回合的检测
	this->MouseTimeManager->InNextRound(0.f);
}

void UMouseManagerComponent::ForceNextEndRound()
{
	//在场所有路线的老鼠全部标记死亡
	for (UMouseLineManager* const CurLineMan : this->MouseLineManager)
	{
		CurLineMan->KillAllMouse();
	}

	//检索最后一个回合，知道遇见钥匙或者末尾
	for (int32 CurRoundIndex = this->MouseTimeManager->GetTimeRound(); CurRoundIndex < this->M_UMouseStructManager->GetMouseConfig().CurRoundConfig.Num(); CurRoundIndex++)
	{
		FRondWidthMouseConfig OutCurRound;
		if (this->M_UMouseStructManager->GetMouseRound(CurRoundIndex, OutCurRound))
		{
			if (OutCurRound.RoundKey.Enable)
			{
				//设置回合索引
				this->MouseTimeManager->SetTimeRound(CurRoundIndex);

				//强行进入下一个回合
				this->ForceNextRound();

				this->UpdateRoundProgress();

				return;
			}
		}
	}

	//直接将当前回合设置为最后一个回合末尾
	this->MouseTimeManager->SetRoundEnd();
}

bool UMouseManagerComponent::IsEndRound()
{
	if (IsValid(this->MouseTimeManager))
	{
		if (
			//配置中的总波数
			CurRoundTotal - 1
			==
			//当前时间继续到的波数
			this->MouseTimeManager->GetCurrentTimeRound()
			)
		{
			return true;
		}

		return false;
	}

	return true;
}

bool UMouseManagerComponent::IsMouseSpawnEnd()
{
	if (IsValid(this->MouseTimeManager))
	{
		return this->MouseTimeManager->GetAllMouseSpawnEnd();
	}

	return false;
}

void UMouseManagerComponent::GameStart()
{
	//初始化老鼠配置管理器
	this->InitMouseStructManager();
	//初始化老鼠线路管理器
	this->InitMouseLineManager();
	//初始化时间配置
	this->InitTimer();
	//初始化老鼠资源配置
	this->InitMouseResource();
	//初始化老鼠生成器
	this->InitMouseSpawnManager();
	//初始化UI
	this->ShowMouseUI(this->M_UMouseStructManager->GetNormalRoundTotal());
}


void UMouseManagerComponent::GameOver()
{
	if (UFVMGameInstance::GetDebug())
		UE_LOG(LogTemp, Warning, TEXT("[UMouseManagerComponent::GameOver]:游戏结束"));

	this->AllMouseData.Empty();
	this->MouseLineManager.Empty();
	this->MouseTimeManager = nullptr;
	this->MouseSpawnManager = nullptr;
	this->M_UMouseStructManager = nullptr;
	this->M_UUI_MouseRound = nullptr;
}

bool UMouseManagerComponent::IsMouseExist()
{
	for (UMouseLineManager* const CurLine : this->MouseLineManager)
	{
		if (CurLine->GetMouseExist())
		{
			return true;
		}
	}
	return false;
}

const int32 UMouseManagerComponent::GetLineCount() const
{
	return this->MouseLineManager.Num();
}

int32 UMouseManagerComponent::GetCurTime()
{
	if (IsValid(this->MouseTimeManager))
	{
		return this->MouseTimeManager->GetTime();
	}

	return 9999;
}

int32 UMouseManagerComponent::GetCurRound()
{
	if (IsValid(this->MouseTimeManager))
	{
		return this->MouseTimeManager->GetTimeRound();
	}

	return -1;
}

AMouseActor* const UMouseManagerComponent::GetLineTopFirstMouse(const int32 _LineIndex)
{
	if (this->MouseLineManager.Num() > 0)
	{
		return this->MouseLineManager[_LineIndex]->GetMouseTopByAllType();
	}

	return nullptr;
}

AMouseActor* const UMouseManagerComponent::GetTopFirstMouse()
{
	AMouseActor* CurTop = nullptr;
	for (UMouseLineManager* const CurLine : this->MouseLineManager)
	{
		if (!IsValid(CurTop))
		{
			CurTop = CurLine->GetMouseTopByAllType();

			continue;
		}

		AMouseActor* CurNextTop = CurLine->GetMouseTopByAllType();
		if (IsValid(CurNextTop) && CurNextTop->GetCurrentHP() > 0.f)
		{
			if (CurNextTop->GetActorLocation().Y < CurTop->GetActorLocation().Y)
			{
				CurTop = CurNextTop;
			}
		}
	}
	return CurTop;
}

AMouseActor* GetMouseType(const UMouseLineManager* const  CurLine, const ELineType& LineType)
{
	switch (LineType)
	{
	case ELineType::Underground:
	{
		return  CurLine->GetMouseTopByUnderGorund();
	}
	case ELineType::Sky:
	{
		return  CurLine->GetMouseTopBySky();
	}
	case ELineType::OnGround: {
		return CurLine->GetMouseTopByGorund();
	}
	default:
	{
		return CurLine->GetMouseTopByAllType();
	}
	break;
	}
	return  nullptr;
}

AMouseActor* const UMouseManagerComponent::GetTopMouseByType(const ELineType& LineType)
{
	AMouseActor* CurTop = nullptr;
	for (UMouseLineManager* const CurLine : this->MouseLineManager)
	{
		if (!IsValid(CurTop))
		{
			CurTop = GetMouseType(CurLine, LineType);
			continue;
		}

		AMouseActor* CurNextTop = GetMouseType(CurLine, LineType);
		if (IsValid(CurNextTop) && CurNextTop->GetCurrentHP() > 0.f)
		{
			if (CurNextTop->GetActorLocation().Y < CurTop->GetActorLocation().Y)
			{
				CurTop = CurNextTop;
			}
		}
	}

	return CurTop;
}

UMouseLineManager* UMouseManagerComponent::GetMouseLineManager(int32 Row)
{
	return this->MouseLineManager[Row];
}

int32 UMouseManagerComponent::GetCurrentRoundTotal() const
{
	return this->CurRoundTotal;
}

void UMouseManagerComponent::RemoveMouse(AMouseActor* _CurMouse)
{
	if (IsValid(_CurMouse))
	{
		//当前老鼠名称
		FString CurMouseName = UKismetSystemLibrary::GetObjectName(_CurMouse);

		if (
			_CurMouse->GetMouseLine().Row < AGameMapInstance::GetGameMapInstance()->
			M_MouseManagerComponent->MouseLineManager.Num()
			)
		{
			//如果移除失败，就进行暴力移除模式[tick]
			if (!AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent->
				MouseLineManager[_CurMouse->GetMouseLine().Row]->RemoveMouse(_CurMouse))
			{

				return;
				/*
				//遍历全部线路
				for (UMouseLineManager* LineManager : AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent->
					MouseLineManager)
				{
					//移除成功即可跳出
					if (LineManager->RemoveMouseByName(CurMouseName))
					{
						break;
					}
				}
				*/
			}
		}
	}
}

void UMouseManagerComponent::AddMouse(AMouseActor* _CurMouse, const int32& Row, bool bInit)
{
	if (IsValid(_CurMouse))
	{
		if (
			Row < AGameMapInstance::GetGameMapInstance()->
			M_MouseManagerComponent->MouseLineManager.Num()
			)
		{
			AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent->
				MouseLineManager[Row]->AddMouse(UKismetSystemLibrary::GetObjectName(_CurMouse), _CurMouse);

			if (bInit)
			{
				_CurMouse->MouseInit();
				_CurMouse->GetMouseManager()->CurSpawnRow = Row;
			}
		}
	}
}

bool UMouseManagerComponent::TestMouseSpawnDeath()
{
	if (IsValid(AGameMapInstance::GetGameMapInstance()))
	{
		if (IsValid(AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent))
		{
			return AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent->bKillMouse;
		}
	}

	return false;
}

bool UMouseManagerComponent::ChangeLine(const FString& MouseObjName, const int32& CurRow, const int32& TargetRow)
{
	if (CurRow == TargetRow)
	{
		return true;
	}

	if (
		CurRow < AGameMapInstance::GetGameMapInstance()->
		M_MouseManagerComponent->MouseLineManager.Num()
		)
	{
		AMouseActor* CurMouse = AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent->
			MouseLineManager[CurRow]->FindMouse(MouseObjName);

		if (IsValid(CurMouse) && CurMouse->GetCurrentHP() > 0.f)
		{
			AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent->
				MouseLineManager[CurRow]->RemoveMouse(CurMouse);

			UMouseManagerComponent::AddMouse(CurMouse, TargetRow, false);
			return true;
		}
	}

	return false;
}

bool UMouseManagerComponent::ForceChangeLine(const FString& MouseObjName, const int32& CurRow, const int32& TargetRow, const int32& TargetCol)
{
	bool bRe = UMouseManagerComponent::ChangeLine(MouseObjName, CurRow, TargetRow);
	if (bRe)
	{
		AMouseActor* CurMouse = AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent->
			MouseLineManager[TargetRow]->FindMouse(MouseObjName);
		if (IsValid(CurMouse))
		{
			CurMouse->SetMouseLine(FLine(TargetRow, TargetCol));
		}
	}

	return bRe;
}

bool UMouseManagerComponent::ChangeMouseLineType(AMouseActor* _CurMouse, int32 CurRow, ELineType TargetType, UBoxComponent* CurCollision, UShapeComponent* CurBodyCollision)
{
	if (
		CurRow < AGameMapInstance::GetGameMapInstance()->
		M_MouseManagerComponent->MouseLineManager.Num()
		)
	{
		AMouseActor* CurMouse = AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent->
			MouseLineManager[CurRow]->FindMouse(UKismetSystemLibrary::GetObjectName(_CurMouse));

		if (IsValid(CurMouse) && CurMouse->GetCurrentHP() > 0.f)
		{
			if (AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent->MouseLineManager[CurRow]->RemoveMouse(CurMouse))
			{
				//切换线路类型
				CurMouse->SetMouseLineType(TargetType);

				if (IsValid(CurCollision))
				{
					CurCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
					CurCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
					//老鼠对象开启
					CurCollision->SetCollisionResponseToChannel(
						UGameSystemFunction::GetMouseCollisionBoxTypeByLineType(ELineTraceType::E_MouseVisibility),
						ECollisionResponse::ECR_Block
					);
					//设置对应的路线碰撞位置
					CurCollision->SetCollisionResponseToChannel(
						UGameSystemFunction::GetMouseCollisionBoxTypeByELineType(TargetType),
						ECollisionResponse::ECR_Block
					);
				}

				if (IsValid(CurBodyCollision))
				{
					CurBodyCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
					CurBodyCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
					CurBodyCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
				}

				//获取当前进入的网格
				ELineType CurTemp = CurMouse->M_Proper_Condition.M_CurrentInType;
				//将之前进入的网格切换到新的类型网格
				CurMouse->M_Proper_Condition.M_CurrentInType = TargetType;
				//重新执行进入网格的功能
				CurMouse->InMapMeshe(CurTemp);
				//添加到管理器
				UMouseManagerComponent::AddMouse(CurMouse, CurRow, false);

				return true;
			}
		}
	}
	return false;
}

void UMouseManagerComponent::GetAllMouse(TArray<AMouseActor*>& OutAllMouse)
{
	for (UMouseLineManager* CurLineMan :
		AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent->MouseLineManager)
	{
		if (IsValid(CurLineMan))
		{
			for (auto& Cur : CurLineMan->GetMouseAll())
			{
				OutAllMouse.Emplace(Cur.Value);
			}
		}
	}
}

void UMouseManagerComponent::SetMouseSpawnKill()
{
	if (this->bKillMouse)
	{
		this->bKillMouse = false;
	}
	else {
		this->bKillMouse = true;

		for (UMouseLineManager* Cur : this->MouseLineManager)
		{
			if (IsValid(Cur))
			{
				Cur->KillAllMouse();
			}
		}
	}
}

void UMouseManagerComponent::SetMouseTimePause()
{
	for (UMouseLineManager* Cur : this->MouseLineManager)
	{
		if (IsValid(Cur))
		{
			if (Cur->bMouseTimePause)
			{
				Cur->bMouseTimePause = false;
			}
			else {
				Cur->bMouseTimePause = true;
			}
		}
	}
}

int32 UMouseManagerComponent::GetCurrentSpawnMouseRow()
{
	return this->CurSpawnRow;
}

UObjectPoolManager* UMouseManagerComponent::GetBurnBuffFxObjPoolManager()
{
	return this->BurnBuffFxObjPoolManager;
}

UObjectPoolManager* UMouseManagerComponent::GetSolidifBuffFxObjPoolManager()
{
	return this->SolidiBuffFxObjPoolManager;
}
