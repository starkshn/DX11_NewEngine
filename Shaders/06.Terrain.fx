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

// Fillter - Ȯ��/��� �߻��� -> �߰����� ó���ϴ� ���
// Address = UV�� 1���� ���� ��, ������ �κ��� ��� ó������(�߿�)
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
