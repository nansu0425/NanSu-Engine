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
// Vertex Shader Input/Output
// -----------------------------------------------------------------------------

struct VSOutput
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
};

// =============================================================================
// Vertex Shader
// =============================================================================
VSOutput VSMain(float3 position : POSITION, float4 color : COLOR)
{
    VSOutput output;

    // Transform vertex position by ViewProjection matrix
    output.Position = mul(float4(position, 1.0f), u_ViewProjection);
    output.Color = color;

    return output;
}

// =============================================================================
// Pixel Shader
// =============================================================================
float4 PSMain(VSOutput input) : SV_TARGET
{
    return input.Color;
}
