// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
	FVector PlayerViewPointLocation; // Will pass into and be set by GetPlayerViewPoint()
	FRotator PlayerViewPointRotation; // Will pass into and be set by GetPlayerViewPoint()
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;
	float Reach = 150.0f;
	void Grab();
	void Release();
	void FindPhysicsHandleComponent();
	void SetupInputComponent();
	const FHitResult GetFirstPhysicsBodyInReach();
	const FVector GetPlayerReachStartPoint();
	const FVector GetPlayerReachEndPoint(); // WARNING: Will update PlayerViewPointLocation and PlayerViewPointRotation
};
