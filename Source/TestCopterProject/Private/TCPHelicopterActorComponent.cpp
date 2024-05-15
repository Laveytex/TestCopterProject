// Fill out your copyright notice in the Description page of Project Settings.


#include "TCPHelicopterActorComponent.h"

#include "TCPHelicopterBase.h"

USTRUCT(BlueprintType)
struct FHelicopterMovementState
{
	//	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Thrust = 0.4f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float ThrustFallOff = 0.75f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SideSlip = 0.20f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Roll = 0.0065f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Pitch = 0.0065f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float FormLift = 0.4f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float AttackLift = 0.012f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveRes = 0.997f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector VecTurnRes = FVector(0.800f, 0.820f, 0.996f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector VecSpeedRes = FVector(0.0f, 0.0f, 7.0f);
};

// Sets default values for this component's properties
UTCPHelicopterActorComponent::UTCPHelicopterActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTCPHelicopterActorComponent::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}


// Called every frame
void UTCPHelicopterActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	 if(!OwnerPawn) return;
	
	ForceCalculate();
}

void UTCPHelicopterActorComponent::Initialize()
{
	if (UWorld* World = GetWorld())
	{
		OwnerPawn = Cast<ATCPHelicopterBase>(GetOwner());
	}
	
}

void UTCPHelicopterActorComponent::ApplyTurnSpeed()
{
	FVector turnvec = TurnSpeed*GetTimeStep() * 100;

	OwnerPawn->GetActorRightVector() += FVector::CrossProduct(turnvec, OwnerPawn->GetActorRightVector());
	OwnerPawn->GetActorForwardVector() += FVector::CrossProduct(turnvec, OwnerPawn->GetActorForwardVector());
	OwnerPawn->GetActorUpVector() += FVector::CrossProduct(turnvec, OwnerPawn->GetActorUpVector());
}

void UTCPHelicopterActorComponent::ApplyMoveSpeed()
{
	FTransform Transform = OwnerPawn->GetActorTransform();
	FVector Offset = MoveSpeed * GetTimeStep() * 100;
	Transform.AddToTranslation(Offset);
	
	OwnerPawn->SetActorTransform(Transform);
}

void UTCPHelicopterActorComponent::ApplyTurnForce(FVector force, FVector point)
{
	FVector com = Multiply3x3(OwnerPawn->GetTransform(), CentreOfMass);
	FVector turnimpulse = FVector::CrossProduct(point-com, force);
	TurnSpeed += turnimpulse*(1.0f/TurnMass);
}

void UTCPHelicopterActorComponent::ApplyMoveForce(FVector force)
{
	MoveSpeed += force*(1.0f/Mass)*1000;
}

void UTCPHelicopterActorComponent::ApplyGravity(float gravity)
{
	MoveSpeed.Z -= gravity * GetTimeStep() * 100;
}

