precision mediump float;
uniform sampler2D u_texture;
uniform float u_alpha;
varying vec2 v_uv;

void main()
{
	vec4 textColor =  texture2D(u_texture,v_uv);
	gl_FragColor = vec4(textColor.rgb, textColor.a * u_alpha);
}
