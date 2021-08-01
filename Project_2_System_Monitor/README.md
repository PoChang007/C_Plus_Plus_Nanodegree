# System Monitor

![System Monitor](images/monitor.png)

## ncurses

[ncurses](https://www.gnu.org/software/ncurses/) is a library that facilitates text-based graphical output in the terminal. This project relies on ncurses for display output. 

Install ncurses within your own Linux environment: `sudo apt install libncurses5-dev libncursesw5-dev`

## Build Instructions

1. Clone the project repository: `git clone https://github.com/PoChang007/C_Plus_Plus_Nanodegree.git`

2. `cd C_Plus_Plus_Nanodegree/Project_2_System_Monitor`

3. `mkdir build && cd build`

4. `cmake .. && make -j4`

5. Run the resulting executable: `./monitor`
