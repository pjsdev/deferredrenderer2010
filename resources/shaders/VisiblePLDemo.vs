uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;

uniform vec3 u_Offset;

attribute vec3 a_VertPos;

void main(void)
{
    vec3 pos = a_VertPos+u_Offset;

    mat4 ProjViewMatrix = u_ProjectionMatrix*u_ViewMatrix;

    gl_Position = ProjViewMatrix * vec4(pos, 1.0);
}
