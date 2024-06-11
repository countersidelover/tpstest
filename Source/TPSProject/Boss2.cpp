#include "Boss2.h"
#include "Boss2FSM.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"  // 추가된 부분
#include "UObject/ConstructorHelpers.h"

// Sets default values
ABoss2::ABoss2()
{
    PrimaryActorTick.bCanEverTick = true;

    // 1. 스켈레탈메시 데이터 로드
    ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/ParagonSunWukong/Characters/Heroes/Wukong/Meshes/Wukong.Wukong'"));
    if (tempMesh.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(tempMesh.Object);
        GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
        GetMesh()->SetRelativeScale3D(FVector(2.5f));
    }

    // Boss2FSM 컴포넌트 추가
    fsm = CreateDefaultSubobject<UBoss2FSM>(TEXT("FSM"));

    // 애니메이션 블루프린트 할당하기
    ConstructorHelpers::FClassFinder<UAnimInstance> tempClass(TEXT("AnimBlueprint'/Game/ABP_Boss2.ABP_Boss2'")); // 적절한 경로로 변경
    if (tempClass.Succeeded())
    {
        GetMesh()->SetAnimInstanceClass(tempClass.Class);
    }

    // CapsuleComponent 콜리전 설정
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
}

// Called when the game starts or when spawned
void ABoss2::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ABoss2::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABoss2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}