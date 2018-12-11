#include <iostream>
#include "ballDotsDetector.h"

int main() {
    ballDotsDetector BD;
//    BD.loadVideo("../data/Spin_Data1.avi");
    BD.ballDotsDetectionFromVideo("../data/videos/Spin_Data1.avi");
    BD.ballDotsDetection();
    return 0;
}