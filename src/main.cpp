#include "main.h"
#include "math.h"
#include "autoSelect/selection.h"

//hi


bool buttonR2Prev = false;
bool buttonL2Prev = false;
bool buttonR1Prev = false;
bool buttonL1Prev = false;
bool buttonAPrev = false;
bool buttonBPrev = false;

bool buttonR2Pressed = false;
bool buttonL2True = false;
bool buttonR1True = false;
bool buttonL1True = false;


bool yoru = false;
bool clampState = false;

void autonShoot() {
  Flywheel.move(-100);
  pros::delay(800);
  Indexer.set_value(true);
  pros::delay(400);
  Indexer.set_value(false);
}

void stopHoldDrive() {

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

void turnDrive(int angle, int tolerance, int turn_speed) {

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
    double kP = 1.1;
    double kI = 0.4;
    double kD = 0;
    int previousError = angle_ - angle;
    double error = angle_ - angle;
    double integral = 0;
    double derivative = error / previousError;

    double power = kP* error + kI * integral + kD* derivative;
    

    while (((specialDown == true &&
            ((angle_ < lowerAngleBound && angle_ > angle + 180) ||
             (angle_ > upperAngleBound && angle_ < angle + 180))) ||
           (specialUp == true &&
            ((angle_ > upperAngleBound && angle_ < angle - 180) ||
             (angle_ < lowerAngleBound && angle_ > angle - 180))) ||
           ((specialUp == false && specialDown == false) &&
            (angle_ > upperAngleBound || angle_ < lowerAngleBound)))
            || power > 10) {
              

              if (integral > 20) integral = 19;

              
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
        if (turn_difference < 90) {

            actual_turn =
                (turn_speed * ((turn_difference / (90)) ))*1.6 + 0.2;
        } else {
            actual_turn = turn_speed;
        }
        power = kP* turn_difference + kI * integral + kD* derivative;
      actual_turn = power;
        /*Special conditions if angle bounds are less than 0 or greater than 360
                Neccesary for proper turning and calculation*/
        if ((angle > angle_ + 180) ||
            (angle > angle_ - 180 && angle_ > 180 && angle < 180) ||
            (angle < angle_ && angle_ - angle < 180)){
            actual_turn = -actual_turn;
              //turn_difference = -turn_difference;
            }



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
        error = angle_ - angle;
        integral += turn_difference;
    }

    stopHoldDrive();

    pros::lcd::set_text(4, "Goal reached");
}

bool isFrontClamp = true;
bool isOpen = true;
bool isConveyerUp = false;
bool isConveyerDown = false;

bool isFlywheel = false;

bool isExpand = false;

void fire() {
  while (true) {
    if (CONTROLLER.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)/* && !buttonR2Prev*/) {
        Indexer.set_value(true);
        pros::delay(400);
        Indexer.set_value(false);
      
    }
    //buttonR2Prev = CONTROLLER.get_digital(pros::E_CONTROLLER_DIGITAL_R2);
    pros::delay(5);
  }
}

void opcontrol() {
    new pros::Task(fire);
    //Flywheel.set_brake_mode(pros::pros::E_MOTOR_BRAKE_COAST)

    while (true) {

        pros::lcd::set_text(
            1, "Front left: " + std::to_string(FrontLeftWheel.get_position()));
        pros::lcd::set_text(
            2, "Back left: " + std::to_string(BackLeftWheel.get_position()));
        pros::lcd::set_text(3,
                            "Front right: " +
                                std::to_string(FrontRightWheel.get_position()));
        pros::lcd::set_text(
            4, "Back right: " + std::to_string(BackRightWheel.get_position()));
        pros::lcd::set_text(5,
                            "IMU heading: " + std::to_string(Inertial.get_heading()));
        // double TURN = CONTROLLER.get_analog(ANALOG_RIGHT_X);
        // double STRAIGHT = CONTROLLER.get_analog(ANALOG_LEFT_Y);
        double TURN = CONTROLLER.get_analog(ANALOG_LEFT_X);
        double STRAIGHT = CONTROLLER.get_analog(ANALOG_RIGHT_Y);
        double leftWheels =
            CONTROLLER.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);

        bool ringIntakeUp = CONTROLLER.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
          // || CONTROLLER2.get_digital(pros::E_CONTROLLER_DIGITAL_R1);;
        bool ringIntakeDown = CONTROLLER.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
          // || CONTROLLER2.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
        bool liftUp = CONTROLLER.get_digital(pros::E_CONTROLLER_DIGITAL_UP);
        bool liftDown = CONTROLLER.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN);

        bool topLiftUp = CONTROLLER.get_digital(pros::E_CONTROLLER_DIGITAL_UP);
          // || CONTROLLER2.get_digital(pros::E_CONTROLLER_DIGITAL_UP);
        bool topLiftDown = CONTROLLER.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN);
          // || CONTROLLER2.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN);

        bool fireDisks =
            CONTROLLER.get_digital(pros::E_CONTROLLER_DIGITAL_R2);

        bool flywheelToggle = CONTROLLER.get_digital(pros::E_CONTROLLER_DIGITAL_L2);

        bool expansion = CONTROLLER.get_digital(pros::E_CONTROLLER_DIGITAL_B);

        // if (ringIntakeUp) {
        //
        //     RingIntake.move(127);
        // }
        // else if (ringIntakeDown) {
        //
        //     RingIntake.move(-127);
        // } else if (!ringIntakeUp || !ringIntakeDown){
        //     RingIntake.move(0);
        // }

        // if (fire && !buttonR2Prev) {
        //     FrontPiston.set_value(isFrontClamp);
        //     //FrontPiston2.set_value(isFrontClamp);
        //     isFrontClamp = !isFrontClamp;
        // }

        if (flywheelToggle && !buttonL2Prev) {
          if (!isFlywheel) {
            Flywheel.move(-100);
            isFlywheel = !isFlywheel;
          } else {
            Flywheel.move(0);
            isFlywheel = !isFlywheel;
          }
        }

        if (ringIntakeUp && !buttonR1Prev) {
            isConveyerUp ? (DiskIntake.move(-127)) : DiskIntake.move(0);
            isConveyerUp = !isConveyerUp;
        }
        if (ringIntakeDown && !buttonL1Prev) {
            isConveyerDown ? DiskIntake.move(127) : DiskIntake.move(0);
            isConveyerDown = !isConveyerDown;
        }


        if (expansion && !buttonBPrev) {
            Expansion.set_value(isExpand);
            isExpand = !isExpand;
        }
        // if (balance && !buttonAPrev) {
        //   //turnDrive(0, 3, 127);
        //   double angle = std::atan(y/x) *180/3.14159;
        //   if (x < 0) angle -= 180;
        //   angle = std::fmod((angle + 180.0),360.0);
        //   //angle += 180;
        //   std::cout<<std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(angle)<<std::endl;
        //   turnDrive (angle, 3, 127);

          

        // }

        FrontLeftWheel.move(STRAIGHT - TURN);
        MiddleLeftWheel.move(STRAIGHT - TURN);
        BackLeftWheel.move(STRAIGHT - TURN);
        FrontRightWheel.move(STRAIGHT + TURN);
        MiddleRightWheel.move(STRAIGHT + TURN);
        BackRightWheel.move(STRAIGHT + TURN);

        buttonR2Prev = fireDisks;
        buttonBPrev = expansion;
        buttonR1Prev = ringIntakeUp;
        buttonL1Prev = ringIntakeDown;
        buttonL2Prev = flywheelToggle;
        
    }
}
