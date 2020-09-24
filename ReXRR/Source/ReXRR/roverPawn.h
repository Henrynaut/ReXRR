// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "roverPawn.generated.h"

/**
 * 
 */
UCLASS()
class REXRR_API AroverPawn : public AWheeledVehicle
{
	GENERATED_BODY()

public:

	AroverPawn();

	virtual void Tick(float DeltaTime) override;

	virual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	//Throttle and Steering
	void ApplyThrottle(float Val);
	void ApplySteering(float Val);

	//Look around
	void LookUp(float Val);
	void Turn(float Val);

	//Handbrake
	void OnHandbrakePressed();
	void OnHandbrakeReleased();

	//Update in air and upsidedown physics
	void UpdateInAirControl(float DeltaTime);
	
};
