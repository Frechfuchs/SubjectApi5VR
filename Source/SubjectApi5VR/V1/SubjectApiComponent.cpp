// Fill out your copyright notice in the Description page of Project Settings.


#include "SubjectApiComponent.h"
#include "SubjectApiSubsystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
USubjectApiComponent::USubjectApiComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// TODO
void USubjectApiComponent::FinishAndSave()
{	
	if (!DataMap.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("SubjectApiComponent: Error - No Data to save!"));
		return;
	}
	SaveToSubjectApiSubsystem();
	ClearDataMap();
}

// Called when the game starts
void USubjectApiComponent::BeginPlay()
{
	Super::BeginPlay();
}

// TODO
void USubjectApiComponent::ClearDataMap()
{
	DataMap.Empty();
}

// TODO
void USubjectApiComponent::SaveToSubjectApiSubsystem()
{	
	static UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("SubjectApiComponent: Error - No GameInstance!"));
		return;
	}

	USubjectApiSubsystem* Subsystem = GameInstance->GetSubsystem<USubjectApiSubsystem>();
	if (!Subsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("SubjectApiComponent: Error - No Subsystem!"));
		return;
	}

	Subsystem->AddDataMapArray(DataMap);
}
