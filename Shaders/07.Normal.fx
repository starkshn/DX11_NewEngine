matrix World;
matrix View;
matrix Projection;
Texture2D Texture0;
float3 LightDir; // Directional Light개념

struct VertexInput
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    output.position = mul(input.position, World);
    output.position = mul(output.position, View);
    output.position = mul(output.position, Projection);
	
    output.uv = input.uv;
    
    output.normal = mul(input.normal, (float3x3)World);
    
    // 위 방법이나 아래 방법 똑같은 결과를 도출한다.
    // matrix temp = World;
    // temp[3][3] = 0;
    // output.normal = mul(input.normal, temp);
    
    return output;
}

// Fillter - 확대/축소 발생시 -> 중간값을 처리하는 방식
// Address = UV가 1보다 컷을 때, 나머지 부분을 어떻게 처리할지(중요)
SamplerState Sampler0;

float4 PS(VertexOutput input) : SV_TARGET
{
    float3 normal = normalize(input.normal);
    float3 light = -LightDir;
    
    // 빛 밝기 잘 먹고 있는지 확인
    // return float4(1, 1, 1, 1) * dot(light, normal);
    
    // uv좌표에 매핑되어서 나온 color값에다가 내적 값을 곱해준다. -> color값이 변경이 됨.
    return Texture0.Sample(Sampler0, input.uv) * dot(light, normal);
}

RasterizerState FillModeWireFrame
{
    FillMode = WireFrame;
};

technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }

    pass P1
    {
        SetRasterizerState(FillModeWireFrame);

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}
