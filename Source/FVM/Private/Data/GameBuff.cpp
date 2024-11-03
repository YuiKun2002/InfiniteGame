// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "Data/GameBuff.h"
//引入老鼠buff系统
#include "Data/Buff/MouseGameBuff.h"

UGameBuff* UGameBuff::MakeGameBuff(UObject* NewBuffChar, EGameBuffCharTag NewBuffTag)
{
	UGameBuff* NewBuff = nullptr;//NewObject<UGameBuff>();

	switch (NewBuffTag)
	{
	case EGameBuffCharTag::Card:NewBuff = NewObject<UGameBuff>(); break;
	case EGameBuffCharTag::Mouse:NewBuff = NewObject<UMouseGameBuff>(); break;
	default:NewBuff = NewObject<UGameBuff>(); break;
	}

	//初始化角色
	NewBuff->BuffChar = NewBuffChar;
	//初始化buff类型
	NewBuff->BuffTag = NewBuffTag;

	return NewBuff;
}

void UGameBuff::AddBuffBySubclass(
	EGameBuffTag NewTag,
	float NewBuffTime,
	const TSubclassOf<UBuffDynamicProperty>& Property
)
{
	if (Property)
	{
		this->AddBuff(NewTag, NewBuffTime, Property.GetDefaultObject());
	}
	else {
		this->AddBuff(NewTag, NewBuffTime, nullptr);
	}
}

void UGameBuff::AddBuff(EGameBuffTag NewTag, float NewBuffTime, UBuffDynamicProperty* Property)
{
	UBuffObject** CurBuff = this->CurBuffs.Find(NewTag);
	if (CurBuff)
	{
		/*
			之前已经存在的buff，再次执行
		*/
		if (IsValid(*CurBuff))
		{
			//更新buff持续时间
			if ((*CurBuff)->CurTime < NewBuffTime)
			{
				(*CurBuff)->CurTime = NewBuffTime;
				(*CurBuff)->BuffUpdate();
			}
		}
	}
	else {
		/*
			新增buff，列表中不存在的buff
		*/
		this->OnExecuteBuffDelegate.ExecuteIfBound(NewTag, NewBuffTime);
		if (NewBuffTime > 0.f)
		{
			//添加buff
			UBuffObject* NewBuff = this->GetNewBuffObject(NewTag, NewBuffTime, Property);
			this->CurBuffs.Emplace(NewTag, NewBuff);
			//设置限制buff的存在
			if (NewBuff->GetConstbuff())
			{
				this->bConstBuff = true;
			}
			NewBuff->BuffInit(NewBuffTime);
		}
	}

	this->OnBuffExecuteFinishedDelegate.ExecuteIfBound(this);
}

void UGameBuff::AddBuffInfor(FGameBuffInfor NewBuff)
{
	for (const auto& Cur : NewBuff.CurBuffs)
	{
		TSubclassOf<UBuffDynamicProperty>* Sub = NewBuff.CurBuffPropertys.Find(Cur.Key);
		if (Sub)
		{
			this->AddBuffBySubclass(Cur.Key, Cur.Value, *Sub);
		}
		else {
			this->AddBuffBySubclass(Cur.Key, Cur.Value, UBuffDynamicProperty::StaticClass());
		}
	}
}

void UGameBuff::ClearBuffs()
{
	this->CurBuffs.Empty();
}

void UGameBuff::EndBuffs()
{
	for (const auto& Cur : this->CurBuffs)
	{
		this->SetBuffCurTime(Cur.Key, 0.f);
	}
}

void UGameBuff::DebugLog(const FString& ActorName, EGameBuffTag NewTag, bool Tirgger)
{
	//打印
	auto LLog = [&](const FString& ActorName, const FString& A, const FString& B) {
		if (Tirgger)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s：%s"), *ActorName, *A);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("~~~%s：%s"), *ActorName, *B);
		}
		};

	switch (NewTag)
	{
	case EGameBuffTag::Accelerate: LLog(ActorName, TEXT("触发加速"), TEXT("加速结束~~~~")); break;
	case EGameBuffTag::Burn: LLog(ActorName, TEXT("触发灼烧"), TEXT("灼烧结束~~~~")); break;
	case EGameBuffTag::Solidification: LLog(ActorName, TEXT("触发凝固"), TEXT("凝固结束~~~~")); break;
	case EGameBuffTag::Freeze: LLog(ActorName, TEXT("触发冻结"), TEXT("冻结结束~~~~")); break;
	case EGameBuffTag::SlowDown: LLog(ActorName, TEXT("触发减速"), TEXT("减速结束~~~~")); break;
	default:
		LLog(ActorName, TEXT("触发【其他buff】"), TEXT("【其他buff】结束~~~~"));
		break;
	}
}

