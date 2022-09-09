#ifndef MAIN_SHADER_FX
#define MAIN_SHADER_FX


float3 g_vMaterialAmbient   : Ambient = float3( 0.2f, 0.2f, 0.2f );   // Material's ambient color
float3 g_vMaterialDiffuse   : Diffuse = float3( 0.8f, 0.8f, 0.8f );   // Material's diffuse color
float3 g_vMaterialSpecular  : Specular = float3( 1.0f, 1.0f, 1.0f );  // Material's specular color
float  g_fMaterialAlpha     : Opacity = 1.0f;
int    g_nMaterialShininess : SpecularPower = 32;
//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
float3 g_vLightColor : LightColor = float3( 1.0f, 1.0f, 1.0f );        // Light color
float3 g_vLightPosition : LightPosition = float3( 0.0f, -40.0f, 0.0f );   // Light position
float3 g_vCameraPosition : CameraPosition;
     
texture  g_MeshTexture : Texture;   // Color texture for mesh           

float4x4 g_mProj : Proj;          
float4x4 g_mView : View;         
//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
sampler MeshTextureSampler = 
sampler_state
{
    Texture = <g_MeshTexture>;    
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};
//--------------------------------------------------------------------------------------
// Name: Projection
// Type: Vertex Shader Fragment
// Desc: Projection transform
//--------------------------------------------------------------------------------------
struct INPUT_PROJECTION
{
	float4 vPosObject;
	float3 vNormalObject;
	float2 vTexCoordIn;
	float4x4 mWorld;
	bool bSpecular;
};
struct OUTPUT_PROJECTION
{
	float4 vPosProj;
	float2 vTexCoordOut;
	float4 vColorOut;
};
//--------------------------------------------------------------------------------------
OUTPUT_PROJECTION MainProjection(INPUT_PROJECTION input)
{
	OUTPUT_PROJECTION output = (OUTPUT_PROJECTION) 0;

	float4x4 mWorldViewProjection = input.mWorld*g_mView*g_mProj;
    // Transform the position into world space for lighting, and projected space
    // for display
    float4 vPosWorld = mul( input.vPosObject, input.mWorld );
    output.vPosProj = mul( input.vPosObject, mWorldViewProjection );
    
    // Transform the normal into world space for lighting
    float3 vNormalWorld = mul( input.vNormalObject, (float3x3)(input.mWorld) );
    
    // Pass the texture coordinate
    output.vTexCoordOut = input.vTexCoordIn;
    
    // Compute the light vector
    float3 vLight = normalize( g_vLightPosition - vPosWorld.xyz );
    
    // Compute the ambient and diffuse components of illumination
    output.vColorOut.rgb = g_vLightColor * g_vMaterialAmbient;
    output.vColorOut.rgb += g_vLightColor * g_vMaterialDiffuse * saturate( dot( vLight, vNormalWorld ) );
    
    // If enabled, calculate the specular term
    if( input.bSpecular )
    {
        float3 vCamera = normalize(vPosWorld.xyz - g_vCameraPosition);
        float3 vReflection = reflect( vLight, vNormalWorld );
        float  fPhongValue = saturate( dot( vReflection, vCamera ) );

        output.vColorOut.rgb += g_vMaterialSpecular * pow(fPhongValue, g_nMaterialShininess);
    }
        
    output.vColorOut.a = g_fMaterialAlpha;
	return output;
}
//--------------------------------------------------------------------------------------
// Name: Lighting
// Type: Pixel Shader
// Desc: Compute lighting and modulate the texture
//--------------------------------------------------------------------------------------
struct INPUT_LIGHT
{
	float2 vTexCoord;
    float4 vColorIn;
    bool bTexture;
};
struct OUTPUT_LIGHT
{
	float4 vColorOut;
};
OUTPUT_LIGHT MainLighting(INPUT_LIGHT input)
{  
	OUTPUT_LIGHT output = (OUTPUT_LIGHT)0;
	output.vColorOut = input.vColorIn;

	// Sample and modulate the texture
	if( input.bTexture )
	  output.vColorOut.rgb *= tex2D( MeshTextureSampler, input.vTexCoord );
	return output;
}


#endif