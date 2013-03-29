uniform sampler2D u_BrightMap;
uniform sampler2D u_ColorMap;

varying vec2 v_TextureCoord;

void main ()
{
    vec4 finalColor;

    vec4 bright = texture2D(u_BrightMap, vec2(v_TextureCoord.s, v_TextureCoord.t));
    vec4 color = texture2D(u_ColorMap, vec2(v_TextureCoord.s, v_TextureCoord.t));

    finalColor = color + bright;
    gl_FragColor = finalColor;
}
