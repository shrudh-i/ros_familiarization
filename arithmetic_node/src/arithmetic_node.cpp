#include <ros/ros.h>
#include <chatbot_node/reply_msg.h>
#include <message_ui/sent_msg.h>
#include <regex>
#include <arithmetic_node/arithmetic_reply.h>

class Arithmetic
{
    public:
        Arithmetic()
        {
            a_pub = n.advertise<arithmetic_node::arithmetic_reply>("arithmetic_reply",1000);
            a_sub = n.subscribe("sent_msg", 1000, &Arithmetic::arithmeticCallback, this);
        }
        void arithmeticCallback (const message_ui::sent_msg::ConstPtr& msg)
        {
            // stamp the time @ enter 
            auto received_time = ros::Time::now();

            // regular expression using std
            std::regex pattern(R"((\d+)\s*([+\-*/])\s*(\d+))");
            std::smatch match;

            if(std::regex_search(msg->message, match, pattern))
            {
                //Decode
                char oper = match[2].str()[0];
                float num1 = std::stoi(match[1]);
                float num2 = std::stoi(match[3]);
                
                // response message
                arithmetic_node::arithmetic_reply reply;
                reply.header.stamp = ros::Time::now();
                reply.time_received = received_time.toSec();
                
                switch(oper){
                    case '+':
                        reply.oper_type = "Add";
                        reply.answer = num1+num2;
                        break;
                    
                    case '-':
                        reply.oper_type = "Subtract";
                        reply.answer = num1-num2;
                        break;
                    
                    case '*':
                        reply.oper_type = "Multiply";
                        reply.answer = num1*num2;
                        break;
                    
                    case '/':
                        reply.oper_type = "Divide";
                        if(num2 != 0){
                            reply.answer = num1/num2;
                        }else{
                            ROS_WARN("Division by zero.");
                            reply.answer = 0;
                        }
                        break;
                    
                    default:
                        ROS_WARN("Unknown operation.");
                        return;
                }
                auto answered_time = ros::Time::now();
                reply.time_answered = answered_time.toSec();
                reply.process_time = (answered_time - received_time).toSec();

                a_pub.publish(reply);
            }
            else
            {
                ROS_WARN("Invalid arithmetic expression format.");
            }
        }


    private:
        ros::NodeHandle n;
        ros::Publisher a_pub;
        ros::Subscriber a_sub;
};


int main(int argc, char **argv) {

  ros::init(argc, argv, "arithmetic_node");

  Arithmetic Aobject;

  ros::Rate loop_rate(20);

  while(ros::ok()) {
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}