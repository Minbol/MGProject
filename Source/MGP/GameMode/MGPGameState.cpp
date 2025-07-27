// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPGameState.h"

#include "MGPExperienceManagerComponent.h"

AMGPGameState::AMGPGameState()
{
	ExperienceManagerComponent = CreateDefaultSubobject<UMGPExperienceManagerComponent>( TEXT( "ExperienceManagerComponent") );
}
