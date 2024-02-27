#include <ros/ros.h>
#include <chatbot_node/reply_msg.h>
#include <message_ui/sent_msg.h>
#include <arithmetic_node/arithmetic_reply.h>
#include "counter_node/counter.h" //service 

int num_reply_msg = 0;
int num_sent_msg = 0;

ros::Time last_sent_msg_time;
ros::Time last_reply_msg_time;

ros::Subscriber reply_msg_sub;
ros::Subscriber arithmetic_reply_msg_sub;
ros::Subscriber sent_msg_sub;

void sent_msg_callback(const message_ui::sent_msg msg)
{
	num_sent_msg++;
	last_sent_msg_time = msg.header.stamp;
}

void reply_msg_callback(const chatbot_node::reply_msg msg)
{
	num_reply_msg++;
	last_reply_msg_time = msg.header.stamp;
}

void arithmetic_reply_msg_callback(const arithmetic_node::arithmetic_reply msg)
{
	num_reply_msg++;
	last_reply_msg_time = msg.header.stamp;
}

bool counterServiceCallback(counter_node::counter::Request& req, counter_node::counter::Response& res)
{
  // switch based on request ID:
  switch(req.req_id)
  {
    case 0: //Total number of messages 
      res.reply = num_reply_msg + num_sent_msg;
      break;

    case 1: //Total number of replied messages
      res.reply = num_reply_msg;
      break;

    case 2: // Total number of sent messages  
      res.reply = num_sent_msg;
      break;

    case 3: // Calculate the time elapsed from the last REPLIED message
      // if(num_reply_msg>0)
      // {
      //   res.reply = (ros::Time::now() - last_reply_msg_time).toSec();
      // }
      // else 
      // {
      //   res.reply = 0;
      // }
      res.reply = (ros::Time::now() - last_reply_msg_time).toSec();
      break;

    case 4: // Calculate the time elapsed since the last SENT message
      res.reply = (ros::Time::now() - last_sent_msg_time).toSec();
      break;

    default:
      ROS_WARN("Invalid request ID");
      return false;
  }
  // ROS_INFO("Sending back response: [%f]", res.reply);
  return true;
}

int main(int argc, char **argv) {

  ros::init(argc, argv, "counter_node");
  ros::NodeHandle n;

  reply_msg_sub = n.subscribe("reply_msg", 1000, reply_msg_callback);
  sent_msg_sub = n.subscribe("sent_msg", 1000, sent_msg_callback);
  arithmetic_reply_msg_sub = n.subscribe("arithmetic_reply", 1000, arithmetic_reply_msg_callback);

  ros::ServiceServer service = n.advertiseService("message_counter", counterServiceCallback);

  ros::Rate loop_rate(20);

  while(ros::ok()) {
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}