/*
 * bobble - Dashcam based on OpenCV
 * Copyright 2014 Robert Joslyn
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstdio>
#include <ctime>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include "gpsd_client.hpp"

using namespace std;
using namespace cv;

bool recordInterval(VideoCapture& videoCapture, unsigned int seconds) {
    bool success = true;

    // GPS variables
    double latitude;
    double longitude;
    double speed;

    // Time variables.
    time_t startTime;
    time_t currentTime;
    char currentTimeAsString[80];

    // Colors
    CvScalar textColor = cvScalar(255, 255, 255);
    CvScalar bgColor = cvScalar(0, 0, 0);

    // Font
    int font = FONT_HERSHEY_DUPLEX;
    double fontScale = 0.4;
    int thickness = 1;
    // Number of pixels to inset text from the edge of the frame.
    int textEdgeBuffer = 10;

    int textYPosition = 12; // Y position value for placing text.
    int rectYHeight = 17; // Y height of the rectangle behind the tex.

    int frameHeight = (int)videoCapture.get(CV_CAP_PROP_FRAME_HEIGHT);
    int frameWidth = (int)videoCapture.get(CV_CAP_PROP_FRAME_WIDTH);
    //double fps = videoCapture.get(CV_CAP_PROP_FPS);
    double fps = 15;
    static int captureNum = 0;
    ostringstream filenameSS; // Stringstream for the filename
    Mat frame;

    // Create the video writer.
    filenameSS << "capture" << captureNum + ".avi";
    VideoWriter videoWriter(filenameSS.str(),
                            CV_FOURCC('V', 'P', '8', '0'), // VP8 Codec
                            fps,
                            Size(frameWidth, frameHeight));
    if (!videoWriter.isOpened()) {
        printf("Failed to open video writer.\n");
        success = false;
    }

    // Get the start time.
    time(&startTime);
    time(&currentTime);
    while (success && (difftime(currentTime, startTime) <= seconds)) {
        videoCapture >> frame; // Get a new frame from the camera.
        time(&currentTime); // Capture the time of the frame.

        // Draw a rectangle as background for time.
        rectangle(frame, // image
                  cvPoint(0, 0), // vertex
                  cvPoint(frameWidth, rectYHeight),
                  bgColor, // color
                  CV_FILLED); // thickness

        // Draw the time to the frame.
        strftime(currentTimeAsString, 80, "%c %Z", gmtime(&currentTime));
        putText(frame,
                currentTimeAsString,
                cvPoint(textEdgeBuffer, textYPosition),
                font,
                fontScale,
                textColor,
                thickness,
                CV_AA);

        // Pull GPS data out of structure.
        g_gps_data_mutex.lock();
        latitude = g_gps_data.latitude;
        longitude = g_gps_data.longitude;
        speed = g_gps_data.speed;
        g_gps_data_mutex.unlock();

        // Draw speed to the frame.
        ostringstream textSS;
        int baseline = 0;
        // Convert m/s to mph
        speed = speed * MPS_TO_MPH;
        textSS << setprecision(1) << speed << " mph";
        Size speedTextSize = getTextSize(textSS.str(), font, fontScale,
                                         thickness, &baseline);
        int speedPosition = (frameWidth / 2) - (speedTextSize.width / 2);
        putText(frame,
                textSS.str(),
                cvPoint(speedPosition, textYPosition),
                font,
                fontScale,
                textColor,
                thickness,
                CV_AA);

        // Draw GPS coordinates to the frame.
        ostringstream gpsSS;
        //int baseline = 0;
        gpsSS << setprecision(8) << "Lat: " << latitude << " Lon: " << longitude;
        Size gpsTextSize = getTextSize(gpsSS.str(), font, fontScale,
                                       thickness, &baseline);
        // Text height is 9
        int gpsPosition = frameWidth - (textEdgeBuffer + gpsTextSize.width);
        putText(frame,
                gpsSS.str(),
                cvPoint(gpsPosition, textYPosition),
                font,
                fontScale,
                textColor,
                thickness,
                CV_AA);

        // Write the frame to a file.
        videoWriter.write(frame);

        // Display the frame.
        imshow("Video Preview", frame);

        // Exit when a key is pressed.
        if (waitKey(30) >= 0) {
            break;
        }
    }
    captureNum++;

    return success;
}

int main(int argc, char** argv) {
    // Video Capture
    VideoCapture cap(0); // Open the default camera.
    if (!cap.isOpened()) {
        printf("Failed to open video capture device.\n");
        return -1;
    }
    // Set the camera frame rate.
    // Broken on SPC900NC camera :-(
    //cap.set(CV_CAP_PROP_FPS, 30);

    thread gps(gps_thread);

    namedWindow("Video Preview", 1);
    //while (true) {
        recordInterval(cap, 60);
    //}


    return 0;
}
