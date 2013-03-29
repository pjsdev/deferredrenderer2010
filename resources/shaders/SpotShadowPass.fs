varying vec4 v_PSPosition;

void main ()
{
    /*************
        variance shadow mapping setup, just incase
        based on:Fabian Sanglards code at:
            http://fabiensanglard.net/shadowmappingVSM/
        *************/

    float depth = v_PSPosition.z / v_PSPosition.w;

    float moment1 = depth;
    float moment2 = depth * depth;

    // Adjusting moments (this is sort of bias per pixel) using partial derivative
    float dx = dFdx(depth);
    float dy = dFdy(depth);
    moment2 += 0.25*(dx*dx+dy*dy) ;

    /****************************/

    gl_FragColor = vec4( moment1, 0.0, 0.0, 1.0 );
}

