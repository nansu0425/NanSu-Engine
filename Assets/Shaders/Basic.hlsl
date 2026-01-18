// =============================================================================
// Basic Shader for NanSu Engine
// =============================================================================

// -----------------------------------------------------------------------------
// Constant Buffers
// -----------------------------------------------------------------------------

// Scene constant buffer (set once per frame via Renderer::BeginScene)
cbuffer SceneData : register(b0)
{
    matrix u_ViewProjection;
};

// -----------------------------------------------------------------------------
// Textures and Samplers
// -----------------------------------------------------------------------------

Texture2D u_Texture : register(t0);
SamplerState u_Sampler : register(s0);

// -----------------------------------------------------------------------------
// Vertex Shader Input/Output
// -----------------------------------------------------------------------------

struct VSOutput
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float2 TexCoord : TEXCOORD0;
};

// =============================================================================
// Vertex Shader
// =============================================================================
VSOutput VSMain(float3 position : POSITION, float4 color : COLOR, float2 texCoord : TEXCOORD)
{
    VSOutput output;

    // Transform vertex position by ViewProjection matrix
    output.Position = mul(float4(position, 1.0f), u_ViewProjection);
    output.Color = color;
    output.TexCoord = texCoord;

    return output;
}

// =============================================================================
// Pixel Shader
// =============================================================================
float4 PSMain(VSOutput input) : SV_TARGET
{
    // Sample texture and multiply with vertex color
    float4 texColor = u_Texture.Sample(u_Sampler, input.TexCoord);
    return texColor * input.Color;
}
