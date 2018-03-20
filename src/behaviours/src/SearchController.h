#ifndef SEARCH_CONTROLLER
#define SEARCH_CONTROLLER

#include <random_numbers/random_numbers.h>
#include "Controller.h"

/**
 * This class implements the search control algorithm for the rovers.
 */
class SearchController : virtual Controller {

public:

    SearchController();

    void Reset() override;

    // performs search pattern
    Result DoWork() override;
    bool ShouldInterrupt() override;
    bool HasWork() override;

    // sets the value of the current location
    //void UpdateData(geometry_msgs::Pose2D currentLocation, geometry_msgs::Pose2D centerLocation);
    void SetCurrentLocation(Point currentLocation);
    void SetCenterLocation(Point centerLocation);
    void SetSuccesfullPickup();

    Result goToStartingPoint(); // sending rover to their starting location
    bool getStartingPoint() {return startingPoint;} // returns true if rover has reached their starting location
    Result searchBehaviourPrelim();
    Result searchBehaviourSemi();
    void setRoverName(string publishedName); // setting the rover name
    void setVariables();
    void setRound(bool round) { prelim = round; }
    bool getRound() { return prelim; }
    bool setOffset();
    float getOffsetX() { return offsetX; }
    float getOffsetY() { return offsetY; }

    // starting theta each rover will have
    float getTheta(string roverName) {
        if (roverName == "achilles") { return (2*M_PI)/3; }
        else if (roverName == "ajax") {return -M_PI/4; }
        else if (roverName == "aeneas") {
            if (prelim) { return M_PI/4; }
            return -M_PI/2;
        }
        else if (roverName == "diomedes") { return M_PI/2; }
        else if (roverName == "paris") { return (4*M_PI)/3; }
        else if (roverName == "hector") { return M_PI/3; }
        else { return 0; }
    }



protected:

    void ProcessData();

private:

    random_numbers::RandomNumberGenerator* rng;
    Point currentLocation;
    Point centerLocation;
    Point searchLocation;
    int attemptCount = 0;


    Result result; //struct for returning data to ROS adapter
    bool startingPoint = false; // true if reached starting point
    string roverName = ""; // name of the rover

    int choice = 0;
    const float C_INCREASE = .5; // constant added to the distance for rover's doing C shapes
    const float S_INCREASE = .2; // constant added to the distance for rover's doing spirls
    int turn = 1; // keeps track of which way rover should turn
    float distance = 0; // will hold the increased distance rovers will go
    float longD = 2.5, shortD = 5.5; // long and short distance rovers travel
    int negation = 1; // used when we want to negate radians to make rovers turn opposite direction
    float THETA_1, THETA_2, THETA_3, THETA_4, THETA_5, THETA_6; // the different thetas the rovers have to turn
    float ADDED_THETA = 0; // used to make code reusable
    bool prelim = true; // Search state
    bool first_waypoint = true; // Flag to allow special behaviour for the first waypoint
    int spiralCount = 0; // tracks how many turns the inner rovers have made
    float startRadiusOuter, startRadiusInner; // how farout the rover goes to get to their starting point
    bool succesfullPickup = false;
    int timeDelayInt = 0; // delay for inner rovers
    bool timeDelayBool = false; // turns truuuu if time delay has passed
    bool firstSpiral = true; // true when rovers are on their first cycle of the spiral
    float offsetX = 0, offsetY = 0; // this is the offset added to the rovers to fix their odom
};

#endif /* SEARCH_CONTROLLER */
