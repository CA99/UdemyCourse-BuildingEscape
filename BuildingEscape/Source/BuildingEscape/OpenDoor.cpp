// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	//ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();

	DoorInitRot = Owner->GetTransform().GetRotation().Rotator();
	DoorOpenRot = DoorInitRot;
	DoorOpenRot = DoorOpenRot.Add(0.0f, OpenAngle, 0.0f);
	//UE_LOG(LogTemp, Warning, TEXT("DoorInitRot: %s"), *DoorInitRot.ToString())
	//UE_LOG(LogTemp, Warning, TEXT("DoorOpenRot: %s"), *DoorOpenRot.ToString())
	//UE_LOG(LogTemp, Warning, TEXT("Test: %s"), *FRotator(0.0f,0.0f,0.0f).ToString())
}

void UOpenDoor::CloseDoor() {
	Owner->SetActorRotation(DoorInitRot);
	//UE_LOG(LogTemp, Warning, TEXT("Door Closed"))
}

void UOpenDoor::OpenDoor() {
	Owner->SetActorRotation(DoorOpenRot);
	//UE_LOG(LogTemp, Warning, TEXT("Door Opened"))
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {
	float TotalMass = 0.0f;

	// find all overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OverlappingActors); // Sets OverlappingActors as OUT parameter
		// iterate thru actors and add their masses
	float ActorMass = 0.0f;
	for (auto& Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("Total Mass: %s"), *Actor->GetName())
	}
	return TotalMass;
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	GetTotalMassOfActorsOnPlate();
	if (GetTotalMassOfActorsOnPlate() > TriggerMass) {
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime >= DoorCloseDelay) {
		CloseDoor();
		//FString DoorRot = Owner->GetTransform().GetRotation().ToString();
	}
}