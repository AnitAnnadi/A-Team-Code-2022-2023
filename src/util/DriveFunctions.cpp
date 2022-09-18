#include "main.h"
using namespace std;

#define close_turn 90
#define auton 5
#define M_PI 3.14159265
#define KPBASETURN 2

void moveVoltage(int i, int angle, int speed) {
    // i = motor speed, angle = angle the robot should turn towards while moving

    FrontRightWheel.tare_position();
    FrontLeftWheel.tare_position();
    BackRightWheel.tare_position();
    BackLeftWheel.tare_position();
    int target = Inertial.get_heading();

    while (std::abs(FrontRightWheel.get_position()) < std::abs(i)) {

        int actual_turn = 0;
        //   int move = i < 0 ? -90 : 90;
        int move = i < 0 ? -127+speed : 127-speed;
        int tolerance = 3;
        int upperAngleBound = angle + tolerance;
        int lowerAngleBound = angle - tolerance;

        int angle_ = (Inertial.get_heading());
        bool specialDown = false;
        bool specialUp = false;
        if (lowerAngleBound < 0) {
            lowerAngleBound = lowerAngleBound + 360;
            specialDown = true;
        }
        if (upperAngleBound > 360) {
            upperAngleBound = upperAngleBound - 360;
            specialUp = true;
        }
        // if the robot is not at the heading
        if ((specialDown == true &&
             ((angle_ < lowerAngleBound && angle_ > angle + 180) ||
              (angle_ > upperAngleBound && angle_ < angle + 180))) ||
            (specialUp == true &&
             ((angle_ > upperAngleBound && angle_ < angle - 180) ||
              (angle_ < lowerAngleBound && angle_ > angle - 180))) ||
            ((specialUp == false && specialDown == false) &&
             (angle_ > upperAngleBound || angle_ < lowerAngleBound))) {
            angle_ = (Inertial.get_heading());

            float turn_difference = angle_ - angle;

            if (turn_difference > 180)
                turn_difference = 360 - turn_difference;
            if (turn_difference < -180)
                turn_difference = 360 + turn_difference;
            if (turn_difference < 0 && turn_difference > -180)
                turn_difference = -turn_difference;

            // Spins in the directions which will allow bot to complete turn
            // fastest
            if (turn_difference > 180)
                turn_difference = 360 - turn_difference;

            // Slows down if close to goal heading and stays fast if it is away
            int turn_speed = 150;
            if (turn_difference < close_turn) {
                actual_turn = (turn_speed *
                               ((turn_difference / (close_turn)) + KPBASETURN));
            } else {
                actual_turn = turn_speed;
            }

            /*Special conditions if angle bounds are less than 0 or greater than
               360 Neccesary for proper turning and calculation*/
            if ((angle > angle_ + 180) ||
                (angle > angle_ - 180 && angle_ > 180 && angle < 180) ||
                (angle < angle_ && angle_ - angle < 180))
                actual_turn = -actual_turn;
        }
        int c = 5;
        if (i < 0) {

            c = -c;
        }
        //Track();
        // FrontLeftWheel.move(-move - actual_turn);
        // MiddleLeftWheel.move(-move - actual_turn);
        // BackLeftWheel.move(+move + actual_turn);
        // FrontRightWheel.move(+move - actual_turn);
        // MiddleRightWheel.move(+move - actual_turn);
        // BackRightWheel.move(-move + actual_turn);
        //actual_turn = 0;
        // FrontLeftWheel.move_voltage((12000/127)*(-move - actual_turn));
        // MiddleLeftWheel.move_voltage((12000/127)*(-move - actual_turn));
        // BackLeftWheel.move_voltage((12000/127)*(+move + actual_turn));
        // FrontRightWheel.move_voltage((12000/127)*(+move - actual_turn));
        // MiddleRightWheel.move_voltage((12000/127)*(+move - actual_turn));
        // BackRightWheel.move_voltage((12000/127)*(-move + actual_turn));
        FrontLeftWheel.move_voltage(-12000);
        MiddleLeftWheel.move_voltage(-12000);
        BackLeftWheel.move_voltage(12000);
        FrontRightWheel.move_voltage(12000);
        MiddleRightWheel.move_voltage(12000);
        BackRightWheel.move_voltage(-12000);
        pros::delay(10);
    }

    // Moves the wheels at a certain "move" speed
    FrontRightWheel.move(0);
    FrontLeftWheel.move(0);
    BackRightWheel.move(0);
    BackLeftWheel.move(0);
    MiddleRightWheel.move(0);
    MiddleLeftWheel.move(0);
}

