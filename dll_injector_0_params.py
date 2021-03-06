import os
import subprocess
import sys
from ctypes import *


PAGE_READWRITE      = 0x04
PROCESS_ALL_ACCESS  = ( 0x000F0000 | 0x00010000 | 0xFFF )
VIRTUAL_MEM         = ( 0x1000 | 0x2000 )



def usage():
	print "usage: dll_injector.py"
	exit(0)
    
def main(argv):

	# Print python version -- make sure you are using the 32-bit version if injecting a 32-bit DLL!
	print (sys.version)

	if len(argv) != 1:
		usage()

	# Initialize variables
	kernel32    = windll.kernel32
	
	# determine the location that this script file resides in
	basedir = os.path.dirname(os.path.abspath(__file__))
	
	# specify details of the .dll file we wish to inject
	dll_name = "Inject-DLL-For-FreeCell.dll"
	dll_subfolder = "Debug"
	dll_path = basedir + "\\" + dll_subfolder + "\\" + dll_name
	print("dll_path=%s") % dll_path
	dll_len     = len(dll_path)
	
	# specify details of the .exe file we wish to run
	exe_name = "freecell.exe"
	exe_path = basedir + "\\" + exe_name
	#print("exe_path=%s") % exe_path	

	# Start up freecell, assuming it is in CWD, and get its pid
	sp = subprocess.Popen (exe_path)
	pid    = sp.pid
    
	# Get a handle to the process we are injecting into
	h_process = kernel32.OpenProcess( PROCESS_ALL_ACCESS, False, int(pid) )
	if not h_process:
		print "Could not acquire a handle to pid %s" % pid
		sys.exit(0)
        
	# Allocate space for the DLL path
	print "Allocating %d bytes for DLL path.\n" % dll_len
	arg_address = kernel32.VirtualAllocEx( h_process, 0, dll_len, VIRTUAL_MEM, PAGE_READWRITE )
	if not arg_address:
		print "Unable to allocate memory in process.\n"
		sys.exit(0)
    
	# Write the DLL path into the allocated space
	written = c_int(0)
	ret = kernel32.WriteProcessMemory ( h_process, arg_address, dll_path, dll_len, byref(written) )
	if not ret:
		print "Error writing to process memory."
		sys.exit(0)
        
	# Get the address for LoadLibrary
	h_kernel32 = kernel32.GetModuleHandleA( "kernel32.dll" )
	print "kernel32 is located @ address 0x%X\n" % h_kernel32
	h_loadlib = kernel32.GetProcAddress( h_kernel32, "LoadLibraryA" )
	if not h_loadlib:
		print "Unable to find the address for LoadLibraryA!\n"
		sys.exit(0)
        
	# Create the remote thread with entry point of LoadLibrary and point to DLL path
	thread_id = c_ulong(0)
	if not kernel32.CreateRemoteThread ( h_process,
                                         None,
                                         0,
                                         h_loadlib,
                                         c_int(arg_address),
                                         0,
                                         byref(thread_id) ):
		print "Failed DLL injection!"
		sys.exit(0)
        
	print "** Remote thread with ID [0x%X] created." % thread_id.value
	
	
    
if __name__ == "__main__":
	main(sys.argv)