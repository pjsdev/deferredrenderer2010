uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;

uniform vec4 u_WSCentreRadius;

attribute vec3 a_VertexPosition;

//pixel pos xy intensity z
varying vec4 v_PSPosition;
varying mat4 v_ProjViewMatrix;
varying vec4 v_WSCentreRadius;


void main(void)
{
    vec3 pos = (a_VertexPosition.xyz * u_WSCentreRadius.w)+u_WSCentreRadius.xyz;

    //the projview matrix for moving between PS and WS
    v_ProjViewMatrix = u_ProjectionMatrix*u_ViewMatrix;

    //the projected space position of this vert
    v_PSPosition = v_ProjViewMatrix * vec4(pos, 1.0);

    //the lights centre xyz and radius
    v_WSCentreRadius = u_WSCentreRadius;

    gl_Position = v_PSPosition;
}
