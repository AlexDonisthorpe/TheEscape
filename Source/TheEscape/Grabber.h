// Copyright Alex Donisthorpe 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
private:
	FString LogText;
	FString OwnerName;
	
	UPROPERTY(EditAnywhere)
	float Reach = 500.f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent =  nullptr;

	void Grab();
	void Release();
	void FindPhysicsHandle();
	void SetupInputComponent();
	FVector GetRayEndpoint() const;
	FVector GetPlayerLocation() const;
	FHitResult GetPhysicsBodyInRange();
};
