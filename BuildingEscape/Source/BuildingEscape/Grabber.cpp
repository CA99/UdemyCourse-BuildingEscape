// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"

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

	FindPhysicsHandleComponent();

	SetupInputComponent();

	//GetFirstPhysicsBodyInReach();
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab."))
	// try to reach any actors with physics collision
	auto Target = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = Target.GetComponent();

	if (Target.GetActor()) {
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None, // no bones
			ComponentToGrab->GetOwner()->GetActorLocation()
		);
	}
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Release."))
	// TODO release physics handle
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::FindPhysicsHandleComponent() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Grabber for %s could not find the PhysicsHandle component."), *GetOwner()->GetName())
	}
}

void UGrabber::SetupInputComponent() {
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Grabber for %s has Input component."), *GetOwner()->GetName())
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Grabber for %s could not find the Input component."), *GetOwner()->GetName())
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach() {
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters(
		FName(TEXT("")),
		false,
		GetOwner()
	);

	GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		GetPlayerReachStartPoint(),
		GetPlayerReachEndPoint(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	return HitResult;
}

const FVector UGrabber::GetPlayerReachStartPoint() { // WARNING: Will update PlayerViewPointLocation and PlayerViewPointRotation
	// Get player viewpoint
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		PlayerViewPointLocation, // GetPlayerViewPoint() sets these vars to their values
		PlayerViewPointRotation // GetPlayerViewPoint() sets these vars to their values
	);
	return PlayerViewPointLocation;
}

const FVector UGrabber::GetPlayerReachEndPoint() { // WARNING: Will update PlayerViewPointLocation and PlayerViewPointRotation
	// Get player viewpoint
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		PlayerViewPointLocation, // GetPlayerViewPoint() sets these vars to their values
		PlayerViewPointRotation // GetPlayerViewPoint() sets these vars to their values
	);
	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
	return LineTraceEnd;
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	
	// if physics handle attached
	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(GetPlayerReachEndPoint());
	}
}