#include<dinosaur_model/dinosaur.h>
#include<dinosaur_model/road.h>
#include<dinosaur_model/obstacle.h>
#include<dinosaur_model/cloud.h>
#include<dinosaur_model/Object.h>

bool draw();
void draw_dinosaur();
void draw_road();
void draw_obstacle();
void draw_cloud();

bool dinosaur_move();
void road_move();
void obstacle_move();
void cloud_move();

void pushImage(Object obj);
void game_start();
void game_over();
void game_init();
void set_max_score();
bool collision_detection(Object dinosaur,Object obstacle);