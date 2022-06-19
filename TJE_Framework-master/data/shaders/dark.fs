
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform float u_time;
uniform vec3 u_camera_position;
uniform vec3 u_sky_ambient;
uniform vec3 u_sun_color;
uniform float u_shininess;

void main()
{
	vec2 uv = v_uv;
	vec3 N = normalize(v_normal);

	vec3 V = normalize(u_camera_position - v_world_position);
	vec3 R = reflect(V,N);
	vec3 L = normalize(v_position - v_world_position);

	vec3 sky_ambient = normalize(u_sky_ambient);
	vec3 sun_color = normalize(u_sun_color);

	vec3 color = 0.15 * u_color.xyz;

	//ambient
	vec3 ambient = color * sky_ambient;

	//diffuse
	vec3 diffuse = color * sun_color * clamp(dot(N,L), 0.0, 1.0);

	//specular (careful with the glossiness levels)
	vec3 specular = color * sun_color * pow( clamp(dot(R, V), 0.0, 1.0), u_shininess);

	color = ambient + diffuse + specular;

	gl_FragColor = vec4(color, 1.0) * texture2D(u_texture, uv);
}
