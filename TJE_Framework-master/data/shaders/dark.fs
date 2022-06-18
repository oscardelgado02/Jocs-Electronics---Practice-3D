
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform vec4 u_color;
uniform sampler2D u_texture;
//uniform sampler2D u_sky_texture;
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

	//sky_color
	//compute the yaw using arc tangent 
	//float yaw = atan(R.x,R.z) / 6.28318531;
	//yaw += -0.25; //in case the 0deg doesn’t match our Z axis, we can rotate 90 degrees

	//compute the pitch using arc sinus
	//float pitch = asin(R.y) / 1.57079633; //0 means bottom, 1 means top

	//build the UV vector for hemisphere (in case pitch is negative, clamp it to 0)
	//vec2 uv_reflection = vec2(yaw, clamp(pitch, 0.0, 1.0) );

	//read the sky texture (ignoring mipmaps to avoid problems)
	//vec3 sky_color = texture2DLod( sky_texture, uv, 0.0);


	vec3 color = 0.15 * u_color.xyz;

	//ambient
	vec3 ambient = color * sky_ambient;

	//diffuse
	vec3 diffuse = color * sun_color * clamp(dot(L, N), 0.0, 1.0);

	//specular (careful with the glossiness levels)
	vec3 specular = color * sun_color * pow( clamp(dot(R, V), 0.0, 1.0), u_shininess);

	color = ambient + diffuse + specular;
	//vec4 color = u_color * 0.2;

	gl_FragColor = vec4(color, 1.0) * texture2D(u_texture, uv);
}
