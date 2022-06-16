#ifndef BASIC_HLSL
#define BASIC_HLSL

#include "LightHelper.hlsli"

Texture2D g_DiffuseMap : register(t0);
Texture2D g_NormalMap : register(t1);
Texture2D g_ShadowMap : register(t2);
Texture2D g_AmbientOcclusionMap : register(t3);
SamplerState g_Sam : register(s0);
SamplerComparisonState g_SamShadow : register(s1);

cbuffer CBChangesEveryInstanceDrawing : register(b0)
{
    matrix g_World;
    matrix g_WorldInvTranspose;
    matrix g_WorldViewProj;
}

cbuffer CBChangesEveryObjectDrawing : register(b1)
{
    Material g_Material;
}

cbuffer CBDrawingStates : register(b2)
{
    float g_DepthBias;
    float g_Pad;
}

cbuffer CBChangesEveryFrame : register(b3)
{
    matrix g_ViewProj;
    matrix g_ShadowTransform; // ShadowView * ShadowProj * T
    float3 g_EyePosW;
    float g_Pad2;
}

cbuffer CBChangesRarely : register(b4)
{
    DirectionalLight g_DirLight[5];
    PointLight g_PointLight[5];
    SpotLight g_SpotLight[5];
}

struct VertexInput
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
#if defined USE_NORMAL_MAP
    float4 TangentL : TANGENT;
#endif
    float2 Tex : TEXCOORD;
};

struct VertexOutput
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION; // 在世界中的位置
    float3 NormalW : NORMAL; // 法向量在世界中的方向
#if defined USE_NORMAL_MAP
    float4 TangentW : TANGENT; // 切线在世界中的方向
#endif 
    float2 Tex : TEXCOORD0;
    float4 ShadowPosH : TEXCOORD1;
#if defined USE_SSAO_MAP
    float4 SSAOPosH : TEXCOORD2;
#endif
};

// 顶点着色器
VertexOutput BasicVS(VertexInput vIn)
{
    VertexOutput vOut;
    
    vector posW = mul(float4(vIn.PosL, 1.0f), g_World);
    
    vOut.PosW = posW.xyz;
    vOut.PosH = mul(float4(vIn.PosL, 1.0f), g_WorldViewProj); // 保持和SSAO的计算一致
    vOut.NormalW = mul(vIn.NormalL, (float3x3) g_WorldInvTranspose);
#if defined USE_NORMAL_MAP
    vOut.TangentW = float4(mul(vIn.TangentL.xyz, (float3x3) g_WorldInvTranspose), vIn.TangentL.w);
#endif 
    vOut.Tex = vIn.Tex;
    vOut.ShadowPosH = mul(posW, g_ShadowTransform);
#if defined USE_SSAO_MAP
    // 从NDC坐标[-1, 1]^2变换到纹理空间坐标[0, 1]^2
    // u = 0.5x + 0.5
    // v = -0.5y + 0.5
    // ((xw, yw, zw, w) + (w, -w, 0, 0)) * (0.5, -0.5, 1, 1) = ((0.5x + 0.5)w, (-0.5y + 0.5)w, zw, w)
    //                                                      = (uw, vw, zw, w)
    //                                                      =>  (u, v, z, 1)
    vOut.SSAOPosH = (vOut.PosH + float4(vOut.PosH.w, -vOut.PosH.w, 0.0f, 0.0f)) * float4(0.5f, -0.5f, 1.0f, 1.0f);
#endif
    return vOut;
}

// 像素着色器
float4 BasicPS(VertexOutput pIn) : SV_Target
{
    uint texWidth, texHeight;
    g_DiffuseMap.GetDimensions(texWidth, texHeight);
    float4 texColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    if (texWidth > 0 && texHeight > 0)
    {
        // 提前进行Alpha裁剪，对不符合要求的像素可以避免后续运算
        texColor = g_DiffuseMap.Sample(g_Sam, pIn.Tex);
        clip(texColor.a - 0.1f);
    }
    
    // 标准化法向量
    pIn.NormalW = normalize(pIn.NormalW);

    // 求出顶点指向眼睛的向量，以及顶点与眼睛的距离
    float3 toEyeW = normalize(g_EyePosW - pIn.PosW);
    float distToEye = distance(g_EyePosW, pIn.PosW);

#if defined USE_NORMAL_MAP
    // 采样法线贴图
    float3 normalMapSample = g_NormalMap.Sample(g_Sam, pIn.Tex).rgb;
    pIn.NormalW = NormalSampleToWorldSpace(normalMapSample, pIn.NormalW, pIn.TangentW);
#endif
    
        // 完成纹理投影变换并对SSAO图采样
    float ambientAccess = 1.0f;
#if defined USE_SSAO_MAP
    pIn.SSAOPosH /= pIn.SSAOPosH.w;
    ambientAccess = g_AmbientOcclusionMap.SampleLevel(g_Sam, pIn.SSAOPosH.xy, 0.0f).r;
#endif
    
    // 初始化为0 
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 A = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 D = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 S = float4(0.0f, 0.0f, 0.0f, 0.0f);
    int i;

    float shadow[5] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
    // 仅第一个方向光用于计算阴影
    shadow[0] = CalcShadowFactor(g_SamShadow, g_ShadowMap, pIn.ShadowPosH, g_DepthBias);
    
    [unroll]
    for (i = 0; i < 5; ++i)
    {
        ComputeDirectionalLight(g_Material, g_DirLight[i], pIn.NormalW, toEyeW, A, D, S);
        ambient += ambientAccess * A;
        diffuse += shadow[i] * D;
        spec += shadow[i] * S;
    }
        
    [unroll]
    for (i = 0; i < 5; ++i)
    {
        ComputePointLight(g_Material, g_PointLight[i], pIn.PosW, pIn.NormalW, toEyeW, A, D, S);
        ambient += A;
        diffuse += D;
        spec += S;
    }

    [unroll]
    for (i = 0; i < 5; ++i)
    {
        ComputeSpotLight(g_Material, g_SpotLight[i], pIn.PosW, pIn.NormalW, toEyeW, A, D, S);
        ambient += A;
        diffuse += D;
        spec += S;
    }
  
    float4 litColor = texColor * (ambient + diffuse) + spec;
    litColor.a = texColor.a * g_Material.Diffuse.a;
    return litColor;
}


#endif
