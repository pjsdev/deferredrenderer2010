uniform sampler2D u_NormalMap;
uniform sampler2D u_AlbedoMap;
uniform sampler2D u_PositionMap;
uniform sampler2D u_ShadowMap;

//pos [xyz] angle [w]
uniform vec4 a_PositionAngle;

//dir [xyz] distance [w]
uniform vec4 a_DirectionDistance;

//col [xyz]
uniform vec4 a_Colour;

varying vec4 v_PSPosition;
varying vec4 v_LSPosition;

void main()
{

    vec4 finalColor;

    /**********************************
        Unpack Samples
     *******************************/

    vec2 pixelPos = v_PSPosition.xy / v_PSPosition.w;
    pixelPos = (pixelPos * 0.5) + 0.5;

    vec2 lightPix = v_LSPosition.xy / v_LSPosition.w;
    lightPix = (lightPix * 0.5) + 0.5;

    // normal xyz depth z
    vec4 normalSample = texture2D(u_NormalMap, pixelPos);

    //albedo xyz
    vec4 albedoSample = texture2D(u_AlbedoMap, pixelPos);

    //specular xyz
    vec4 position = texture2D(u_PositionMap, pixelPos);

    vec4 shadowMap = texture2D(u_ShadowMap, lightPix);

    //DECODE
    vec3 WSNormal = (vec3( normalSample.xyz )*2.0)-1.0;

    //////////////////////////////////////

    /**********************************
        Unpack Light Vars
     *******************************/

    float lightSpaceDepth = v_LSPosition.z / v_LSPosition.w;
    float shadowMapDepth = shadowMap.x;

    vec3 lightPos = a_PositionAngle.xyz;
    float coneAngle = a_PositionAngle.w;

    vec3 lightDirection = a_DirectionDistance.xyz;
    float lightDistance = a_DirectionDistance.w;

    vec3 lightColour = a_Colour.xyz;

    //////////////////////////////////////

    //check if pixel is inside light
    vec3 lightToFrag = position.xyz - lightPos;
    float lightToFragLength = length(lightToFrag);
    lightToFrag = normalize(lightToFrag);
    float lightFragAngle = dot(lightDirection, lightToFrag);

    if(lightFragAngle > coneAngle)
    {        
        float attenuation = smoothstep(0.95,1.0, lightFragAngle);
        float NdotL = max(0.0, dot(WSNormal, -lightToFrag));
        finalColor =  attenuation * NdotL * (albedoSample * vec4(lightColour,1.0) );
///        if(lightSpaceDepth > shadowMapDepth-0.01)
///        {
///            finalColor =  vec4(0.1);
///        }
    }
    else
    {
        finalColor = vec4(0);
    }

    //finalColor = vec4(shadowMapDepth);

    gl_FragColor = finalColor;//+ finalColor;
}
