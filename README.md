# Keeper - Process Monitoring System  

## Overview  

A C program demonstrating mutual process monitoring where two processes ensure each other's continued execution.  

## Key Components  

- **main.c**: Entry point and process router  
- **primary.c**: Main process that initiates monitoring  
- **dummy.c**: Temporary process launcher  
- **keepalive.c**: Secondary monitoring process  

## Operation  

1. Primary process spawns a dummy process  
2. Dummy process launches keepalive then terminates  
3. Primary and keepalive processes:  
   - Continuously verify each other's status  
   - Restart the counterpart if terminated  
   - Maintain process IDs via `/tmp/keepalive.pid`  

## Compilation  

```bash  
gcc main.c primary.c dummy.c keepalive.c -o keeper  
```  

## Execution  

```bash  
./keeper  
```  

## Process Termination  

To stop both processes:  

```bash  
kill <primary_pID> <keepalive_PID>  
```  
or  
```bash  
pkill keeper  
```  
## Demonstrates
Demonstrates:  
- Process creation and management  
- Basic inter-process communication  
- Persistent process patterns  
