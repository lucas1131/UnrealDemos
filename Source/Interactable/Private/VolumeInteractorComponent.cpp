#include "VolumeInteractorComponent.h"

#include "InteractableComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

enum EDebugPrintLevel
{
	Info,
	Warning,
	Error
};

void DebugPrint(const UVolumeInteractorComponent* Caller, const EDebugPrintLevel Level, const FString& Message)
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

/* Construction and Initialization */
UVolumeInteractorComponent::UVolumeInteractorComponent(){
	Debug = false;
	ChosenInteractableInRange = nullptr;
	CurrentInteractable = nullptr;
	TriggerVolume = nullptr;
	VolumeSize = FVector(25.0f, 25.0f, 25.0f);
	VolumeRadius = 25.0f;

	PrimaryComponentTick.bCanEverTick = true;
}

UBoxComponent* UVolumeInteractorComponent::InitBox(AActor* Owner)
{
	UBoxComponent* BoxComponent = NewObject<UBoxComponent>(Owner);
	if (BoxComponent)
	{
		Owner->AddInstanceComponent(BoxComponent);
		Owner->AddOwnedComponent(BoxComponent);
		BoxComponent->RegisterComponent();
		BoxComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
		BoxComponent->SetBoxExtent(VolumeSize);
		BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	return BoxComponent;
}

USphereComponent* UVolumeInteractorComponent::InitSphere(AActor* Owner)
{
	USphereComponent* SphereComponent = NewObject<USphereComponent>(Owner);
	if (SphereComponent)
	{
		Owner->AddInstanceComponent(SphereComponent);
		Owner->AddOwnedComponent(SphereComponent);
		SphereComponent->RegisterComponent();
		SphereComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
		SphereComponent->SetSphereRadius(VolumeRadius);
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	return SphereComponent;
}

void UVolumeInteractorComponent::OnRegister() 
{
    Super::OnRegister();

    if (AActor* Owner = GetOwner())
    {
    	// Ensure no duplicate components exist
        if (TriggerVolume != nullptr)
        {
            TriggerVolume->DestroyComponent();
            TriggerVolume = nullptr;
        }
    	
    	switch (DetectionVolumeType)
		{
		case EInteractableDetectionVolume::Cube:
			TriggerVolume = InitBox(Owner);
			break;
		case EInteractableDetectionVolume::Sphere:
			TriggerVolume = InitSphere(Owner);
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("Invalid volume type: %d. Defaulting to Sphere."), DetectionVolumeType)
			DetectionVolumeType = EInteractableDetectionVolume::Sphere;
			TriggerVolume = InitSphere(Owner);
			break;
		}
    }
}

void UVolumeInteractorComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);

	if (TriggerVolume != nullptr)
	{
		// I don't think I should need to do this since the component was registered to the owner if it was created but
		// cursed things are happening without this Destroyed in the actor blueprint viewport, so I'm leaving this here
		TriggerVolume->DestroyComponent();
		TriggerVolume = nullptr;
	}
}
/* End Construction and Initialization */

void UVolumeInteractorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UVolumeInteractorComponent::UpdateInteractableInRange(UInteractableComponent* InteractableComponent)
{
	if (ChosenInteractableInRange == InteractableComponent)
	{
		return;
	}

	if (InteractableComponent == nullptr)
	{
		OnInteractableUpdated.Execute(nullptr);
		ChosenInteractableInRange->DeactivateInteractionOutline();
		ChosenInteractableInRange = nullptr;
		return;
	}

	if (ChosenInteractableInRange != nullptr)
	{
		ChosenInteractableInRange->DeactivateInteractionOutline();
	}

	ChosenInteractableInRange = InteractableComponent;
	ChosenInteractableInRange->ActivateInteractionOutline();
	OnInteractableUpdated.Execute(InteractableComponent);
}

/* IInteractableInterface */
bool UVolumeInteractorComponent::TryBeginInteraction()
{
	if (ChosenInteractableInRange != nullptr)
	{
		ChosenInteractableInRange->BeginInteraction(GetOwner(), this);
		return true;
	}

	return false;
}

bool UVolumeInteractorComponent::TryEndInteraction()
{
	if (CurrentInteractable != nullptr)
	{
		CurrentInteractable->EndInteraction(GetOwner(), this);
		UpdateInteractableInRange(nullptr);
		return true;
	}

	return false;
}

void UVolumeInteractorComponent::AttachInteractable(UInteractableComponent* Interactable)
{
	CurrentInteractable = Interactable;
}

void UVolumeInteractorComponent::SendDataToInteractable(const FInteractionData& Data)
{
	if (IsInteracting())
	{
		CurrentInteractable->ReceiveOnHandleNewData(Data);
	}
}

void UVolumeInteractorComponent::BindOnInteractableUpdatedEvent(const FOnInteractableUpdatedSignature& Callback)
{
	OnInteractableUpdated = Callback;
}
/* End IInteractableInterface */
