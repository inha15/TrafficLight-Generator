#include <iostream>
#include <cmath>
#include <vector>
#include <queue>
#include <set>
#include <utility>
#include <algorithm>
#include <string>
#include <time.h>
#include <ros/ros.h>
#include <boost/format.hpp>
#include <std_msgs/String.h>
#include <std_msgs/Int32.h>
#include <sensor_msgs/Image.h>
#include <opencv2/opencv.hpp>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <sensor_msgs/image_encodings.h>
#include <Eigen/Dense>
#include <termios.h>



using namespace std;

ros::Publisher TFF_SIG_PUB;
std_msgs::String tmp;

int terminate_flag = 0;

int getch(void)
{
  int ch;
  struct termios oldt;
  struct termios newt;

  // Store old settings, and copy to new settings
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;

  // Make required changes and apply the settings
  newt.c_lflag &= ~(ICANON | ECHO);
  newt.c_iflag |= IGNBRK;
  newt.c_iflag &= ~(INLCR | ICRNL | IXON | IXOFF);
  newt.c_lflag &= ~(ICANON | ECHO | ECHOK | ECHOE | ECHONL | ISIG | IEXTEN);
  newt.c_cc[VMIN] = 1;
  newt.c_cc[VTIME] = 0;
  tcsetattr(fileno(stdin), TCSANOW, &newt);

  // Get the current character
  ch = getchar();

  // Reapply old settings
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

  return ch;
}

bool Check_validKey(char tmp){
    if(tmp == 'R') return 1;
    if(tmp == 'r') return 1;
    if(tmp == 'G') return 1;
    if(tmp == 'g') return 1;
    if(tmp == 'L') return 1;
    if(tmp == 'l') return 1;
    if(tmp == 'Q') return 1;
    if(tmp == 'q') return 1;
    if(tmp == 'S') return 1;
    if(tmp == 's') return 1;
    if(tmp == '\x03') return 1;
    return 0;
}


int main(int argc, char** argv){
    ros::init(argc, argv, "TFF_SIG_GEN"); //node name 
	ros::NodeHandle nh;                   //nodehandle
    TFF_SIG_PUB = nh.advertise<std_msgs::String> ("/TFF_SIGN_GEN",  1);
    tmp.data = "G";
    char key_val = NULL;
    ros::Rate r(10);
    
    while(ros::ok()){
        cout << "========================================================\n" << endl;
        cout << "Press KEYBOARD(SIGN : R, r, G, g, L, l or STOP : S, s or exit : Q, q, crtl + c) : " << endl;
        key_val = getch();
        cout << "Input Key : " << key_val << endl;

        // exit case 
        if(key_val == '\x03') terminate_flag = 1; //ctrl + c is exit
        if(key_val == 'Q' || key_val == 'q') terminate_flag = 1;

        if(!Check_validKey(key_val)){
            cout << "Wrong Input!! you must input R, r, G, g, Q, q, ctrl + c" << endl;
            cout << "\n========================================================\n" << endl;
            continue;
        }

        //STOP
        if(key_val == 'S' || key_val == 's'){
            tmp.data = "EXIT";
            cout << "Send DATA : " << tmp.data << endl;
            cout << "\n############ Input STOP ############n*" << endl;
            TFF_SIG_PUB.publish(tmp);
            tmp.data = " ";
            continue;
        }

        //EXIT
        if(terminate_flag){
            tmp.data = "EXIT";
            cout << "Send DATA : " << tmp.data << endl;
            cout << "\n********** Input Terminate *********\n*" << endl;
            TFF_SIG_PUB.publish(tmp);
            break;
        }

        //G, g, R, r, L, l
        tmp.data[0] = key_val;
        cout << "Send DATA : " << tmp.data << endl;
        cout << "\n========================================================\n" << endl;
        TFF_SIG_PUB.publish(tmp);
        //r.sleep();
        ros::spinOnce();
    }
    

}
