matrix World;
matrix View;
matrix Projection;
Texture2D Texture0;
Texture2D Texture1;
uint Address;

struct VertexInput
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    output.position = mul(input.position, World);
    output.position = mul(output.position, View);
    output.position = mul(output.position, Projection);
	
    output.uv = input.uv;
	
    return output;
}

// Fillter - 확대/축소 발생시 -> 중간값을 처리하는 방식
// Address = UV가 1보다 컷을 때, 나머지 부분을 어떻게 처리할지(중요)
SamplerState Sampler0
{
    // Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

float4 PS(VertexOutput input) : SV_TARGET
{
    if (Address == 0)
        return Texture0.Sample(Sampler0, input.uv);
    
    return Texture0.Sample(Sampler0, input.uv);
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
