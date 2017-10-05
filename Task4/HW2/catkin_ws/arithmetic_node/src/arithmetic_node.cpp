#include <ros/ros.h>
#include <chatbot_node/reply_msg.h>
#include <message_ui/sent_msg.h>
#include <arithmetic_node/arithmetic_reply.h>
#include <typeinfo>
#include <counter_node/counter.h>

using namespace std;


ros::Publisher rply_msg;
std::string arith_msg;

void reply_Arith(const message_ui::sent_msg msg){
	
	arithmetic_node::arithmetic_reply reply;
	
	reply.header.stamp = ros::Time::now();
	std::ostringstream os;
	os << msg.message.c_str();

	// The actual string message that you want to parse through
	arith_msg = os.str();

	// Parse throught the string and detect the numbers
	int Operand1;
	int Operand2;
	char Operator;
	std::stringstream s(arith_msg);

	// Will have the format 2+5 or 6-7 or 30/5
	s >> Operand1; s >> Operator; s >> Operand2;

	if (Operator == '+'){
		float ans = Operand1 + Operand2;
		reply.answer = ans;
		reply.oper_type = "Add";
		reply.time_received = reply.header.stamp.toSec();
		cout << "Inside Additoin" << endl;
		reply.time_answered = ros::Time::now().toSec();
		reply.process_time = reply.time_answered - reply.time_received;
		//Publish the message that is chosen
		rply_msg.publish(reply);}
	else if (Operator == '-'){
		float ans = Operand1 - Operand2;
		reply.answer = ans;
		reply.oper_type = "Subtract";
		reply.time_received = reply.header.stamp.toSec();
		reply.time_answered = ros::Time::now().toSec();
		reply.process_time = reply.time_answered - reply.time_received;
		//Publish the message that is chosen
		rply_msg.publish(reply);}
	else if (Operator == '*'){
		float ans = Operand1 * Operand2;
		reply.answer = ans;
		reply.oper_type = "Multiply";
		reply.time_received = reply.header.stamp.toSec();
		reply.time_answered = ros::Time::now().toSec();
		reply.process_time = reply.time_answered - reply.time_received;
		//Publish the message that is chosen
		rply_msg.publish(reply);}
	else if (Operator == '/'){
		float ans = (float)Operand1/(float)Operand2;
		reply.answer = ans;
		reply.oper_type = "Divide";
		reply.time_received = reply.header.stamp.toSec();
		reply.time_answered = ros::Time::now().toSec();
		reply.process_time = reply.time_answered - reply.time_received;
		//Publish the message that is chosen
		rply_msg.publish(reply);}

}

int main(int argc, char **argv) {

  ros::init(argc, argv, "arithmetic_node");
  ros::NodeHandle n;

  rply_msg = n.advertise<arithmetic_node::arithmetic_reply>("arithmetic_reply", 1000);
  ros::Rate loop_rate(20);
  ros::Subscriber sent_msg = n.subscribe("sent_msg", 1000, reply_Arith);

  while(ros::ok()) {
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
