# MRSD-Assignments
All of the MRSD assignments we had to do for the MRSD Project Class

You need to download the rest of the project. What is provided here is every file that you have changed and every other file in its directory.

You need to follow the installation instructions which can be found on the pdf document

How to get this to run is to open one terminal, and open up the catkin_ws/ directory:

''
cd catkin_ws/
''

Inside that terminal open up ros by entering:
''
roscore
''


If you have not already done so, open up a new terminal and open up the catkin_ws/ directory and recompile your code using:
'';
catkin_make install
'''

Afterwards for the remainder of the time that you make any modifications you only need to enter in that same terminal:
'';
catkin_make
'''


Open up another terminal and go to the catkin_ws/ directory and run:
'''
roslaunch chatbot_node launch_chatbot.launch 
'''

You can open up the dialogue gui by opening up another terminal, going to the catkin_ws/ directory and entering:
'''
rqt
'''

If you want a photo of your ROS graph, open up:
'''
rqt_graph
'''
