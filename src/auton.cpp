#include "main.h"
#include "math.h"
#include <chrono>
#include <cstdint>
#include <iostream>
#include "autoSelect/selection.h"

#define close_turn 90
#define auton 0
#define M_PI 3.14159265
using namespace std;
//using namespace pros;

// GLOBAL POSITION VARIABLES
double x = 0;
double y= 0;
// CURRENT MOTOR ENCODER TICKS
double BL, BR, ML, MR, FL, FR;
/*
 * @param x - the untransformed displacement vector
 * @param theta - current IMU reading for the bot IN RADIANS
 * @return - pair of doubles of the transformed displacements x, y
 */

static pair<double, double> Transform(double x, double theta) {
    double x2 = (x * cos(theta));
    double y2 = (x * sin(theta));
    // std::pair<double, double> coords;
    // coords.first = x2;
    // coords.second = y2;
    return make_pair(x2, y2);
}

/*
 * updates the encoder values
 * @return - double of the averaged motor displacement
 */

static double Average() {
     double x1 = (BackLeftWheel.get_position() - BL)
     - (BackRightWheel.get_position() - BR);//(-(FrontLeftWheel.get_position() - FR) -
    //            (MiddleLeftWheel.get_position() - ML) +
    //            (BackLeftWheel.get_position() - BL) +
    //            (FrontRightWheel.get_position() - FR) +
    //            (MiddleRightWheel.get_position() - MR) -
    //            (BackRightWheel.get_position() - BR));

        //        FrontLeftWheel.move_voltage(-12000);
        // MiddleLeftWheel.move_voltage(-12000);
        // BackLeftWheel.move_voltage(12000);
        // FrontRightWheel.move_voltage(12000);
        // MiddleRightWheel.move_voltage(12000);
        // BackRightWheel.move_voltage(-12000);


    BR = BackRightWheel.get_position();
    BL = BackLeftWheel.get_position();
    ML = MiddleLeftWheel.get_position();
    MR = MiddleRightWheel.get_position();
    FR = FrontRightWheel.get_position();
    FL = FrontLeftWheel.get_position();
    return x1/2;
}

/*
 * Init the autonomous tracking
 * Run at thew beginning of any autonomous routine
 */

static void Setup() {

    FrontRightWheel.tare_position();
    FrontLeftWheel.tare_position();
    MiddleRightWheel.tare_position();
    MiddleLeftWheel.tare_position();
    BackRightWheel.tare_position();
    BackLeftWheel.tare_position();
    x = 0;
    y = 0;
    pros::lcd::set_text(5, "a;lksdjf");
}

void Track(void * param) {
    while (true){
    double displacement = Average();
    pair<double, double> disp =
        Transform(displacement, Inertial.get_heading() * M_PI / 180);
    x += disp.first;
    y += disp.second;
    pros::lcd::set_text(6, "X: " + to_string(x) + ", " + to_string(atan(y/x)));
    pros::lcd::set_text(7, "Y:" + to_string(y));
    pros::delay(3);
    
    }
}


void initialize() {
//     lv_obj_t * img_src = lv_img_create(lv_scr_act(), NULL); /*Crate an image
//     // object*/ lv_img_set_src(img_src, &gojo);  /*Set the created file as image
//     // (a red fl  ower)*/ lv_obj_set_pos(img_src, 0, 0);      /*Set the
//     // positions*/ lv_obj_set_drag(img_src, true);
    // pros::delay(15000);
    selector::init();
    pros::lcd::initialize();
    Inertial.reset();
    pros::delay(3000);
    //pros::lcd::set_text(1, "Init");
    pros::lcd::set_text(3, to_string(auton));
    Setup();
    pros::task_t  trackTask = pros::c::task_create(Track, (void*)"", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Track");
  pros::Task tracking(trackTask);

}
//-1 == gilbert's left side autonomous
// 0 == elims
// 1 == small and large neutral
// 2 == winpoint auton
// 3 == do nothing
// 4 == prog skills
// 5 = left side neutral rush w/ pos tracking
// 6 == right side neutral rush
// double KPBASETURN = 0.2;
double KPBASETURN = 0.2;
//LV_IMG_DECLARE(gojo);




void competition_initialize() { initialize(); }






bool toClamp = false;
bool val = false;

void backClamp(bool v) {
  val = v;
  toClamp = true;
}

// void fire() {
//     Indexer.set_value(false);
//     pros::delay(400);
//     Indexer.set_value(true);
// }

// void clamp(void * param) {

//   while (true) {
//     if (val && toClamp) {
//       Tilter.set_value(val);

//       pros::delay(400);
//       BackPiston.set_value(val);
//       toClamp = false;
//     }
//     else if (toClamp){



//       BackPiston.set_value(val);
//       pros::delay(200);
//       Tilter.set_value(val);
//       toClamp = false;

//     }
//     pros::delay(5);
//   }
// }

void autonomous() {

  // pros::task_t  task = pros::c::task_create(clamp, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Task Name");
  // pros::Task bClamp(task);

    stopHold();
    if (auton == 0) {
        //      DiskIntake.move_relative(500,85);

        // move(550, 90, 127);
        // turn(180, 3, 127);
        // move(800, 180, 127);
        // turn(270, 3, 127);

        FrontLeftWheel.move(+50);
        MiddleLeftWheel.move(+50);
        BackLeftWheel.move(+50);
        FrontRightWheel.move(+50);
        MiddleRightWheel.move(+50);
        BackRightWheel.move(+50);
        pros::delay(1000);
        DiskIntake.move_relative(450,85);
        FrontLeftWheel.move(0);
        MiddleLeftWheel.move(0);
        BackLeftWheel.move(0);
        FrontRightWheel.move(0);
        MiddleRightWheel.move(0);
        BackRightWheel.move(0);
    } else if (auton == 1) {
        autonShoot();
        pros::delay(1200);
        autonShoot();
        Flywheel.move(0);
    }

    // test
    else {

        pros::lcd::set_text(3, "running...");
        //Setup();
        pros::lcd::set_text(4, "Init over");

        //while (true) Track();
    }
}

