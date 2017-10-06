# Chatbot Node
This is the brains behind MRSD Siri. This node houses all the replies for your messages. <br />
To run this:<br />
1. Clone this repository into your workspace.<br />
2. Clone https://github.com/yogaanna/counter_node repository ito your workspace.<br />
3. Clone https://github.com/yogaanna/message_ui repository ito your workspace.<br />
4. Run catkin_make install.<br />
5. Launch chatbot and counter using the command: roslaunch chatbot_node launch_chatbot.launch.<br />
6. Run rqt and select the MessageGUI plugin.<br />

**NOTE:** <br />
If you can't find the plugin try rqt --force-plugin <br />
If there are other errors make sure you have rqt for ros installed on your laptop. <br />