void UGameBuff::UpdateBuff(const float& DeltaTime)
{
	//存储结束的buff
	TMap<EGameBuffTag, UBuffObject*> CurEndBuffs;

	//更新buff时间
	for (auto& CurBuffObj : this->CurBuffs)
	{
		CurBuffObj.Value->Tick(DeltaTime);
		CurBuffObj.Value->CurTime -= DeltaTime;
		if (CurBuffObj.Value->CurTime <= 0.f)
		{
			CurEndBuffs.Emplace(CurBuffObj.Key, CurBuffObj.Value);
		}
	}

	//消耗结束的buff
	if (CurEndBuffs.Num())
	{
		//添加还存在的buff，替换之前的buff列表
		TMap<EGameBuffTag, UBuffObject*> NewBuffs;
		//刷新是否还存在限制类型的buff
		bool bConstBuffExist = false;
		for (auto& CurBuffObj : this->CurBuffs)
		{
			if (CurBuffObj.Value->CurTime > 0.f)
			{
				if (CurBuffObj.Value->GetConstbuff())
				{
					bConstBuffExist = true;
				}
				NewBuffs.Emplace(CurBuffObj.Key, CurBuffObj.Value);
			}
		}

		//设置是否存在限制类型的buff
		this->bConstBuff = bConstBuffExist;
		//重新拷贝buff
		this->CurBuffs = NewBuffs;
		//依次执行结束的buff
		for (auto& CurBuffObj : CurEndBuffs)
		{
			CurBuffObj.Value->BuffEnd();
			this->OnExitBuffDelegate.ExecuteIfBound(CurBuffObj.Key);
		}
		//当前动作全部执行完毕
		this->OnBuffExecuteFinishedDelegate.ExecuteIfBound(this);
	}
}

void UGameBuff::RemoveBuff(EGameBuffTag NewTag)
{
	this->SetBuffCurTime(NewTag, 0.f);
}

bool UGameBuff::GetBuffExist() const
{
	return (CurBuffs.Num() > 0);
}

bool UGameBuff::GetBuffExistByTag(EGameBuffTag NewTag)
{
	if (IsValid(this->GetBuff(NewTag)))
	{
		return true;
	}

	return false;
}

bool UGameBuff::GetConstBuff() const
{
	return this->bConstBuff;
}

UBuffObject* UGameBuff::GetBuff(EGameBuffTag BuffTag)
{
	UBuffObject** CurBuff = this->CurBuffs.Find(BuffTag);
	if (CurBuff && IsValid(*CurBuff))
	{
		return ((*CurBuff)->GetCurTime() > 0.f) ? *CurBuff : nullptr;
	}
	return nullptr;
}

float UGameBuff::GetTickRate() const
{
	return this->UpdateTickRate;
}

UObject* UGameBuff::GetBuffChar()
{
	return this->BuffChar;
}

UBuffObject* UGameBuff::GetNewBuffObject(EGameBuffTag NewTag, float NewBuffTime, UBuffDynamicProperty* Property)
{
	return NewObject<UBuffObject>();
}

void UGameBuff::SetBuffCurTime(const EGameBuffTag& CurTag, const float& NewTime)
{
	UBuffObject** CurBuffObj = this->CurBuffs.Find(CurTag);
	if (CurBuffObj && IsValid(*CurBuffObj))
	{
		(*CurBuffObj)->CurTime = NewTime;
	}
}

void UGameBuff::ExecuteBuffs()
{
	//重新初始化buff
	for (auto& CurBuffObj : this->CurBuffs)
	{
		//当前buff的时间
		float CurTime = CurBuffObj.Value->GetCurTime();
		//执行buff
		this->OnExecuteBuffDelegate.ExecuteIfBound(CurBuffObj.Key, CurTime);
		if (CurTime > 0.f)
		{
			CurBuffObj.Value->BuffInit(CurBuffObj.Value->CurTime);
		}
		else {
			this->SetBuffCurTime(CurBuffObj.Key, 0.f);
		}
	}

	//当前动作全部执行完毕
	this->OnBuffExecuteFinishedDelegate.ExecuteIfBound(this);
}

