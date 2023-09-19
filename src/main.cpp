#include "main.h"
#include "okapi/api.hpp"
using namespace okapi;

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off
 */
std::shared_ptr<ChassisController> myChassis =
  ChassisControllerBuilder()
    .withMotors({-15, 13, 20}, {16, -8, -14})
    // Green gearset, 4 in wheel diam, 11.5 in wheel track
    .withDimensions(AbstractMotor::gearset::green, {{3.250_in, 3.250_in}, imev5GreenTPR})
    .build();

std::shared_ptr<AsyncMotionProfileController> profileController = 
  AsyncMotionProfileControllerBuilder()
    .withLimits({
      10.0, // Maximum linear velocity of the Chassis in m/s
      3.0, // Maximum linear acceleration of the Chassis in m/s/s
      10.0 // Maximum linear jerk of the Chassis in m/s/s/s
    })
    .withOutput(myChassis)
    .buildMotionProfileController();


void opcontrol() {
	// std::shared_ptr<ChassisController> drive = 
	// ChassisControllerBuilder()
	// .withMotors({-15, 13, 20} , {16, -8, -14})
	// .withDimensions(AbstractMotor::gearset::blue, {{3.250_in, 3.250_in}, imev5GreenTPR})
	// .build();

	// Controller controller;

	// ControllerButton runAutoButton(ControllerDigital::X);


	// while(true){
	// 	drive->getModel()->arcade(controller.getAnalog(ControllerAnalog::leftY),
    //                               controller.getAnalog(ControllerAnalog::leftX));

	// 	if (runAutoButton.changedToPressed()) {
    //         // Drive the robot in a square pattern using closed-loop control
    //         for (int i = 0; i < 4; i++) {
    //             drive->moveDistance(12_in); // Drive forward 12 inches
    //             drive->turnAngle(90_deg);   // Turn in place 90 degrees
    //         }
    //     }

    //     // Wait and give up the time we don't need to other tasks.
    //     // Additionally, joystick values, motor telemetry, etc. all updates every 10 ms.
    //     pros::delay(10);						  
	// }

	profileController->generatePath(
    {{0_ft, 0_ft, 0_deg},    // Waypoint A: At (0 feet, 0 feet) with 0 degrees orientation
    {3_ft, 0_ft, 45_deg},   // Waypoint B: At (3 feet, 0 feet) with 45 degrees orientation
    {5_ft, 2_ft, 90_deg},   // Waypoint C: At (5 feet, 2 feet) with 90 degrees orientation
    {2_ft, 4_ft, -30_deg}},
	"A");
  	profileController->setTarget("A");
  	profileController->waitUntilSettled();
}
