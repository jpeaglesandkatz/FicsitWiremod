﻿// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Components/SignComponentBase.h"
#include "Components/SignComponentDescriptor.h"
#include "ManagedSign.generated.h"

USTRUCT(BlueprintType)
struct FManagedSignData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FVector2D Size;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FLinearColor BackgroundColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool IsStatic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FSignComponentData> Components;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FManagedSignConnectionSettings> Connections;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FSignComponentVariableData> Variables;
};

UCLASS()
class FICSITWIREMOD_API AManagedSign : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override
	{
		Super::BeginPlay();
		OnGenerateSign();
	}
	
	void ApplySignLayout(FManagedSignData NewData, UObject* Setter)
	{
		if(!GetCanConfigure(Setter)) return;
		Data = NewData;

		InitSign();
	}

	virtual void ClientProcess_Implementation(double DeltaTime) override
	{
		for (auto Component : ActiveComponents)
		{
			if(Component) Component->OnUpdate(this);
		}
	}

	void InitSign()
	{
		ConnectionsInfo.Inputs.Empty();
		for (auto Connection : Data.Connections)
		{
			ConnectionsInfo.Inputs.Add(FBuildingConnection(Connection.Name, "", Connection.Type.GetValue()));
		}

		OnGenerateSign();
	}
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnGenerateSign();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Replicated)
	FManagedSignData Data;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USignComponentBase*> ActiveComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USignComponentBase*> StaticComponents;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AManagedSign, Data)
	}
};