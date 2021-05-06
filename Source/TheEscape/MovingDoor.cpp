// Copyright Alex Donisthorpe 2021

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "MovingDoor.h"

#define LOG_TO_SCREEN(Text) \
GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, (TEXT("%s"), (FString)Text));

// Sets default values for this component's properties
UMovingDoor::UMovingDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UMovingDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialLocation = GetOwner()->GetActorLocation();
	CurrentLocation = InitialLocation;
	TargetPosition += InitialLocation;

	if(!PressurePlate)
	{
		const FString LogText = FString::Printf(TEXT("%s has open door component, but no trigger set!"), *GetOwner()->GetName());
		UE_LOG(LogTemp, Error, TEXT("%s"), *LogText);
		LOG_TO_SCREEN(*LogText);
	}

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UMovingDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		MoveDoor(DeltaTime);
	} else
	{
		CloseDoor(DeltaTime);
	}
}
	
void UMovingDoor::MoveDoor(const float DeltaTime)
{
	CurrentLocation = FMath::Lerp(CurrentLocation, TargetPosition, DeltaTime * 1.f);
	GetOwner()->SetActorLocation(CurrentLocation);		
}

void UMovingDoor::CloseDoor(const float DeltaTime)
{
	CurrentLocation = FMath::Lerp(CurrentLocation, InitialLocation, DeltaTime * 1.f);
	GetOwner()->SetActorLocation(CurrentLocation);
}

