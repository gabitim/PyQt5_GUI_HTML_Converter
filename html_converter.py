import os
import sys
from PyQt5.QtWidgets import QWidget, QApplication, QFileDialog, QTextEdit
from PyQt5.uic import loadUi
from PyQt5 import QtCore

# for calling c function from script
from ctypes import *
import sysv_ipc

# this is the script which converts 
import converter

def debug_trace(ui=None):
    from pdb import set_trace
    QtCore.pyqtRemoveInputHook()
    set_trace()
    # QtCore.pyqtRestoreInputHook()


class HTMLConverter(QWidget):
    ROOT_DIR = os.path.dirname(os.path.abspath(__file__))
    html = ''

    def __init__(self):
        super(HTMLConverter, self).__init__()
        ui_path = os.path.join(self.ROOT_DIR, 'html_converter.ui')
        loadUi(ui_path, self)
        # file_path is the file we want to convert 
        self.file_path = self.browse_btn.clicked.connect(self.browse)
        self.convert.clicked.connect(self.convert_to)
        self.verify.clicked.connect(self.send_to_C)
        
        # for writing in the result field
        
        self.file_path = None

    def convert_to(self):
        # function which converts the text to html
        self.html = converter.to_html(self.file_path)
       
        self.output_text.append(str(self.html))

    def send_to_C(self):
        
        sender = CDLL("./sender.so")
        receiver = CDLL("./receiver.so")
        
        sender.connect()
        receiver.connect()
        
        try:
            name = "simplehtml.txt"
            f = open(name, "r")
            test = f.read()
            sender.main(str(name[:-4]))        
  
            # put the key (integer) as parameter (in this case: -1)
            message_queue = sysv_ipc.MessageQueue(-1)
            
            send_message(message_queue, str(name[:-4]))
            send_message(message_queue, test)
            print("aaa")

            receiver.main() 
            print("bbb")

        except sysv_ipc.ExistentialError:
            print("Message queue not initialized. Please run the C program first")


    def browse(self):
        options = QFileDialog.Options()
        options |= QFileDialog.DontUseNativeDialog
        file, _ = QFileDialog.getOpenFileName(self,
                                              caption='Select file',
                                              directory='',
                                              filter="Text Files (*.txt)",
                                              options=options)
        if file:
            self.file_path = file
            self.path_line_edit.setText(file)
            print(file)
            return file


def send_message(message_queue, message):
    message_queue.send(message)
    


if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = HTMLConverter()
    window.show()
    sys.exit(app.exec_())
