// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameSystem/Tools/DynamicProperty.h"

const FString& UDynamicProperty::GetCurrentVarableName()
{
	return this->VarableName;
}

void UDynamicProperty::GetCurrentVarableName(FString& OutVarableName)
{
	OutVarableName = this->GetCurrentVarableName();
}

void UDynamicProperty::Init()
{
	this->OnInit();
}

void UDynamicProperty::SetIntProperty(const FString& VariableName, int32 Value)
{
	this->EditProperty(this->IntPropertys, VariableName, Value);
}
bool UDynamicProperty::GetIntProperty(const FString& VariableName, int32& Value)
{
	return this->GetProperty(this->IntPropertys, VariableName, Value);
}

void UDynamicProperty::SetIntArrayProperty(const FString& VariableName, TArray<int32> Value)
{
	this->SetArrayProperty<UDynamicPropertyArrayIntObject>(VariableName,Value);
}

bool UDynamicProperty::GetIntArrayProperty(const FString& VariableName, TArray<int32>& Value)
{
	return this->GetArrayProperty<UDynamicPropertyArrayIntObject>(VariableName,Value);
}

void UDynamicProperty::SetFloatProperty(const FString& VariableName, float Value)
{
	this->EditProperty(this->FloatPropertys, VariableName, Value);
}

bool UDynamicProperty::GetFloatProperty(const FString& VariableName, float& Value)
{
	return this->GetProperty(this->FloatPropertys, VariableName, Value);
}

bool UDynamicProperty::GetFloatPropertyPtr(const FString& VariableName, float*& Value)
{
	return this->GetPropertyPtr(this->FloatPropertys, VariableName, Value);
}

void UDynamicProperty::SetFloatArrayProperty(const FString& VariableName, TArray<float> Value)
{
	this->SetArrayProperty<UDynamicPropertyArrayFloatObject>(VariableName,Value);
}

bool UDynamicProperty::GetFloatArrayProperty(const FString& VariableName, TArray<float>& Value)
{
	return this->GetArrayProperty<UDynamicPropertyArrayFloatObject>(VariableName, Value);
}

void UDynamicProperty::SetStringProperty(const FString& VariableName, const FString& Value)
{
	this->EditProperty(this->FStringPropertys, VariableName, Value);
}

bool UDynamicProperty::GetStringProperty(const FString& VariableName, FString& Value)
{
	return this->GetProperty(this->FStringPropertys, VariableName, Value);
}

void UDynamicProperty::SetObjectProperty(const FString& VariableName, UObject* Value)
{
	this->EditProperty(this->UObjectPropertys, VariableName, Value);
}

bool UDynamicProperty::GetObjectProperty(const FString& VariableName, UObject*& Value)
{
	return this->GetProperty(this->UObjectPropertys, VariableName, Value);
}