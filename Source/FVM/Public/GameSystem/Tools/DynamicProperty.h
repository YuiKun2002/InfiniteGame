// 该游戏是同人游戏，提供学习使用，禁止贩卖，如有侵权立刻删除

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DynamicProperty.generated.h"

//动态数组对象

//对象数组类
UCLASS()
class FVM_API UDynamicPropertyArrayObject : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<UObject*> TypeValue;
};
//整型数组类
UCLASS()
class FVM_API UDynamicPropertyArrayIntObject : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<int32> TypeValue;
};
//浮点数组类
UCLASS()
class FVM_API UDynamicPropertyArrayFloatObject : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<float> TypeValue;
};
//字符串组类
UCLASS()
class FVM_API UDynamicPropertyArrayFStringObject : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<FString> TypeValue;
};


/**
 * 动态属性
 */
UCLASS(Blueprintable)
class FVM_API UDynamicProperty : public UObject
{
	GENERATED_BODY()
public:
	//初始化类型
	template<class Type>
	static Type* MakeDynamicPropertyByClass(Type* Property = nullptr) {

		if (IsValid(Property))
		{
			Property->Init();
			return Property;
		}

		Type* Obj = NewObject<Type>();
		Obj->Init();
		return Obj;
	}

	//初始化
	template<class Type>
	static Type* MakeDynamicPropertyByClass(const TSubclassOf<Type>& Property) {

		if (Property)
		{
			Type* Obj = Property.GetDefaultObject();
			if (IsValid(Obj))
			{
				Obj->Init();
				return Obj;
			}
		}

		Type* Obj = NewObject<Type>();
		Obj->Init();
		return Obj;
	}

	//初始化
	UFUNCTION(BlueprintPure)
	static UDynamicProperty* MakeDynamicPropertyByClass(TSoftClassPtr<UDynamicProperty> Property) {

		if (UClass* Class = Property.LoadSynchronous())
		{
			UDynamicProperty* Obj = NewObject<UDynamicProperty>(Class);
			Obj->Init();
			return Obj;
		}

		UDynamicProperty* Obj = NewObject<UDynamicProperty>();
		Obj->Init();
		return Obj;
	}

	//初始化
	UFUNCTION(BlueprintPure)
	static UDynamicProperty* MakeDynamicProperty(UDynamicProperty* Property = nullptr) {
		if (IsValid(Property))
		{
			Property->Init();
			return Property;
		}

		UDynamicProperty* Obj = MakeDynamicPropertyByClass<UDynamicProperty>();
		Obj->Init();
		return Obj;
	}

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
	bool GetFloatPropertyPtr(const FString& VariableName, float*& Value);
	//浮点数组
	UFUNCTION(BlueprintCallable)
	void SetFloatArrayProperty(const FString& VariableName, TArray<float> Value);
	UFUNCTION(BlueprintPure)
	bool GetFloatArrayProperty(const FString& VariableName, TArray<float>& Value);
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

	//当前变动的变量
	const FString& GetCurrentVarableName();
	UFUNCTION(BlueprintPure)
	void GetCurrentVarableName(FString& OutVarableName);
protected:
	UFUNCTION()
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
	UPROPERTY()
	FString VarableName = FString();
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

		this->VarableName = VariableName;
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

	//获取属性
	template<class TypeValue>
	bool GetPropertyPtr(
		const TMap<FString, TypeValue>& Propertys,
		const FString& VariableName,
		TypeValue*& Value
	) {
		const TypeValue* TargetValue = Propertys.Find(VariableName);
		if (TargetValue)
		{
			Value = const_cast<TypeValue*>(TargetValue);
			return true;
		}
		return false;
	}

	//设置数组对象
	template<class TypeClass, class TypeValue>
	UObject* SetArrayObject(const TArray<TypeValue>& Value) {
		TypeClass* Obj = NewObject<
			TypeClass
		>();
		Obj->TypeValue = Value;
		return Cast<UObject>(Obj);
	};

	//获取对象
	template<class TypeClass, class TypeValue>
	bool GetArrayObject(UObject*& ArrayObject, TArray<TypeValue>& Value) {
		TypeClass* TargetObj = Cast<
			TypeClass
		>(ArrayObject);
		if (IsValid(TargetObj))
		{
			Value = TargetObj->TypeValue;
			return true;
		}
		return false;
	}

	//设置数组属性
	template<class TypeClass, class TypeValue>
	void SetArrayProperty(const FString& VariableName, TArray<TypeValue> Value) {
		UObject* Obj = this->SetArrayObject<TypeClass>(Value);
		this->EditProperty(this->UObjectPropertys, VariableName, Obj);
	}

	//获取数组属性
	template<class TypeClass, class TypeValue>
	bool GetArrayProperty(const FString& VariableName, TArray<TypeValue> Value) {
		UObject* ArrayObject = nullptr;
		if (this->GetProperty(this->UObjectPropertys, VariableName, ArrayObject))
		{
			return this->GetArrayObject<TypeClass>(ArrayObject, Value);
		}
		return false;
	}
};
