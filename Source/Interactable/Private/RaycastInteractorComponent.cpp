// Fill out your copyright notice in the Description page of Project Settings.

#include "RaycastInteractorComponent.h"
#include "InteractableComponent.h"

enum EDebugPrintLevel
{
	Info,
	Warning,
	Error
};

void DebugPrint(const URaycastInteractorComponent* Caller, const EDebugPrintLevel Level, const FString& Message)
{
#if not UE_BUILD_SHIPPING
	if (Caller->Debug)
	{
		FColor Color;
		switch (Level)
		{
		case Info:
			Color = FColor::Cyan;
			break;
		case Warning:
			Color = FColor::Orange;
			break;
		case Error:
			Color = FColor::Red;
			break;
		}
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, Color, Message);
	}
#endif
}

URaycastInteractorComponent::URaycastInteractorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Camera = nullptr;
	Debug = false;
	InteractableInRange = nullptr;
	CurrentInteractable = nullptr;
}

void URaycastInteractorComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Camera == nullptr)
	{
		SetRaycastCamera(GetOwner()->GetComponentByClass<UCameraComponent>());
		if (Camera == nullptr)
		{
			UE_LOG(LogTemp, Warning,
			       TEXT("No camera found on '%s', raycast interactor will not work until a camera is assigned. "
				       "Trying to use interactor without a camera will cause a crash."),
			       *GetOwner()->GetName());
		}
	}
}

void URaycastInteractorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	try
	{
		DoInteractionTest();
	}
	catch (const std::exception& e)
	{
		UE_LOG(LogTemp, Error, TEXT("Error: %hs"), e.what());
	}
}

void URaycastInteractorComponent::UpdateInteractableInRange(UInteractableComponent* InteractableComponent)
{
	if (InteractableInRange == InteractableComponent)
	{
		return;
	}

	if (InteractableComponent == nullptr)
	{
		OnInteractableUpdated.Execute(nullptr);
		InteractableInRange->DeactivateInteractionOutline();
		InteractableInRange = nullptr;
		return;
	}

	if (InteractableInRange != nullptr)
	{
		InteractableInRange->DeactivateInteractionOutline();
	}

	InteractableInRange = InteractableComponent;
	InteractableInRange->ActivateInteractionOutline();
	OnInteractableUpdated.Execute(InteractableComponent);
}

void URaycastInteractorComponent::SetRaycastCamera(UCameraComponent* InCamera)
{
	this->Camera = InCamera;
	PrimaryComponentTick.bCanEverTick = this->Camera != nullptr;
}

/* IInteractorInterface */
void URaycastInteractorComponent::DoInteractionTest()
{
	if (IsInteracting())
	{
		return;
	}

	FHitResult Result;
	const FVector Start = Camera->GetComponentLocation();
	const FVector End = Start + Camera->GetForwardVector() * InteractionRaycastDistance;

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
		if (InteractableComponent != nullptr && InteractableComponent->CanInteract())
		{
			UpdateInteractableInRange(InteractableComponent);
			DebugPrint(this, Info, FString::Printf(TEXT("Hit actor: %s"), *HitActor->GetName()));
		}
		else
		{
			UpdateInteractableInRange(nullptr);
			DebugPrint(this, Warning,
			           FString::Printf(TEXT("Hit non interactable actor: %s"), *HitActor->GetName()));
		}
	}
	else
	{
		UpdateInteractableInRange(nullptr);
		DebugPrint(this, Error, TEXT("No hits"));
	}
}

bool URaycastInteractorComponent::TryBeginInteraction()
{
	if (InteractableInRange != nullptr)
	{
		InteractableInRange->BeginInteraction(GetOwner(), this);
		return true;
	}

	return false;
}

bool URaycastInteractorComponent::TryEndInteraction()
{
	if (CurrentInteractable != nullptr)
	{
		CurrentInteractable->EndInteraction(GetOwner(), this);
		return true;
	}

	return false;
}

void URaycastInteractorComponent::SetCurrentInteractable(UInteractableComponent* Interactable)
{
	CurrentInteractable = Interactable;
}

void URaycastInteractorComponent::SendDataToInteractable(const FInteractionData& Data)
{
	if (IsInteracting())
	{
		CurrentInteractable->ReceiveOnHandleNewData(Data);
	}
}

void URaycastInteractorComponent::BindOnInteractableUpdatedEvent(const FOnInteractableUpdatedSignature& Callback)
{
	OnInteractableUpdated = Callback;

	GetWorld()->GetFirstPlayerController()->GetControlRotation().Vector().RotateAngleAxis()
}

/* End IInteractorInterface */
