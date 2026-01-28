import tkinter as tk
from tkinter import font
import serial
import time

port = '/dev/ttyACM0'
baud = 115200
ser = None
ser = serial.Serial(port, baud, timeout=1)
    
def SendCommand(command):
    ser.write(f"{command}\n".encode('utf-8'))
    ser.flush()

    time.sleep(1);

    response = ser.readline().decode('utf-8').strip()
    if response:
        print(f"Device Response: {response}")
    else:
        print("No response from device.")

def ScanNetwork():
    SendCommand("SCAN")

nodes = [
    ["Edge", "0x8383564009964fec", "0x000000000000000"],
    ["Node", "0x8383564009964fec", "0x000000000000000"],
    ["Node", "0x8383564009964fec", "0x000000000000000"]
]

SendCommand("SCAN")

root = tk.Tk()
root.title("Domotica network controller")
root.geometry("400x200")
default_font = font.nametofont("TkDefaultFont")
default_font.configure(family="monospace", size=10)

activeButtons={}

selectionSource = tk.StringVar(value="None")
selectionTarget = tk.StringVar(value="None")

def Select(mac):
    if selectionSource.get() == mac:
        selectionSource.set("None")
    elif selectionTarget.get() == mac:
        selectionTarget.set("None")
    elif selectionSource.get() == "None":
        selectionSource.set(mac)
    elif selectionTarget.get() == "None":
        selectionTarget.set(mac)
    UpdateButtons()

def UpdateButtons():
    for mac, btn in activeButtons.items():
        if mac == selectionSource.get() or mac == selectionTarget.get():
            btn.config(relief="sunken")
        else:
            btn.config(relief="raised")

def ClearSelection():
    selectionSource.set("None")
    selectionTarget.set("None")
    UpdateButtons()


def Action(mac):
    Command(f'ACTION {mac}')

def LinkSelection():
    Command(f'LINK {selectionSource} {selectionTarget}')
    ClearSelection()


headers = ["ID", "Type", "Node MAC", "Target MAC", "Command", "Select"]
for col, text in enumerate(headers):
    tk.Label(root, text=text).grid(row=0, column=col, padx=10, pady=10)

for i, (nodeType, nodeMac, targetMac) in enumerate(nodes):
    rowNum = i + 1
    tk.Label(root, text=str(i)).grid(row=rowNum, column=0, padx=10, pady=5)
    tk.Label(root, text=nodeType).grid(row=rowNum, column=1, padx=10, pady=5)
    tk.Label(root, text=nodeMac).grid(row=rowNum, column=2, padx=10, pady=5)
    tk.Label(root, text=targetMac).grid(row=rowNum, column=3, padx=10, pady=5)
    tk.Button(root, text="Action", command=lambda n=targetMac: Action(n)).grid(row=rowNum, column=4, padx=10, pady=5)
    btn = tk.Button(root, text="Select", command=lambda n=nodeMac: Select(n))
    btn.grid(row=rowNum, column=5, padx=10, pady=5)
    activeButtons[nodeMac] = btn



tk.Frame(root, height=2, bd=1, relief="sunken").grid(row=10, column=0, columnspan=5, sticky="we", pady=20)

tk.Label(root, text="Source Node:").grid(row=11, column=1, sticky="e")
tk.Label(root, textvariable=selectionSource).grid(row=11, column=2, sticky="w")

tk.Label(root, text="Target Node:").grid(row=12, column=1, sticky="e")
tk.Label(root, textvariable=selectionTarget).grid(row=12, column=2, sticky="w")

tk.Button(root, text="Clear Selection", command=ClearSelection).grid(row=13, column=2, pady=10)
tk.Button(root, text="Link", command=LinkSelection).grid(row=13, column=3, pady=10)

root.mainloop()
