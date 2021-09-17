#include "AlsCharacter.h"

#include "DisplayDebugHelpers.h"
#include "DrawDebugHelpers.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Canvas.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Utility/AlsConstants.h"
#include "Utility/AlsEnumerationUtility.h"
#include "Utility/AlsMath.h"
#include "Utility/AlsUtility.h"

void AAlsCharacter::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& Unused, float& VerticalPosition)
{
	const auto Scale{FMath::Min(Canvas->SizeX / (1280.0f * Canvas->GetDPIScale()), Canvas->SizeY / (720.0f * Canvas->GetDPIScale()))};

	const auto RowOffset{12.0f * Scale};
	const auto ColumnOffset{200.0f * Scale};

	auto MaxVerticalPosition{VerticalPosition};
	auto HorizontalPosition{5.0f * Scale};

	static const auto DebugModeHeaderText{FText::AsCultureInvariant(TEXT("Debug mode is enabled! Press (Shift + 0) to disable."))};

	DisplayDebugHeader(Canvas, DebugModeHeaderText, FLinearColor::Green, Scale, HorizontalPosition, VerticalPosition);

	VerticalPosition += RowOffset;
	MaxVerticalPosition = FMath::Max(MaxVerticalPosition, VerticalPosition);

	if (!DebugDisplay.IsDisplayOn(UAlsConstants::CurvesDisplayName()) && !DebugDisplay.IsDisplayOn(UAlsConstants::StateDisplayName()) &&
	    !DebugDisplay.IsDisplayOn(UAlsConstants::ShapesDisplayName()) && !DebugDisplay.IsDisplayOn(UAlsConstants::TracesDisplayName()) &&
	    !DebugDisplay.IsDisplayOn(UAlsConstants::MantlingDisplayName()))
	{
		VerticalPosition = MaxVerticalPosition;

		Super::DisplayDebug(Canvas, DebugDisplay, Unused, VerticalPosition);
		return;
	}

	const auto InitialVerticalPosition{VerticalPosition};

	static const auto CurvesHeaderText{FText::AsCultureInvariant(TEXT("ALS.Curves (Shift + 1)"))};

	if (DebugDisplay.IsDisplayOn(UAlsConstants::CurvesDisplayName()))
	{
		DisplayDebugHeader(Canvas, CurvesHeaderText, FLinearColor::Green, Scale, HorizontalPosition, VerticalPosition);
		DisplayDebugCurves(Canvas, Scale, HorizontalPosition, VerticalPosition);

		MaxVerticalPosition = FMath::Max(MaxVerticalPosition, VerticalPosition + RowOffset);
		VerticalPosition = InitialVerticalPosition;
		HorizontalPosition += ColumnOffset;
	}
	else
	{
		DisplayDebugHeader(Canvas, CurvesHeaderText, {0.0f, 0.333333f, 0.0f}, Scale, HorizontalPosition, VerticalPosition);

		VerticalPosition += RowOffset;
	}

	MaxVerticalPosition = FMath::Max(MaxVerticalPosition, VerticalPosition);

	static const auto StateHeaderText{FText::AsCultureInvariant(TEXT("ALS.State (Shift + 2)"))};

	if (DebugDisplay.IsDisplayOn(UAlsConstants::StateDisplayName()))
	{
		DisplayDebugHeader(Canvas, StateHeaderText, FLinearColor::Green, Scale, HorizontalPosition, VerticalPosition);
		DisplayDebugState(Canvas, Scale, HorizontalPosition, VerticalPosition);
	}
	else
	{
		DisplayDebugHeader(Canvas, StateHeaderText, {0.0f, 0.333333f, 0.0f}, Scale, HorizontalPosition, VerticalPosition);
	}

	VerticalPosition += RowOffset;
	MaxVerticalPosition = FMath::Max(MaxVerticalPosition, VerticalPosition);

	static const auto ShapesHeaderText{FText::AsCultureInvariant(TEXT("ALS.Shapes (Shift + 3)"))};

	if (DebugDisplay.IsDisplayOn(UAlsConstants::ShapesDisplayName()))
	{
		DisplayDebugHeader(Canvas, ShapesHeaderText, FLinearColor::Green, Scale, HorizontalPosition, VerticalPosition);
		DisplayDebugShapes(Canvas, Scale, HorizontalPosition, VerticalPosition);
	}
	else
	{
		DisplayDebugHeader(Canvas, ShapesHeaderText, {0.0f, 0.333333f, 0.0f}, Scale, HorizontalPosition, VerticalPosition);
	}

	VerticalPosition += RowOffset;
	MaxVerticalPosition = FMath::Max(MaxVerticalPosition, VerticalPosition);

	static const auto TracesHeaderText{FText::AsCultureInvariant(TEXT("ALS.Traces (Shift + 4)"))};

	if (DebugDisplay.IsDisplayOn(UAlsConstants::TracesDisplayName()))
	{
		DisplayDebugHeader(Canvas, TracesHeaderText, FLinearColor::Green, Scale, HorizontalPosition, VerticalPosition);
		DisplayDebugTraces(Canvas, Scale, HorizontalPosition, VerticalPosition);
	}
	else
	{
		DisplayDebugHeader(Canvas, TracesHeaderText, {0.0f, 0.333333f, 0.0f}, Scale, HorizontalPosition, VerticalPosition);
	}

	VerticalPosition += RowOffset;
	MaxVerticalPosition = FMath::Max(MaxVerticalPosition, VerticalPosition);

	static const auto MantlingHeaderText{FText::AsCultureInvariant(TEXT("ALS.Mantling (Shift + 5)"))};

	if (DebugDisplay.IsDisplayOn(UAlsConstants::MantlingDisplayName()))
	{
		DisplayDebugHeader(Canvas, MantlingHeaderText, FLinearColor::Green, Scale, HorizontalPosition, VerticalPosition);
		DisplayDebugMantling(Canvas, Scale, HorizontalPosition, VerticalPosition);
	}
	else
	{
		DisplayDebugHeader(Canvas, MantlingHeaderText, {0.0f, 0.333333f, 0.0f}, Scale, HorizontalPosition, VerticalPosition);
	}

	VerticalPosition += RowOffset;
	MaxVerticalPosition = FMath::Max(MaxVerticalPosition, VerticalPosition);

	VerticalPosition = MaxVerticalPosition;

	Super::DisplayDebug(Canvas, DebugDisplay, Unused, VerticalPosition);
}