void UTCPHelicopterActorComponent::ForceCalculate()
{
	//tFlyingHandlingData* flyingHandling = pFlyingHandling;
	FHelicopterMovementState HelicopterMovementState;
	
	const float Gravity = 0.0008f;
	
	ApplyGravity(Gravity);
	
	float rm = FMath::Pow(HelicopterMovementState.MoveRes, GetTimeStep());
	MoveSpeed *= rm;

	float fUpSpeed = FVector::DotProduct(MoveSpeed, OwnerPawn->GetActorUpVector());
	
	//float fThrust = Input->GetAccelerate() - Input->GetBrake();
	float fThrust = OwnerPawn->moveUpValue;
	
	if(fThrust < 0.0f)
		fThrust *= 2.0f;
	
	fThrust = HelicopterMovementState.Thrust * fThrust + 0.95f;	
	fThrust -= HelicopterMovementState.ThrustFallOff * fUpSpeed;
	
	if( OwnerPawn->GetActorLocation().Z > 80.0f)
		fThrust *= 10.0f/(OwnerPawn->GetActorLocation().Z - 70.0f);
	ApplyMoveForce(Gravity * OwnerPawn->GetActorUpVector() * fThrust * Mass * GetTimeStep());

	if (OwnerPawn->GetActorUpVector().Z > 0.0f){
		float upRight = FMath::Clamp(OwnerPawn->GetActorRightVector().Z, -HelicopterMovementState.FormLift, HelicopterMovementState.FormLift);
		float upImpulseRight = -upRight * HelicopterMovementState.AttackLift * TurnMass * GetTimeStep();
		ApplyTurnForce(upImpulseRight * OwnerPawn->GetActorUpVector(), OwnerPawn->GetActorRightVector());

		float upFwd = FMath::Clamp(OwnerPawn->GetActorForwardVector().Z, -HelicopterMovementState.FormLift, HelicopterMovementState.FormLift);
		float upImpulseFwd = -upFwd * HelicopterMovementState.AttackLift * TurnMass * GetTimeStep();
		ApplyTurnForce(upImpulseFwd * OwnerPawn->GetActorUpVector(), OwnerPawn->GetActorForwardVector());
		
	}else{
		float upRight = OwnerPawn->GetActorRightVector().Z < 0.0f ? -HelicopterMovementState.FormLift : HelicopterMovementState.FormLift;
		float upImpulseRight = -upRight * HelicopterMovementState.AttackLift * TurnMass * GetTimeStep();
		ApplyTurnForce(upImpulseRight * OwnerPawn->GetActorUpVector(), OwnerPawn->GetActorRightVector());

		float upFwd = OwnerPawn->GetActorForwardVector().Z < 0.0f ? -HelicopterMovementState.FormLift : HelicopterMovementState.FormLift;
		float upImpulseFwd = -upFwd * HelicopterMovementState.AttackLift * TurnMass * GetTimeStep();
		ApplyTurnForce(upImpulseFwd * OwnerPawn->GetActorUpVector(), OwnerPawn->GetActorForwardVector());
	}

	float fRoll, fPitch;
	fPitch = 0.0f;
	
	//fRoll = -Input->GetTiltLeftRight();
	fRoll = -OwnerPawn->TurnRightValue;

	/*if(FVector::GetAbs(Input->GetTiltForwardBack()) > 1.0f) 
		fPitch = -Input->GetTiltForwardBack();*/
	if(FMath::Abs(OwnerPawn->TurnUpValue) > 1.0f)
		fPitch = -OwnerPawn->TurnUpValue;
	
	ApplyTurnForce(fPitch * OwnerPawn->GetActorUpVector() * HelicopterMovementState.Pitch * TurnMass * GetTimeStep(), OwnerPawn->GetActorForwardVector());
	ApplyTurnForce(fRoll * OwnerPawn->GetActorUpVector() * HelicopterMovementState.Roll * TurnMass * GetTimeStep(), OwnerPawn->GetActorRightVector());

	float fSideSpeed = -FVector::DotProduct(MoveSpeed, OwnerPawn->GetActorRightVector());
	float fSideSlipAccel = HelicopterMovementState.SideSlip * fSideSpeed * FMath::Abs(fSideSpeed);
	ApplyMoveForce(Mass * OwnerPawn->GetActorRightVector() * fSideSlipAccel * GetTimeStep());

	
	float rX = FMath::Pow(HelicopterMovementState.VecTurnRes.X, GetTimeStep());
	float rY = FMath::Pow(HelicopterMovementState.VecTurnRes.Y, GetTimeStep());
	float rZ = FMath::Pow(HelicopterMovementState.VecTurnRes.Z, GetTimeStep());
	
	FVector TurnSpeedLocal = Multiply3x3(OwnerPawn->GetTransform(), TurnSpeed);
	float fResistanceMultiplier = FMath::Pow(1.0f / (HelicopterMovementState.VecSpeedRes.Z * FMath::Square(TurnSpeedLocal.Z) + 1.0f) * rZ, GetTimeStep());
	float fResistance = TurnSpeedLocal.Z * fResistanceMultiplier - TurnSpeedLocal.Z;
	TurnSpeedLocal.X *= rX;
	TurnSpeedLocal.X *= rY;
	TurnSpeedLocal.Z *= fResistanceMultiplier;
	TurnSpeed = Multiply3x3(OwnerPawn->GetTransform(), TurnSpeedLocal);
	ApplyTurnForce(-OwnerPawn->GetActorRightVector() * fResistance * TurnMass, OwnerPawn->GetActorForwardVector() + Multiply3x3(OwnerPawn->GetTransform(), CentreOfMass));

	ApplyMoveSpeed();
	ApplyTurnSpeed();
}

const float& UTCPHelicopterActorComponent::GetTimeStep()
{
	static float ms_TimeStep = 0.02f;
	return ms_TimeStep;
}

float UTCPHelicopterActorComponent::GetTimeStepInSeconds()
{
	return GetTimeStep() / 50.0f;
}

uint32 UTCPHelicopterActorComponent::GetTimeStepInMilliseconds()
{
	return GetTimeStepInSeconds();
}

FVector UTCPHelicopterActorComponent::Multiply3x3(const FTransform& Transform, const FVector& Vector)
{
	return Transform.TransformPosition(Vector);
}

