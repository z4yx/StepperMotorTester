#include "params.h"

struct ItemNumberModel MotorVelocity = {.min=14, .max=200, .val=60};
struct ItemNumberModel MotorFreq = {.min=1, .max=3000, .val=1000};
struct ItemNumberModel MotorSteps = {.min=1, .max=200*16, .val=100};
struct ItemStringModel MotorMicroStep = {
	.size = 4, .selected = 0, .name = {
		"1", "2", "4", "16"
	}
};

struct ItemNumberModel MeasureVelocity = {.val=0};
struct ItemNumberModel MeasureAngle = {.val=0};


