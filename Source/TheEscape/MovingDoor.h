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
	void FindPressurePlate();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void ReturnDoor(const float DeltaTime);
	void MoveDoor(float DeltaTime);
	float TotalMassOfActors() const;
	void FindAudioComponent();
	
private:
	FVector InitialLocation;
	FVector CurrentLocation;

	UPROPERTY(EditAnywhere);
	FVector TargetPosition;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	AActor* ActorThatOpens = nullptr;

	UPROPERTY(EditAnywhere)
	float MassToOpenDoors = 50.f;
	
	UPROPERTY(EditAnywhere)
	float DoorMoveSpeed = 1.f;
	
	float DoorLastMoved = 0.f;

	UPROPERTY(EditAnywhere)
	float DoorReturnDelay = 2.f;

	bool MoveSoundPlayed = false;
	bool ReturnSoundPlayed = true;
	
	UPROPERTY()
	UAudioComponent* AudioComponent;
};
