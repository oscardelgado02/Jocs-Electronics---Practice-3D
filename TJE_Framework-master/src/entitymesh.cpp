#include "entitymesh.h"


EntityMesh::EntityMesh(std::string name, Matrix44 model, Mesh* mesh, Texture* texture, Shader* shader, Vector4 color) : Entity(name, model) {
	this->mesh = mesh;
	this->texture = texture;
	this->shader = shader;
	this->color = color;
}

EntityMesh::~EntityMesh() {
	/*delete mesh;
	delete texture;
	delete shader;
	*/
}

void EntityMesh::render()
{
	//get the last camera that was activated
	Camera* camera = Camera::current;
	Matrix44 model = this->model;
	std::vector<Light*> lights = LightManager::getInstance()->getLights();

	if (shader && checkFrustum())
	{
		//enable shader
		shader->enable();

		//multipass
		multiPass(lights, camera);

		//disable shader
		shader->disable();
	}
}

void EntityMesh::update(float dt) {
	
}

bool EntityMesh::checkFrustum() {
	//Frustum variables
	float loadDistance = 200.0f;
	float no_render_dist = 1000.0f;

	//current camera
	Camera* cam = Camera::current;

	float dist = distanceToCam();
	if (dist > no_render_dist) {
		return false;
	}

	if (dist < loadDistance) {
		//set la calidad a mas pocha de los modelos
	}

	BoundingBox worldAABB = transformBoundingBox(model, mesh->box);
	if (!cam->testBoxInFrustum(worldAABB.center, worldAABB.halfsize)) {
		return false;
	}

	return true;
}

void EntityMesh::setUniforms(Light* light, Camera* camera) {
	shader->setUniform("u_color", color);
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_texture", texture, 0);
	shader->setUniform("u_model", model);
	shader->setUniform("u_time", time);
	shader->setUniform("u_camera_position", camera->eye);
	shader->setUniform("u_light_color", light->color);
	shader->setUniform("u_light_position", light->position);
	shader->setUniform("u_shininess", light->shininess);
	shader->setUniform("u_max_distance", light->max_distance);
	shader->setUniform("u_intensity", light->intensity);
}

void EntityMesh::multiPass(std::vector<Light*> lights, Camera* camera) {
	//allow to render pixels that have the same depth as the one in the depth buffer
	glDepthFunc(GL_LEQUAL);

	//set blending mode to additive
	//this will collide with materials with blend...
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	for (int i = 0; i < lights.size(); ++i)
	{
		//first pass doesn't use blending
		if (i == 0)
			glDisable(GL_BLEND);
		else
			glEnable(GL_BLEND);

		//pass the light data to the shader
		setUniforms(lights[i], camera);

		//do the draw call
		mesh->render(GL_TRIANGLES);
	}

	glDisable(GL_BLEND);
	glDepthFunc(GL_LESS); //as default

}