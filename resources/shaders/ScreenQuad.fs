uniform sampler2D u_Sampler;

varying vec2 v_TextureCoord;

void main ()
{
    vec4 finalColor;

    vec4 baseColor = texture2D(u_Sampler, vec2(v_TextureCoord.s, v_TextureCoord.t));
    float greyColour = (0.21*baseColor.r) + (0.71*baseColor.g) + (0.07*baseColor.b);
    finalColor = vec4(greyColour);

    gl_FragColor = baseColor;
}
