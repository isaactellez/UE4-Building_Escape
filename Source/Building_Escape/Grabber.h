// Copyright Isaac Tellez 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UGrabber : public UActorComponent
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
	float Reach = 150.f;

	UPROPERTY(EditAnywhere)
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UPROPERTY(EditAnywhere)
	UInputComponent* InputComponent = nullptr;

	void Grab();
	void Release();
	void FindPhysicsHandle();
	void SetupInputComponent();

	//Return the first physics body within reach
	FHitResult GetFirstPhysicsBodyInReach() const;

	//Return a vector that represents the reach of an actor
	FVector GetLineTraceEnd() const;

	//Get Players position in the world
	FVector GetPlayerWorldPos() const;
		
};
