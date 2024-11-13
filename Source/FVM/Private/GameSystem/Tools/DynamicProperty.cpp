// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameSystem/Tools/DynamicProperty.h"

void UDynamicProperty::Init()
{
	this->OnInit();
}

void UDynamicProperty::SetIntProperty(const FString& VariableName, int32 Value)
{
	this->SetIntPropertyPtr(VariableName, Value);
}

void UDynamicProperty::SetIntPropertyPtr(const FString& VariableName, const int32& Value)
{
	this->EditPropertyPtr(this->IntPropertys_Ptr, VariableName, Value);
}

bool UDynamicProperty::GetIntProperty(const FString& VariableName, int32& Value)
{
	TSharedPtr<int32> Ptr;
	if (this->GetIntPropertyPtr(VariableName, Ptr))
	{
		if (Ptr.Get())
		{
			Value = *Ptr;
			return true;
		}
	}
	return false;
}

bool UDynamicProperty::GetIntPropertyPtr(const FString& VariableName, TSharedPtr<int32>& Value)
{
	return this->GetPropertyPtr(this->IntPropertys_Ptr, VariableName, Value);
}

void UDynamicProperty::SetIntArrayProperty(const FString& VariableName, TArray<int32> Value)
{
	this->SetArrayProperty<UDynamicPropertyArrayIntObject>(VariableName, Value);
}

bool UDynamicProperty::GetIntArrayProperty(const FString& VariableName, TArray<int32>& Value)
{
	return this->GetArrayProperty<UDynamicPropertyArrayIntObject>(VariableName, Value);
}

void UDynamicProperty::SetFloatProperty(const FString& VariableName, float Value)
{
	this->SetFloatPropertyPtr(VariableName, Value);
}

bool UDynamicProperty::GetFloatProperty(const FString& VariableName, float& Value)
{
	TSharedPtr<float> Ptr;
	if (this->GetFloatPropertyPtr(VariableName, Ptr))
	{
		if (Ptr.Get())
		{
			Value = *Ptr;
			return true;
		}
	}
	return false;
}

void UDynamicProperty::SetFloatPropertyPtr(const FString& VariableName, const float& Value)
{
	this->EditPropertyPtr<TSharedPtr<float>, float>(this->FloatPropertys_Ptr, VariableName, Value);
}

bool UDynamicProperty::GetFloatPropertyPtr(const FString& VariableName, TSharedPtr<float>& Value)
{
	return this->GetPropertyPtr(this->FloatPropertys_Ptr, VariableName, Value);
}

void UDynamicProperty::SetFloatArrayProperty(const FString& VariableName, TArray<float> Value)
{
	this->SetArrayProperty<UDynamicPropertyArrayFloatObject>(VariableName, Value);
}

bool UDynamicProperty::GetFloatArrayProperty(const FString& VariableName, TArray<float>& Value)
{
	return this->GetArrayProperty<UDynamicPropertyArrayFloatObject>(VariableName, Value);
}

void UDynamicProperty::SetStringProperty(const FString& VariableName, const FString& Value)
{
	this->EditPropertyPtr(this->StringPropertys_Ptr, VariableName, Value);
}

void UDynamicProperty::SetStringPropertyPtr(const FString& VariableName, const FString& Value)
{
	this->EditPropertyPtr(this->StringPropertys_Ptr, VariableName, Value);
}

bool UDynamicProperty::GetStringProperty(const FString& VariableName, FString& Value)
{
	TSharedPtr<FString> Ptr;
	if (this->GetStringPropertyPtr(VariableName, Ptr))
	{
		if (Ptr.Get())
		{
			Value = *Ptr;
			return true;
		}
	}
	return false;
}

bool UDynamicProperty::GetStringPropertyPtr(const FString& VariableName, TSharedPtr<FString>& Value)
{
	return this->GetPropertyPtr(this->StringPropertys_Ptr, VariableName, Value);
}

void UDynamicProperty::SetObjectProperty(const FString& VariableName, UObject* Value)
{
	this->SetObjectPropertyPtr(VariableName, Value);
}

void UDynamicProperty::SetObjectPropertyPtr(const FString& VariableName, UObject*& Value)
{
	UObject** TargetValue = this->ObjectPropertys_Ptr.Find(VariableName);
	if (TargetValue)
	{
		(*TargetValue) = Value;
	}
	else {
		//新增对象
		this->ObjectPropertys_Ptr.Emplace(VariableName, Value);
	}
}

bool UDynamicProperty::GetObjectProperty(const FString& VariableName, UObject*& Value)
{
	return this->GetObjectPropertyPtr(VariableName, Value);
}

bool UDynamicProperty::GetObjectPropertyPtr(const FString& VariableName, UObject*& Value)
{
	UObject** TargetValue = this->ObjectPropertys_Ptr.Find(VariableName);
	if (TargetValue)
	{
		Value = *TargetValue;
		return true;
	}
	return false;
}
