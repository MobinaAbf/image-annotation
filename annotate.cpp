#include <opencv2/opencv.hpp>
#include <fstream>
using namespace cv;
using namespace std;

bool drawing = false;
int shapeType = 0;  // 0 for rectangle, 1 for circle
Rect selectedRectangle; // structure of rectangle with width & height
Point2f circleCenter;    // float
int radius = 0;

void drawShapes(Mat &image) {
    if (shapeType == 0) {
        rectangle(image, selectedRectangle, Scalar(0, 255, 0), 2);

        string rectCoords = "Top_Left: (" + to_string(selectedRectangle.x) + ", " +
        to_string(selectedRectangle.y) + ") Down_Right: (" +   
        to_string(selectedRectangle.x + selectedRectangle.width) + ", " +
        to_string(selectedRectangle.y + selectedRectangle.height) + ")";
        putText(image, rectCoords, Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1);

    } else if (shapeType == 1) {
        circle(image, circleCenter, radius, Scalar(0, 0, 255), 2);

        string circleCoords = "Center: (" + to_string(circleCenter.x) + ", " +
        to_string(circleCenter.y) + ") Radius: " + to_string(radius);

        putText(image, circleCoords, Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 1);
    }
}

void saveCoordinatesToFile(const string &filename, int shapeType, const Rect &rect, const Point2f &center, int radius) {
    ofstream file(filename, ios::app); 

    if (!file.is_open()) {
        cerr << "Error opening file for writing!" << endl;
        return;
    }

    if (shapeType == 0) {
        file << "Rectangle, Top_Left: (" << rect.x << ", " << rect.y << "), Down_Right: ("
             << rect.x + rect.width << ", " << rect.y + rect.height << ")" << endl;
    } else if (shapeType == 1) {
        file << "Circle, Center: (" << center.x << ", " << center.y << "), Radius: " << radius << endl;
    }

    file.close();
}

void mouseCallback(int event, int x, int y, int flags, void *userdata) {
    if (event == EVENT_LBUTTONDOWN) {
        drawing = true;
        if (shapeType == 0) {
            selectedRectangle = Rect(x, y, 0, 0);
        } else if (shapeType == 1) {
            circleCenter = Point(x, y);
            radius = 0;
        }
    } else if (event == EVENT_MOUSEMOVE) {
        if (drawing) {
            if (shapeType == 0) {
                selectedRectangle.width = x - selectedRectangle.x;
                selectedRectangle.height = y - selectedRectangle.y;
            } else if (shapeType == 1) {
                radius = static_cast<int>(sqrt(pow(x - circleCenter.x, 2) + pow(y - circleCenter.y, 2)));
            }
        }
    } else if (event == EVENT_LBUTTONUP) {
        drawing = false;
    }
}

int main() {
    Mat image = imread("sample.jpg");
    if (image.empty()) {
        cerr << "Error loading image!" << endl;
        return -1;
    }

    namedWindow("Image");
    setMouseCallback("Image", mouseCallback);

    while (true) {
        Mat tempImage = image.clone();
        drawShapes(tempImage);
        imshow("Image", tempImage);

        char key = waitKey(1);

        if (key == 'r') {
            shapeType = 0; // Set to rectangle
        } else if (key == 'c') {
            shapeType = 1; // Set to circle
        } else if (key == ' ') {
            imwrite("labeled_image.jpg", tempImage);
            saveCoordinatesToFile("coordinates.txt", shapeType, selectedRectangle, circleCenter, radius);
            cout << "Labeled image and coordinates saved." << endl;
        } else if (key == 27) {
            // Pressing Esc key breaks the loop and exits the program
            break;
        }
    }

    destroyAllWindows();
    return 0;
}
