// Copyright Alex Donisthorpe 2021


#include "Grabber.h"

#define LOG_TO_SCREEN(Text) \
GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, (TEXT("%s"), (FString)Text));

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

	// ...
	const FString LogText(TEXT("Grabber Reporting for duty"));
	
	UE_LOG(LogTemp, Error, TEXT("%s"), *LogText);
	LOG_TO_SCREEN(*LogText);
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get Players viewpoint
	

	
	// Raycast from player to distance (Reach)

	// See what it hits
}

