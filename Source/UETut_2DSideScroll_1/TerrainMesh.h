#pragma once

#include "GameFramework/Actor.h"
#include "RuntimeMeshComponent.h"
#include "TerrainMesh.generated.h"

DECLARE_STATS_GROUP(TEXT("MeshGen"), STATGROUP_MeshGen, STATCAT_Advanced);

DECLARE_CYCLE_STAT_EXTERN(TEXT("MeshGen ~ GenerateMesh"), STAT_GenerateMesh, STATGROUP_MeshGen, UETUT_2DSIDESCROLL_1_API);
DECLARE_CYCLE_STAT_EXTERN(TEXT("MeshGen ~ CheckMapData"), STAT_CheckMapData, STATGROUP_MeshGen, UETUT_2DSIDESCROLL_1_API);
DECLARE_CYCLE_STAT_EXTERN(TEXT("MeshGen ~ CreateFVecs"), STAT_CreateFVecs, STATGROUP_MeshGen, UETUT_2DSIDESCROLL_1_API);
DECLARE_CYCLE_STAT_EXTERN(TEXT("MeshGen ~ BuildQuads"), STAT_BuildQuads, STATGROUP_MeshGen, UETUT_2DSIDESCROLL_1_API);
DECLARE_CYCLE_STAT_EXTERN(TEXT("MeshGen ~ SetupBuffers"), STAT_SetupBuffers, STATGROUP_MeshGen, UETUT_2DSIDESCROLL_1_API);

UCLASS()
class UETUT_2DSIDESCROLL_1_API ATerrainMesh : public AActor
{
	GENERATED_BODY()

public:
	ATerrainMesh();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
	FVector Size = FVector(100.0f, 100.0f, 100.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
	UMaterialInterface* Material;

	virtual void PostLoad() override;
	virtual void PostActorCreated() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

	UFUNCTION(BlueprintCallable, Category = "Debug")
	void GenerateMesh();

	UFUNCTION(BlueprintCallable, Category = "Updates")
	void RemoveSquare(FVector holePos, FVector holeSize);

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Echo)
	UTexture2D* MapTexture;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Default)
		USceneComponent* RootNode;

	UPROPERTY()
		URuntimeMeshComponent* MeshComponent;

private:
	void BuildQuad(TArray<FRuntimeMeshVertexSimple>& InVertices, TArray<int32>& InTriangles, FVector BottomLeft, FVector BottomRight, FVector TopRight, FVector TopLeft, FPackedNormal Normal, FPackedNormal Tangent);

	// Mesh buffers
	void SetupBuffers(FVector Size);
	//bool bHaveBuffersBeenInitialized = false;
	TArray<FRuntimeMeshVertexSimple> Vertices;
	TArray<int32> Triangles;

	TArray<FRuntimeMeshVertexSimple> FrontVertices;
	TArray<int32> FrontTriangles;
	TBitArray<FDefaultBitArrayAllocator> MapData;

	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY()
	TArray<FColor> data;

	FUpdateTextureRegion2D *updateTextureRegion;
};