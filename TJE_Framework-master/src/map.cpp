#include "map.h"

Map::Map() {
	world = World::getInstance();
}

void Map::loadEntity(int type, Mesh* mesh, Texture* texture, Vector3 position, Vector3 rotation) {
	
	//creating model for the entity
	Matrix44 model;
	model.translate(position.x, position.y, position.z);
	//APPLY ROTATION
	model.rotate((PI / 180) * rotation.x, Vector3(0, 1, 0)); //rotate in x
	model.rotate((PI / 180) * rotation.y, Vector3(0, 1, 0)); //rotate in y
	model.rotate((PI / 180) * rotation.z, Vector3(0, 0, 1)); //rotate in z

	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	switch (type) {
	case PLAYER: world->addPlayerEntity("player", model, mesh, texture, shader, Vector4(1, 1, 1, 1)); break;
	case ENEMY: world->addEnemyEntity("enemy", model, mesh, texture, shader, Vector4(1, 1, 1, 1)); break;
	case WALL: world->addWallEntity("player", model, mesh, texture, shader, Vector4(1, 1, 1, 1)); break;
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
		Texture* text = new Texture();
		text->load(line[TEXTURE].c_str());
		
		//Position
		std::vector<std::string> position_str = split(line[POSITION], ',');
		Vector3 position = Vector3(stof(position_str[0]), stof(position_str[1]), stof(position_str[2]));
		position = position * 10; //adapt scale

		//Rotation
		std::vector<std::string> rotation_str = split(line[ROTATION], ',');
		Vector3 rotation = Vector3(stof(rotation_str[0]), stof(rotation_str[1]), stof(rotation_str[2]));

		loadEntity(type, mesh, text, position, rotation);
	}
}