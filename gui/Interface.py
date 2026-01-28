import tkinter as tk
import ttkbootstrap as tb
import threading
import SerialManager
from tkinter import ttk
from tkinter import messagebox

class Interface:
    def __init__(self, root, serialManager: SerialManager):
        self.root = root
        self.serial = serialManager

        self.root.title("Domotica Network Manager")
        self.root.attributes("-type", "utility")

        self.style = tb.Style(theme="flatly")
        self.style.configure('.', font=('Monospace', 10))
        
        self.nodes = []
        self.newSourceHWID = tk.StringVar(value="0000000000000000")
        self.newTargetHWID = tk.StringVar(value="0000000000000000")

        self.selectedPort = tk.StringVar()
        self.selectedBaud = tk.StringVar(value="115200")

        self.SetupGUI()

    def SetupGUI(self):
        self.frame = tb.Frame(self.root, padding=30)
        self.frame.pack(fill="both", expand=True)

        
        self.serialFrame = tb.LabelFrame(self.frame, text="Serial", padding=15)
        self.serialFrame.grid(row=0, column=0, pady=(0, 20))

        self.updateFrame = tb.LabelFrame(self.frame, text="Update", padding=15)
        self.updateFrame.grid(row=1, column=0, pady=(0, 20))
        
        self.nodesFrame = tb.LabelFrame(self.frame, text="Nodes", padding=15)
        self.nodesFrame.grid(row=2, column=0)
    
        self.RenderSerialSection()
        self.RenderUpdateSection()
        self.RenderNodesSection()

    def RenderSerialSection(self):
        tb.Label(self.serialFrame, text="Port:").grid(row=0, column=0)
        self.portMenu = ttk.Combobox(self.serialFrame, textvariable=self.selectedPort, values=self.serial.ListPorts())
        self.portMenu.grid(row=0, column=1)

        tb.Label(self.serialFrame, text="Baudrate:").grid(row=1, column=0)
        self.baudMenu = ttk.Combobox(self.serialFrame, textvariable=self.selectedBaud, values=self.serial.ListBaudrates())
        self.baudMenu.grid(row=1, column=1)
        
        self.serialFrameButtons = tb.Frame(self.serialFrame)
        self.serialFrameButtons.grid(row=2, column=0, columnspan=2)
        tb.Button(self.serialFrameButtons, text="Connect", command=self.HandleConnectSerial).grid(row=2, column=0, padx=(0, 5), pady=(10, 0))
        tb.Button(self.serialFrameButtons, text="Refresh", command=self.HandleRefreshSerial).grid(row=2, column=1, padx=(5, 0), pady=(10, 0))
    
    def RenderUpdateSection(self):
        tb.Label(self.updateFrame, text="Source:").grid(row=0, column=0)
        tb.Label(self.updateFrame, textvariable=self.newSourceHWID).grid(row=0, column=1)

        tb.Label(self.updateFrame, text="Target:").grid(row=1, column=0)
        tb.Label(self.updateFrame, textvariable=self.newTargetHWID).grid(row=1, column=1)

        self.updateFrameButtons = tb.Frame(self.updateFrame)
        self.updateFrameButtons.grid(row=2, column=0, columnspan=2)
        tb.Button(self.updateFrameButtons, text="Update", command=self.HandleUpdate).grid(row=0, column=0, padx=(0, 5), pady=(10, 0))
        tb.Button(self.updateFrameButtons, text="Refresh", command=self.HandleScan).grid(row=0, column=1, padx=(5, 0), pady=(10, 0))
    
    def RenderNodesSection(self):
        self.RenderNodesHeader()

    def RenderNodesHeader(self):
        headers = [("Source HWID", 40), ("Target HWID", 40), ("Identify", 10), ("Source", 10), ("Target", 10)]
        for col, (text, padding) in enumerate(headers):
            tk.Label(self.nodesFrame, text=text, padx=padding).grid(row=0, column=col)
        tb.Separator(self.nodesFrame, orient="horizontal").grid(row=1, column=0, columnspan=5, sticky="ew", pady=(10, 10))
    
    def RenderNodeList(self):
        for widget in self.nodesFrame.winfo_children():
            widget.destroy()
        self.RenderNodesHeader()

        for i, (source, target) in enumerate(self.nodes):
            row = i + 2
            tb.Label(self.nodesFrame, text=source).grid(row=row, column=0)
            tb.Label(self.nodesFrame, text=target).grid(row=row, column=1)

            identifyVar = tk.BooleanVar()
            tb.Checkbutton(
                self.nodesFrame, 
                variable=identifyVar, 
                command=lambda hwid=source, state=identifyVar: self.HandleIdentify(hwid, state)
            ).grid(row=row, column=2)

            tb.Checkbutton(
                self.nodesFrame, 
                variable=self.newSourceHWID,
                onvalue=source,
                offvalue="0000000000000000",
            ).grid(row=row, column=3)

            tb.Checkbutton(
                self.nodesFrame, 
                variable=self.newTargetHWID,
                onvalue=source,
                offvalue="0000000000000000",
            ).grid(row=row, column=4)

    def HandleConnectSerial(self):
        success = self.serial.Connect(self.selectedPort.get(), self.selectedBaud.get())
        if not success:
            messagebox.showerror("Connection Error", f"Failed to connect to '{self.selectedPort.get()}'")

    def HandleRefreshSerial(self):
        for widget in self.serialFrame.winfo_children():
            widget.destroy()

        self.selectedPort = tk.StringVar()
        self.selectedBaud = tk.StringVar(value="115200")
        self.RenderSerialSection()

    def HandleScan(self):
        if self.serial.conn is None or not self.serial.conn.is_open:
            messagebox.showerror("Connection Error", f"Failed to communicate with '{self.selectedPort.get()}'")
        else:
            def task():
                self.nodes = self.serial.ScanNodes()
                self.RenderNodeList()
            threading.Thread(target=task, daemon=True).start()

    def HandleUpdate(self):
        if self.serial.conn is None or not self.serial.conn.is_open:
                    messagebox.showerror("Connection Error", f"Failed to communicate with '{self.selectedPort.get()}'")
        elif self.newSourceHWID.get() == "0000000000000000" or self.newTargetHWID.get() == "0000000000000000":
            messagebox.showerror("Update Error", f"Please select two valid HWIDs")
        else:
            self.serial.UpdateNode(self.newSourceHWID.get(), self.newTargetHWID.get())
            self.newSourceHWID = tk.StringVar(value="0000000000000000")
            self.newTargetHWID = tk.StringVar(value="0000000000000000")

    def HandleIdentify(self, hwid, state):
        value = 1 if state.get() else 0
        threading.Thread(target=self.serial.IdentifyNode, args=(hwid, value), daemon=True).start()
