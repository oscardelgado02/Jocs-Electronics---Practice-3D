#include "ia.h"

IA::IA() {
	//the map info should be an array W*H of bytes where 0 means block, 1 means walkable
	grid = new uint8[W * H];

	for (size_t i = 0; i < W * H; i++) {
		grid[i] = 1;
	}
}

Vector3 IA::sendStep(int idx) {
	if (path_steps > 0) {
		int index = output[idx];
		int x = index % W;
		int y = index / W;
		Vector3 pos;
		pos.x = x * tileSizeX;
		pos.z = y * tileSizeY;

		return pos;
	}

	return Vector3(NULL, NULL, NULL);
}

void IA::setCenter(Vector3 position) {
	startx = clamp(position.x / tileSizeX, 0, W);
	starty = clamp(position.z / tileSizeY, 0, H);
}

void IA::setTarget() {
	Camera* camera = Camera::current;

	targetx = clamp(camera->eye.x / tileSizeX, 0, W);
	targety = clamp(camera->eye.z / tileSizeY, 0, H);

	//we call the path function, it returns the number of steps to reach target, otherwise 0
	path_steps = AStarFindPathNoTieDiag(
		startx, starty, //origin (tienen que ser enteros)
		targetx, targety, //target (tienen que ser enteros)
		grid, //pointer to map data
		W, H, //map width and height
		output, //pointer where the final path will be stored
		100); //max supported steps of the final path

	//check if there was a path
	if (path_steps != -1)
	{
		for (int i = 0; i < path_steps; ++i)
			std::cout << "X: " << (output[i] % W) << ", Y: " << floor(output[i] / W) << std::endl;
	}
}