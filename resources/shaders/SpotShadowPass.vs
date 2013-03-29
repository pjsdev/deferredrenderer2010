uniform mat4 u_LightProj;
uniform mat4 u_LightView;
uniform mat4 u_ModelMatrix;

varying vec4 v_PSPosition;

void main(void)
{
    mat4 transform = u_LightProj * (u_LightView * u_ModelMatrix);

    vec4 position = transform*gl_Vertex;

    gl_Position = position;
    v_PSPosition = position;
}