void move(int i, int angle, int speed) {
    // i = motor speed, angle = angle the robot should turn towards while moving

    FrontRightWheel.tare_position();
    FrontLeftWheel.tare_position();
    BackRightWheel.tare_position();
    BackLeftWheel.tare_position();
    int target = Inertial.get_heading();

    while (std::abs(FrontRightWheel.get_position()) < std::abs(i)) {

        int actual_turn = 0;
        //   int move = i < 0 ? -90 : 90;
        int move = i < 0 ? -127+speed : 127-speed;
        int tolerance = 3;
        int upperAngleBound = angle + tolerance;
        int lowerAngleBound = angle - tolerance;

        int angle_ = (Inertial.get_heading());
        bool specialDown = false;
        bool specialUp = false;
        if (lowerAngleBound < 0) {
            lowerAngleBound = lowerAngleBound + 360;
            specialDown = true;
        }
        if (upperAngleBound > 360) {
            upperAngleBound = upperAngleBound - 360;
            specialUp = true;
        }
        // if the robot is not at the heading
        if ((specialDown == true &&
             ((angle_ < lowerAngleBound && angle_ > angle + 180) ||
              (angle_ > upperAngleBound && angle_ < angle + 180))) ||
            (specialUp == true &&
             ((angle_ > upperAngleBound && angle_ < angle - 180) ||
              (angle_ < lowerAngleBound && angle_ > angle - 180))) ||
            ((specialUp == false && specialDown == false) &&
             (angle_ > upperAngleBound || angle_ < lowerAngleBound))) {
            angle_ = (Inertial.get_heading());

            float turn_difference = angle_ - angle;

            if (turn_difference > 180)
                turn_difference = 360 - turn_difference;
            if (turn_difference < -180)
                turn_difference = 360 + turn_difference;
            if (turn_difference < 0 && turn_difference > -180)
                turn_difference = -turn_difference;

            // Spins in the directions which will allow bot to complete turn
            // fastest
            if (turn_difference > 180)
                turn_difference = 360 - turn_difference;

            // Slows down if close to goal heading and stays fast if it is away
            int turn_speed = 150;
            if (turn_difference < close_turn) {
                actual_turn = (turn_speed *
                               ((turn_difference / (close_turn)) + KPBASETURN));
            } else {
                actual_turn = turn_speed;
            }

            /*Special conditions if angle bounds are less than 0 or greater than
               360 Neccesary for proper turning and calculation*/
            if ((angle > angle_ + 180) ||
                (angle > angle_ - 180 && angle_ > 180 && angle < 180) ||
                (angle < angle_ && angle_ - angle < 180))
                actual_turn = -actual_turn;
        }
        int c = 5;
        if (i < 0) {

            c = -c;
        }
        //Track();
        // FrontLeftWheel.move(-move - actual_turn);
        // MiddleLeftWheel.move(-move - actual_turn);
        // BackLeftWheel.move(+move + actual_turn);
        // FrontRightWheel.move(+move - actual_turn);
        // MiddleRightWheel.move(+move - actual_turn);
        // BackRightWheel.move(-move + actual_turn);
        FrontLeftWheel.move_voltage((12000/127)*(-move - actual_turn));
        MiddleLeftWheel.move_voltage((12000/127)*(-move - actual_turn));
        BackLeftWheel.move_voltage((12000/127)*(+move + actual_turn));
        FrontRightWheel.move_voltage((12000/127)*(+move - actual_turn));
        MiddleRightWheel.move_voltage((12000/127)*(+move - actual_turn));
        BackRightWheel.move_voltage((12000/127)*(-move + actual_turn));
        pros::delay(10);
    }

    // Moves the wheels at a certain "move" speed
    FrontRightWheel.move(0);
    FrontLeftWheel.move(0);
    BackRightWheel.move(0);
    BackLeftWheel.move(0);
    MiddleRightWheel.move(0);
    MiddleLeftWheel.move(0);
}