void AAlsCharacter::DisplayDebugHeader(const UCanvas* Canvas, const FText& HeaderText, const FLinearColor& HeaderColor,
                                       const float Scale, const float HorizontalPosition, float& VerticalPosition)
{
	FCanvasTextItem Text{
		{HorizontalPosition, VerticalPosition},
		HeaderText,
		GEngine->GetMediumFont(),
		HeaderColor
	};

	Text.Scale = {Scale, Scale};
	Text.EnableShadow(FLinearColor::Black);

	Text.Draw(Canvas->Canvas);

	VerticalPosition += 15.0f * Scale;
}

void AAlsCharacter::DisplayDebugCurves(const UCanvas* Canvas, const float Scale,
                                       const float HorizontalPosition, float& VerticalPosition) const
{
	VerticalPosition += 4.0f * Scale;

	FCanvasTextItem Text{
		FVector2D::ZeroVector,
		FText::GetEmpty(),
		GEngine->GetMediumFont(),
		FLinearColor::White
	};

	Text.Scale = {Scale * 0.75f, Scale * 0.75f};
	Text.EnableShadow(FLinearColor::Black);

	const auto RowOffset{12.0f * Scale};
	const auto ColumnOffset{145.0f * Scale};

	static TArray<FName> CurveNames;
	check(CurveNames.Num() <= 0)

	GetMesh()->GetAnimInstance()->GetAllCurveNames(CurveNames);

	CurveNames.Sort([](const FName& A, const FName& B) { return A.LexicalLess(B); });

	for (const auto& CurveName : CurveNames)
	{
		const auto CurveValue{GetMesh()->GetAnimInstance()->GetCurveValue(CurveName)};

		Text.SetColor(FMath::Lerp(FLinearColor::Gray, FLinearColor::White, UAlsMath::Clamp01(CurveValue)));

		Text.Text = FText::AsCultureInvariant(FName::NameToDisplayString(CurveName.ToString(), false));
		Text.Draw(Canvas->Canvas, {HorizontalPosition, VerticalPosition});

		Text.Text = FText::AsCultureInvariant(FString::Printf(TEXT("%.2f"), CurveValue));
		Text.Draw(Canvas->Canvas, {HorizontalPosition + ColumnOffset, VerticalPosition});

		VerticalPosition += RowOffset;
	}

	CurveNames.Reset();
}

