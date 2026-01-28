import tkinter as tk
from SerialManager import SerialManager
from Interface import Interface

if __name__ == "__main__":
    serialManager = SerialManager()
    
    root = tk.Tk()
    app = Interface(root, serialManager)
    root.mainloop()
