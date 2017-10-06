# Copyright (c) 2011, Dirk Thomas, TU Darmstadt
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#   * Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#   * Redistributions in binary form must reproduce the above
#     copyright notice, this list of conditions and the following
#     disclaimer in the documentation and/or other materials provided
#     with the distribution.
#   * Neither the name of the TU Darmstadt nor the names of its
#     contributors may be used to endorse or promote products derived
#     from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

from __future__ import division
import os
import rospkg

import rospy
from python_qt_binding import loadUi
from python_qt_binding.QtCore import Qt, QTimer, Slot
from python_qt_binding.QtGui import QKeySequence
from python_qt_binding.QtWidgets import QShortcut, QWidget
from rqt_gui_py.plugin import Plugin
from message_ui.msg import sent_msg
from chatbot_node.msg import reply_msg
from arithmetic_node.msg import arithmetic_reply 
from std_msgs.msg import Int16
from counter_node.srv import *


class MessageGUI(Plugin):

    def reply_msg_callback(self, msg_in):
        self._widget.reply.setText(msg_in.message)

    def arithmetic_reply_msg_callback(self, msg_in):
        display_text = 'Operation Type: '+msg_in.oper_type+'\n'+ \
                       'Answer: '+str(msg_in.answer)+'\n'+ \
                       'Time Received: '+str(msg_in.time_received)+'\n'+ \
                       'Time Answered: '+str(msg_in.time_answered)+'\n'+ \
                       'Process Time: '+str(msg_in.process_time)
        self._widget.reply.setText(display_text)

    def message_count_display(self, counter_val):
        display_text = ''
        if self.counter_req_id == 0:
            display_text=display_text+'Total messages:'
        elif self.counter_req_id == 1:
            display_text=display_text+'Total replied messages:'
        elif self.counter_req_id == 2:
            display_text=display_text+'Total sent messages:'
        elif self.counter_req_id == 3:
            display_text=display_text+'Time since last replied message:'
        elif self.counter_req_id == 4:
            display_text=display_text+'Time since last sent message:'
        print(display_text)
        self._widget.counter_reply.setText(display_text+str(counter_val.reply))

    def __init__(self, context):
        super(MessageGUI, self).__init__(context)
        self.setObjectName('MessageGUI')

        self.message_pub = rospy.Publisher("sent_msg", sent_msg, queue_size=1000)

        rospy.Subscriber("reply_msg", reply_msg, self.reply_msg_callback)
        rospy.Subscriber("arithmetic_reply", arithmetic_reply, self.arithmetic_reply_msg_callback)

        self.msg_to_send = sent_msg()
        self.counter_req_id = -1

        self._widget = QWidget()
        rp = rospkg.RosPack()
        ui_file = os.path.join(rp.get_path('message_ui'), 'resource', 'MessageGUI.ui')
        loadUi(ui_file, self._widget)
        self._widget.setObjectName('MessageGUIui')
        if context.serial_number() > 1:
            self._widget.setWindowTitle(self._widget.windowTitle() + (' (%d)' % context.serial_number()))
        context.add_widget(self._widget)

        self._widget.message_to_send.textChanged.connect(self._on_msg_to_send_changed)
        self._widget.counter_val_to_get.textChanged.connect(self._on_counter_val_to_get_changed) 
        self._widget.counter_val_to_get.setInputMask('9')   

        self._widget.send_message.pressed.connect(self._on_send_message_pressed)
        self._widget.send_request.pressed.connect(self._on_send_request_pressed)
        
    def _on_msg_to_send_changed(self, msg):
        msg = str(msg)
        self.msg_to_send.message = msg

    def _on_send_message_pressed(self):
        self.msg_to_send.header.stamp = rospy.Time.now()
        self.message_pub.publish(self.msg_to_send)

    def _on_counter_val_to_get_changed(self, id):
        try:
            self.counter_req_id = int(id)
        except ValueError:
            print('String input is not an integer')

    def _on_send_request_pressed(self):
         rospy.wait_for_service('message_counter')
         try:
             counter_serv = rospy.ServiceProxy('message_counter',counter)
             response = counter_serv(self.counter_req_id)
             self.message_count_display(response)
             return response
         except rospy.ServiceException, ex:
             print "Service call to get message counter failed. %s"%e


    def shutdown_plugin(self):
        pass

    def save_settings(self, plugin_settings, instance_settings):
        pass

    def restore_settings(self, plugin_settings, instance_settings):
        pass
        
        
        
        
        
        
        
        
