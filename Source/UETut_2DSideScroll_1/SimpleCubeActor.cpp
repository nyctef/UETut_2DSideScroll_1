// Copyright Sigurdur Gunnarsson. All Rights Reserved. 
// Licensed under the MIT License.
// Example cube mesh

//#include "ProceduralMeshesPrivatePCH.h"
#include "SimpleCubeActor.h"

ASimpleCubeActor::ASimpleCubeActor()
{
	RootNode = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = RootNode;

	MeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	//MeshComponent->bShouldSerializeMeshData = false;
	MeshComponent->SetupAttachment(RootComponent);
}

// This is called when actor is spawned (at runtime or when you drop it into the world in editor)
void ASimpleCubeActor::PostActorCreated()
{
	Super::PostActorCreated();
	GenerateMesh();
}

// This is called when actor is already in level and map is opened
void ASimpleCubeActor::PostLoad()
{
	Super::PostLoad();
	GenerateMesh();
}

void ASimpleCubeActor::SetupMeshBuffers(FVector Size)
{
	int32 SideCount = (int32)Size.X * (int32)Size.Y;
	int32 VertexCount = SideCount * 4; // 4 verts each
	Vertices.AddUninitialized(VertexCount);
	Triangles.AddUninitialized(SideCount * 2 * 3); // 2x triangles per side, 3 verts each
}

void ASimpleCubeActor::GenerateMesh()
{
	// The number of vertices or polygons wont change at runtime, so we'll just allocate the arrays once
	if (!bHaveBuffersBeenInitialized)
	{
		SetupMeshBuffers(Size);
		bHaveBuffersBeenInitialized = true;
	}

	FBox BoundingBox = FBox(-Size / 2.0f, Size / 2.0f);
	GenerateCube(Vertices, Triangles, Size);

	MeshComponent->ClearAllMeshSections();
	MeshComponent->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), true);
	MeshComponent->SetMaterial(0, Material);
}

void ASimpleCubeActor::GenerateCube(TArray<FVector>& InVertices, TArray<int32>& InTriangles, FVector InSize)
{
	// NOTE: Unreal uses an upper-left origin UV
	// NOTE: This sample uses a simple UV mapping scheme where each face is the same
	// NOTE: For a normal facing towards us, be build the polygon CCW in the order 0-1-2 then 0-2-3 to complete the quad.
	//
	//  +Z 
	//   |
	//   o-- +X
	//  /
	// +Y

	// Calculate a half offset so we get correct center of object
	float OffsetX = InSize.X / 2.0f;
	float OffsetY = 0.0f;//InSize.Y / 2.0f;
	float OffsetZ = InSize.Z / 2.0f;

	// Define the 8 corners of the cube

	// Now we create 6x faces, 4 vertices each
	int32 VertexOffset = 0;
	int32 TriangleOffset = 0;
	FVector Normal = FVector::ZeroVector;
	FVector Tangent = FVector::ZeroVector;


	Normal = FVector(0, 1, 0);
	Tangent = FVector(-1, 0, 0);

	for (int32 x = -OffsetX; x < OffsetX; x++) {
		for (int32 z = -OffsetZ; z < OffsetZ; z++) {

	FVector p0 = FVector(x+1, 0, z);
	FVector p3 = FVector(x+1, 0, z+1);
	FVector p4 = FVector(x, 0, z);
	FVector p7 = FVector(x, 0, z+1);

			BuildQuad(InVertices, InTriangles, p4, p0, p3, p7, VertexOffset, TriangleOffset, Normal, Tangent);
		}
	}
	



}

void ASimpleCubeActor::BuildQuad(TArray<FVector>& InVertices, TArray<int32>& InTriangles, FVector BottomLeft, FVector BottomRight, FVector TopRight, FVector TopLeft, int32& VertexOffset, int32& TriangleOffset, FVector Normal, FVector Tangent)
{
	int32 Index1 = VertexOffset++;
	int32 Index2 = VertexOffset++;
	int32 Index3 = VertexOffset++;
	int32 Index4 = VertexOffset++;
	InVertices[Index1] = BottomLeft;
	InVertices[Index2] = BottomRight;
	InVertices[Index3] = TopRight;
	InVertices[Index4] = TopLeft;
	//InVertices[Index1].UV0 = FVector2D(0.0f, 1.0f);
	//InVertices[Index2].UV0 = FVector2D(1.0f, 1.0f);
	//InVertices[Index3].UV0 = FVector2D(1.0f, 0.0f);
	//InVertices[Index4].UV0 = FVector2D(0.0f, 0.0f);
	InTriangles[TriangleOffset++] = Index1;
	InTriangles[TriangleOffset++] = Index2;
	InTriangles[TriangleOffset++] = Index3;
	InTriangles[TriangleOffset++] = Index1;
	InTriangles[TriangleOffset++] = Index3;
	InTriangles[TriangleOffset++] = Index4;
	// On a cube side, all the vertex normals face the same way
	//InVertices[Index1].Normal = InVertices[Index2].Normal = InVertices[Index3].Normal = InVertices[Index4].Normal = Normal;
	//InVertices[Index1].Tangent = InVertices[Index2].Tangent = InVertices[Index3].Tangent = InVertices[Index4].Tangent = Tangent;
}

#if WITH_EDITOR
void ASimpleCubeActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName MemberPropertyChanged = (PropertyChangedEvent.MemberProperty ? PropertyChangedEvent.MemberProperty->GetFName() : NAME_None);

	if ((MemberPropertyChanged == GET_MEMBER_NAME_CHECKED(ASimpleCubeActor, Size)))
	{
		GenerateMesh();
	}
	else if ((MemberPropertyChanged == GET_MEMBER_NAME_CHECKED(ASimpleCubeActor, Material)))
	{
		MeshComponent->SetMaterial(0, Material);
	}
}
#endif // WITH_EDITOR