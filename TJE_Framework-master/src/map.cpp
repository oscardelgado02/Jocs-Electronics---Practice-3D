#include "map.h"

Map::Map() {
	world = World::getInstance();
}

void Map::loadEntity(int type, Mesh* mesh, Texture* texture, Vector3 position, Vector3 rotation, Vector3 scale) {
	
	//creating model for the entity
	Matrix44 model;
	model.translate(position.x, position.y, position.z);
	//APPLY ROTATION
	model.rotate((PI / 180) * rotation.x, Vector3(0, 1, 0)); //rotate in x
	model.rotate((PI / 180) * rotation.y, Vector3(0, 0, 1)); //rotate in y
	model.rotate((PI / 180) * rotation.z, Vector3(1, 0, 0)); //rotate in z

	model.scale(10.0f*scale.x, 10.0f * scale.y, 10.0f * scale.z);

	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/dark.fs");

	switch (type) {
	case PLAYER: world->addPlayerEntity("player", model, mesh, texture, shader, Vector4(1, 1, 1, 1)); break;
	case ENEMY: world->addEnemyEntity("enemy", model, mesh, texture, Shader::Get("data/shaders/skinning.vs", "data/shaders/dark.fs"), Vector4(1, 1, 1, 1)); break;
	case WALL: world->addWallEntity("wall", model, mesh, texture, shader, Vector4(1, 1, 1, 1)); break;
	case GOAL: world->addWallEntity("goal", model, mesh, texture, shader, Vector4(1, 1, 1, 1)); break;
	}
}

void Map::loadMap(char* path) {
	std::string map_content;
	readFile(path, map_content);

	std::vector<std::string> lines = split(map_content, '\n');

	for (int i = 0; i < lines.size(); ++i) {
		std::vector<std::string> line = split(lines[i], ' ');

		int type = stoi(line[TYPE]);
		Mesh* mesh = Mesh::Get(line[MESH].c_str());
		Texture* text = Texture::Get(line[TEXTURE].c_str());
		
		//Position
		std::vector<std::string> position_str = split(line[POSITION], ',');
		Vector3 position = Vector3(stof(position_str[0]), stof(position_str[1]), stof(position_str[2]));
		position = position * 10; //adapt scale

		//Rotation
		std::vector<std::string> rotation_str = split(line[ROTATION], ',');
		Vector3 rotation = Vector3(stof(rotation_str[0]), stof(rotation_str[1]), stof(rotation_str[2]));

		//Scale
		std::vector<std::string> scale_str = split(line[SCALE], ',');
		Vector3 scale = Vector3(stof(scale_str[0]), stof(scale_str[1]), stof(scale_str[2]));

		loadEntity(type, mesh, text, position, rotation, scale);
	}
}

void Map::restartEntities(char* path) {
	std::string map_content;
	readFile(path, map_content);

	std::vector<std::string> lines = split(map_content, '\n');

	for (int i = 0; i < lines.size(); ++i) {
		std::vector<std::string> line = split(lines[i], ' ');

		//Position
		std::vector<std::string> position_str = split(line[POSITION], ',');
		Vector3 position = Vector3(stof(position_str[0]), stof(position_str[1]), stof(position_str[2]));
		position = position * 10; //adapt scale

		//Rotation
		std::vector<std::string> rotation_str = split(line[ROTATION], ',');
		Vector3 rotation = Vector3(stof(rotation_str[0]), stof(rotation_str[1]), stof(rotation_str[2]));

		//Scale
		std::vector<std::string> scale_str = split(line[SCALE], ',');
		Vector3 scale = Vector3(stof(scale_str[0]), stof(scale_str[1]), stof(scale_str[2]));

		world->restartEntity(position, rotation, scale, i);
	}
}