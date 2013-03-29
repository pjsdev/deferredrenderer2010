uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix;
uniform mat4 u_LightView;
uniform mat4 u_LightProj;

varying vec4 v_PSPosition;
varying vec4 v_LSPosition;

void main()
{
    gl_Position = (u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix) * gl_Vertex;
    v_PSPosition = gl_Position;
    v_LSPosition = u_LightProj * u_LightView * u_ModelMatrix * gl_Vertex;
}
