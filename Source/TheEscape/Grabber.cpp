// Copyright Alex Donisthorpe 2021

#include "DrawDebugHelpers.h"
#include "Grabber.h"

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

	if(PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetRayEndpoint());
	}
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

 	if(PhysicsHandle == nullptr)
	{
 		LogText = FString::Printf(TEXT("%s: Physics Handler not found."), *OwnerName);
	}
	
	UE_LOG(LogTemp, Error, TEXT("%s"), *LogText);
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
	FHitResult HitResult = GetPhysicsBodyInRange();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	if(HitResult.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation(
		ComponentToGrab,
		NAME_None,
		GetRayEndpoint()
		);
	}
}

void UGrabber::Release()
{
	if(PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
}

FVector UGrabber::GetRayEndpoint() const
{
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewLocation, OUT PlayerViewRotation);

	return PlayerViewLocation + (PlayerViewRotation.Vector() * Reach);
}

FVector UGrabber::GetPlayerLocation() const
{
	// Get Players viewpoint
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewLocation,
		OUT PlayerViewRotation
		);

	return PlayerViewLocation;
}

FHitResult UGrabber::GetPhysicsBodyInRange()
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(OUT Hit, GetPlayerLocation(), GetRayEndpoint(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);

	AActor* ActorHit = Hit.GetActor();

	if(ActorHit)
	{
		LogText = FString::Printf(TEXT("%s"), *ActorHit->GetName());

		UE_LOG(LogTemp, Warning, TEXT("%s"), *LogText);
		LOG_TO_SCREEN(*LogText);
	}

	return Hit;
}



