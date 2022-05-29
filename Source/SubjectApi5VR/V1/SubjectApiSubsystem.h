// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Http.h"
#include "SubjectApiSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SUBJECTAPI5VR_API USubjectApiSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// TODO: Deprecated -- Just for reference
	void SendRequest();	
	void BuildAndSendRequest(TMap<FString, FString> DataMap);

	UFUNCTION(BlueprintCallable)
	void AddDataMapArray(TMap<FString, FString> DataMapArray);
	UFUNCTION(BlueprintCallable)
	void BuildAndSendFromDataMapArrayRequest();

private:
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	// URLs
	// TODO: Use Urls
	FString UrlLogin = "";
	FString UrlCreateSubjectData = "localhost:3000/api/v1/subject_data";

	UPROPERTY(Category = MapsAndSets, EditAnywhere)
	TMap<FString, FString> LoginMap;
	TMap<FString, FString> RequestHeaderMap;

	// Store Map to send
	TArray<TMap<FString, FString>> DataMapArray;
};
