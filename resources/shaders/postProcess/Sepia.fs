uniform sampler2D u_Sampler;

varying vec2 v_TextureCoord;

void main ()
{
    vec4 finalColor;

    vec4 baseColor = texture2D(u_Sampler, vec2(v_TextureCoord.s, v_TextureCoord.t));

    float r = (baseColor.r * 0.39)+(baseColor.g * 0.77)+(baseColor.b * 0.19);
    float g = (baseColor.r * 0.35)+(baseColor.g * 0.68)+(baseColor.b * 0.17);
    float b = (baseColor.r * 0.27)+(baseColor.g * 0.53)+(baseColor.b * 0.13);

    finalColor = vec4(r,g,b,1.0);

    gl_FragColor = finalColor;
}
