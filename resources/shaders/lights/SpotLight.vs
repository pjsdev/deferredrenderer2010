uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix;

varying vec4 v_PSPosition;

void main()
{
    gl_Position = (u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix) * gl_Vertex;
    v_PSPosition = gl_Position;
}