void move(int i, int angle) {
    // i = motor speed, angle = angle the robot should turn towards while moving

    FrontRightWheel.tare_position();
    FrontLeftWheel.tare_position();
    BackRightWheel.tare_position();
    BackLeftWheel.tare_position();
    int target = Inertial.get_heading();

    while (std::abs(FrontRightWheel.get_position()) < std::abs(i)) {

        int actual_turn = 0;
        //   int move = i < 0 ? -90 : 90;
        int move = i < 0 ? -127 : 127;
        int tolerance = 3;
        int upperAngleBound = angle + tolerance;
        int lowerAngleBound = angle - tolerance;

        int angle_ = (Inertial.get_heading());
        bool specialDown = false;
        bool specialUp = false;
        if (lowerAngleBound < 0) {
            lowerAngleBound = lowerAngleBound + 360;
            specialDown = true;
        }
        if (upperAngleBound > 360) {
            upperAngleBound = upperAngleBound - 360;
            specialUp = true;
        }
        // if the robot is not at the heading
        if ((specialDown == true &&
             ((angle_ < lowerAngleBound && angle_ > angle + 180) ||
              (angle_ > upperAngleBound && angle_ < angle + 180))) ||
            (specialUp == true &&
             ((angle_ > upperAngleBound && angle_ < angle - 180) ||
              (angle_ < lowerAngleBound && angle_ > angle - 180))) ||
            ((specialUp == false && specialDown == false) &&
             (angle_ > upperAngleBound || angle_ < lowerAngleBound))) {
            angle_ = (Inertial.get_heading());

            float turn_difference = angle_ - angle;

            if (turn_difference > 180)
                turn_difference = 360 - turn_difference;
            if (turn_difference < -180)
                turn_difference = 360 + turn_difference;
            if (turn_difference < 0 && turn_difference > -180)
                turn_difference = -turn_difference;

            // Spins in the directions which will allow bot to complete turn
            // fastest
            if (turn_difference > 180)
                turn_difference = 360 - turn_difference;

            // Slows down if close to goal heading and stays fast if it is away
            int turn_speed = 150;
            if (turn_difference < close_turn) {
                actual_turn = (turn_speed *
                               ((turn_difference / (close_turn)) + KPBASETURN));
            } else {
                actual_turn = turn_speed;
            }

            /*Special conditions if angle bounds are less than 0 or greater than
               360 Neccesary for proper turning and calculation*/
            if ((angle > angle_ + 180) ||
                (angle > angle_ - 180 && angle_ > 180 && angle < 180) ||
                (angle < angle_ && angle_ - angle < 180))
                actual_turn = -actual_turn;
        }
        int c = 5;
        if (i < 0) {

            c = -c;
        }
        //Track();
        // FrontLeftWheel.move(-move - actual_turn);
        // MiddleLeftWheel.move(-move - actual_turn);
        // BackLeftWheel.move(+move + actual_turn);
        // FrontRightWheel.move(+move - actual_turn);
        // MiddleRightWheel.move(+move - actual_turn);
        // BackRightWheel.move(-move + actual_turn);
        FrontLeftWheel.move_voltage((12000/127)*(-move - actual_turn));
        MiddleLeftWheel.move_voltage((12000/127)*(-move - actual_turn));
        BackLeftWheel.move_voltage((12000/127)*(+move + actual_turn));
        FrontRightWheel.move_voltage((12000/127)*(+move - actual_turn));
        MiddleRightWheel.move_voltage((12000/127)*(+move - actual_turn));
        BackRightWheel.move_voltage((12000/127)*(-move + actual_turn));
        pros::delay(10);
    }

    // Moves the wheels at a certain "move" speed
    FrontRightWheel.move(0);
    FrontLeftWheel.move(0);
    BackRightWheel.move(0);
    BackLeftWheel.move(0);
    MiddleRightWheel.move(0);
    MiddleLeftWheel.move(0);
    FrontLeftWheel.move_voltage(0);
    MiddleLeftWheel.move_voltage(0);
    BackLeftWheel.move_voltage(0);
    FrontRightWheel.move_voltage(0);
    MiddleRightWheel.move_voltage(0);
    BackRightWheel.move_voltage(0);
}

