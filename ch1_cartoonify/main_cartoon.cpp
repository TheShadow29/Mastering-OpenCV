#include "cartoon.cpp"

using namespace cv;
using namespace std;

void init_cam(VideoCapture &camera, int camera_number)
{
	camera.open(camera_number);
	if (!camera.isOpened())
	{
		cerr << "Error: Could not open Camera" << endl;
		exit(1);
	}
	camera.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	camera.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
}

int main(int argc, char *argv[])
{
	int camera_number = 0;
	bool sketch_mode = 0;
	bool evil_mode = 0;
	bool alien_mode = 0;
	if (argc > 1)
	{
		camera_number = atoi(argv[1]);
	}
	VideoCapture camera;
	init_cam(camera, camera_number);

	while (true)
	{
		Mat camera_frame;
		camera >> camera_frame;

		if (camera_frame.empty())
		{
			cerr << "Couldn't grab camera frame" << endl;
			exit(1);
		}

		Mat displayed_framed (camera_frame.size(), CV_8UC3);

		cartoonify_image(camera_frame, displayed_framed, sketch_mode, evil_mode, alien_mode);

		imshow("Cartoonified", displayed_framed);

		char keypress = waitKey(20);
		if (keypress == 27) //ESC key
		{
			break;
		}
		if (keypress == 's') //sketch
		{
			sketch_mode = !sketch_mode;
		}
		if (keypress == 'e')
		{
			evil_mode = !evil_mode;
		}
		if(keypress == 'a')
		{
			alien_mode = !alien_mode;
		}

	}
}