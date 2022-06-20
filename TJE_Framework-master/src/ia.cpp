#include "ia.h"

IA::IA() {
	//the map info should be an array W*H of bytes where 0 means block, 1 means walkable
	W = 100;
	H = 100;
	tileSizeX = 10.0f;
	tileSizeY = 10.0f;

	grid = new uint8[W * H];

	for (size_t i = 0; i < W * H; i++) {
		grid[i] = 1;
	}
}

Vector3 IA::sendStep(Vector3 position) {

	Camera* camera = Camera::current;


	int startx = clamp(position.x / tileSizeX, 0, W);
	int starty = clamp(position.z / tileSizeY, 0, H);

	int targetx = clamp(camera->eye.x / tileSizeX, 0, W);
	int targety = clamp(camera->eye.z / tileSizeY, 0, H);

	int output[100];

	//we call the path function, it returns the number of steps to reach target, otherwise 0
	int path_steps = AStarFindPathNoTieDiag(
		startx, starty, //origin (tienen que ser enteros)
		targetx, targety, //target (tienen que ser enteros)
		grid, //pointer to map data
		W, H, //map width and height
		output, //pointer where the final path will be stored
		100); //max supported steps of the final path

	//check if there was a path
	if (path_steps != -1)
	{
		std::cout << "X: " << (output[0] % W) << ", Y: " << floor(output[0] / W) << std::endl;
	}


	if (path_steps > 0) {
		int index = output[0];
		int x = index % W;
		int y = index / W;
		Vector3 pos = Vector3(0.0f, 0.0f, 0.0f);
		pos.x = x * tileSizeX;
		pos.z = y * tileSizeY;

		return pos;
	}

	return Vector3(rand(), 0.0f, rand());
}