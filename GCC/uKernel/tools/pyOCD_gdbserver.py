# from pyOCD.board import MbedBoard
# print("-------------------0")  
# board = MbedBoard.chooseBoard()
 
# target = board.target
# flash = board.flash
# print("-------------------1") 
# target.resume()
# print("2") 
# target.halt()
# print("3") 
# print( "pc: 0x%X" % target.readCoreRegister("pc"))
 
# target.step()
# print( "pc: 0x%X" % target.readCoreRegister("pc"))
 
# flash.flashBinary("binaries/l1_lpc1768.bin")
 
# print( "pc: 0x%X" % target.readCoreRegister("pc"))
 
# target.reset()
# target.halt()
# print( "pc: 0x%X" % target.readCoreRegister("pc"))
 
# board.uninit()
from pyOCD.gdbserver import GDBServer
from pyOCD.board import MbedBoard

import logging
logging.basicConfig(level=logging.INFO)

board = MbedBoard.chooseBoard()

# start gdbserver
gdb = GDBServer(board, 3333)


while True:
    pass
