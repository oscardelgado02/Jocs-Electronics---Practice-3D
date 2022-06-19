
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform float u_time;
uniform vec3 u_camera_position;
uniform vec3 u_light_color;
uniform vec3 u_light_position;
uniform float u_shininess;
uniform float u_max_distance;
uniform float u_angle;
uniform float u_intensity;

void main()
{
	vec2 uv = v_uv;
	vec3 N = normalize(v_normal);

	vec3 light_color = normalize(u_light_color);

	vec3 V = normalize(u_camera_position - v_world_position);
	vec3 L = u_light_position - v_world_position;

	float distance = length(L);

	float global_intensity = 0.15;

	L/=distance;
	float atenuation = u_max_distance - distance;
	atenuation = atenuation/u_max_distance;
	atenuation = max(atenuation, 0.0);
	atenuation *= pow(atenuation,2);

	float NdotL = clamp(dot(N,L), 0.0, 1.0);

	vec3 R = L-2.0*NdotL*N;

	light_color+=((NdotL*u_light_color)*atenuation);

	vec3 color = global_intensity*u_color.xyz*(light_color);


	gl_FragColor = vec4(color, 1.0) * texture2D(u_texture, uv);
}
