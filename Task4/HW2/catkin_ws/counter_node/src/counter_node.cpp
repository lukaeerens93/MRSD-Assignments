#include <ros/ros.h>
#include <chatbot_node/reply_msg.h>
#include <message_ui/sent_msg.h>
#include <arithmetic_node/arithmetic_reply.h>
#include <typeinfo>
#include <counter_node/counter.h>

int num_reply_msg = 0;
int num_sent_msg = 0;

ros::Time last_sent_msg_time;
ros::Time last_reply_msg_time;

ros::Subscriber reply_msg_sub;
ros::Subscriber arithmetic_reply_msg_sub;
ros::Subscriber sent_msg_sub;

ros::ServiceServer service;

void sent_msg_callback(const message_ui::sent_msg msg)		// (1)
{
	num_sent_msg++;
	last_sent_msg_time = msg.header.stamp;
}


void reply_msg_callback(const chatbot_node::reply_msg msg)	// (2)
{
	num_reply_msg++;
	last_reply_msg_time = msg.header.stamp;
}


bool counter(counter_node::counter::Request &req, counter_node::counter::Response &res)
{
	if (req.req_id == 0){res.reply = (float)num_reply_msg + (float)num_sent_msg;}
	if (req.req_id == 1){res.reply = (float)num_reply_msg;}		// You need to add the arithmetic_node replies too
	if (req.req_id == 2){res.reply = (float)num_sent_msg;}		// You need to add the arithmetic_node send messages too
	if (req.req_id == 3){
		res.reply = (ros::Time::now() - last_reply_msg_time).toSec();
		if (num_reply_msg == 0){res.reply = 0;}
	}		// Explanation (a) found below

	if (req.req_id == 4){
		res.reply = (ros::Time::now() - last_sent_msg_time).toSec();
		if (num_sent_msg == 0){res.reply = 0;}	
}		// Explanation (b) found below 

	ROS_INFO("%f", res.reply);
	return true;
}

void arithmetic_reply_msg_callback(const arithmetic_node::arithmetic_reply msg){
 	num_reply_msg++;
 	last_reply_msg_time = msg.header.stamp;
 }

int main(int argc, char **argv) {

  ros::init(argc, argv, "counter_node");
  ros::NodeHandle n;

  reply_msg_sub = n.subscribe("reply_msg", 1000, reply_msg_callback); 	// (1)
  sent_msg_sub = n.subscribe("sent_msg", 1000, sent_msg_callback);	// (2)

  service = n.advertiseService("message_counter", counter);
  arithmetic_reply_msg_sub = n.subscribe("arithmetic_reply", 1000, arithmetic_reply_msg_callback);

  ros::Rate loop_rate(20);

  while(ros::ok()) {
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
