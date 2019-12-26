#version 330 core

// Interpolated values from the vertex shaders
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;
in float property_field;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
uniform mat4 MV;
uniform vec3 LightPosition_worldspace;

vec3 HSV2RGB( vec3 hsv )
{
    hsv.x = mod( 100.0 + hsv.x, 1.0 ); // Ensure [0,1[
    float   HueSlice = 6.0 * hsv.x; // In [0,6[
    float   HueSliceInteger = floor( HueSlice );
    float   HueSliceInterpolant = HueSlice - HueSliceInteger; // In [0,1[ for each hue slice
    vec3  TempRGB = vec3(   hsv.z * (1.0 - hsv.y), hsv.z * (1.0 - hsv.y * HueSliceInterpolant), hsv.z * (1.0 - hsv.y * (1.0 - HueSliceInterpolant)) );
    float   IsOddSlice = mod( HueSliceInteger, 2.0 ); // 0 if even (slices 0, 2, 4), 1 if odd (slices 1, 3, 5)
    float   ThreeSliceSelector = 0.5 * (HueSliceInteger - IsOddSlice); // (0, 1, 2) corresponding to slices (0, 2, 4) and (1, 3, 5)
    vec3  ScrollingRGBForEvenSlices = vec3( hsv.z, TempRGB.zx );           // (V, Temp Blue, Temp Red) for even slices (0, 2, 4)
    vec3  ScrollingRGBForOddSlices = vec3( TempRGB.y, hsv.z, TempRGB.x );  // (Temp Green, V, Temp Red) for odd slices (1, 3, 5)
    vec3  ScrollingRGB = mix( ScrollingRGBForEvenSlices, ScrollingRGBForOddSlices, IsOddSlice );
    float   IsNotFirstSlice = clamp( ThreeSliceSelector, 0.0,1.0 );                   // 1 if NOT the first slice (true for slices 1 and 2)
    float   IsNotSecondSlice = clamp( ThreeSliceSelector-1.0, 0.0,1. );              // 1 if NOT the first or second slice (true only for slice 2)
    return  mix( ScrollingRGB.xyz, mix( ScrollingRGB.zxy, ScrollingRGB.yzx, IsNotSecondSlice ), IsNotFirstSlice );    // Make the RGB rotate right depending on final slice index
}

void main(){

	// Light emission properties
	// You probably want to put them as uniforms
	vec3 LightColor = vec3(1,1,1);
	float LightPower = 50.0f;
	
	// Material properties
    vec3 MaterialDiffuseColor = HSV2RGB(vec3(property_field, 1., 1.));
	vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);

	// Distance to the light
	float distance = length( LightPosition_worldspace - Position_worldspace );

	// Normal of the computed fragment, in camera space
	vec3 n = normalize( Normal_cameraspace );
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize( LightDirection_cameraspace );
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp( dot( n,l ), 0,1 );
	
	// Eye vector (towards the camera)
	vec3 E = normalize(EyeDirection_cameraspace);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-l,n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp( dot( E,R ), 0,1 );
	
	color = 
		// Ambient : simulates indirect lighting
		MaterialAmbientColor +
		// Diffuse : "color" of the object
		MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
		// Specular : reflective highlight, like a mirror
		MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);

        //color = 0.25*color + 0.75*vec3(property_field, 1.-property_field,0.);
        //color = 0.25*color + 0.75*HSV2RGB(vec3(property_field, 1., 1.));
}
