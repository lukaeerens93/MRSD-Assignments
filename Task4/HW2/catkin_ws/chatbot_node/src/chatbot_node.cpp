#include <ros/ros.h>
#include <chatbot_node/reply_msg.h>
#include <message_ui/sent_msg.h>
#include <string>
#include <sstream>

using namespace std;

// Things I learnt:
//
// Open rqt and then click on plugins and go to message gui in order to open up the GUI
//
// Saving the following in the launch file WITHIN A NODE means that this variable is private 
//  <param name = "name" type = "str" value = "Luka"\>
//
// You thus need to ensure that they are saved outside the node (I put them on top of the node)
//---------------------------------------------------------------------------------------------


std::string NAME;		// because used in void function
chatbot_node::reply_msg msgs;	// Declare the message to be published as a global variable
ros::Publisher rply_msg;	// Declare the publisher object as a global variable

//Add your code here
void sent_message_callback(const message_ui::sent_msg msg){
	
	chatbot_node::reply_msg reply;
	
	reply.header.stamp = ros::Time::now();
	std::ostringstream os;
	os << msg.message.c_str();

	// Have a conditional statement set in place that changes the output based on the input
	if (os.str() == "Hello"){
		reply.message = "Hello " + NAME;
		//Publish the message that is chosen
		rply_msg.publish(reply);}
	if (os.str()  == "What is your name?"){
		reply.message = "My name is MRSD Siri";
		//Publish the message that is chosen
		rply_msg.publish(reply);}
	if (os.str()  == "How are you?"){
		reply.message = "I am fine thank you";
		//Publish the message that is chosen
		rply_msg.publish(reply);}

}


int main(int argc, char **argv) {

  ros::init(argc, argv, "chatbot_node");
  ros::NodeHandle n;

  //Add your code here
	
  // Define each of the global string variables
  n.getParam("name", NAME);

  // Publisher function
  rply_msg = n.advertise<chatbot_node::reply_msg>("reply_msg", 1000);
  ros::Rate loop_rate(20);
  
  // Subscriber Node
  ros::Subscriber snt_msg = n.subscribe("sent_msg", 1000, sent_message_callback);

  while(ros::ok()) {
    ros::spinOnce();
    loop_rate.sleep();
    
  }

  return 0;
}

