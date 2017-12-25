//Begin page baseStation
void baseStation() {
    api.setPositionTarget(baseStationPos);
    api.setAttitudeTarget(finalAtt);
    if (game.atBaseStation() == true) {
        for (int i = 0; i <= 4; i++) {
            game.dropSample(i);
        }
    }
}
//End page baseStation
//Begin page checkPos
void checkPos(float pos[3]) {
    //DEBUG(("pos[0] = %f", pos[0]));
    //DEBUG(("pos[1] = %f", pos[1]));
    //DEBUG(("pos[2] = %f", pos[2]));

    //DEBUG(("myState[0] = %f", myState[0]));
    //DEBUG(("myState[1] = %f", myState[1]));
    //DEBUG(("myState[2] = %f", myState[2]));
    if (myState[0] < pos[0] + .02 && myState[0] > pos[0] - .02 && myState[1] < pos[1] + .02 && myState[1] > pos[1] - .02 && myState[2] < pos[2] + .02 && myState[2] > pos[2] - .02) {
        TorFPos = 1;
    }
}
//End page checkPos
//Begin page drill
void drill() {
    //DEBUG(("drill is running"));
    //DEBUG(("counter inside drill = %d", counter));
    //initT = api.getTime();
    if (initT == counter) {

        initAtt[0] = myState[6];
        initAtt[1] = myState[7];
        initAtt[2] = myState[8];

        //DEBUG(("initAtt[0] = %f", initAtt[0]));
        //DEBUG(("initAtt[1] = %f", initAtt[1]));
        //DEBUG(("initAtt[2] = %f", initAtt[2]));
        currentAtt[0] = myState[6];
        currentAtt[1] = myState[7];
        currentAtt[2] = myState[8];
        //DEBUG(("currentAtt[0] = %f", currentAtt[0]));
        //DEBUG(("currentAtt[1] = %f", currentAtt[1]));
        //DEBUG(("currentAtt[2] = %f", currentAtt[2]));
        game.startDrill();
        counter++;
    }
    else {
        currentAtt[0] = myState[6];
        currentAtt[1] = myState[7];
        currentAtt[2] = myState[8];
        //DEBUG(("currentAtt[0] = %f", currentAtt[0]));
        //DEBUG(("currentAtt[1] = %f", currentAtt[1]));
        //DEBUG(("currentAtt[2] = %f", currentAtt[2]));
    }
    dot = mathVecInner(initAtt, currentAtt, 3);
    //DEBUG(("dot = %f", dot));
    angle = acosf(dot);
    //DEBUG(("angle = %f", angle));
    if (game.getDrillError() == true) {
        game.stopDrill();
        counter--;
    }
    else if (game.getDrillEnabled() == true && angle < PI/4) {
        api.setAttRateTarget(spin);
    }
    else {
        //DEBUG(("last drill else statement running"));
        api.setAttRateTarget(stop);
        if (game.checkSample() == true) {
            game.pickupSample();
            game.stopDrill();
            counter--;
        }
    }

}
//End page drill
//Begin page drillNext
void drillNext() {
    checkPos(spot);
    numDrill = game.getDrills(currentXY);
    if (numSamplesHeld == 5) {
        baseStation();
    }
    else if (numSamplesHeld < 3 && TorFPos == 0) {
        //DEBUG(("api.setPositionTarget(spot) running"));
        api.setPositionTarget(spot);
        //api.setAttitudeTarget(attitude);
    }
    else if (numSamplesHeld < 3 && TorFPos == 1) {
        //DEBUG(("drill running"));
        drill();
    }
    else if (numSamplesHeld == 3 && TorFPos == 1 && numDrill > 2) {
        //DEBUG(("3 samples held"));
        spot[1] = spot[1] - .08;
        //DEBUG(("spot[1] = %f", spot[1]));
    }
    else if (numSamplesHeld < 5 && TorFPos == 0) {
        //DEBUG(("setting position to new spot"));
        api.setPositionTarget(spot);
    }
    else if (numSamplesHeld < 5 && TorFPos == 1) {
        //DEBUG(("drilling new spot"));
        drill();
    }

}
//End page drillNext
//Begin page main
//common variables
int counter;
float myState[12];
int initT;
float baseStationPos[3];
float fBTarget[3];
float fRTarget[3];
int numSamplesHeld;
float score;
float currentXY[2];
float currentXYZ[3];
float spot[3];
int square[3];
float squareXY[2];
float posOfSquare[3];
float attitude[3];
float edgeSquare[3];
int numDrill;
float finalAtt[3];

