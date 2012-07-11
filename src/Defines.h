
#ifndef _AQUAERO_DEFINES
#define _AQUAERO_DEFINES

/* =================== BOOLEAN DEFINES ===================== */
//#define _WEBCAM_INPUT
#define _DEPLOYMENT
#define _DRAW_ALL_VIDEOS
#define _PLAY_MUSIC
#define _THREADING

/* ===================== VIDEO INPUT ======================= */

#ifdef _WEBCAM_INPUT
	#define WEBCAM_ID 1
	#define MIC_ID 4
	#ifndef _DRAW_ALL_VIDEOS
		#define CAM_W 640
		#define CAM_H 480
		#define CAM_SCALE_X 2 //  = (1 / CAM_W * WIN_W)
		#define CAM_SCALE_Y 1.666 // = (1 / CAM_H * WIN_H)
	#else
		#define CAM_W 320
		#define CAM_H 240
		#define CAM_SCALE_X 4
		#define CAM_SCALE_Y 3.333
	#endif
#else
	#define CAM_W 320
	#define CAM_H 240
	#define CAM_SCALE_X 4
	#define CAM_SCALE_Y 3.333
#endif
#define CAM_AREA_DIV CAM_W * CAM_H / 3

#define WIN_W 1280
#define WIN_H 800
#define WIN_W_HALF 640 // = 1280/2
#define WIN_H_HALF 400

#define CAM_INV_SCALE_Y 0.3

#define FLIP_V true
#define FLIP_H true


/* =================== BLOB DETECTION ====================== */
#define THRESHOLD 90
#define MAX_POINT_RANGE 0.65f


/* ===================== CREATURES ======================== */
#define SPAWN_TIMEOUT 3600
#define SPAWN_THRESHOLD 20 // frames in a row
#define CREATURE_WIGGLE_AMT 8
#define CREATURE_WIGGLE_SPEED .0001
#define CREATURE_COUNT_MAX 5

 
/* ====================== 3D CAMERA ======================== */
#define SCROLL_SPEED 1.2f
#define MAX_SCREEN_HEIGHT -6350//-6280
#define MIN_SCREEN_HEIGHT 800


/* ============== ENVINRONMENTAL REACTIONS ================= */
enum ENV_STATE { 
	ENV_BASIC, ENV_VENTS, ENV_BUBBLES, 
	ENV_STORM, ENV_COMET, ENV_METEOR 
};

enum ENV_ZONE { 
	DEEP_SEA, CORAL_REEF, MOUNTAIN_RANGE, 
	ATMOSPHERE, WACKY_SPACE 
};

#define FLOCK_COUNT_MAX 10
#define JELLY_FRAMES 59

/* ==================== VECTOR FIELD ======================= */
#define VF_FADE_AMOUNT 0.5f//0.99f
#define VF_RADIUS 100//40
#define VF_STRENGTH 10.0f //0.03f


/* ===================== Z-INDICES ========================= */
#define DEPTH_VIDEO 198
#define DEPTH_CREATURES 200
#define DEPTH_HANDS 201
#define DEPTH_ENV_REACTIVE 199
#define DEPTH_ENV 202
#define DEPTH_VENTS -2


/* =================== BOX2D PHYSICS ======================= */
#define FRAMERATE 60.0f
#define GRAVITY_X 0.0f
#define GRAVITY_Y 0.0f

#endif