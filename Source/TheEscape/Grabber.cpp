// Copyright Alex Donisthorpe 2021


#include "Grabber.h"

#include <Windows.ApplicationModel.Appointments.h>

#include "DrawDebugHelpers.h"

#define LOG_TO_SCREEN(Text) \
GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, (TEXT("%s"), (FString)Text));

#define OUT 

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	const FString OwnerName = GetOwner()->GetName();

	// Check for physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if(!PhysicsHandle)
	{
		LogText = FString::Printf(TEXT("%s: Physics Handler not found."), *OwnerName);
	} else
	{
		LogText = FString::Printf(TEXT("%s: Physics Handler found."), *OwnerName);
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), *LogText);
	LOG_TO_SCREEN(*LogText);

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComponent)
	{
		LogText = FString::Printf(TEXT("%s: Input Component found."), *OwnerName);
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
	} else
	{
		LogText = FString::Printf(TEXT("%s: Input Component missing."), *OwnerName);
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), *LogText);
	LOG_TO_SCREEN(*LogText);
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get Players viewpoint
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewLocation, OUT PlayerViewRotation);

	LogText = FString::Printf(TEXT("Player Position: %s\nPlayer Rotation:%s"),
		*PlayerViewLocation.ToString(),
		*PlayerViewRotation.ToString()
	);
	
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *LogText);
	//LOG_TO_SCREEN(*LogText);
	
	// Raycast from player to distance (Reach)
	FVector LineTraceEnd = PlayerViewLocation + (PlayerViewRotation.Vector() * Reach);
	
	DrawDebugLine(GetWorld(),
		PlayerViewLocation,
		LineTraceEnd,
		FColor(255, 0, 0, 255),
		false,
		0,
		0,
		5
		);

	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, PlayerViewLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);
		// See what it hits
	AActor* ActorHit = Hit.GetActor();

	if(ActorHit)
	{
		LogText = FString::Printf(TEXT("%s"), *ActorHit->GetName());

		UE_LOG(LogTemp, Warning, TEXT("%s"), *LogText);
		LOG_TO_SCREEN(*LogText);
	}
}

void UGrabber::Grab()
{
	LogText = TEXT("Grabber Key Pressed");
	UE_LOG(LogTemp, Warning, TEXT("%s"), *LogText);
	LOG_TO_SCREEN(*LogText);
}

