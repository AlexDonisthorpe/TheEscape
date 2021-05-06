// Copyright Alex Donisthorpe 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"

#include "MovingDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEESCAPE_API UMovingDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMovingDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void CloseDoor(const float DeltaTime);
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void MoveDoor(float DeltaTime);

private:
	FVector InitialLocation;
	FVector CurrentLocation;

	UPROPERTY(EditAnywhere);
	FVector TargetPosition;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	UPROPERTY(EditAnywhere)
	AActor* ActorThatOpens;
};
