#include <ros/ros.h>
#include <chatbot_node/reply_msg.h>
#include <message_ui/sent_msg.h>
#include <string>

using namespace std;

//Add your code here

class subandpub
{
  public:
    subandpub()
    {
      msg_pub = n.advertise<chatbot_node::reply_msg>("reply_msg", 1000);

      msg_sub = n.subscribe("sent_msg", 1000, &subandpub::subscriberCallback, this);
    }
    void subscriberCallback(const message_ui::sent_msg::ConstPtr& msg)
    {
      std::string name;
      if(!n.getParam("name",name))
      {
        ROS_ERROR("failed to get the parameter 'name'");
      }

      string s_msg = msg->message;
      // ROS_INFO("receieved msg with this data= %s", s_msg.c_str());

      // User msg vs Reply
      chatbot_node::reply_msg reply;

      if(s_msg=="Hello")
      {
        reply.message = "Hello, " + name;
        // ROS_INFO("Hi Shrudhi");
      }
      else if(s_msg=="What is your name?")
      {
        reply.message = "My name is MRSD Siri";
      }
      else if(s_msg == "How are you?")
      {
        reply.message = "I am fine, thank you.";
      }
      else
      {
        // reply.message = "What?";
        return;
      }

      reply.header.stamp = ros::Time::now();

      // ROS_INFO("%s", reply.message.c_str());
      msg_pub.publish(reply);
    }

  private:
    ros::NodeHandle n;
    ros::Publisher msg_pub;
    ros::Subscriber msg_sub;
};

int main(int argc, char **argv) {

  ros::init(argc, argv, "chatbot_node");

  // ROS_INFO("\nI'm in the main function");

  //Add your code here
  subandpub Tobject;

  // Publisher:
  // ros::Publisher msg_pub = n.advertise<chatbot_node::reply_msg>("reply_msg", 1000);
  // Subscriber:
  // ros::Subscriber sub = n.subscribe("sent_msg", 1000, subscriberCallback);
  // ros::spin();
  
  // /*
  ros::Rate loop_rate(20);

  while(ros::ok()) {
    ros::spinOnce();
    loop_rate.sleep();
  }
  // */

  return 0;
}