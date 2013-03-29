uniform sampler2D u_NormalMap;
uniform sampler2D u_AlbedoMap;
uniform sampler2D u_PositionMap;

//intensity xyz radius w
uniform vec3 u_RGBIntensity;

varying vec4 v_PSPosition;
//light centre xyz radius w
varying vec4 v_WSCentreRadius;

void main()
{
    //Centre and radius of light in World Space
    vec3 WSLightCentre = v_WSCentreRadius.xyz;
    float radiusSq = v_WSCentreRadius.w*v_WSCentreRadius.w;

    /************************************
        Get our samples
    *************************************/

    //convert from proj space to texture space
    vec2 pixelPos = v_PSPosition.xy / v_PSPosition.w;
    pixelPos = (pixelPos*0.5) + 0.5;

    // normal xyz depth z
    vec4 normalSample = texture2D(u_NormalMap, pixelPos);

    //albedo xyz
    vec4 albedoSample = texture2D(u_AlbedoMap, pixelPos);

    //specular xyz
    vec4 posSample = texture2D(u_PositionMap, pixelPos);

    /***************************************/

    //////////////
    //decode normal
    vec3 WSNormal = (vec3( normalSample.xyz )*2.0)-1.0;
    //////////////

    vec3 lightRay = WSLightCentre - posSample.xyz;

    //get length squared of point to light
    float distance = length(lightRay);
    float distanceSq = distance*distance;

    float nDotL = 0.0;
    float attenuation = 0.0;

    if(distanceSq < radiusSq)
    {
        vec3 L = normalize(lightRay);
        nDotL = max(0.0,dot(WSNormal, L));

        float lengthByRad = distance / v_WSCentreRadius.w;
        //constantAtt / (1+( linearAtt * dist )* ( 1+ ( quadAtt * (dist*dist) ) ) )

        attenuation = 1.0-smoothstep(0.0,1.0,lengthByRad);
    }

    gl_FragColor =  attenuation * nDotL * ( vec4(albedoSample.xyz, 1.0) *  vec4(u_RGBIntensity,1.0) );
}
