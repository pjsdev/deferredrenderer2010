uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix;

attribute vec3 inVert;
attribute vec2 inUV;
attribute vec3 inNormal;
attribute vec3 inTangent;
attribute vec3 inBinormal;

varying vec3 v_WSFragmentNormal;
varying vec4 v_WSPosition;

varying vec2 v_UV;

void main(void)
{
    mat4 transform = u_ProjectionMatrix * (u_ViewMatrix * u_ModelMatrix);

    //used if we had glsl >1.5
    //mat4 NormalMatrix = transpose(inverse(u_ModelMatrix));

    vec4 position = transform*vec4(inVert,1.0);
    gl_Position = position;

    //the proj space position of this vertex
    v_WSPosition = u_ModelMatrix * vec4(inVert,1.0);

    //the world space normal of this vertex
    v_WSFragmentNormal = (u_ModelMatrix*vec4(inNormal, 0.0)).xyz;
    v_UV = inUV;
 }
