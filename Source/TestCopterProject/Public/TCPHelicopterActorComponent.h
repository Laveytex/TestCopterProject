// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TCPHelicopterActorComponent.generated.h"


class ATCPHelicopterBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTCOPTERPROJECT_API UTCPHelicopterActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTCPHelicopterActorComponent();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	virtual void BeginPlay() override;
	
	void Initialize();
	
	void ApplyTurnSpeed();
	void ApplyMoveSpeed();
	
	void ApplyTurnForce(FVector force, FVector point);
	void ApplyMoveForce(FVector force);
	void ApplyGravity(float gravity);

	void ForceCalculate();

	

private:
	static const float& GetTimeStep();
	static float GetTimeStepInSeconds();
	static uint32 GetTimeStepInMilliseconds();

	FVector Multiply3x3(const FTransform& Transform, const FVector& Vector);

	
protected:

	UPROPERTY()
	ATCPHelicopterBase* OwnerPawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Mass = 5000.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float TurnMass = 29270.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector CentreOfMass = FVector(0.0f, 0.0f, -0.1f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector MoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector TurnSpeed;
	


private:
		
};