void AAlsCharacter::DisplayDebugState(const UCanvas* Canvas, const float Scale,
                                      const float HorizontalPosition, float& VerticalPosition) const
{
	VerticalPosition += 4.0f * Scale;

	FCanvasTextItem Text{
		FVector2D::ZeroVector,
		FText::GetEmpty(),
		GEngine->GetMediumFont(),
		FLinearColor::White
	};

	Text.Scale = {Scale * 0.75f, Scale * 0.75f};
	Text.EnableShadow(FLinearColor::Black);

	const auto RowOffset{12.0f * Scale};
	const auto ColumnOffset{120.0f * Scale};

	static const auto DesiredStanceText{
		FText::AsCultureInvariant(FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(ThisClass, DesiredStance), false))
	};

	Text.Text = DesiredStanceText;
	Text.Draw(Canvas->Canvas, {HorizontalPosition, VerticalPosition});

	Text.Text = FText::AsCultureInvariant(FName::NameToDisplayString(GetEnumValueString(DesiredStance), false));
	Text.Draw(Canvas->Canvas, {HorizontalPosition + ColumnOffset, VerticalPosition});

	VerticalPosition += RowOffset;

	static const auto StanceText{
		FText::AsCultureInvariant(FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(ThisClass, Stance), false))
	};

	Text.Text = StanceText;
	Text.Draw(Canvas->Canvas, {HorizontalPosition, VerticalPosition});

	Text.Text = FText::AsCultureInvariant(FName::NameToDisplayString(GetEnumValueString(Stance), false));
	Text.Draw(Canvas->Canvas, {HorizontalPosition + ColumnOffset, VerticalPosition});

	VerticalPosition += RowOffset;

	static const auto DesiredGaitText{
		FText::AsCultureInvariant(FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(ThisClass, DesiredGait), false))
	};

	Text.Text = DesiredGaitText;
	Text.Draw(Canvas->Canvas, {HorizontalPosition, VerticalPosition});

	Text.Text = FText::AsCultureInvariant(FName::NameToDisplayString(GetEnumValueString(DesiredGait), false));
	Text.Draw(Canvas->Canvas, {HorizontalPosition + ColumnOffset, VerticalPosition});

	VerticalPosition += RowOffset;

	static const auto GaitText{
		FText::AsCultureInvariant(FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(ThisClass, Gait), false))
	};

	Text.Text = GaitText;
	Text.Draw(Canvas->Canvas, {HorizontalPosition, VerticalPosition});

	Text.Text = FText::AsCultureInvariant(FName::NameToDisplayString(GetEnumValueString(Gait), false));
	Text.Draw(Canvas->Canvas, {HorizontalPosition + ColumnOffset, VerticalPosition});

	VerticalPosition += RowOffset;

	static const auto DesiredAimingText{
		FText::AsCultureInvariant(FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(ThisClass, bDesiredAiming), true))
	};

	Text.Text = DesiredAimingText;
	Text.Draw(Canvas->Canvas, {HorizontalPosition, VerticalPosition});

	Text.Text = FText::AsCultureInvariant(FName::NameToDisplayString(ToString(bDesiredAiming), false));
	Text.Draw(Canvas->Canvas, {HorizontalPosition + ColumnOffset, VerticalPosition});

	VerticalPosition += RowOffset;

	static const auto DesiredRotationModeText{
		FText::AsCultureInvariant(FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(ThisClass, DesiredRotationMode), false))
	};

	Text.Text = DesiredRotationModeText;
	Text.Draw(Canvas->Canvas, {HorizontalPosition, VerticalPosition});

	Text.Text = FText::AsCultureInvariant(FName::NameToDisplayString(GetEnumValueString(DesiredRotationMode), false));
	Text.Draw(Canvas->Canvas, {HorizontalPosition + ColumnOffset, VerticalPosition});

	VerticalPosition += RowOffset;

	static const auto RotationModeText{
		FText::AsCultureInvariant(FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(ThisClass, RotationMode), false))
	};

	Text.Text = RotationModeText;
	Text.Draw(Canvas->Canvas, {HorizontalPosition, VerticalPosition});

	Text.Text = FText::AsCultureInvariant(FName::NameToDisplayString(GetEnumValueString(RotationMode), false));
	Text.Draw(Canvas->Canvas, {HorizontalPosition + ColumnOffset, VerticalPosition});

	VerticalPosition += RowOffset;

	static const auto ViewModeText{
		FText::AsCultureInvariant(FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(ThisClass, ViewMode), false))
	};

	Text.Text = ViewModeText;
	Text.Draw(Canvas->Canvas, {HorizontalPosition, VerticalPosition});

	Text.Text = FText::AsCultureInvariant(FName::NameToDisplayString(GetEnumValueString(ViewMode), false));
	Text.Draw(Canvas->Canvas, {HorizontalPosition + ColumnOffset, VerticalPosition});

	VerticalPosition += RowOffset;

	static const auto OverlayModeText{
		FText::AsCultureInvariant(FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(ThisClass, OverlayMode), false))
	};

	Text.Text = OverlayModeText;
	Text.Draw(Canvas->Canvas, {HorizontalPosition, VerticalPosition});

	Text.Text = FText::AsCultureInvariant(FName::NameToDisplayString(UAlsUtility::GetSimpleTagName(OverlayMode).ToString(), false));
	Text.Draw(Canvas->Canvas, {HorizontalPosition + ColumnOffset, VerticalPosition});

	VerticalPosition += RowOffset;

	static const auto LocomotionModeText{
		FText::AsCultureInvariant(FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(ThisClass, LocomotionMode), false))
	};

	Text.Text = LocomotionModeText;
	Text.Draw(Canvas->Canvas, {HorizontalPosition, VerticalPosition});

	Text.Text = FText::AsCultureInvariant(FName::NameToDisplayString(GetEnumValueString(LocomotionMode), false));
	Text.Draw(Canvas->Canvas, {HorizontalPosition + ColumnOffset, VerticalPosition});

	VerticalPosition += RowOffset;

	static const auto LocomotionActionText{
		FText::AsCultureInvariant(FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(ThisClass, LocomotionAction), false))
	};

	Text.Text = LocomotionActionText;
	Text.Draw(Canvas->Canvas, {HorizontalPosition, VerticalPosition});

	Text.Text = FText::AsCultureInvariant(FName::NameToDisplayString(GetEnumValueString(LocomotionAction), false));
	Text.Draw(Canvas->Canvas, {HorizontalPosition + ColumnOffset, VerticalPosition});

	VerticalPosition += RowOffset;
}

