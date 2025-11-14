struct InputVertex
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct RasterVertex
{
    float3 worldPosition;
    float3 worldNormal;
    float2 uv;
};

[shader("vertex")]
[require(sm_6_0)]
RasterVertex vertexMain(in InputVertex vertex)
{
    return RasterVertex(float3(0, 0, 0), float3(0, 0, 0), float2(0, 0));
}

[shader("fragment")]
[require(sm_6_0)]
float4 fragmentMain(in RasterVertex vertex: R) : SV_Target0
{
    return float4(0, 0, 0, 0);  
}