//reachTargetPos variables
float myOrigPos[3];

//float halfTargetPos[3];
float targetPos[3];
float currentPos[3];
float vecBetweenHalf[3];
float vecBetweenTarget[3];
float halfDistance;
float distance;
float positionYZ[2];
float initXPos;

//SandC variables

//drill variables
float spin[3];
float stop[3];
float initAtt[3];
float currentAtt[3];
float dot;
float angle;

//checkPos variables
int TorFPos;

void init(){

    //general variable initiation
    counter = 0;

    api.getMyZRState(myState);
    initXPos = myState[0];

    baseStationPos[0] = 0;
    baseStationPos[1] = 0;
    baseStationPos[2] = 0;

    fBTarget[0] = .2;
    fBTarget[1] = .2;
    fBTarget[2] = .35;

    fRTarget[0] = -.2;
    fRTarget[1] = -.2;
    fRTarget[2] = .35;

    attitude[2] = 0;

    finalAtt[0] = 0;
    finalAtt[1] = 0;
    finalAtt[2] = -1;

    //drill variable initiation
    spin[0] = 0.0;
    spin[1] = 0.0;
    spin[2] = -3.0;

    stop[0] = 0.0;
    stop[1] = 0.0;
    stop[2] = 0.0;

    //checkPos variable initiation
    TorFPos = 0;

}

void loop(){
    //myState holds satellite position, attitude, and velocity
    api.getMyZRState(myState);
    numSamplesHeld = game.getNumSamplesHeld();
    score = game.getScore();
    TorFPos = 0;
    //hold position relative to X and Y axis
    currentXY[0] = myState[0];
    currentXY[1] = myState[1];
    //hold attitude relative to X and Y axis
    attitude[0] = myState[6];
    attitude[1] = myState[7];

    initT = api.getTime();

    //DEBUG(("counterinit = %d", counter));
    //DEBUG(("game.getNumGeysers() = %d", ))
    //if statement escapes Geyser
    if (game.isGeyserHere(currentXY) == true) {
        //DEBUG(("this one actually works"));
        //sets list with position relative to X,Y, and Z axis
        currentXYZ[0] = myState[0];
        currentXYZ[1] = myState[1];
        currentXYZ[2] = myState[2];
        game.pos2square(currentXYZ, square);
        //DEBUG(("square[0] = %d", square[0]));
        //DEBUG(("square[1] = %d", square[1]));
        //DEBUG(("square[2] = %d", square[2]));

        game.square2pos(square, posOfSquare);
        //DEBUG(("posOfSquare[0] = %f", posOfSquare[0]));
        //DEBUG(("posOfSquare[1] = %f", posOfSquare[1]));
        //DEBUG(("posOfSquare[2] = %f", posOfSquare[2]));
        //sets target
        spot[0] = posOfSquare[0];
        spot[1] = posOfSquare[1] - .08;
        spot[2] = .35;

        //DEBUG(("spot[0] = %f", spot[0]));
        //DEBUG(("spot[1] = %f", spot[1]));
        //DEBUG(("spot[2] = %f", spot[2]));
        reachTargetPos(spot);
        //DEBUG(("currentXYZ[1] = %f", currentXYZ[1]));
        if (currentXYZ[1] < posOfSquare[1] - .03) {
            //DEBUG(("counter-- running"));
            counter--;
        }
        //}
    }
    //brings satellite to drop samples
    else if (initT > 150) {
        baseStation();
    }
    //resets attitude
    else if ((myState[8] > .05 && game.getDrillEnabled() == false)|| (myState[8] < -.05 && game.getDrillEnabled() == false)) {
        //DEBUG(("attitute target running"));
        api.setAttitudeTarget(attitude);
    }
    //drilling algorithm for red satellite
    else if (initXPos > .1) {
        if (score == 0) {
            spot[0] = fBTarget[0];
            spot[1] = fBTarget[1];
            spot[2] = fBTarget[2];
            checkPos(spot);
            //DEBUG(("TorFPos = %d", TorFPos));
            if (TorFPos == 0 && game.getDrillEnabled() == false) {
                reachTargetPos(spot);
                //DEBUG(("reachTargetPos(spot) is running"));
            }
            else if (TorFPos == 1 && game.getDrillEnabled() == false) {
                //DEBUG(("resetting counter running"));
                counter--;
                drill();
            }
            else {
                //DEBUG(("else statement running"));
                api.setVelocityTarget(stop);
                drill();
            }
        }
        else {
            //DEBUG(("after first score running"));
            drillNext();
        }

    }
    else {
        if (score == 0) {
            spot[0] = fRTarget[0];
            spot[1] = fRTarget[1];
            spot[2] = fRTarget[2];
            checkPos(spot);
            //DEBUG(("TorFPos = %d", TorFPos));
            if (TorFPos == 0 && game.getDrillEnabled() == false) {
                reachTargetPos(spot);
            }
            else if (TorFPos == 1 && game.getDrillEnabled() == false) {
                counter--;
                drill();
            }
            else {
                api.setVelocityTarget(stop);
                drill();
            }
        }
        else {
            //DEBUG(("drillNext is running"));
            drillNext();
        }

    }
	counter++;
	//DEBUG(("counterfinal = %d", counter));
}

