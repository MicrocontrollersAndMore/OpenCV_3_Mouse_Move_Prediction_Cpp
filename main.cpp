// Mouse_Move_Prediction_Cpp.sln
// main.cpp

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/tracking.hpp"

#include<iostream>
#include<conio.h>           // it may be necessary to change or remove this line if not using Windows

// function prototypes ////////////////////////////////////////////////////////////////////////////
cv::Point predictNextPosition(std::vector<cv::Point> &positions);
void mouseMoveCallback(int event, int x, int y, int flags, void* userData);
void drawCross(cv::Mat &img, cv::Point center, cv::Scalar color);

// global variables ///////////////////////////////////////////////////////////////////////////////
const cv::Scalar SCALAR_WHITE = cv::Scalar(255.0, 255.0, 255.0);
const cv::Scalar SCALAR_BLACK = cv::Scalar(0.0, 0.0, 0.0);
const cv::Scalar SCALAR_BLUE = cv::Scalar(255.0, 0.0, 0.0);
const cv::Scalar SCALAR_GREEN = cv::Scalar(0.0, 200.0, 0.0);
const cv::Scalar SCALAR_RED = cv::Scalar(0.0, 0.0, 255.0);

cv::Point mousePosition(0, 0);

///////////////////////////////////////////////////////////////////////////////////////////////////
int main(void) {

    cv::Mat imgBlank(700, 900, CV_8UC3, cv::Scalar::all(0));            // declare a blank image for moving the mouse over

    std::vector<cv::Point> mousePositions;

    cv::Point predictedMousePosition;

    cv::namedWindow("imgBlank");                                // declare window
    cv::setMouseCallback("imgBlank", mouseMoveCallback);        // 

    while (true) {

        mousePositions.push_back(mousePosition);            // get the current position

        predictedMousePosition = predictNextPosition(mousePositions);        // predict the next position

        std::cout << "current position        = " << mousePositions.back().x << ", " << mousePositions.back().y << "\n";
        std::cout << "next predicted position = " << predictedMousePosition.x << ", " << predictedMousePosition.y << "\n";
        std::cout << "--------------------------------------------------\n";

        drawCross(imgBlank, mousePositions.back(), SCALAR_WHITE);
        drawCross(imgBlank, predictedMousePosition, SCALAR_BLUE);                      // draw a cross at the most recent predicted, actual, and corrected positions

        cv::imshow("imgBlank", imgBlank);         // show the image

        cv::waitKey(10);                    // pause for a moment to get operating system to redraw the imgBlank

        imgBlank = cv::Scalar::all(0);         // blank the imgBlank for next time around
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
cv::Point predictNextPosition(std::vector<cv::Point> &positions) {
    cv::Point predictedPosition;        // this will be the return value
    int numPositions;

    numPositions = positions.size();

    if (numPositions == 0) {

        std::cout << "error, predictNextPosition was called with zero points\n";

    } else if (numPositions == 1) {

        return(positions[0]);

    } else if (numPositions == 2) {

        int deltaX = positions[1].x - positions[0].x;
        int deltaY = positions[1].y - positions[0].y;

        predictedPosition.x = positions.back().x + deltaX;
        predictedPosition.y = positions.back().y + deltaY;

    } else if (numPositions == 3) {

        int sumOfXChanges = ((positions[2].x - positions[1].x) * 2) +
            ((positions[1].x - positions[0].x) * 1);

        int deltaX = (int)std::round((float)sumOfXChanges / 3.0);

        int sumOfYChanges = ((positions[2].y - positions[1].y) * 2) +
            ((positions[1].y - positions[0].y) * 1);

        int deltaY = (int)std::round((float)sumOfYChanges / 3.0);

        predictedPosition.x = positions.back().x + deltaX;
        predictedPosition.y = positions.back().y + deltaY;

    } else if (numPositions == 4) {

        int sumOfXChanges = ((positions[3].x - positions[2].x) * 3) +
            ((positions[2].x - positions[1].x) * 2) +
            ((positions[1].x - positions[0].x) * 1);

        int deltaX = (int)std::round((float)sumOfXChanges / 6.0);

        int sumOfYChanges = ((positions[3].y - positions[2].y) * 3) +
            ((positions[2].y - positions[1].y) * 2) +
            ((positions[1].y - positions[0].y) * 1);

        int deltaY = (int)std::round((float)sumOfYChanges / 6.0);

        predictedPosition.x = positions.back().x + deltaX;
        predictedPosition.y = positions.back().y + deltaY;

    } else if (numPositions >= 5) {

        int sumOfXChanges = ((positions[numPositions - 1].x - positions[numPositions - 2].x) * 4) +
            ((positions[numPositions - 2].x - positions[numPositions - 3].x) * 3) +
            ((positions[numPositions - 3].x - positions[numPositions - 4].x) * 2) +
            ((positions[numPositions - 4].x - positions[numPositions - 5].x) * 1);

        int deltaX = (int)std::round((float)sumOfXChanges / 10.0);

        int sumOfYChanges = ((positions[numPositions - 1].y - positions[numPositions - 2].y) * 4) +
            ((positions[numPositions - 2].y - positions[numPositions - 3].y) * 3) +
            ((positions[numPositions - 3].y - positions[numPositions - 4].y) * 2) +
            ((positions[numPositions - 4].y - positions[numPositions - 5].y) * 1);

        int deltaY = (int)std::round((float)sumOfYChanges / 10.0);

        predictedPosition.x = positions.back().x + deltaX;
        predictedPosition.y = positions.back().y + deltaY;

    } else {
        // should never get here
    }

    return(predictedPosition);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void mouseMoveCallback(int event, int x, int y, int flags, void* userData) {
    if (event == CV_EVENT_MOUSEMOVE) {
        mousePosition.x = x;
        mousePosition.y = y;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawCross(cv::Mat &img, cv::Point center, cv::Scalar color) {
    cv::line(img, cv::Point(center.x - 5, center.y - 5), cv::Point(center.x + 5, center.y + 5), color, 2);
    cv::line(img, cv::Point(center.x + 5, center.y - 5), cv::Point(center.x - 5, center.y + 5), color, 2);

}