void AAlsCharacter::DisplayDebugShapes(const UCanvas* Canvas, const float Scale,
                                       const float HorizontalPosition, float& VerticalPosition) const
{
	VerticalPosition += 4.0f * Scale;

	FCanvasTextItem Text{
		FVector2D::ZeroVector,
		FText::GetEmpty(),
		GEngine->GetMediumFont(),
		FLinearColor::White
	};

	Text.Scale = {Scale * 0.75f, Scale * 0.75f};
	Text.EnableShadow(FLinearColor::Black);

	const auto RowOffset{12.0f * Scale};
	const auto ColumnOffset{120.0f * Scale};

	static const auto ViewRotationText{
		FText::AsCultureInvariant(FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(ThisClass, ViewRotation), false))
	};

	auto Color{FLinearColor::Red};
	Text.SetColor(Color);

	Text.Text = ViewRotationText;
	Text.Draw(Canvas->Canvas, {HorizontalPosition, VerticalPosition});

	Text.Text = FText::AsCultureInvariant(FString::Printf(TEXT("R: %.2f P: %.2f Y: %.2f"),
	                                                      ViewRotation.Roll, ViewRotation.Pitch, ViewRotation.Yaw));
	Text.Draw(Canvas->Canvas, {HorizontalPosition + ColumnOffset, VerticalPosition});

#if ENABLE_DRAW_DEBUG
	DrawDebugCone(GetWorld(), GetPawnViewLocation(),
	              ViewRotation.Vector(), 100.0f, FMath::DegreesToRadians(15.0f), FMath::DegreesToRadians(15.0f),
	              8, Color.ToFColor(true), false, -1.0f, SDPG_World, 1.0f);
