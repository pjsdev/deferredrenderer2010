uniform sampler2D u_Sampler;

varying vec2 v_TextureCoord;

void main ()
{
    vec4 finalColor;

    vec4 baseColor = texture2D(u_Sampler, vec2(v_TextureCoord.s, v_TextureCoord.t));


    float r = smoothstep(0.3,1.0,baseColor.r);
    float g = smoothstep(0.3,1.0,baseColor.g);
    float b = smoothstep(0.3,1.0,baseColor.b);

    finalColor = vec4(r,g,b,1.0);//step(0.5, baseColor);

    gl_FragColor = finalColor;
}
