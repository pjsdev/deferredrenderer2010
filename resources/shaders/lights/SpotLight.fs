uniform sampler2D u_NormalMap;
uniform sampler2D u_AlbedoMap;
uniform sampler2D u_PositionMap;

//pos [xyz] angle [w]
uniform vec4 a_PositionAngle;

//dir [xyz] distance [w]
uniform vec4 a_DirectionDistance;

//col [xyz]
uniform vec4 a_Colour;

varying vec4 v_PSPosition;

void main()
{

    /**********************************
        Unpack Samples
     *******************************/

    vec2 pixelPos = v_PSPosition.xy / v_PSPosition.w;
    pixelPos = (pixelPos * 0.5) + 0.5;

    // normal xyz depth z
    vec4 normalSample = texture2D(u_NormalMap, pixelPos);

    //albedo xyz
    vec4 albedoSample = texture2D(u_AlbedoMap, pixelPos);

    //specular xyz
    vec4 position = texture2D(u_PositionMap, pixelPos);

    //DECODE
    vec3 WSNormal = (vec3( normalSample.xyz )*2.0)-1.0;

    //////////////////////////////////////

    /**********************************
        Unpack Light Vars
     *******************************/

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
        float NdotL = max(0.0,dot(WSNormal, -lightToFrag));
        gl_FragColor = attenuation * NdotL * (albedoSample * vec4(lightColour,1.0) );
    }
    else
    {
        gl_FragColor = vec4(0);
    }
}
