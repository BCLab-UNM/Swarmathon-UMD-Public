#ifndef DROPOFCONTROLLER_H
#define DROPOFCONTROLLER_H
#define HEADERFILE_H

#include "Controller.h"
#include "Tag.h"
#include <math.h>
#include <ros/ros.h>
#include "ghost_srv/dropOffCheckIn.h"
#include "ghost_srv/dropOffQueue.h"

class DropOffController : virtual Controller
{
public:
    DropOffController();
    ~DropOffController();

    void Reset() override;
    Result DoWork() override;
    bool ShouldInterrupt() override;
    bool HasWork() override;

    bool IsChangingMode();
    void SetCenterLocation(Point center);
    void SetCurrentLocation(Point current);
    void SetTargetPickedUp();
    void SetBlockBlockingUltrasound(bool blockBlock);
    void SetTargetData(vector<Tag> tags);
    bool HasTarget() {return targetHeld;}

    // for more accuracy in telling the rover where to go
    void setOffsets(float x, float y) {
        offsetX = 0 - x;
        offsetY = 0 - y;
    }

    float GetSpinner() {return spinner;}

    void UpdateData(vector<Tag> tags);

    void SetCurrentTimeInMilliSecs( long int time );

    void setDropOffClient(ros::ServiceClient dropOffCheckInClient,
                          ros::ServiceClient dropOffQueueClient) {
        dropCheckIn = dropOffCheckInClient;
        dropQueue = dropOffQueueClient;
    }
    void setRoverName(string publishedName) {
        roverName = publishedName;
    }

private:

    void ProcessData();

    //Constants

    const float cameraOffsetCorrection = 0.020; //meters
    const float centeringTurnRate = 0.15; //radians
    const int centerTagThreshold = 8;
    const int lostCenterCutoff = 4; //seconds before giving up on drop off beacuse center cannot be seen anymore
    const float collectionPointVisualDistance = 0.2; //in meters
    const float initialSpinSize = 0.05; //in meters aka 10cm
    const float spinSizeIncrement = 0.50; //in meters
    const float searchVelocity = 0.15; //in meters per second
    const float dropDelay = 0.5; //delay in seconds for dropOff



    //Instance Variables

    /*
     *  Timers and Accumulators
     */

    //keep track of progression around a circle when driving in a circle
    float spinner;

    //Timer for return code (dropping the cube in the center)- used for timerTimeElapsed
    long int returnTimer;

    //Time since last exceeding the tag threshold
    long int lastCenterTagThresholdTime;

    //Previous tag count
    int prevCount;

    /*
     *  Cached External Information
     */

    //Count of tags on the left and right, respectively
    int countLeft;
    int countRight;

    //Center and current locations as of the last call to setLocationData
    Point centerLocation;
    Point currentLocation;

    //Time since modeTimer was started, in seconds
    float timerTimeElapsed;

    //The amount over initialSpinSize we've gotten to
    float spinSizeIncrease;

    /*
   *  Flags
   */

    //Flag indicating that a target has been picked up and is held
    bool targetHeld;

    //Flag indicating that we're in the center
    bool reachedCollectionPoint;

    //Flag indicating that we're driving in a circle to find the nest
    bool circularCenterSearching;

    //Flag for when we are entering the center circle
    bool centerApproach;

    //we have seen enough central collection tags to be certain we are either in or driving towards the nest.
    bool seenEnoughCenterTags;

    //Flag to indicate a switch to precision driving
    bool isPrecisionDriving;

    //Flag to indicate that we're starting to follow waypoints
    bool startWaypoint;

    Result result;

    long int current_time;

    bool interrupt = false;
    bool precisionInterrupt = false;
    bool finalInterrupt = false;
    bool first_center = true;

    string roverName;
    int roverNum;
    // if the rover has let the server know they're dropping off
    bool checkedIn = false;
    // for error correction in telling the roveres where to go
    float offsetX = 0, offsetY = 0;

    // sevice client for queue and check in
    ros::ServiceClient dropCheckIn, dropQueue;
};
#endif // end header define
