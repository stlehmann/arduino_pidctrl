#! python3

import sys
import json
import serial
from PyQt5.QtWidgets import QApplication, QWidget, QLabel, QSpinBox, \
    QGridLayout, QPushButton, QGroupBox, QVBoxLayout
from PyQt5.QtCore import QTimer

COMPORT = "/dev/cu.usbmodemfd121"


class MySpinBox(QSpinBox):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setRange(-100000, 100000)


class CtrlTestGui(QWidget):

    def __init__(self, parent=None):
        super().__init__(parent)

        self.timer = QTimer()
        self.timer.setInterval(100)
        self.timer.timeout.connect(self.fromjson)

        self.sendtimer = QTimer()
        self.sendtimer.setInterval(1000)
        self.sendtimer.timeout.connect(self.tojson)

        self._dirty = False
        self.__resetpid = False

        # Control Parameters
        # ------------------
        ctrlparamGroup = QGroupBox("Control Parameters:")

        # P-part
        self.pLabel = QLabel("P:")
        self.pSpinBox = MySpinBox()
        self.pSpinBox.valueChanged.connect(self.set_dirty)
        self.pLabel.setBuddy(self.pSpinBox)

        # I-part
        self.iLabel = QLabel("I:")
        self.iSpinBox = MySpinBox()
        self.iSpinBox.valueChanged.connect(self.set_dirty)
        self.iLabel.setBuddy(self.iSpinBox)
        
        # D-part
        self.dLabel = QLabel("D:")
        self.dSpinBox = MySpinBox()
        self.dSpinBox.valueChanged.connect(self.set_dirty)
        self.dLabel.setBuddy(self.dSpinBox)
        
        # Input
        self.inLabel= QLabel("In:")
        self.inSpinBox = MySpinBox()
        self.inSpinBox.valueChanged.connect(self.set_dirty)
        self.inLabel.setBuddy(self.inSpinBox)
        
        # Layout
        layout = QGridLayout()
        layout.addWidget(self.pLabel, 0, 0)
        layout.addWidget(self.pSpinBox, 0, 1)
        layout.addWidget(self.iLabel, 1, 0)
        layout.addWidget(self.iSpinBox, 1, 1)
        layout.addWidget(self.dLabel, 2, 0)
        layout.addWidget(self.dSpinBox, 2, 1)
        layout.addWidget(self.inLabel, 3, 0)
        layout.addWidget(self.inSpinBox, 3, 1)
        ctrlparamGroup.setLayout(layout)

        # Controller Status
        # -----------------
        statusGroup = QGroupBox("Status:")

        # yp
        self.ypLabel = QLabel("yp:")
        self.ypSpinBox = MySpinBox()
        self.ypSpinBox.setReadOnly(True)
        self.ypSpinBox.setButtonSymbols(QSpinBox.NoButtons)
        self.ypLabel.setBuddy(self.ypSpinBox)

        # yi
        self.yiLabel = QLabel("yi:")
        self.yiSpinBox = MySpinBox()
        self.yiSpinBox.setReadOnly(True)
        self.yiSpinBox.setButtonSymbols(QSpinBox.NoButtons)
        self.yiLabel.setBuddy(self.yiSpinBox)

        # yd
        self.ydLabel = QLabel("yd:")
        self.ydSpinBox = MySpinBox()
        self.ydSpinBox.setReadOnly(True)
        self.ydSpinBox.setButtonSymbols(QSpinBox.NoButtons)
        self.ydLabel.setBuddy(self.ydSpinBox)

        # y
        self.yLabel = QLabel("y:")
        self.ySpinBox = MySpinBox()
        self.ySpinBox.setReadOnly(True)
        self.ySpinBox.setButtonSymbols(QSpinBox.NoButtons)
        font = self.yLabel.font()
        font.setBold(True)
        self.yLabel.setFont(font)
        self.ySpinBox.setFont(font)
        self.yLabel.setBuddy(self.ySpinBox)

        # Reset Button
        self.resetButton = QPushButton("Reset")
        self.resetButton.pressed.connect(self.reset_pid)

        layout = QGridLayout()
        layout.addWidget(self.ypLabel, 0, 0)
        layout.addWidget(self.ypSpinBox, 0, 1)
        layout.addWidget(self.yiLabel, 1, 0)
        layout.addWidget(self.yiSpinBox, 1, 1)
        layout.addWidget(self.ydLabel, 2, 0)
        layout.addWidget(self.ydSpinBox, 2, 1)
        layout.addWidget(self.yLabel, 3, 0)
        layout.addWidget(self.ySpinBox, 3, 1)
        layout.addWidget(self.resetButton, 4, 0, 1, 2)
        statusGroup.setLayout(layout)

        layout = QGridLayout()
        layout.addWidget(ctrlparamGroup, 0, 0)
        layout.addWidget(statusGroup, 0, 1)
        self.setLayout(layout)

        self.serial = serial.Serial(COMPORT)
        self.serial.baudrate = 9600

        self.timer.start()
        self.sendtimer.start()

    def closeEvent(self, event):
        self.serial.close()

    def tojson(self, *argv):
        self.dirty = False
        json_string = json.dumps({
            "kp": self.pSpinBox.value(),
            "ki": self.iSpinBox.value(),
            "kd": self.dSpinBox.value(),
            "in": self.inSpinBox.value(),
            "reset": self.__resetpid
            })
        self.serial.write(bytearray(json_string, 'utf-8'))

        self.__resetpid = False

    def fromjson(self):
        if self.serial.inWaiting():
            buf = ""
            brackets = 0
            while self.serial.inWaiting() or brackets:
                c = self.serial.read().decode("utf-8")
                buf += c
                if c == "{": brackets += 1
                elif c == "}": brackets -= 1
            print(buf)
            objects = json.loads(buf)
            try:
                self.ySpinBox.setValue(int(objects["y"]))
            except (KeyError, ValueError) as e:
                print(e)

    def reset_pid(self):
    	self.__resetpid = True

    def set_dirty(self):
        self.dirty = True


if __name__ == "__main__":
    app = QApplication(sys.argv)
    w = CtrlTestGui()
    w.show()
    app.exec_()
