uniform sampler2D u_ColorMap;
uniform sampler2D u_NormalMap;
uniform sampler2D u_PositionMap;

uniform vec2 u_pixel;

//normBarrier[x] depthBarrier[y] normWeight[z] depthWeight[w]
uniform vec4 u_barrierWeights;

uniform float u_kernel;

varying vec2 v_TextureCoord;

void main ()
{
    float normBarrier = u_barrierWeights.x;
    float depthBarrier = u_barrierWeights.y;
    float normWeightMod = u_barrierWeights.z;
    float depthWeightMod = u_barrierWeights.w;

    /************************************
        Get our samples
    *************************************/
    vec3 normal = vec4(texture2D(u_NormalMap, v_TextureCoord)).xyz;
    normal = (normal * 2.0) - 1.0;

    vec4 pos = texture2D(u_PositionMap, v_TextureCoord);

    vec4 color = texture2D(u_ColorMap, v_TextureCoord);
    /************************************/

    /*************************************
        The Surround Tex Coords
    *************************************/
    vec2 topLeft = vec2(v_TextureCoord.x-u_pixel.x, v_TextureCoord.y+u_pixel.y);
    vec2 top = vec2(v_TextureCoord.x, v_TextureCoord.y+u_pixel.y);
    vec2 topRight = vec2(v_TextureCoord.x+u_pixel.x, v_TextureCoord.y+u_pixel.y);

    vec2 left = vec2(v_TextureCoord.x-u_pixel.x, v_TextureCoord.y);
    vec2 right = vec2(v_TextureCoord.x+u_pixel.x, v_TextureCoord.y);

    vec2 bottomLeft = v_TextureCoord - u_pixel;
    vec2 bottom = vec2(v_TextureCoord.x, v_TextureCoord.y-u_pixel.y);
    vec2 bottomRight = vec2(v_TextureCoord.x+u_pixel.x, v_TextureCoord.y-u_pixel.y);
    /*************************************/

    /*************************************
        Normal Weighting
    *************************************/
    vec4 surroundingAngles;
    surroundingAngles.x = dot(normal, ((texture2D(u_NormalMap, topLeft).xyz)*2.0)-1.0 );
    surroundingAngles.y = dot(normal, ((texture2D(u_NormalMap, bottomRight).xyz)*2.0)-1.0 );
    surroundingAngles.z = dot(normal, ((texture2D(u_NormalMap, topRight).xyz)*2.0)-1.0 );
    surroundingAngles.w = dot(normal, ((texture2D(u_NormalMap, bottomLeft).xyz)*2.0)-1.0 );

    // e.barrier.x (0.8)
    surroundingAngles -= normBarrier;
    surroundingAngles = step(vec4(0.0), surroundingAngles);

    // e_weight.x vec4(1.0)
    float normalWeight = clamp(dot(surroundingAngles, vec4(normWeightMod)), 0.0,1.0);

    /*************************************/

    /*************************************
        Depth Weighting
    *************************************/
    // Depth filter : compute gradiental difference:
    // (c-sample1)+(c-sample1_opposite)
    vec4 depthDifference;
    depthDifference.x = texture2D(u_PositionMap, topLeft).z + texture2D(u_PositionMap, bottomRight).z;
    depthDifference.y = texture2D(u_PositionMap, topRight).z + texture2D(u_PositionMap, bottomLeft).z;
    depthDifference.z = texture2D(u_PositionMap, left).z + texture2D(u_PositionMap, right).z;
    depthDifference.w = texture2D(u_PositionMap, top).z + texture2D(u_PositionMap, bottom).z;

    // e_barrier.y (0.5)
    depthDifference = abs(2* pos.z - depthDifference)- depthBarrier;
    depthDifference = step(depthDifference, vec4(0.0));
    // e_weight.y vec4(1.0)
    float depthWeight = clamp(dot(depthDifference, vec4(depthWeightMod)), 0.0, 1.0);


    /************************************/

    // WEIGHT
    //e_kernel.x (0.5)
    float finalWeighting = (1.0 - depthWeight * normalWeight) * u_kernel;

    //SMOOTH
    vec2 offset = v_TextureCoord * (1.0-finalWeighting);

    vec4 s0 = vec4(texture2D(u_ColorMap, offset + top * finalWeighting).xyz, 1.0);
    vec4 s1 = vec4(texture2D(u_ColorMap, offset + bottom * finalWeighting).xyz, 1.0);
    vec4 s2 = vec4(texture2D(u_ColorMap, offset + right * finalWeighting).xyz, 1.0);
    vec4 s3 = vec4(texture2D(u_ColorMap, offset + left * finalWeighting).xyz, 1.0);

    gl_FragColor = (s0 + s1 + s2 + s3)/4.0;

}