void UBuffDynamicProperty::Init()
{
	this->OnInit();
}

void UBuffDynamicProperty::SetIntProperty(const FString& VariableName, int32 Value)
{
	this->EditProperty(this->IntPropertys, VariableName, Value);
}
bool UBuffDynamicProperty::GetIntProperty(const FString& VariableName, int32& Value)
{
	return this->GetProperty(this->IntPropertys, VariableName, Value);
}

void UBuffDynamicProperty::SetFloatProperty(const FString& VariableName, float Value)
{
	this->EditProperty(this->FloatPropertys, VariableName, Value);
}

bool UBuffDynamicProperty::GetFloatProperty(const FString& VariableName, float& Value)
{
	return this->GetProperty(this->FloatPropertys, VariableName, Value);
}

void UBuffDynamicProperty::SetStringProperty(const FString& VariableName, const FString& Value)
{
	this->EditProperty(this->FStringPropertys, VariableName, Value);
}

bool UBuffDynamicProperty::GetStringProperty(const FString& VariableName, FString& Value)
{
	return this->GetProperty(this->FStringPropertys, VariableName, Value);
}

void UBuffDynamicProperty::SetObjectProperty(const FString& VariableName, UObject* Value)
{
	this->EditProperty(this->UObjectPropertys, VariableName, Value);
}

bool UBuffDynamicProperty::GetObjectProperty(const FString& VariableName, UObject*& Value)
{
	return this->GetProperty(this->UObjectPropertys, VariableName, Value);
}

void UBuffDynamicProperty::SetDefObject(UObject* Value)
{
	this->SetObjectProperty(GAMEBUFF_VAR_DEFOBJECT, Value);
}

void UBuffDynamicProperty::GetDefObject(UObject*& Value)
{
	this->GetObjectProperty(GAMEBUFF_VAR_DEFOBJECT, Value);
}

void UBuffObject::BuffInit(float BuffTime)
{
	//如果当前buff时间小于新的时间，刷新buff的时间
	if (this->CurTime < BuffTime)
	{
		this->CurTime = BuffTime;
	}

	if (IsValid(this->DynamicProperty))
	{
		this->DynamicProperty->Init();
	}

	this->BuffUpdate();
}

void UBuffObject::BuffUpdate()
{

}

void UBuffObject::Tick(float BuffTime)
{

}

void UBuffObject::BuffEnd()
{
	this->CurTime = 0.f;
}

bool UBuffObject::GetDebuff()
{
	return true;
}

bool UBuffObject::GetConstbuff()
{
	return false;
}

class UGameBuff* UBuffObject::GetGameBuff()
{
	return this->CurBuffObject;
}

UBuffObject* UBuffObject::GetBuffByTag(EGameBuffTag NewTag)
{
	UBuffObject** CurBuffObj = this->CurBuffObject->CurBuffs.Find(NewTag);
	if (CurBuffObj && IsValid(*CurBuffObj))
	{
		if ((*CurBuffObj)->CurTime > 0.f)
		{
			return (*CurBuffObj);
		}
	}

	return nullptr;
}

void UBuffObject::SetTickRate(float NewRate)
{
	if (NewRate < 0.f)
	{
		this->CurBuffObject->UpdateTickRate = 0.f;
	}
	else {
		this->CurBuffObject->UpdateTickRate = NewRate;
	}
}

void UBuffObject::SetBuffCurTime(const EGameBuffTag& NewTag, const float& NewTime)
{
	this->CurBuffObject->SetBuffCurTime(NewTag, NewTime);
}

void UBuffObject::SetForceCurTime(const float& NewTime)
{
	this->CurTime = NewTime;
}

float UBuffObject::GetCurTime() const
{
	return this->CurTime;
}

const EGameBuffTag& UBuffObject::GetCurTag() const
{
	return this->CurTag;
}

UBuffDynamicProperty* UBuffObject::GetDynamicProperty()
{
	return this->DynamicProperty;
}