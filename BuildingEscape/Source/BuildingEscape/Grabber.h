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

	UPROPERTY(EditAnywhere)
	float Reach = 200.0f;

	UPROPERTY(EditAnywhere)
	float HoldDistance = 100.0f;

	UPROPERTY(EditAnywhere)
	bool FollowPlayerRotation = true;

	void Grab();
	void Release();
	void FindPhysicsHandleComponent();
	void SetupInputComponent();
	const FHitResult GetFirstPhysicsBodyInReach();
	const FVector GetPlayerReachStartPoint();
	const FVector GetPlayerReachEndPoint();
	const FRotator GetPlayerRotation(); // WARNING: Will update PlayerViewPointLocation and PlayerViewPointRotation
};
