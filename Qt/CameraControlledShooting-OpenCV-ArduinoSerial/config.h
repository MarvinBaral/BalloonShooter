#ifndef CONFIG_H
#define CONFIG_H

//syntax: file_name
enum cam_params {MINIMUM_CTR = 0, WIDTH, HEIGHT, ANGLE_OF_VIEW_X, ANGLE_OF_VIEW_Y};
enum cam_color {BLUE = 0, GREEN, RED};
enum main_versions {V1_0 = 0, V1_1};

struct config {
	public:
		struct cam {

		};
		struct servo {

		};
		struct mc {

		};
		struct main {

		};
};

#endif // CONFIG_H
