SamplerState gsamPointWrap        : register(s0);
SamplerState gsamPointClamp       : register(s1);
SamplerState gsamLinearWrap       : register(s2);
SamplerState gsamLinearClamp      : register(s3);
SamplerState gsamAnisotropicWrap  : register(s4);
SamplerState gsamAnisotropicClamp : register(s5);
SamplerComparisonState gsamShadow : register(s6);



struct db_struct
{
    float db_s_value;
    float4x4 db_s_matrix;
};

//用了才会被反射捕捉到
//float value_debug : register(b1);
//float3 vector_debug : register(b2);
//Texture2D texture_debug_group[20] : register(t1);
Texture2D texture_debug : register(t0);
//StructuredBuffer<db_struct> debugt_struct : register(t3);



struct vs_in {
    float3 local_postion    : POSITION;
    float3 local_normal : NORMAL;
    float2 texcoord    : TEXCOORD;
    float3 tangentU : TANGENT;
};

struct vs_out {
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

vs_out VS(vs_in vIn)
{
    vs_out vOut;
    vOut.position = float4(vIn.local_postion, 1.0f);
    vOut.texcoord = vIn.texcoord;
    return vOut;
}

float4 PS(vs_out pIn) : SV_TARGET
{
    float4 rescolor = texture_debug.Sample(gsamPointClamp, pIn.texcoord);

    return rescolor;
}