import serial
import time
import serial.tools.list_ports
from typing import List, Tuple

class SerialManager:
    conn = None

    def Connect(self, port: str, baudrate: int) -> bool:
        try:
            if not self.conn == None and self.conn.is_open:
                self.conn.close()
            self.conn = serial.Serial(port, baudrate, timeout=1)
        except:
            return False
        return True

    def SendCommand(self, command: str) -> List[str]:
        self.conn.write(f"{command}\r".encode("ascii"))
        self.conn.flush()
        
        time.sleep(1)
        
        response = []
        self.conn.readline() 
        
        while True:
            line = self.conn.readline().decode("ascii").strip()
            if line == "--END--" or not line:
                break
            response.append(line)
        return response

    def ScanNodes(self) -> List[Tuple[str, str]]:
        response = self.SendCommand("SCAN")
        return [tuple(entry.split()) for entry in response if entry]

    def UpdateNode(self, source_id: str, target_id: str):
        self.SendCommand(f"UPDATE {source_id} {target_id}")

    def IdentifyNode(self, hwid: str, state: int):
        self.SendCommand(f"IDENTIFY {hwid} {state}")

    def ListPorts(self) -> List[str]:
        ports = serial.tools.list_ports.comports()
        return [port.device for port in ports]

    def ListBaudrates(self) -> List[int]:
        return [1250, 2400, 9600, 19200, 38400, 57600, 115200]
