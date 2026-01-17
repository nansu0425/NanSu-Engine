// =============================================================================
// Basic Shader for NanSu Engine
// =============================================================================

// Vertex Shader Output / Pixel Shader Input
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
    output.Position = float4(position, 1.0f);
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
