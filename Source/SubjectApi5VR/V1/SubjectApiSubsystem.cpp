// Fill out your copyright notice in the Description page of Project Settings.


#include "SubjectApiSubsystem.h"
#include "Json.h"
#include "Serialization/JsonSerializer.h"

void USubjectApiSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
  Super::Initialize(Collection);
  UE_LOG(LogTemp, Warning, TEXT("Subsystem loaded."));
}

void USubjectApiSubsystem::Deinitialize()
{
  Super::Deinitialize();
  
  // if we have unsend elements, send them now
  if (DataMapArray.Num() > 0)
  {
    BuildAndSendFromDataMapArrayRequest();
  }
}

void USubjectApiSubsystem::SendRequest()
{
  TSharedPtr<FJsonObject> RequestObj = MakeShared<FJsonObject>();
  RequestObj->SetStringField("name", "subject1");
  RequestObj->SetStringField("pin", "1234");
  TSharedPtr<FJsonObject> PayloadObj = MakeShared<FJsonObject>();
  TSharedPtr<FJsonObject> DataObj = MakeShared<FJsonObject>();
  DataObj->SetStringField("message", "from UE5");
  DataObj->SetNumberField("solution", 1337);
  PayloadObj->SetObjectField("data", DataObj);
  RequestObj->SetObjectField("payload", PayloadObj);
  FString RequestBody;
  TSharedRef<TJsonWriter<TCHAR>> Writer = TJsonWriterFactory<TCHAR>::Create(&RequestBody);
  FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer);

  FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
  Request->OnProcessRequestComplete().BindUObject(this, &USubjectApiSubsystem::OnResponseReceived);
  Request->SetURL("localhost:3000/api/v1/subject_data");
  Request->SetHeader("Content-Type", "application/json");
  Request->SetContentAsString(RequestBody);
  Request->SetVerb("POST");
  Request->ProcessRequest();
}

void USubjectApiSubsystem::BuildAndSendRequest(TMap<FString, FString> DataMap)
{
  TSharedPtr<FJsonObject> DataObj = MakeShared<FJsonObject>();
  for (auto& Elem : DataMap)
  {
    DataObj->SetStringField(Elem.Key, Elem.Value);
  }
  TSharedPtr<FJsonObject> RequestObj = MakeShared<FJsonObject>();
  RequestObj->SetStringField("name", "subject1");
  RequestObj->SetStringField("pin", "1234");
  TSharedPtr<FJsonObject> PayloadObj = MakeShared<FJsonObject>();
  PayloadObj->SetObjectField("data", DataObj);
  RequestObj->SetObjectField("payload", PayloadObj);
  FString RequestBody;
  TSharedRef<TJsonWriter<TCHAR>> Writer = TJsonWriterFactory<TCHAR>::Create(&RequestBody);
  FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer);

  FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
  Request->OnProcessRequestComplete().BindUObject(this, &USubjectApiSubsystem::OnResponseReceived);
  Request->SetURL("localhost:3000/api/v1/subject_data");
  Request->SetHeader("Content-Type", "application/json");
  Request->SetContentAsString(RequestBody);
  Request->SetVerb("POST");
  Request->ProcessRequest();
}
void USubjectApiSubsystem::AddDataMapArray(TMap<FString, FString> DataMap)
{
  DataMapArray.Add(DataMap);
}

void USubjectApiSubsystem::BuildAndSendFromDataMapArrayRequest()
{
  TArray<TSharedPtr<FJsonValue>> DataObjArray;
  for (auto& Elem : DataMapArray)
  {
    TSharedPtr<FJsonObject> DataEntryObj = MakeShared<FJsonObject>();
    for (auto& Child : Elem) 
    {
      DataEntryObj->SetStringField(Child.Key, Child.Value);
    }
    TSharedRef< FJsonValueObject > JsonValue = MakeShareable( new FJsonValueObject( DataEntryObj) );
    DataObjArray.Add(JsonValue);
  }
  
  TSharedPtr<FJsonObject> RequestObj = MakeShared<FJsonObject>();
  RequestObj->SetStringField("name", "subject1");
  RequestObj->SetStringField("pin", "1234");
  TSharedPtr<FJsonObject> PayloadObj = MakeShared<FJsonObject>();
  PayloadObj->SetArrayField("data", DataObjArray);
  RequestObj->SetObjectField("payload", PayloadObj);
  FString RequestBody;
  TSharedRef<TJsonWriter<TCHAR>> Writer = TJsonWriterFactory<TCHAR>::Create(&RequestBody);
  FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer);

  FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
  Request->OnProcessRequestComplete().BindUObject(this, &USubjectApiSubsystem::OnResponseReceived);
  Request->SetURL("localhost:3000/api/v1/subject_data");
  Request->SetHeader("Content-Type", "application/json");
  Request->SetContentAsString(RequestBody);
  Request->SetVerb("POST");
  Request->ProcessRequest();
}

void USubjectApiSubsystem::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
  check(IsInGameThread());

  if (!bConnectedSuccessfully)
  {
    UE_LOG(LogTemp, Warning, TEXT("Couldn't connect to server!"));
    return;
  }

  TSharedPtr<FJsonObject> ResponseObj;
  TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());

  if (!FJsonSerializer::Deserialize(JsonReader, ResponseObj))
  {
    UE_LOG(LogTemp, Error, TEXT("Couldn't handle response content. %s"), *Response->GetContentAsString());
    return;
  }
  
  UE_LOG(LogTemp, Display, TEXT("Response %s"), *Response->GetContentAsString());
  // Clear sent request
  DataMapArray.Empty();

  // @TODO: Handle a real response of SubjectApi -- This is just for reference
  if (ResponseObj->HasField("title"))
  {
    UE_LOG(LogTemp, Display, TEXT("Title: %s"), *ResponseObj->GetStringField("title"));
  }  

}

