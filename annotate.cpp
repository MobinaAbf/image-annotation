#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

bool drawing = false;
int shapeType = 0;  // 0 for rectangle, 1 for circle
Rect selectedRectangle;//strust of rectangle with width & height 
Point2f circleCenter;//float
int radius = 0;

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