//End page main
//Begin page reachTargetPos
// general function to move Satallite to any X, Y, Z position on map
void reachTargetPos(float targetPos[3]) {
    //initT = api.getTime();
    //DEBUG(("initT = %d", initT));
    //DEBUG(("targetPos[0] = %f", targetPos[0]));
    //DEBUG(("targetPos[1] = %f", targetPos[1]));
    //DEBUG(("targetPos[2] = %f", targetPos[2]));
    if (initT == counter) {
        myOrigPos[0] = myState[0];
        myOrigPos[1] = myState[1];
        myOrigPos[2] = myState[2];
        //DEBUG(("myOrigPos[0] = %f", myOrigPos[0]));
        //DEBUG(("myOrigPos[1] = %f", myOrigPos[1]));
        //DEBUG(("myOrigPos[2] = %f", myOrigPos[2]));
        vecBetweenHalf[0] = (targetPos[0] - myOrigPos[0]) / 2;
        vecBetweenHalf[1] = (targetPos[1] - myOrigPos[1]) / 2;
        vecBetweenHalf[2] = (targetPos[2] - myOrigPos[2]) / 2;
        //DEBUG(("vecBetweenHalf[0] = %f", vecBetweenHalf[0]));
        //DEBUG(("vecBetweenHalf[1] = %f", vecBetweenHalf[1]));
        //DEBUG(("vecBetweenHalf[2] = %f", vecBetweenHalf[2]));
        halfDistance = mathVecMagnitude(vecBetweenHalf, 3) + .01;
        //DEBUG(("halfDistance = %f", halfDistance));
        counter++;
    }
    else {
        currentPos[0] = myState[0];
        currentPos[1] = myState[1];
        currentPos[2] = myState[2];
        //DEBUG(("currentPos[0] = %f", currentPos[0]));
        //DEBUG(("currentPos[1] = %f", currentPos[1]));
        //DEBUG(("currentPos[2] = %f", currentPos[2]));
        vecBetweenTarget[0] = targetPos[0] - currentPos[0];
        vecBetweenTarget[1] = targetPos[1] - currentPos[1];
        vecBetweenTarget[2] = targetPos[2] - currentPos[2];
        //DEBUG(("vecBetweenTarget[0] = %f", vecBetweenTarget[0]));
        //DEBUG(("vecBetweenTarget[1] = %f", vecBetweenTarget[1]));
        //DEBUG(("vecBetweenTarget[2] = %f", vecBetweenTarget[2]));
        distance = mathVecMagnitude(vecBetweenTarget, 3);
        //DEBUG(("distance = %f", distance));
        if (distance > halfDistance) {
            api.setVelocityTarget(vecBetweenTarget);
        }
        else {
            api.setPositionTarget(targetPos);
            //DEBUG(("api.setPositionTarget is running"));
        }
    }
}
//End page reachTargetPos