#endif

	VerticalPosition += RowOffset;

	static const auto InputYawAngleText{
		FText::AsCultureInvariant(
			FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(FAlsLocomotionCharacterState, InputYawAngle), false))
	};

	Color = LocomotionState.bHasInput ? FLinearColor{1.0f, 0.5f, 0.0f} : FLinearColor{0.5f, 0.25f, 0.0f};
	Text.SetColor(Color);

	Text.Text = InputYawAngleText;
	Text.Draw(Canvas->Canvas, {HorizontalPosition, VerticalPosition});

	Text.Text = FText::AsCultureInvariant(FString::Printf(TEXT("%.2f"), LocomotionState.InputYawAngle));
	Text.Draw(Canvas->Canvas, {HorizontalPosition + ColumnOffset, VerticalPosition});

#if ENABLE_DRAW_DEBUG
	const auto FeetLocation{LocomotionState.Location - FVector(0.0f, 0.0f, GetCapsuleComponent()->Bounds.BoxExtent.Z)};

	DrawDebugDirectionalArrow(GetWorld(),
	                          FeetLocation + FVector{0.0f, 0.0f, 3.0f},
	                          FeetLocation + FVector{0.0f, 0.0f, 3.0f} +
	                          UAlsMath::AngleToDirection2D(LocomotionState.InputYawAngle) * 50.0f,
	                          50.0f, Color.ToFColor(true), false, -1.0f, SDPG_World, 3.0f);
#endif

	VerticalPosition += RowOffset;

	static const auto SpeedText{
		FText::AsCultureInvariant(FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(FAlsLocomotionCharacterState, Speed), false))
	};

	Color = LocomotionState.bHasSpeed ? FLinearColor{0.75f, 0.0f, 1.0f} : FLinearColor{0.375f, 0.0f, 0.5f};
	Text.SetColor(Color);

	Text.Text = SpeedText;
	Text.Draw(Canvas->Canvas, {HorizontalPosition, VerticalPosition});

	Text.Text = FText::AsCultureInvariant(FString::Printf(TEXT("%.2f"), LocomotionState.Speed));
	Text.Draw(Canvas->Canvas, {HorizontalPosition + ColumnOffset, VerticalPosition});

	VerticalPosition += RowOffset;

	static const auto VelocityDirectionText{FText::AsCultureInvariant(TEXT("Velocity Direction"))};

	const auto VelocityDirection{LocomotionState.Velocity.GetSafeNormal()};

	Text.Text = VelocityDirectionText;
	Text.Draw(Canvas->Canvas, {HorizontalPosition, VerticalPosition});

	Text.Text = FText::AsCultureInvariant(FString::Printf(TEXT("X: %.2f Y: %.2f Z: %.2f"),
	                                                      VelocityDirection.X, VelocityDirection.Y, VelocityDirection.Z));
	Text.Draw(Canvas->Canvas, {HorizontalPosition + ColumnOffset, VerticalPosition});

	VerticalPosition += RowOffset;

	static const auto VelocityYawAngleText{
		FText::AsCultureInvariant(
			FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(FAlsLocomotionCharacterState, VelocityYawAngle), false))
	};

	Text.Text = VelocityYawAngleText;
	Text.Draw(Canvas->Canvas, {HorizontalPosition, VerticalPosition});

	Text.Text = FText::AsCultureInvariant(FString::Printf(TEXT("%.2f"), LocomotionState.VelocityYawAngle));
	Text.Draw(Canvas->Canvas, {HorizontalPosition + ColumnOffset, VerticalPosition});

#if ENABLE_DRAW_DEBUG
	DrawDebugDirectionalArrow(GetWorld(),
	                          FeetLocation,
	                          FeetLocation +
	                          UAlsMath::AngleToDirection2D(LocomotionState.VelocityYawAngle) *
	                          FMath::GetMappedRangeValueClamped({0.0f, GetCharacterMovement()->GetMaxSpeed()},
	                                                            {50.0f, 75.0f}, LocomotionState.Speed),
	                          50.0f, Color.ToFColor(true), false, -1.0f, SDPG_World, 3.0f);
#endif

	VerticalPosition += RowOffset;

	static const auto TargetActorRotationText{
		FText::AsCultureInvariant(
			FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(FAlsLocomotionCharacterState, SmoothTargetYawAngle), false))
	};

	Color = {0.0f, 0.75f, 1.0f};
	Text.SetColor(Color);

	Text.Text = TargetActorRotationText;
	Text.Draw(Canvas->Canvas, {HorizontalPosition, VerticalPosition});

	Text.Text = FText::AsCultureInvariant(FString::Printf(TEXT("%.2f"), LocomotionState.SmoothTargetYawAngle));
	Text.Draw(Canvas->Canvas, {HorizontalPosition + ColumnOffset, VerticalPosition});

