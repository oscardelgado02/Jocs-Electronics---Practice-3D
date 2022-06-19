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

		//upload uniforms
		setUniforms(lights[0], camera);

		//do the draw call
		mesh->render(GL_TRIANGLES);

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
	shader->setUniform("u_shininess", light->shininess);
	shader->setUniform("u_intensity", light->intensity);
}