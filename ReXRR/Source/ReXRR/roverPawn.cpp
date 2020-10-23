// Fill out your copyright notice in the Description page of Project Settings.


#include "roverPawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "UObject/UObjectGlobals.h"

//Define steering and input constant names
static const FName NAME_SteerInput("Steer");
static const FName NAME_ThrottleInput("Throttle");

//Constructor
AroverPawn::AroverPawn()
{
    UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());

    //Adjust the tire loading
    Vehicle4W->MinNormalizedTireLoad = 0.0f;
    Vehicle4W->MinNormalizedTireLoadFiltered = 0.2f;
    Vehicle4W->MinNormalizedTireLoad = 2.0f;
    Vehicle4W->MinNormalizedTireLoadFiltered = 2.0f;

    //Torque curve setup
    Vehicle4W->MaxEngineRPM = 5700.0f;
    Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
    Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.0f, 400.0f);
    Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(1890.0f, 500.0f);

    //At 5730 rpm, we will have 400 units of torque
    Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(5730.0f, 400.0f);

    //Adjust the steering curve
    Vehicle4W->SteeringCurve.GetRichCurve()->Reset();
    Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(0.0f, 1.0f);
    Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(50.0f, 0.7f);

    //At a 120 degree angle, we will have 0.6 units of steering
    Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(120.0f, 0.6f);

    //Differential system setup
    Vehicle4W->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_4W;
    Vehicle4W->DifferentialSetup.FrontRearSplit = 0.65;

    //Automatic gearbox
    Vehicle4W->TransmissionSetup.bUseGearAutoBox = true;
    Vehicle4W->TransmissionSetup.GearSwitchTime = 0.15f;
    Vehicle4W->TransmissionSetup.GearAutoBoxLatency = 1.0f;

    //Create a spring arm component for our chase camera
    // SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    // SpringArm->SetupAttachment(RootComponent);  
    // SpringArm->TargetArmLength = 250.0f;
    // SpringArm->bUsePawnControlRotation=true;

    // //Create the chase camera component
    // Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ChaseCamera"));
    // Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    // Camera->FieldOfView = 90.0f;
    
}

void AroverPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateInAirControl(DeltaTime);
}

void AroverPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(NAME_ThrottleInput, this, &AroverPawn::ApplyThrottle);
    PlayerInputComponent->BindAxis(NAME_SteerInput, this, &AroverPawn::ApplySteering);
    PlayerInputComponent->BindAxis("LookUp", this, &AroverPawn::LookUp);
    PlayerInputComponent->BindAxis("Turn", this, &AroverPawn::Turn);

    PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &AroverPawn::OnHandbrakePressed);
    PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &AroverPawn::OnHandbrakeReleased);
}

void AroverPawn::ApplyThrottle(float Val)
{
    GetVehicleMovementComponent()->SetThrottleInput(Val);
}
void AroverPawn::ApplySteering(float Val)
{
    GetVehicleMovementComponent()->SetSteeringInput(Val);
}

void AroverPawn::LookUp(float Val)
{
    if (Val != 0.f)
    {
        AddControllerPitchInput(Val);
    }
}

void AroverPawn::Turn(float Val)
{
    if (Val != 0.f)
    {
        AddControllerYawInput(Val);
    }
}

void AroverPawn::OnHandbrakePressed()
{
    GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void AroverPawn::OnHandbrakeReleased()
{
    GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void AroverPawn::UpdateInAirControl(float DeltaTime)
{

}

