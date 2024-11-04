// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除


#include "GameSystem/Tools/DynamicProperty.h"

void UDynamicProperty::Init()
{
	this->OnInit();
}

UDynamicProperty::UDynamicProperty()
{
	this->Init();
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
	this->SetArrayProperty(VariableName,Value);
}

bool UDynamicProperty::GetIntArrayProperty(const FString& VariableName, TArray<int32>& Value)
{
	return this->GetArrayProperty(VariableName,Value);
}

void UDynamicProperty::SetFloatProperty(const FString& VariableName, float Value)
{
	this->EditProperty(this->FloatPropertys, VariableName, Value);
}

bool UDynamicProperty::GetFloatProperty(const FString& VariableName, float& Value)
{
	return this->GetProperty(this->FloatPropertys, VariableName, Value);
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