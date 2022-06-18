
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform float u_time;

void main()
{
	vec2 uv = v_uv;
	vec3 N = normalize(v_normal);

	vec4 color = u_color*0.25;

	gl_FragColor = color * texture2D( u_texture, uv );
}
