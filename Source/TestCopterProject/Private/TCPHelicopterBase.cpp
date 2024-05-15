// Fill out your copyright notice in the Description page of Project Settings.


#include "TCPHelicopterBase.h"

#include "TCPHelicopterActorComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
ATCPHelicopterBase::ATCPHelicopterBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HelicopterBody = CreateDefaultSubobject<UStaticMeshComponent>("HelicopterBody");
	HelicopterBody->SetupAttachment(RootComponent);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArm->SetupAttachment(HelicopterBody);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SocketOffset = FVector(0.0f, 0.0f,80.0f);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArm);
	CameraComponent->bUsePawnControlRotation = false;

	HelicopterActorComponent = CreateDefaultSubobject<UTCPHelicopterActorComponent>("HelicopterComponent");
}

// Called when the game starts or when spawned
void ATCPHelicopterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATCPHelicopterBase::MoveUp(float value)
{
	//if (value == 0) return;
	moveUpValue = value;
}

void ATCPHelicopterBase::TurnUp(float value)
{
	TurnUpValue = value;
}

void ATCPHelicopterBase::TurnRight(float value)
{
	TurnRightValue = value;
}

void ATCPHelicopterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveUp", this, &ATCPHelicopterBase::MoveUp);
	PlayerInputComponent->BindAxis("TurnUp", this, &ATCPHelicopterBase::TurnUp);
	PlayerInputComponent->BindAxis("TurnRight", this, &ATCPHelicopterBase::TurnRight);
}

// Called every frame
void ATCPHelicopterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

