// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "TCPHelicopterBase.generated.h"

class USpringArmComponent;
class UTCPHelicopterActorComponent;
class UCameraComponent;

UCLASS()
class TESTCOPTERPROJECT_API ATCPHelicopterBase : public APawn
{
	GENERATED_BODY()
	
public:	
	ATCPHelicopterBase();

	float moveUpValue = 0;
	float TurnUpValue = 0;
	float TurnRightValue = 0;
	
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UTCPHelicopterActorComponent* HelicopterActorComponent = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* HelicopterBody;

	
	
protected:
	virtual void BeginPlay() override;

	void MoveUp(float value);
	void TurnUp(float value);
	void TurnRight(float value);

private:

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	
};
