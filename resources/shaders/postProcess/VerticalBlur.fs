uniform sampler2D u_Sampler;
uniform float u_pixel;

varying vec2 v_TextureCoord;

void main ()
{
   vec4 sum = vec4(0.0);

   // blur in y (vertical)
   // take nine samples, with the distance blurSize between them
   sum += texture2D(u_Sampler, vec2(v_TextureCoord.x, v_TextureCoord.y - 5.0*u_pixel)) * 0.04;
   sum += texture2D(u_Sampler, vec2(v_TextureCoord.x, v_TextureCoord.y - 4.0*u_pixel)) * 0.04;
   sum += texture2D(u_Sampler, vec2(v_TextureCoord.x, v_TextureCoord.y - 3.0*u_pixel)) * 0.08;
   sum += texture2D(u_Sampler, vec2(v_TextureCoord.x, v_TextureCoord.y - 2.0*u_pixel)) * 0.12;
   sum += texture2D(u_Sampler, vec2(v_TextureCoord.x, v_TextureCoord.y - u_pixel)) * 0.14;
   sum += texture2D(u_Sampler, vec2(v_TextureCoord.x, v_TextureCoord.y)) * 0.18;
   sum += texture2D(u_Sampler, vec2(v_TextureCoord.x, v_TextureCoord.y + u_pixel)) * 0.14;
   sum += texture2D(u_Sampler, vec2(v_TextureCoord.x, v_TextureCoord.y + 2.0*u_pixel)) * 0.12;
   sum += texture2D(u_Sampler, vec2(v_TextureCoord.x, v_TextureCoord.y + 3.0*u_pixel)) * 0.08;
   sum += texture2D(u_Sampler, vec2(v_TextureCoord.x, v_TextureCoord.y + 4.0*u_pixel)) * 0.04;
   sum += texture2D(u_Sampler, vec2(v_TextureCoord.x, v_TextureCoord.y + 5.0*u_pixel)) * 0.04;

   gl_FragColor = sum;
}
