// Copyright Isaac Tellez 2021

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

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

	//Check for PhysicsHandle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle) {
		// PhysicsHandle is found
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandle component on %s"), *(GetOwner()->GetName()) );
	}

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Input component found on %s"), *(GetOwner()->GetName()));
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
	}
	/*else {
		UE_LOG(LogTemp, Error, TEXT("No Input component found on %s"), *(GetOwner()->GetName()));
	}*/
	
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber pressed"));
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player's viewpoint
	FVector PlayerVPLocation;
	FRotator PlayerVPRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerVPLocation, 
		OUT PlayerVPRotation
	);

	// Draw a line from player to show reach
	FVector LineTraceEnd = PlayerVPLocation + PlayerVPRotation.Vector() * Reach;

	DrawDebugLine(
		GetWorld(),
		PlayerVPLocation,
		LineTraceEnd,
		FColor(0,255,0),
		false,
		0.f,
		0,
		5.f
	);

	FHitResult Hit;
	// Ray-cast out to a certain distance (reach)
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerVPLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	
	// See what it hits
	AActor* ActorHit = Hit.GetActor();
	
	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Collided with %s"), *(ActorHit->GetName()) );
	}
	
	// Logging to test
}

