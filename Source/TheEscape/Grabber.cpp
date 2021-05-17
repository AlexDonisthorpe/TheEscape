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
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	OwnerName = GetOwner()->GetName();

	FindPhysicsHandle();
	SetupInputComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

 	if(PhysicsHandle)
	{
		LogText = FString::Printf(TEXT("%s: Physics Handler found."), *OwnerName);
	} else
	{
		LogText = FString::Printf(TEXT("%s: Physics Handler not found."), *OwnerName);
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), *LogText);
	LOG_TO_SCREEN(*LogText);
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	} 
}

void UGrabber::Grab()
{
	LogText = TEXT("Grabber Key Pressed");
	UE_LOG(LogTemp, Warning, TEXT("%s"), *LogText);
	LOG_TO_SCREEN(*LogText);

	GetPhysicsBodyInRange();
}

void UGrabber::Release()
{
	LogText = TEXT("Grabber Key Released");
	UE_LOG(LogTemp, Warning, TEXT("%s"), *LogText);
	LOG_TO_SCREEN(*LogText);
}

FHitResult UGrabber::GetPhysicsBodyInRange()
{
	// Get Players viewpoint
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewLocation, OUT PlayerViewRotation);

	LogText = FString::Printf(TEXT("Player Position: %s\nPlayer Rotation:%s"),
		*PlayerViewLocation.ToString(),
		*PlayerViewRotation.ToString()
	);

	// Raycast from player to distance (Reach)
	FVector LineTraceEnd = PlayerViewLocation + (PlayerViewRotation.Vector() * Reach);

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

	return Hit;
}



