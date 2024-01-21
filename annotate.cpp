#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main(){
    Mat image = imread("sample.jpg");

    if (image.empty()){
        cerr<<"error loading image \n";
        return -1;
    }

    
}