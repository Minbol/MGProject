// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPEquipmentDefinition.h"

#include "MGPEquipmentInstance.h"

UMGPEquipmentDefinition::UMGPEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstanceType = UMGPEquipmentInstance::StaticClass();
}