void stopHold() {

    FrontLeftWheel.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    FrontRightWheel.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

    MiddleLeftWheel.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    MiddleRightWheel.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

    BackRightWheel.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    BackLeftWheel.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

    FrontLeftWheel.move(0);
    FrontRightWheel.move(0);

    MiddleRightWheel.move(0);
    MiddleLeftWheel.move(0);

    BackRightWheel.move(0);
    BackLeftWheel.move(0);
}

void turn(int angle, int tolerance, int turn_speed, int PIDRATIO) {

    int upperAngleBound = angle + tolerance;
    int lowerAngleBound = angle - tolerance;

    long begin_time = pros::millis();

    int angle_ = (Inertial.get_heading());

    // Special conditions (If the angle is at 0 for example)

    bool specialDown = false;
    bool specialUp = false;

    if (lowerAngleBound < 0) {
        lowerAngleBound = lowerAngleBound + 360;
        specialDown = true;
    }
    if (upperAngleBound > 360) {
        upperAngleBound = upperAngleBound - 360;
        specialUp = true;
    }

    while ((specialDown == true &&
            ((angle_ < lowerAngleBound && angle_ > angle + 180) ||
             (angle_ > upperAngleBound && angle_ < angle + 180))) ||
           (specialUp == true &&
            ((angle_ > upperAngleBound && angle_ < angle - 180) ||
             (angle_ < lowerAngleBound && angle_ > angle - 180))) ||
           ((specialUp == false && specialDown == false) &&
            (angle_ > upperAngleBound || angle_ < lowerAngleBound))) {

        angle_ = (Inertial.get_heading());

        double turn_difference = angle_ - angle;
        double actual_turn = 0;
        if (turn_difference > 180)
            turn_difference = 360 - turn_difference;
        if (turn_difference < -180)
            turn_difference = 360 + turn_difference;
        if (turn_difference < 0 && turn_difference > -180)
            turn_difference = -turn_difference;

        // Spins in the directions which will allow bot to complete turn fastest
        if (turn_difference > 180)
            turn_difference = 360 - turn_difference;

        // Slows down if close to goal heading and stays fast if it is away
        if (turn_difference < close_turn) {

            actual_turn =
                (turn_speed * ((turn_difference / (close_turn)) ))*1.6* PIDRATIO + KPBASETURN;
        } else {
            actual_turn = turn_speed;
        }

        /*Special conditions if angle bounds are less than 0 or greater than 360
                Neccesary for proper turning and calculation*/
        if ((angle > angle_ + 180) ||
            (angle > angle_ - 180 && angle_ > 180 && angle < 180) ||
            (angle < angle_ && angle_ - angle < 180))
            actual_turn = -actual_turn;

            //Track();
        FrontLeftWheel.move(-actual_turn);
        MiddleLeftWheel.move(-actual_turn);
        BackLeftWheel.move(actual_turn);
        FrontRightWheel.move(-actual_turn);
        MiddleRightWheel.move(-actual_turn);
        BackRightWheel.move(actual_turn);

        pros::lcd::set_text(1, std::to_string(angle_));
        pros::lcd::set_text(2, std::to_string(angle));
        pros::lcd::set_text(3, std::to_string(actual_turn));

        pros::delay(5);
    }

    stopHold();

    pros::lcd::set_text(4, "Goal reached");
}

