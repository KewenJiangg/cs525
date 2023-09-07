# Assignment 1: Storage Manager

The goal of this assignment is to implement a simple storage manager - a module that is capable of reading blocks
from a file on disk into memory and writing blocks from memory to a file on disk. 

## Group members and contributions

|     Name      |    ID     |         Email         | Contribution |
| :-----------: | :-------: | :-------------------: | :----------: |
| Huaqing Jiang | A20482610 | hjiang30@hawk.iit.edu |              |
|               |           |                       |              |
|               |           |                       |              |
|               |           |                       |              |

## Project ideas

1. At the beginning of all functions, we check whether the parameters are valid. If the parameters are not reasonable, we will return an error message.
2. In the ***manipulating page files*** , we  use `fopen`,`fwrite`,`fclose` and `remove` to implement page creation, opening, closing, and destruction operations.
3. In the ***reading blocks from disc***  and ***writing blocks to a page file***, we use `fread` and `fwrite` to implement `readBlock` and `writeBlock`. Then, perform other operations by calling `readBlock` and `writeBlock`.
4. In the ***ensureCapacity***, we first confirm whether it is necessary to increase the number of pages,if so, we loop through the `appendEmptyBlock`  to complete the increase operation.

## Project environment

- Linux ubuntu64 6.2.0-32-generic
- g++ (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0

## Run

After entering directory assign1, run the `make` command to compile the file：

![1 (1)](C:\Users\江华庆\Desktop\1 (1).png)

![1 (2)](C:\Users\江华庆\Desktop\1 (2).png)

Input the `./test_assign1 ` command to execute generated files：

![1 (3)](C:\Users\江华庆\Desktop\1 (3).png)