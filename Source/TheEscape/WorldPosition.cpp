#include "WorldPosition.h"
#include "GameFramework/Actor.h"

#define LOG_TO_SCREEN(Text) \
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, (TEXT("%s"), (FString)Text));

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

	TArray<FStringFormatArg> args;
	args.Add(FStringFormatArg(GetName()));
	args.Add(FStringFormatArg(GetOwner()->GetName()));
	FString Test = FString::Format(TEXT("Component: {0} - Object Name is: {1}"), args);
	//LogMessage.Append((TEXT("%s"), GetName()));
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Test);
	LOG_TO_SCREEN(*Test);
}

// Called every frame
void UWorldPosition::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