void turn(int angle, int tolerance, int turn_speed) {

    int upperAngleBound = angle + tolerance;
    int lowerAngleBound = angle - tolerance;

    long begin_time = pros::millis();

    int angle_ = (Inertial.get_heading());

    // Special conditions (If the angle is at 0 for example)

    bool specialDown = false;
    bool specialUp = false;

    if (lowerAngleBound < 0) {
        lowerAngleBound = lowerAngleBound + 360;
        specialDown = true;
    }
    if (upperAngleBound > 360) {
        upperAngleBound = upperAngleBound - 360;
        specialUp = true;
    }

    while ((specialDown == true &&
            ((angle_ < lowerAngleBound && angle_ > angle + 180) ||
             (angle_ > upperAngleBound && angle_ < angle + 180))) ||
           (specialUp == true &&
            ((angle_ > upperAngleBound && angle_ < angle - 180) ||
             (angle_ < lowerAngleBound && angle_ > angle - 180))) ||
           ((specialUp == false && specialDown == false) &&
            (angle_ > upperAngleBound || angle_ < lowerAngleBound))) {

        angle_ = (Inertial.get_heading());

        double turn_difference = angle_ - angle;
        double actual_turn = 0;
        if (turn_difference > 180)
            turn_difference = 360 - turn_difference;
        if (turn_difference < -180)
            turn_difference = 360 + turn_difference;
        if (turn_difference < 0 && turn_difference > -180)
            turn_difference = -turn_difference;

        // Spins in the directions which will allow bot to complete turn fastest
        if (turn_difference > 180)
            turn_difference = 360 - turn_difference;

        // Slows down if close to goal heading and stays fast if it is away
        if (turn_difference < close_turn) {

            actual_turn =
                (turn_speed * ((turn_difference / (close_turn)) ))*1.6 + KPBASETURN;
        } else {
            actual_turn = turn_speed;
        }

        /*Special conditions if angle bounds are less than 0 or greater than 360
                Neccesary for proper turning and calculation*/
        if ((angle > angle_ + 180) ||
            (angle > angle_ - 180 && angle_ > 180 && angle < 180) ||
            (angle < angle_ && angle_ - angle < 180))
            actual_turn = -actual_turn;

            //Track();
        FrontLeftWheel.move(-actual_turn);
        MiddleLeftWheel.move(-actual_turn);
        BackLeftWheel.move(actual_turn);
        FrontRightWheel.move(-actual_turn);
        MiddleRightWheel.move(-actual_turn);
        BackRightWheel.move(actual_turn);

        pros::lcd::set_text(1, std::to_string(angle_));
        pros::lcd::set_text(2, std::to_string(angle));
        pros::lcd::set_text(3, std::to_string(actual_turn));

        pros::delay(5);
    }

    stopHold();

    pros::lcd::set_text(4, "Goal reached");
}