#if ENABLE_DRAW_DEBUG
	DrawDebugDirectionalArrow(GetWorld(),
	                          FeetLocation + FVector{0.0f, 0.0f, 6.0f},
	                          FeetLocation + FVector{0.0f, 0.0f, 6.0f} +
	                          UAlsMath::AngleToDirection2D(LocomotionState.SmoothTargetYawAngle) * 50.0f,
	                          50.0f, Color.ToFColor(true), false, -1.0f, SDPG_World, 3.0f);
#endif

	VerticalPosition += RowOffset;

#if ENABLE_DRAW_DEBUG
	DrawDebugCapsule(GetWorld(), LocomotionState.Location, GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),
	                 GetCapsuleComponent()->GetScaledCapsuleRadius(), GetCapsuleComponent()->GetComponentQuat(),
	                 FColor::Green, false, -1.0f, SDPG_World, 1.0f);
#endif
}

void AAlsCharacter::DisplayDebugTraces(const UCanvas* Canvas, const float Scale,
                                       const float HorizontalPosition, float& VerticalPosition) const
{
	VerticalPosition += 4.0f * Scale;

	FCanvasTextItem Text{
		FVector2D::ZeroVector,
		FText::GetEmpty(),
		GEngine->GetMediumFont(),
		FLinearColor::White
	};

	Text.Scale = {Scale * 0.75f, Scale * 0.75f};
	Text.EnableShadow(FLinearColor::Black);

	const auto RowOffset{12.0f * Scale};

	static const auto FootOffsetTraceText{FText::AsCultureInvariant(TEXT("Foot Offset"))};

	Text.SetColor({0.0f, 0.75f, 1.0f});

	Text.Text = FootOffsetTraceText;
	Text.Draw(Canvas->Canvas, {HorizontalPosition, VerticalPosition});

	VerticalPosition += RowOffset;

	static const auto GroundPredictionTraceText{FText::AsCultureInvariant(TEXT("Ground Prediction"))};

	Text.SetColor({0.75f, 0.0f, 1.0f});

	Text.Text = GroundPredictionTraceText;
	Text.Draw(Canvas->Canvas, {HorizontalPosition, VerticalPosition});

	VerticalPosition += RowOffset;

	static const auto FootstepEffectsTraceText{FText::AsCultureInvariant(TEXT("Footstep Effects"))};

	Text.SetColor(FLinearColor::Red);

	Text.Text = FootstepEffectsTraceText;
	Text.Draw(Canvas->Canvas, {HorizontalPosition, VerticalPosition});

	VerticalPosition += RowOffset;
}

void AAlsCharacter::DisplayDebugMantling(const UCanvas* Canvas, const float Scale,
                                         const float HorizontalPosition, float& VerticalPosition) const
{
	VerticalPosition += 4.0f * Scale;

	FCanvasTextItem Text{
		FVector2D::ZeroVector,
		FText::GetEmpty(),
		GEngine->GetMediumFont(),
		FLinearColor::White
	};

	Text.Scale = {Scale * 0.75f, Scale * 0.75f};
	Text.EnableShadow(FLinearColor::Black);

	const auto RowOffset{12.0f * Scale};

	static const auto ForwardTraceText{FText::AsCultureInvariant(TEXT("Forward Trace"))};

	Text.SetColor({0.0f, 0.75f, 1.0f});

	Text.Text = ForwardTraceText;
	Text.Draw(Canvas->Canvas, {HorizontalPosition, VerticalPosition});

	VerticalPosition += RowOffset;

	static const auto DownwardTraceText{FText::AsCultureInvariant(TEXT("Downward Trace"))};

	Text.SetColor({0.75f, 0.0f, 1.0f});

	Text.Text = DownwardTraceText;
	Text.Draw(Canvas->Canvas, {HorizontalPosition, VerticalPosition});

	VerticalPosition += RowOffset;

	static const auto FreeSpaceOverlapText{FText::AsCultureInvariant(TEXT("Free Space Overlap (Only if Failed)"))};

	Text.SetColor(FLinearColor::Red);

	Text.Text = FreeSpaceOverlapText;
	Text.Draw(Canvas->Canvas, {HorizontalPosition, VerticalPosition});

	VerticalPosition += RowOffset;
}
