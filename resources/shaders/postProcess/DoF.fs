uniform sampler2D u_ColorMap;
uniform sampler2D u_BlurMap;
uniform sampler2D u_PositionMap;

uniform float u_FocalDepth;

uniform mat4 u_ProjView;

varying vec2 v_TextureCoord;

void main ()
{
    vec4 finalColor = vec4(0);

    /************************************
        Get our samples
    *************************************/

    vec4 pos = texture2D(u_PositionMap, v_TextureCoord);

    vec4 PSpos = u_ProjView * pos;
    PSpos = PSpos / PSpos.w;

    vec4 color = texture2D(u_ColorMap, v_TextureCoord);
    vec4 blur = texture2D(u_BlurMap, v_TextureCoord);

    /************************************/


    float blurFac = smoothstep(0.4, 0.45, abs(PSpos.z-u_FocalDepth));

    finalColor = color*(1.0-blurFac) + blur*(blurFac);
    //finalColor = vec4(blurFac);

    gl_FragColor = finalColor;
}
