// =============================================================================
// Renderer2D Shader for NanSu Engine
// Single texture shader for 2D quad rendering
// Color-only quads use 1x1 white texture multiplied by color
// =============================================================================

// -----------------------------------------------------------------------------
// Constant Buffers
// -----------------------------------------------------------------------------

// Scene constant buffer (slot b0) - set once per BeginScene()
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
// Vertex Shader Input
// -----------------------------------------------------------------------------

struct VSInput
{
    float3 Position : POSITION;
    float4 Color : COLOR;
    float2 TexCoord : TEXCOORD;
    float TexIndex : TEXINDEX;          // Reserved for future batching
    float TilingFactor : TILINGFACTOR;
};

// -----------------------------------------------------------------------------
// Vertex Shader Output / Pixel Shader Input
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

VSOutput VSMain(VSInput input)
{
    VSOutput output;

    // Transform vertex position by ViewProjection matrix
    output.Position = mul(float4(input.Position, 1.0f), u_ViewProjection);
    output.Color = input.Color;
    output.TexCoord = input.TexCoord * input.TilingFactor;

    return output;
}

// =============================================================================
// Pixel Shader
// =============================================================================

float4 PSMain(VSOutput input) : SV_TARGET
{
    // Sample texture and multiply with vertex color (tint)
    // For color-only quads, texture is 1x1 white, so result = color
    float4 texColor = u_Texture.Sample(u_Sampler, input.TexCoord);
    return texColor * input.Color;
}
