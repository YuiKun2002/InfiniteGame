// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorDestroy.h"

bool UActorDestroy::DoesActorExist(AActor* _Actor)
{
	//如果Actor存在
	if (_Actor && !_Actor->IsPendingKillPending())
		//判断Actor是否被标记等待回收
		return true;
	//如果Actor是无效的那么直接返回
	return false;
}