varying lowp vec4 colorVarying;
varying lowp vec2 texcoord;

uniform sampler2D texture;

void main()
{
    mediump vec4 sum = vec4(0.0);
    int i;
    int j;
    
    for(i=-4; i<4; i++) {
   		for(j=-3; j<3; j++) {
   			sum += texture2D(texture, texcoord+vec2(j,i)*0.004) * 0.025;
   		}
    }
    lowp vec4 col = texture2D(texture, texcoord);
    sum = sum * sum;
    if(col.x < 0.3) {
   		gl_FragColor = sum*0.012 + col;
    } else {
   		if(col.x < 0.5) {
   			gl_FragColor = sum*0.009 + col;
   		} else {
   			gl_FragColor = sum*0.0075 + col;
   		}
    }
    gl_FragColor  = gl_FragColor * 2.0;
}
