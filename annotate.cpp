#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

bool drawing = false;
int shapeType = 0;  // 0 for rectangle, 1 for circle
Rect selectedRectangle;//strust of rectangle with width & height 
Point2f circleCenter;//float
int radius = 0;

void drawShapes(Mat &image) {
    if (shapeType == 0) {
        rectangle(image, selectedRectangle, Scalar(0, 255, 0), 2);
    } else if (shapeType == 1) {
        circle(image, circleCenter, radius, Scalar(0, 0, 255), 2);
    }
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

int main(){
    Mat image = imread("sample.jpg");

    if (image.empty()){
        cerr<<"error loading image \n";
        return -1;
    }

    while (1){
        Mat tempImage = image.clone();
        drawShapes(tempImage);
        imshow("Image", tempImage);

        char key = waitKey(1);

        if (key == 'r') {
            shapeType = 0;  // Set to rectangle
        } else if (key == 'c') {
            shapeType = 1;  // Set to circle
        } else if (key == ' ') {
            imwrite("labeled_image.jpg", tempImage);
            std::cout << "Labeled image saved." << std::endl;
        } else if (key == 27) {
            // Pressing Esc key breaks the loop and exits the program
            break;
        }
    }

    destroyAllWindows();
    return 0;
    
}