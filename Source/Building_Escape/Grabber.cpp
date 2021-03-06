// Copyright Isaac Tellez 2021

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

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

	FindPhysicsHandle();

	SetupInputComponent();
}

//Check for Physics Handle Component
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandle component on %s"), *(GetOwner()->GetName()));
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::Grab()
{
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentGrabbed = HitResult.GetComponent();

	// Try and reach any actors with a physics body collision channel set

	// If hit, attach the physics handle
	if (HitResult.GetActor())
	{
		if (!PhysicsHandle) return;
		PhysicsHandle->GrabComponentAtLocation(
			ComponentGrabbed,
			NAME_None,
			GetLineTraceEnd()
		);
	}
}

void UGrabber::Release()
{	
	if (!PhysicsHandle) return;
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) return;
	//If physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		// Move the object we are holding
		PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
	}
}

FVector UGrabber::GetLineTraceEnd() const
{
	// Get player's viewpoint
	FVector PlayerVPLocation;
	FRotator PlayerVPRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerVPLocation,
		OUT PlayerVPRotation
	);

	return PlayerVPLocation + PlayerVPRotation.Vector() * Reach;
}

FVector UGrabber::GetPlayerWorldPos() const
{
	// Get player's viewpoint
	FVector PlayerVPLocation;
	FRotator PlayerVPRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerVPLocation,
		OUT PlayerVPRotation
	);

	return PlayerVPLocation;
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{

	// Draw a line from player to show reach (OPTIONAL)
	/*DrawDebugLine(
		GetWorld(),
		PlayerVPLocation,
		LineTraceEnd,
		FColor(0, 255, 0),
		false,
		0.f,
		0,
		5.f
	);*/

	FHitResult Hit;
	// Ray-cast out to a certain distance (reach)
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerWorldPos(),
		GetLineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	// See what it hits
	AActor* ActorHit = Hit.GetActor();

	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Collided with %s"), *(ActorHit->GetName()));
	}

	return Hit;
}
