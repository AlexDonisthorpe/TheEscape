#include "WorldPosition.h"

#define LOG_TO_SCREEN(Text) \
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT(Text));

// Sets default values for this component's properties
UWorldPosition::UWorldPosition()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWorldPosition::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("Log"));
	UE_LOG(LogTemp, Warning, TEXT("Warning!"));
	UE_LOG(LogTemp, Error, TEXT("Error!"));

	LOG_TO_SCREEN("Test");
}


// Called every frame
void UWorldPosition::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

