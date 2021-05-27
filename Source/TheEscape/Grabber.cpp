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

	CheckForObjectHighlight();	
	
	if(!PhysicsHandle){return;}
	if(PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocationAndRotation(GetGrabbedLocation(), GetGrabbedRotation());
	}
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if(!PhysicsHandle)
	{
 		FString LogText = FString::Printf(TEXT("%s: Physics Handler not found."), *OwnerName);
 		UE_LOG(LogTemp, Error, TEXT("%s"), *LogText);
 		LOG_TO_SCREEN(*LogText);
	}
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
	HitResult = GetPhysicsBodyInRange();
	ComponentToGrab = HitResult.GetComponent();
	
	if(HitResult.GetActor())
	{
		if(!PhysicsHandle){return;}
		
		PhysicsHandle->GrabComponentAtLocationWithRotation(
		ComponentToGrab,
		NAME_None,
		HitResult.Location,
		HitResult.Actor->GetActorRotation()
		);
	}

}

void UGrabber::Release()
{
	if(!PhysicsHandle){return;}
	
	if(PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
}

FVector UGrabber::GetRayEndpoint() const
{
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;
	
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewLocation,
		OUT PlayerViewRotation
		);

	return PlayerViewLocation + (PlayerViewRotation.Vector() * Reach);
}

FVector UGrabber::GetGrabbedLocation()
{
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;
	
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewLocation,
		OUT PlayerViewRotation
		);

	return PlayerViewLocation + (PlayerViewRotation.Vector() * HitResult.Distance);
}

FRotator UGrabber::GetGrabbedRotation()
{
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;
	
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
	OUT PlayerViewLocation,
	OUT PlayerViewRotation
	);

	return FRotator(PlayerViewRotation.Pitch, PlayerViewRotation.Yaw + 90.f, PlayerViewRotation.Roll);
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

FHitResult UGrabber::GetPhysicsBodyInRange() const
{
	FHitResult Hit;
	const FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(OUT Hit, GetPlayerLocation(), GetRayEndpoint(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);

	return Hit;
}

void UGrabber::CheckForObjectHighlight()
{
	FHitResult Hit = GetPhysicsBodyInRange();

	// If the Line Trace hits a physics object
	if(Hit.GetActor())
	{
		// If it's the same object we've already got highlighted, then return
		if(Hit.GetActor() == HighlightedObject){ return; }
		
		HighlightedObject = Hit.GetActor();

		// Set Render Depth for the highlight material to activate
		auto ActorArray = HighlightedObject->FindComponentByClass<UPrimitiveComponent>();
		ActorArray[0].SetRenderCustomDepth(true);
	} else
	{
		if(HighlightedObject)
		{
			// Turn off the highlight on the object if we're no longer hovering over it
			auto ActorArray = HighlightedObject->FindComponentByClass<UPrimitiveComponent>();
			ActorArray[0].SetRenderCustomDepth(false);

			HighlightedObject = nullptr;
		}
	}
}


