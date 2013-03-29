uniform sampler2D u_Texture;

//ambient light
uniform vec3 u_AmbientLight;

//ambient (x) diffuse (y) specular(z) coefficients
uniform vec3 u_KaKdKs;

//directional light
uniform vec3 u_DirectionalLight;

//direction light direction
uniform vec3 u_DirectionalLightVec;

// world space frag normal
varying vec3 v_WSFragmentNormal;

//world space position
varying vec4 v_WSPosition;

varying vec2 v_UV;

void main ()
{
    vec4 texColor = texture2D(u_Texture,v_UV);

    float Ka = u_KaKdKs.x;
    float Kd = u_KaKdKs.y;

    // normalize the vertex normal
    vec3 N = normalize(v_WSFragmentNormal);
    vec3 L = -(normalize(u_DirectionalLightVec));

    float NdotL = dot(N,L);

    ////////////////////
    //encode WS normal
    N = (N*0.5)+0.5;
    ////////////////////

    // pre declare the colour contribution values
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
    vec4 emissive;
    vec4 directional;

    ambient = gl_FrontMaterial.ambient * (vec4(u_AmbientLight,1.0) * Ka);
    diffuse = gl_FrontMaterial.diffuse;
    shininess = gl_FrontMaterial.shininess;
    emissive = gl_FrontMaterial.emission;
    directional = (Kd * diffuse) * (NdotL * vec4(u_DirectionalLight,1.0));

    vec4 constant = clamp(ambient+directional+emissive,0.0,1.0);//*texColor;

    // finally set the fragdata
    gl_FragData[0] = vec4(constant.xyz, 1.0);

    //NEED TO DECODE
    gl_FragData[1] = vec4(N,1.0);
    gl_FragData[2] = vec4(diffuse.xyz,1.0);
    gl_FragData[3] = vec4(v_WSPosition);//vec4(specular.xyz, shininess);
}

