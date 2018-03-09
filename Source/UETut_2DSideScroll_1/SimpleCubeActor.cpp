// Copyright Sigurdur Gunnarsson. All Rights Reserved. 
// Licensed under the MIT License. See LICENSE file in the project root for full license information. 
// Example cube mesh

//#include "ProceduralMeshesPrivatePCH.h"
#include "SimpleCubeActor.h"

ASimpleCubeActor::ASimpleCubeActor()
{
	RootNode = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = RootNode;

	MeshComponent = CreateDefaultSubobject<URuntimeMeshComponent>(TEXT("ProceduralMesh"));
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
	auto sx = (int32)Size.X;
	auto sy = (int32)Size.Y;
	auto sz = (int32)Size.Z;
	//int32 SideCount = (int32)Size.X * (int32)Size.Z;
	//int32 SideCount = sx + 1; // plus 1 for the front quad
	int32 VertexCount = (sx + 1) * (sz + 1);
	MapData.Init(true, sx * sz);
	for (int32 x = 0; x < sx; x++) {
		MapData[x] = false;
		MapData[(sx*sz) - x - 1] = false;
	}
	for (int32 z = 0; z < sz; z++) {
		MapData[z*sx] = false;
		MapData[(z*sx) + (sx - 1)] = false;
	}
	//Vertices.AddUninitialized(VertexCount);
	//Triangles.AddUninitialized(SideCount * 2 * 3); // 2x triangles per side, 3 verts each
}

bool MapDataAt(TBitArray<FDefaultBitArrayAllocator> bitArray, int32 rowSize, int32 x, int32 z) {
	return bitArray[(z*rowSize) + x];
}

