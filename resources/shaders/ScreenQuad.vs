//centre [xy] depth [z] extent [w]
uniform vec4 u_CentreDepthExtent;

attribute vec3 a_VertexPosition;
attribute vec2 a_TextureCoord;

varying vec2 v_TextureCoord;

void main(void)
{
    v_TextureCoord = a_TextureCoord;

    vec2  centre = u_CentreDepthExtent.xy;
    float extent = u_CentreDepthExtent.w;
    float depth  = u_CentreDepthExtent.z;

    vec2 ssPos = (a_VertexPosition.xy*extent)+centre;
    gl_Position = vec4(ssPos.x, ssPos.y, depth, 1);
}
