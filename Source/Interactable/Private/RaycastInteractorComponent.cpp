// Fill out your copyright notice in the Description page of Project Settings.

#include "RaycastInteractorComponent.h"
#include "InteractableComponent.h"

void DebugPrint(const URaycastInteractorComponent* Caller, const FColor Color, const FString& Message)
{
#if not UE_BUILD_SHIPPING
	if (Caller->Debug)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, Color, Message);
	}
#endif
}

URaycastInteractorComponent::URaycastInteractorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Camera = nullptr;
	Debug = false;
	CurrentInteractable = nullptr;
	bIsInteracting = false;
}

void URaycastInteractorComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Camera == nullptr)
	{
		Camera = GetOwner()->GetComponentByClass<UCameraComponent>();
		if (Camera == nullptr)
		{
			UE_LOG(LogTemp, Warning,
			       TEXT("No camera found on '%s', raycast interactor will not work until a camera is assigned."),
			       *GetOwner()->GetName());
			PrimaryComponentTick.bCanEverTick = false;
		}
	}
}

void URaycastInteractorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	DoInteractionTest();
}

void URaycastInteractorComponent::UpdateCachedInteractable(UInteractableComponent* InteractableComponent)
{
	if (CurrentInteractable == InteractableComponent)
	{
		return;
	}

	if (InteractableComponent == nullptr){
		OnInteractableUpdated.Execute(nullptr);
		CurrentInteractable->DeactivateInteractionOutline();
		CurrentInteractable = nullptr;
		return;
	}
	
	if (CurrentInteractable != nullptr)
	{
		CurrentInteractable->DeactivateInteractionOutline();
	}

	CurrentInteractable = InteractableComponent;
	CurrentInteractable->ActivateInteractionOutline();
	OnInteractableUpdated.Execute(InteractableComponent);
}

void URaycastInteractorComponent::DoInteractionTest()
{
	if (bIsInteracting)
	{
		return; 
	}

	FHitResult Result;
	const FVector Start = Camera->GetComponentLocation();
	const FVector End = Start + Camera->GetForwardVector() * InteractionDistance;

#if not UE_BUILD_SHIPPING
	if (Debug)
	{
		DrawDebugLine(
			GetWorld(),
			Start,
			End,
			FColor(0, 255, 255),
			false,
			3.0f,
			0,
			2.0f
		);
	}
#endif

	if (GetWorld()->LineTraceSingleByChannel(Result, Start, End, ECC_Visibility, {}, {}))
	{
		const AActor* HitActor = Result.GetActor();
		if (HitActor == nullptr)
		{
			return;
		}

		UInteractableComponent* InteractableComponent = HitActor->GetComponentByClass<UInteractableComponent>();
		if (InteractableComponent == nullptr)
		{
			UpdateCachedInteractable(nullptr);
			DebugPrint(this, FColor::Orange,
			           FString::Printf(TEXT("Hit non interactable actor: %s"), *HitActor->GetName()));
			return;
		}

		UpdateCachedInteractable(InteractableComponent);
		DebugPrint(this, FColor::Cyan, FString::Printf(TEXT("Hit actor: %s"), *HitActor->GetName()));
	}
	else
	{
		UpdateCachedInteractable(nullptr);
		DebugPrint(this, FColor::Red, TEXT("No hits"));
	}
}

bool URaycastInteractorComponent::TryBeginInteraction()
{
	if (CurrentInteractable != nullptr && CurrentInteractable->CanInteract())
	{
		CurrentInteractable->ReceiveOnBeginInteraction(GetOwner());
		bIsInteracting = true;
		return true;
	}

	return false;
}

bool URaycastInteractorComponent::TryEndInteraction()
{
	if (CurrentInteractable != nullptr && CurrentInteractable->CanInteract())
	{
		CurrentInteractable->ReceiveOnEndInteraction(GetOwner());
		bIsInteracting = false;
		return true;
	}

	return false;
}

void URaycastInteractorComponent::SetRaycastCamera(UCameraComponent* InCamera)
{
	this->Camera = InCamera;
	PrimaryComponentTick.bCanEverTick = this->Camera != nullptr;
}
