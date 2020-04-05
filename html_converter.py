import os
import sys
from PyQt5.QtWidgets import QWidget, QApplication, QFileDialog, QTextEdit
from PyQt5.uic import loadUi
from PyQt5 import QtCore

# for calling c function from script
from ctypes import *
import sysv_ipc

# this is the python script which converts 
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
        
	# for conection with the C program
        sender = CDLL("./sender.so")
        receiver = CDLL("./receiver.so")
        
        sender.connect()
        receiver.connect()
        
        try:
            # file directory path 
            dir_path = os.path.dirname(os.path.realpath(self.file_path))
                     
            file_name = os.path.relpath(self.file_path, dir_path)
           
            # initialize the queue
            print("before sender")
            sender.main()        
            print("after sender")
            # put the key (integer) as parameter (in this case: -1)
            message_queue = sysv_ipc.MessageQueue(-1)
            
            # send the file name for output
            send_message(message_queue, str(file_name[:-4]))
            
            # we send chunks of 2000 bytes from the file 
        
            no_of_full_chunks = int(len(self.html) / 2000)
            last_chunk = len(self.html) - no_of_full_chunks * 2000
            
            chunks = [] # the list of chunks
                                 
            if no_of_full_chunks > 0:            
                chunks = [self.html[i:i+2000] for i in range(0, no_of_full_chunks* 2000,2000)]
                chunks.append(self.html[-last_chunk:])
               
            else:
                chunks.append(self.html)
            
            
            # we send the number of chunks and then the actual chunks 
            send_message(message_queue,str(len(chunks)))
            for chunk in chunks:
                send_message(message_queue, chunk)
           
            
            print("before receiver")
            receiver.main() 
            print("after receiver")

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
    
            return file


def send_message(message_queue, message):
    message_queue.send(message)
    


if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = HTMLConverter()
    window.show()
    sys.exit(app.exec_())
