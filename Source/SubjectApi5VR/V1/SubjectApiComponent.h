// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SubjectApiComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUBJECTAPI5VR_API USubjectApiComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USubjectApiComponent();

	UFUNCTION(BlueprintCallable)
	void FinishAndSave();

	// Map to store datasets
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TMap<FString, FString> DataMap;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// Clears the DataMap, if the event needs to rerun 
	void ClearDataMap();
	// Saves the array to subsystem list 
	void SaveToSubjectApiSubsystem();
};