void moveCoords(int x1, int y1, int fowardSpeed, bool backwards) {
    int angle = 0;
    if (x1-x == 0) angle = 0;
    else angle = atan((y1-y)/(x1-x)) * 180/ M_PI;
    //1,10
    //

    //if (!simul) turnPID(angle, 3);
    int moveTolerance = 100;
    int dx = x1 - x;
    int dy = y1 - y;
    while (true) {
        if (abs(dx) < moveTolerance && abs(dy) < moveTolerance) break;
      dx = x1 - x;
      dy = y1 - y;
      double distance = sqrt(dx*dx + dy*dy);
      //if (abs(x-x1) < moveTolerance || abs(y-y1) < moveTolerance) break;
      float angle_ = Inertial.get_heading();
      if (x-x1 == 0) angle = 90;
      angle = atan((y1-y)/(x1-x)) * 180/ M_PI;
      //if (backwards) angle = 360 - angle + 180;

      angle +=360;
      angle %=360;
         if (dx < 0 &&  dy > 0) angle = 360-angle; //if 2nd quadrant
         if (dx < 0 && dy < 0)  angle = 360 - angle; // if 3rd quadrant
      //angle += 360;
      //if (backwards) angle += 180; // detect if robot moves backwards
      angle +=360;
      angle %=360;
      pros::lcd::set_text(1, "Target: " + to_string(angle));
      pros::lcd::set_text(2, "Current: " + to_string(angle_));

      //int turnDifference = angle_ - angle;

      int actualTurn = 0;
        bool specialDown = false;
        bool specialUp = false;
        int angleTolerance = 1;
        double upperAngleBound = angle + angleTolerance;
        double lowerAngleBound = angle - angleTolerance;
        if (lowerAngleBound < 0) {
            lowerAngleBound = lowerAngleBound + 360;
            specialDown = true;
        }
        if (upperAngleBound > 360) {
            upperAngleBound = upperAngleBound - 360;
            specialUp = true;
        }
        if ((specialDown == true &&
                ((angle_ < lowerAngleBound && angle_ > angle + 180) ||
                 (angle_ > upperAngleBound && angle_ < angle + 180))) ||
               (specialUp == true &&
                ((angle_ > upperAngleBound && angle_ < angle - 180) ||
                 (angle_ < lowerAngleBound && angle_ > angle - 180))) ||
               ((specialUp == false && specialDown == false) &&
                (angle_ > upperAngleBound || angle_ < lowerAngleBound))) {
                  int turn_difference = angle_ - angle;
                  //actual_turn = 0;
                  if (turn_difference > 180)
                      turn_difference = 360 - turn_difference;
                  if (turn_difference < -180)
                      turn_difference = 360 + turn_difference;
                  if (turn_difference < 0 && turn_difference > -180)
                      turn_difference = -turn_difference;

                  // Spins in the directions which will allow bot to complete turn fastest
                  if (turn_difference > 180)
                      turn_difference = 360 - turn_difference;

                      if (turn_difference > 180)
                          turn_difference = 360 - turn_difference;
                      int turn_speed = 90;
                      // Slows down if close to goal heading and stays fast if it is away
                      if (turn_difference < close_turn) {

                          actualTurn =
                              (turn_speed * ((turn_difference / (close_turn))*2.5 + KPBASETURN));
                      } else {
                          actualTurn = turn_speed;
                      }


              }



      //int angleDifference = (Inertial.get_heading() - angle) * 0.2;


      int move = backwards ? -fowardSpeed : fowardSpeed;
    //   if (PID && distance < 500) {
    //       move = (distance/500)*2+10;
    //   }

      // int closeTarget = 300;
      // if ((abs(x - x1) < closeTarget || abs(y-y1) < closeTarget) && fowardPID == true) {
      //   int closer = abs(x - x1) < abs(y-y1) ? abs(x - x1) : abs(y-y1);
      //   move = fowardSpeed * (closer / closeTarget);
      // }
      int difference = backwards ? 2*actualTurn : 2*actualTurn;

      //Track();
      FrontLeftWheel.move(-move-difference);
      MiddleLeftWheel.move(-move-difference);
      BackLeftWheel.move(move+difference);
      FrontRightWheel.move(move-difference);
      MiddleRightWheel.move(move-difference);
      BackRightWheel.move(-move+difference);

     }
     stopHold();
}