void ASimpleCubeActor::GenerateMesh()
{
	SetupMeshBuffers(Size);

	auto sx = (int32)Size.X;
	auto sy = (int32)Size.Y;
	auto sz = (int32)Size.Z;

	Vertices.Reset();
	Triangles.Reset();

	auto Left = -Size.X / 2.0f;
	auto Bottom = -Size.Z / 2.0f;
	auto Right = +Size.X / 2.0f;
	auto Top = +Size.Z / 2.0f;

	// ref: https://en.wikipedia.org/wiki/Marching_squares
	for (int mapX = 0; mapX < sx - 1; mapX++) {
		for (int mapZ = 0; mapZ < sz - 1; mapZ++) {
			uint8 cell = 0;
			if (MapDataAt(MapData, sx, mapX, mapZ)) { cell += 1; }
			if (MapDataAt(MapData, sx, mapX + 1, mapZ)) { cell += 2; }
			if (MapDataAt(MapData, sx, mapX + 1, mapZ + 1)) { cell += 4; }
			if (MapDataAt(MapData, sx, mapX, mapZ + 1)) { cell += 8; }

			auto cellLeftInner   = FVector(Left + mapX - 0.5, 1, Bottom + mapZ);
			auto cellLeftOuter   = FVector(Left + mapX - 0.5, 0, Bottom + mapZ);
			auto cellBottomInner = FVector(Left + mapX,       1, Bottom + mapZ - 0.5);
			auto cellBottomOuter = FVector(Left + mapX,       0, Bottom + mapZ - 0.5);
			auto cellRightInner  = FVector(Left + mapX + 0.5, 1, Bottom + mapZ);
			auto cellRightOuter  = FVector(Left + mapX + 0.5, 0, Bottom + mapZ);
			auto cellTopInner    = FVector(Left + mapX,       1, Bottom + mapZ + 0.5);
			auto cellTopOuter    = FVector(Left + mapX,       0, Bottom + mapZ + 0.5);

			// TODO: make sure normals are correct + check if vertex ordering is important

			switch (cell) {
			case 0b0000:
				break;
			case 0b0001:
				BuildQuad(Vertices, Triangles, cellBottomInner, cellBottomOuter, cellLeftOuter, cellLeftInner, FVector::ZeroVector, FVector::ZeroVector);
				break;
			case 0b0010:
				BuildQuad(Vertices, Triangles, cellRightInner, cellRightOuter, cellBottomInner, cellBottomOuter, FVector::ZeroVector, FVector::ZeroVector);
				break;
			case 0b0011:
				BuildQuad(Vertices, Triangles, cellRightInner, cellRightOuter, cellLeftOuter, cellLeftInner, FVector::ZeroVector, FVector::ZeroVector);
				break;
			case 0b0100:
				BuildQuad(Vertices, Triangles, cellRightInner, cellRightOuter, cellTopInner, cellTopOuter, FVector::ZeroVector, FVector::ZeroVector);
				break;
			case 0b0101:
				BuildQuad(Vertices, Triangles, cellRightInner, cellRightOuter, cellBottomInner, cellBottomOuter, FVector::ZeroVector, FVector::ZeroVector);
				BuildQuad(Vertices, Triangles, cellLeftInner, cellLeftOuter, cellTopOuter, cellTopInner, FVector::ZeroVector, FVector::ZeroVector);
				break;
			case 0b0110:
				BuildQuad(Vertices, Triangles, cellTopInner, cellTopOuter, cellBottomOuter, cellBottomInner, FVector::ZeroVector, FVector::ZeroVector);
				break;
			case 0b0111:
				BuildQuad(Vertices, Triangles, cellLeftInner, cellLeftOuter, cellTopOuter, cellTopInner, FVector::ZeroVector, FVector::ZeroVector);
				break;
			case 0b1000:
				BuildQuad(Vertices, Triangles, cellLeftInner, cellLeftOuter, cellTopOuter, cellTopInner, FVector::ZeroVector, FVector::ZeroVector);
				break;
			case 0b1001:
				BuildQuad(Vertices, Triangles, cellTopInner, cellTopOuter, cellBottomOuter, cellBottomInner, FVector::ZeroVector, FVector::ZeroVector);
				break;
			case 0b1010:
				BuildQuad(Vertices, Triangles, cellBottomInner, cellBottomOuter, cellLeftOuter, cellLeftInner, FVector::ZeroVector, FVector::ZeroVector);
				BuildQuad(Vertices, Triangles, cellRightInner, cellRightOuter, cellTopInner, cellTopOuter, FVector::ZeroVector, FVector::ZeroVector);
				break;
			case 0b1011:
				BuildQuad(Vertices, Triangles, cellRightInner, cellRightOuter, cellTopInner, cellTopOuter, FVector::ZeroVector, FVector::ZeroVector);
				break;
			case 0b1100:
				BuildQuad(Vertices, Triangles, cellRightInner, cellRightOuter, cellLeftOuter, cellLeftInner, FVector::ZeroVector, FVector::ZeroVector);
				break;
			case 0b1101:
				BuildQuad(Vertices, Triangles, cellRightInner, cellRightOuter, cellBottomInner, cellBottomOuter, FVector::ZeroVector, FVector::ZeroVector);
				break;
			case 0b1110:
				BuildQuad(Vertices, Triangles, cellBottomInner, cellBottomOuter, cellLeftOuter, cellLeftInner, FVector::ZeroVector, FVector::ZeroVector);
				break;
			case 0b1111:
				break;
			}
		}
	}

	FBox BoundingBox = FBox(-Size / 2.0f, Size / 2.0f);

	MeshComponent->ClearAllMeshSections();
	MeshComponent->CreateMeshSection(0, Vertices, Triangles, BoundingBox, true, EUpdateFrequency::Infrequent);


	FrontVertices.Reset();
	FrontTriangles.Reset();

	BuildQuad(FrontVertices, FrontTriangles, FVector(Left, 1, Bottom), FVector(Right, 1, Bottom), FVector(Right, 1, Top), FVector(Left, 1, Top), FVector::RightVector, FVector::ZeroVector);
	MeshComponent->CreateMeshSection(1, FrontVertices, FrontTriangles, BoundingBox, false, EUpdateFrequency::Infrequent);

	MeshComponent->SetMaterial(1, Material);
}

void ASimpleCubeActor::BuildQuad(TArray<FRuntimeMeshVertexSimple>& InVertices, TArray<int32>& InTriangles, FVector BottomLeft, FVector BottomRight, FVector TopRight, FVector TopLeft, FPackedNormal Normal, FPackedNormal Tangent)
{
	int32 Index1 = InVertices.Add(FRuntimeMeshVertexSimple(BottomLeft, Normal));
	int32 Index2 = InVertices.Add(FRuntimeMeshVertexSimple(BottomRight, Normal));
	int32 Index3 = InVertices.Add(FRuntimeMeshVertexSimple(TopRight, Normal));
	int32 Index4 = InVertices.Add(FRuntimeMeshVertexSimple(TopLeft, Normal));
	InTriangles.Add(Index1);
	InTriangles.Add(Index2);
	InTriangles.Add(Index3);
	InTriangles.Add(Index1);
	InTriangles.Add(Index3);
	InTriangles.Add(Index4);
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