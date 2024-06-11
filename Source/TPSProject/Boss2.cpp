#include "Boss2.h"
#include "Boss2FSM.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"  // �߰��� �κ�
#include "UObject/ConstructorHelpers.h"

// Sets default values
ABoss2::ABoss2()
{
    PrimaryActorTick.bCanEverTick = true;

    // 1. ���̷�Ż�޽� ������ �ε�
    ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/ParagonSunWukong/Characters/Heroes/Wukong/Meshes/Wukong.Wukong'"));
    if (tempMesh.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(tempMesh.Object);
        GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
        GetMesh()->SetRelativeScale3D(FVector(2.5f));
    }

    // Boss2FSM ������Ʈ �߰�
    fsm = CreateDefaultSubobject<UBoss2FSM>(TEXT("FSM"));

    // �ִϸ��̼� �������Ʈ �Ҵ��ϱ�
    ConstructorHelpers::FClassFinder<UAnimInstance> tempClass(TEXT("AnimBlueprint'/Game/ABP_Boss2.ABP_Boss2'")); // ������ ��η� ����
    if (tempClass.Succeeded())
    {
        GetMesh()->SetAnimInstanceClass(tempClass.Class);
    }

    // CapsuleComponent �ݸ��� ����
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