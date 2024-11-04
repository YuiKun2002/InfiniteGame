// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DynamicProperty.generated.h"

//动态数组对象
template<class Type>
class FVM_API UDynamicPropertyArrayObject : public UObject
{
public:
	//数组属性
	TArray<Type> TypeValue;
};

template<>
class FVM_API UDynamicPropertyArrayObject<int32> : public UObject{
public:
	UPROPERTY()
	TArray<int32> TypeValue;
}

/**
 * 动态属性
 */
UCLASS()
class FVM_API UDynamicProperty : public UObject
{
	GENERATED_BODY()
public:
	//构造
	UDynamicProperty();
	//初始化
	UFUNCTION(BlueprintImplementableEvent)
	void OnInit();
public:
	//整形
	UFUNCTION(BlueprintCallable)
	void SetIntProperty(const FString& VariableName, int32 Value);
	UFUNCTION(BlueprintPure)
	bool GetIntProperty(const FString& VariableName, int32& Value);
	//整形数组
	UFUNCTION(BlueprintCallable)
	void SetIntArrayProperty(const FString& VariableName, TArray<int32> Value);
	UFUNCTION(BlueprintPure)
	bool GetIntArrayProperty(const FString& VariableName, TArray<int32>& Value);
	//浮点
	UFUNCTION(BlueprintCallable)
	void SetFloatProperty(const FString& VariableName, float Value);
	UFUNCTION(BlueprintPure)
	bool GetFloatProperty(const FString& VariableName, float& Value);
	//字符串
	UFUNCTION(BlueprintCallable)
	void SetStringProperty(const FString& VariableName, const FString& Value);
	UFUNCTION(BlueprintPure)
	bool GetStringProperty(const FString& VariableName, FString& Value);
	//对象
	UFUNCTION(BlueprintCallable)
	void SetObjectProperty(const FString& VariableName, UObject* Value);
	UFUNCTION(BlueprintPure)
	bool GetObjectProperty(const FString& VariableName, UObject*& Value);
protected:
	virtual void Init();
private:
	UPROPERTY()
	TMap<FString, int32> IntPropertys;
	UPROPERTY()
	TMap<FString, float> FloatPropertys;
	UPROPERTY()
	TMap<FString, FString> FStringPropertys;
	UPROPERTY()
	TMap<FString, UObject*> UObjectPropertys;
private:
	//修改属性
	template<class TypeValue>
	void EditProperty(
		TMap<FString, TypeValue>& Propertys,
		const FString& VariableName,
		const TypeValue& Value
	) {
		TypeValue* TargetValue = Propertys.Find(VariableName);
		if (TargetValue)
		{
			*TargetValue = Value;
		}
		else {
			Propertys.Emplace(VariableName, Value);
		}
	}

	//获取属性
	template<class TypeValue>
	bool GetProperty(
		const TMap<FString, TypeValue>& Propertys,
		const FString& VariableName,
		TypeValue& Value
	) {
		const TypeValue* TargetValue = Propertys.Find(VariableName);
		if (TargetValue)
		{
			Value = *TargetValue;
			return true;
		}
		return false;
	}

	//设置数组对象
	template<class TypeValue>
	UObject* SetArrayObject(const TArray<TypeValue>& Value) {
		UDynamicPropertyArrayObject<TypeValue>* Obj = NewObject<
			UDynamicPropertyArrayObject<TypeValue>
		>();
		Obj->TypeValue = Value;
		return Cast<UObject>(Obj);
	};

	//获取对象
	template<class TypeValue>
	bool GetArrayObject(UObject*& ArrayObject, TArray<TypeValue>& Value) {
		UDynamicPropertyArrayObject<TypeValue>* TargetObj = Cast<
			UDynamicPropertyArrayObject<TypeValue>
		>(ArrayObject);
		if (IsValid(TargetObj))
		{
			Value = TargetObj->TypeValue;
			return true;
		}
		return false;
	}

	//设置数组属性
	template<class TypeValue>
	void SetArrayProperty(const FString& VariableName, TArray<TypeValue> Value) {
		UObject* Obj = this->SetArrayObject(Value);
		this->EditProperty(this->UObjectPropertys, VariableName, Obj);
	}

	//获取数组属性
	template<class TypeValue>
	bool GetArrayProperty(const FString& VariableName, TArray<TypeValue> Value) {
		UObject* ArrayObject = nullptr;
		if (this->GetProperty(this->UObjectPropertys, VariableName, ArrayObject))
		{
			return this->GetArrayObject(ArrayObject, Value);
		}
		return false;
	}
};
