// Copyright sumikiru. All Rights Reserved,


#include "BlitzPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Blitz/BlitzGameplayTags.h"
#include "Blitz/AbilitySystem/BlitzAbilitySystemComponent.h"
#include "Blitz/Input/BlitzInputComponent.h"
#include "Blitz/Player/BlitzPlayerController.h"
#include "Blitz/Player/BlitzPlayerState.h"
#include "Blitz/UI/View/RootEnter.h"
#include "Blueprint/UserWidget.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BlitzPlayerCharacter)

ABlitzPlayerCharacter::ABlitzPlayerCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bInheritRoll = false;

	ViewCam = CreateDefaultSubobject<UCameraComponent>(TEXT("View Camera"));
	ViewCam->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// 不需要同步，仅用于本地
	BlitzInputComponent = CreateDefaultSubobject<UBlitzInputComponent>(TEXT("Input Component"));
}

ABlitzPlayerController* ABlitzPlayerCharacter::GetBlitzPlayerController() const
{
	return CastChecked<ABlitzPlayerController>(Controller, ECastCheckedType::NullAllowed);
}

ABlitzPlayerState* ABlitzPlayerCharacter::GetBlitzPlayerState() const
{
	return CastChecked<ABlitzPlayerState>(GetPlayerState(), ECastCheckedType::NullAllowed);
}

void ABlitzPlayerCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();

	if (const UBlitzPawnData* LoadedData = DefaultPawnData.LoadSynchronous())
	{
		if (const UBlitzInputConfig* InputConfig = LoadedData->InputConfig)
		{
			if (const APlayerController* OwningPlayerController = GetController<APlayerController>())
			{
				if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = OwningPlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
				{
					/*InputSubsystem->RemoveMappingContext(GameplayInputMappingContext);
					InputSubsystem->AddMappingContext(GameplayInputMappingContext, 0);*/
					BlitzInputComponent->RemoveInputMappingContexts(InputConfig, InputSubsystem);
					BlitzInputComponent->AddInputMappingContexts(InputConfig, InputSubsystem);
				}
			}
		}
	}
}

void ABlitzPlayerCharacter::ServerInit()
{
	// 注意要先设置好ASC和AS。对于Enemy直接在BeginPlay中初始化ASC即可
	InitBlitzAbilityActorInfo();
	GrantPawnData();
	ConfigureOverheadStatsWidget(); // 同样需要用于Listen Server/Standalone
	SpawnRootEnterWidget(); // 同样需要用于Listen Server/Standalone
}

void ABlitzPlayerCharacter::ClientInit()
{
	// 注意要先设置好ASC和AS。对于Enemy直接在BeginPlay中初始化ASC即可
	InitBlitzAbilityActorInfo();
	ConfigureOverheadStatsWidget();
	SpawnRootEnterWidget();
}

void ABlitzPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 注意此时不是UBlitzInputComponent* BlitzIC = CastChecked<UBlitzInputComponent>(PlayerInputComponent);
	// 一是有自己的BlitzInputComponent负责输入，二是这个变量BlitzIC会导致access error
	if (const UBlitzPawnData* LoadedData = DefaultPawnData.LoadSynchronous())
	{
		if (const UBlitzInputConfig* InputConfig = LoadedData->InputConfig)
		{
			BlitzInputComponent->BindNativeAction(InputConfig, BlitzGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::HandleLookInput, true);
			BlitzInputComponent->BindNativeAction(InputConfig, BlitzGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::HandleMoveInput, true);
			BlitzInputComponent->BindNativeAction(InputConfig, BlitzGameplayTags::InputTag_Jump, ETriggerEvent::Triggered, this, &ThisClass::Jump, true);
			BlitzInputComponent->BindNativeAction(InputConfig, BlitzGameplayTags::InputTag_Aim, ETriggerEvent::Triggered, this, &ThisClass::HandleAimInput, true);

			// @note: 输入绑定流程详见https://zhuanlan.zhihu.com/p/611735265
			// 以及https://zhuanlan.zhihu.com/p/690425805
			TArray<uint32> BindHandles;
			BlitzInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, BindHandles);
		}
	}
}

void ABlitzPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 注意要先设置好ASC和AS。对于Enemy直接在Begin中初始化ASC即可
	ServerInit();
}

void ABlitzPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ClientInit();
}

void ABlitzPlayerCharacter::InitBlitzAbilityActorInfo()
{
	ABlitzPlayerState* HeroPS = GetPlayerState<ABlitzPlayerState>();
	checkf(HeroPS, TEXT("Player State is not found!"));

	// 为Pawn中的ASC和AC赋值
	BlitzAbilitySystemComponent = HeroPS->GetBlitzAbilitySystemComponent();
	BlitzAttributeSet = HeroPS->GetBlitzAttributeSet();

	// 初始化ASC的OwnerActor和AvatarActor
	BlitzAbilitySystemComponent->InitAbilityActorInfo(HeroPS, this);
}

void ABlitzPlayerCharacter::HandleLookInput(const FInputActionValue& InputActionValue)
{
	const FVector2D InputVal = InputActionValue.Get<FVector2D>();

	AddControllerPitchInput(-InputVal.Y);
	AddControllerYawInput(InputVal.X);
}

void ABlitzPlayerCharacter::HandleMoveInput(const FInputActionValue& InputActionValue)
{
	FVector2D InputVal = InputActionValue.Get<FVector2D>();
	// 注意，同时按下w和d时对角向量长度为根号2，比单位向量要大，这会让实际移动变快
	InputVal.Normalize();

	AddMovementInput(GetMoveForwardDirection() * InputVal.Y + GetLookRightDirection() * InputVal.X);
}

void ABlitzPlayerCharacter::HandleAimInput(const FInputActionValue& InputActionValue)
{
	// 涉及Timeline，图方便直接写在BP里
	BP_HandleAimZooming();
}

FVector ABlitzPlayerCharacter::GetLookRightDirection() const
{
	return ViewCam->GetRightVector();
}

FVector ABlitzPlayerCharacter::GetLookForwardDirection() const
{
	return ViewCam->GetForwardVector();
}

FVector ABlitzPlayerCharacter::GetMoveForwardDirection() const
{
	return FVector::CrossProduct(GetLookRightDirection(), FVector::UpVector);
}

void ABlitzPlayerCharacter::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	checkf(BlitzAbilitySystemComponent, TEXT("BlitzAbilitySystemComponent has not initialized already."));

	BlitzAbilitySystemComponent->AbilityInputTagPressed(InputTag);
}

void ABlitzPlayerCharacter::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	checkf(BlitzAbilitySystemComponent, TEXT("BlitzAbilitySystemComponent has not initialized already."));

	BlitzAbilitySystemComponent->AbilityInputTagReleased(InputTag);
}

void ABlitzPlayerCharacter::SpawnRootEnterWidget() const
{
	if (!IsLocallyControlled())
	{
		return;
	}

	// 注意CreateWidget的第一个参数OwningObject*应该是PlayerController，而不是PlayerCharacter
	if (URootEnter* RootEnterWidget = CreateWidget<URootEnter>(GetBlitzPlayerController(), RootEnterWidgetClass))
	{
		RootEnterWidget->AddToViewport();
	}
}
